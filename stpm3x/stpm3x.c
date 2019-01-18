/*
 * Copyright (C) 2019 eeproperty Ltd.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/**
 * @ingroup     drivers_stpm3x
 * @{
 *
 * @file
 * @brief       Driver for the ST STPM33 made for measurement of power and energy.
 *              You can adapt this driver to make it works with other STPM3x chips in the serie (32/34).
 *              Only SPI communication is implemented. The chips support UART too but it's not suported by this driver.
 *              Auto-latch (DSP_REG3, bit 23) is used to retrieve physical values. Other modes have to be implemented if needed.
 *              The driver only allow to read voltages and currents measured on channels 1&2. All others physical values have to be implemented if needed.
 *
 * @author      Joël Carron <jo.carron@cartondu.ch>
 *
 * @}
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "assert.h"
#include "periph/spi.h"
#include "periph/gpio.h"
#include "xtimer.h"

#include "stpm3x.h"
#include "stpm3x_internals.h"
#include "stpm3x_params.h"

#define ENABLE_DEBUG                                  (1U)
#include "debug.h"

static void _stpm3x_errors_spi_cb(void *arg);

/* Internal function prototypes
 * Two functions taken from user manuel from ST "UM2066"- "Getting started with the STPM3x"
 */
static uint8_t _spi_calc_crc8(uint8_t *buf);
static void _spi_crc8_calc(uint8_t data, uint8_t *crc_checksum);

uint8_t stpm3x_init(stpm3x_t *dev, const stpm3x_params_t *params)
{
    assert(dev && params);

    dev->params = *params;

    gpio_init(STPM3X_PARAM_SCS, GPIO_OUT);
    gpio_init(STPM3X_PARAM_SYN, GPIO_OUT);
    gpio_init(STPM3X_PARAM_EN, GPIO_OUT);

    if (gpio_init_int(STPM3X_PARAM_INT1, GPIO_IN, GPIO_RISING, _stpm3x_errors_spi_cb, "INT1") != 0) 
    {
        DEBUG("[stpm3x] Error: could not initialize GPIO INT1 pin\n");
        return STPM3X_ERROR_GPIO;
    }

    if (gpio_init_int(STPM3X_PARAM_INT2, GPIO_IN, GPIO_RISING, _stpm3x_errors_spi_cb, "INT2") != 0)
    {
        DEBUG("[stpm3x] Error: could not initialize GPIO INT2 pin\n");
        return STPM3X_ERROR_GPIO;
    }

    uint32_t gain;

    switch (dev->params.gain)
    {
        // values of gain taken from p.88 & p.103-104 of datasheet
        // rest of register is the default value
        case 2:
            gain = 0x3270327;
            break;
        case 4:
            gain = 0x7270327;
            break;
        case 8:
            gain = 0xB270327;
            break;
        case 16:
            gain = 0xF270327;
            break;
        default:
            gain = 0x3270327; // default value for gain = 2
    }

    if (spi_init_cs(dev->params.spi, dev->params.scs) != SPI_OK)
    {
        DEBUG("%s : error while initializing CS pin\n", DEBUG_FUNC);
        return STPM3X_ERROR;
    }

    xtimer_nanosleep(STPM3X_T_EN_MIN);

    // timings taken from 'Getting started with the STPM3X' p.6
    gpio_clear(dev->params.en);
    gpio_clear(dev->params.scs);

    xtimer_nanosleep(STPM3X_T_IF_TYP);

    gpio_set(dev->params.syn);
    gpio_set(dev->params.en);

    xtimer_nanosleep(STPM3X_T_STARTUP_TYP - STPM3X_T_IF_TYP);

    gpio_set(dev->params.scs);

    xtimer_usleep(100);

    for (uint8_t i = 0; i < 3; i++)
    {
        gpio_clear(dev->params.syn);
        xtimer_nanosleep(STPM3X_T_RPW_TYP);
        gpio_set(dev->params.syn);
        xtimer_nanosleep(STPM3X_T_RPW_TYP);
    }

    xtimer_nanosleep(STPM3X_T_SCS_TYP);
    gpio_clear(dev->params.scs);
    xtimer_nanosleep(STPM3X_T_RPW_TYP);
    gpio_set(dev->params.scs);

    // init of registers
    uint32_t row2  = 0x008004E0; // DSP control register : default value + bit23 at 1 (S/W Auto Latch)
    uint32_t row18 = 0x00504007; // Default value + 80ms SPI timeout
    stpm3x_write_reg(dev, STPM3X_REG_DSP_CR3, &row2);
    stpm3x_write_reg(dev, STPM3X_REG_US_REG1, &row18);
    stpm3x_write_reg(dev, STPM3X_REG_DFE_CR1, &gain); // same gain on both channels
    stpm3x_write_reg(dev, STPM3X_REG_DFE_CR2, &gain);

    uint32_t test_value = 0;
    stpm3x_read_reg(dev, (const uint8_t) STPM3X_REG_DSP_CR3, &test_value);

    if (test_value != row2)
    {
        DEBUG("%s : bad initialization of STPM3x device driver!\n", DEBUG_FUNC);
        return STPM3X_ERROR;
    }

    xtimer_usleep(3000000);

    return STPM3X_OK;
}


uint8_t stpm3x_read_reg(const stpm3x_t *dev, const uint8_t reg, uint32_t *value)
{
    uint8_t data_out[10] = {reg, 0xff, 0, 0, 0, 0xff, 0xff, 0xff, 0xff, 0};
    data_out[4] = _spi_calc_crc8(data_out);
    data_out[9] = _spi_calc_crc8(data_out+5);
    uint8_t data_in[10];

    /**
     * SPI bus of STPM3x becomes more and more sensible to timings as its temperature rises
     * Care with modifying SPI timings
     */
    gpio_clear(dev->params.scs);
    xtimer_nanosleep(STPM3X_T_SCS_MIN);

    spi_acquire(dev->params.spi, dev->params.scs, STPM3X_SPI_MODE, dev->params.sclk);
    spi_transfer_bytes(dev->params.spi, dev->params.scs, true, data_out, data_in, 5);
    xtimer_nanosleep(STPM3X_T_SCS_MIN); // "SPI timings" p.70 of datasheet, 4us wait between consecutive write or read
    spi_transfer_bytes(dev->params.spi, dev->params.scs, true, data_out+5, data_in, 5);
    spi_release(dev->params.spi);

    gpio_set(dev->params.scs);

    *value = (data_in[0] | (data_in[1] << 8) | (data_in[2] << 16) | (data_in[3] << 24));

    return STPM3X_OK;
}

uint8_t stpm3x_write_reg(const stpm3x_t *dev, const uint8_t reg, const uint32_t *value)
{
    uint8_t i = 0;
    uint8_t data_out[5] = {0};
    uint8_t data_in[5] = {0};

    data_out[0] = 0xff;

    gpio_clear(dev->params.scs);
    xtimer_nanosleep(STPM3X_T_SCS_MIN);

    /**
     * SPI bus of STPM3x becomes more and more sensible to timings as its temperature rises
     * Care with modifying SPI timings
     */
    spi_acquire(dev->params.spi, dev->params.scs, STPM3X_SPI_MODE, dev->params.sclk);

    for (i = 0; i < 2; i++)
    {
        data_out[1] = reg + i; // address
        data_out[2] = (uint8_t)(*value >> (i * 16)) & 0xff;
        data_out[3] = (uint8_t)(*value >> (8 + (i * 16))) & 0xff;
        data_out[4] = (uint8_t) _spi_calc_crc8(data_out);

        spi_transfer_bytes(dev->params.spi, dev->params.scs, true, data_out, data_in, 5);
        xtimer_nanosleep(STPM3X_T_SCS_MIN); // "SPI timings" p.70 of datasheet, 4us wait between consecutive write or read
     }

    spi_release(dev->params.spi);

    gpio_set(dev->params.scs);
    xtimer_nanosleep(STPM3X_T_SCS_MIN);
     return STPM3X_OK;
}

static void _stpm3x_errors_spi_cb(void *arg)
{
    printf("%s\n", (char *) arg);
}

uint16_t stpm3x_read_current_rms_1(const stpm3x_t *dev)
{
    uint32_t value = 0;
    
    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG14, &value);
    
    return (uint16_t)((((value & STPM3X_MASK_C1_RMS_DATA) >> 15) & 0xFFFF) * dev->params.currentRMSLSBValue);
}

uint16_t stpm3x_read_voltage_rms_1(const stpm3x_t *dev)
{
    uint32_t value = 0;

    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG14, &value);

    return (uint16_t)(value & STPM3X_MASK_V1_RMS_DATA) * dev->params.voltageRMSLSBValue;
}

uint16_t stpm3x_read_current_rms_2(const stpm3x_t *dev)
{
    uint32_t value = 0;

    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG15, &value);

    return (uint16_t)(((value & STPM3X_MASK_C2_RMS_DATA) >> 15) & 0xFFFF) * dev->params.currentRMSLSBValue;
}

uint16_t stpm3x_read_voltage_rms_2(const stpm3x_t *dev)
{
    uint32_t value = 0;

    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG15, &value);

    return (uint16_t)(value & STPM3X_MASK_V2_RMS_DATA) * dev->params.voltageRMSLSBValue;
}

/* Two functions taken from user manuel from ST "UM2066"- "Getting started with the STPM3x" */
static uint8_t _spi_calc_crc8(uint8_t *buf)
{
    uint8_t crc_checksum = 0x00;

    for (uint8_t i = 0; i < STPM3X_FRAME_LEN - 1; i++)
    {
        _spi_crc8_calc(buf[i], &crc_checksum);
    }

    return crc_checksum;
}

static void _spi_crc8_calc(uint8_t data, uint8_t *crc_checksum)
{
    uint8_t tmp;

    for (uint8_t i = 0; i < 8; ++i)
    {
        tmp = data ^ *crc_checksum;
        *crc_checksum <<= 1;

        if (tmp & 0x80)
        {
            *crc_checksum ^= STPM3X_CRC_8;
        }

        data <<= 1;
    }
}
