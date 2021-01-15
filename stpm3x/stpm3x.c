/*
 * Copyright (C) 2020 eeproperty Ltd.
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
 *              You can adapt this driver to make it work with other STPM3x chips in the serie (32/34)
 *              Only SPI communication is implemented. The chips support UART too but it's not supported by this driver.
 *              S/W Latch 1&2 (DSP_REG3, bit 21+22) are used to retrieve physical values. Other modes have to be implemented if needed.
 *              The driver only allow to read voltages and currents measured on channels 1&2. All others physical values have to be implemented if needed.
 *
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

#define ENABLE_DEBUG    (DEBUG_MODE)
#include "debug.h"

#if ENABLE_DEBUG==1
struct irq_callback_args
{
    char *name;
    stpm3x_t *dev;
};
#endif

/* 
 * Internal function prototypes
 * Two functions taken from user manuel from ST "UM2066"- "Getting started with the STPM3x"
 */
static uint8_t _spi_calc_crc8(uint8_t *buf);
static void _spi_crc8_calc(uint8_t data, uint8_t *crc_checksum);

#if ENABLE_DEBUG==1
static void _stpm3x_spi_error_cb(void *arg);
#endif

uint8_t stpm3x_init(stpm3x_t *dev, const stpm3x_params_t *params)
{
    assert(dev && params);

    dev->params = *params;

    gpio_init(STPM3X_PARAM_SYN, GPIO_OUT);
    gpio_init(STPM3X_PARAM_EN, GPIO_OUT);

    if (spi_init_cs(dev->params.spi, dev->params.scs) != SPI_OK)
    {
        DEBUG("%s : error while initializing CS pin\n", DEBUG_FUNC);
        return STPM3X_ERROR;
    }

    stpm3x_lock_spi_interface(dev);

    stpm3x_reset_hw(dev);

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

    // init of registers
#if ENABLE_DEBUG==1
    uint32_t row14 = 0xFFFFFFFF; // Activate all physical values error interrupts on INT1
    stpm3x_write_reg(dev, STPM3X_REG_DSP_IRQ1, &row14);
    uint32_t row15 = 0xFFFFFFFF; // Activate all physical values error interrupts on INT2
    stpm3x_write_reg(dev, STPM3X_REG_DSP_IRQ2, &row15);
    uint32_t row20 = 0x00007F00; // Activate IRQ on SPI errors
    stpm3x_write_reg(dev, STPM3X_REG_US_REG3, &row20);
#endif
    uint32_t row18 = 0x00504007; // Default value + 80ms SPI timeout
    stpm3x_write_reg(dev, STPM3X_REG_US_REG1, &row18);
    stpm3x_write_reg(dev, STPM3X_REG_DFE_CR1, &gain); // same gain on both channels
    stpm3x_write_reg(dev, STPM3X_REG_DFE_CR2, &gain);

    uint32_t test_value = 0;
    stpm3x_read_reg(dev, STPM3X_REG_US_REG1, &test_value);

    if (test_value != row18)
    {
        DEBUG("%s : bad initialization of STPM3x device driver!\n", DEBUG_FUNC);
        return STPM3X_ERROR;
    }

#if ENABLE_DEBUG==1
    // We must configure the interrupt pins after initialising registers related to the interrupts.
    // Otherwise we get spammed with false positive errors (infinite interrupts triggered).
    DEBUG("%s : Debug mode enabled on STPM3X driver\n", DEBUG_FUNC);

    struct irq_callback_args int1 = {
        .name = "INT1",
        .dev = dev
    };
    struct irq_callback_args int2 = {
        .name = "INT2",
        .dev = dev
    };

    if (gpio_init_int(STPM3X_PARAM_INT1, GPIO_IN, GPIO_RISING, _stpm3x_spi_error_cb, &int1) != 0)
    {
        DEBUG("%s: could not initialize GPIO INT1 pin\n", DEBUG_FUNC);
        return STPM3X_ERROR_GPIO;
    }

    if (gpio_init_int(STPM3X_PARAM_INT2, GPIO_IN, GPIO_RISING, _stpm3x_spi_error_cb, &int2) != 0)
    {
        DEBUG("%s: could not initialize GPIO INT2 pin\n", DEBUG_FUNC);
        return STPM3X_ERROR_GPIO;
    }
#endif

    DEBUG("%s : Initialization of STPM3X driver done!\n", DEBUG_FUNC);

    return STPM3X_OK;
}

void stpm3x_lock_spi_interface(stpm3x_t *dev)
{
    gpio_clear(dev->params.en);
    gpio_clear(dev->params.scs);
    xtimer_usleep(STPM3X_T_SCS_CUST);

    gpio_set(dev->params.syn);
    gpio_set(dev->params.en);

    xtimer_usleep(STPM3X_T_STARTUP_TYP);

    gpio_set(dev->params.scs);

    xtimer_usleep(STPM3X_T_SCS_CUST);
}

void stpm3x_reset_hw(stpm3x_t *dev)
{
    // DSP reset
    for (uint8_t i = 0; i < 3; i++)
    {
        gpio_clear(dev->params.syn);
        xtimer_usleep(STPM3X_T_RPW_TYP);
        gpio_set(dev->params.syn);
        xtimer_usleep(STPM3X_T_RPW_TYP);
    }

    // communication reset
    xtimer_usleep(STPM3X_T_SCS_TYP);
    gpio_clear(dev->params.scs);
    xtimer_usleep(STPM3X_T_RPW_TYP);
    gpio_set(dev->params.scs);
}

uint8_t stpm3x_read_reg(const stpm3x_t *dev, uint8_t reg, uint32_t *value)
{
    uint8_t data_out[STPM3X_DATA_SIZE] = {reg, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t data_in[STPM3X_DATA_SIZE];

    data_out[4] = _spi_calc_crc8(data_out);
    data_out[9] = _spi_calc_crc8(data_out+STPM3X_DATA_SIZE_STEP);

    spi_acquire(dev->params.spi, dev->params.scs, STPM3X_SPI_MODE, dev->params.sclk);
    spi_transfer_bytes(dev->params.spi, dev->params.scs, true, data_out, data_in, STPM3X_DATA_SIZE_STEP);
    spi_transfer_bytes(dev->params.spi, dev->params.scs, true, data_out+STPM3X_DATA_SIZE_STEP, data_in, STPM3X_DATA_SIZE_STEP);
    spi_release(dev->params.spi);

    *value = (data_in[0] | (data_in[1] << 8) | (data_in[2] << 16) | (data_in[3] << 24));

    return STPM3X_OK;
}

uint8_t stpm3x_write_reg(const stpm3x_t *dev, uint8_t reg, const uint32_t *value)
{
    uint8_t data_out[STPM3X_DATA_SIZE_STEP] = {0};
    uint8_t data_in[STPM3X_DATA_SIZE_STEP] = {0};

    data_out[0] = 0xff;

    spi_acquire(dev->params.spi, dev->params.scs, STPM3X_SPI_MODE, dev->params.sclk);

    for (uint8_t i = 0; i < 2; i++)
    {
        data_out[1] = reg + i; // address
        data_out[2] = (*value >> (i * 16)) & 0xff;
        data_out[3] = (*value >> (8 + (i * 16))) & 0xff;
        data_out[4] = _spi_calc_crc8(data_out);

        spi_transfer_bytes(dev->params.spi, dev->params.scs, true, data_out, data_in, STPM3X_DATA_SIZE_STEP);
    }

    spi_release(dev->params.spi);

    return STPM3X_OK;
}

#if ENABLE_DEBUG==1
static void _stpm3x_spi_error_cb(void *arg)
{
    struct irq_callback_args *int_args = arg;
    uint32_t status_register = 0x0;

    stpm3x_read_reg(int_args->dev, STPM3X_REG_US_REG3, &status_register);
    DEBUG("%s : SPI/UART errors are 0x%02lX\n", int_args->name, (status_register & 0x7F000000) >> 6);
    DEBUG(" Status : 0x%08lX\n", status_register);
    // p.77: US_REG3 is reset by writing in it
    status_register = 0x00007F00;
    stpm3x_write_reg(int_args->dev, STPM3X_REG_US_REG3, &status_register);

    stpm3x_read_reg(int_args->dev, STPM3X_REG_DSP_SR1, &status_register);
    DEBUG("%s : DSP_SR1 = 0x%08lX\n", int_args->name, status_register);
    // p.64: DSP_SR1 must be cleared by a write operation
    stpm3x_write_reg(int_args->dev, STPM3X_REG_DSP_SR1, 0x00000000);

    stpm3x_read_reg(int_args->dev, STPM3X_REG_DSP_SR2, &status_register);
    DEBUG("%s : DSP_SR2 = 0x%08lX\n", int_args->name, status_register);
    // p.64: DSP_SR2 must be cleared by a write operation
    stpm3x_write_reg(int_args->dev, STPM3X_REG_DSP_SR2, 0x00000000);
}
#endif

static void _stpm3x_sw_latch(const stpm3x_t *dev)
{
    uint32_t row2 = 0;
    stpm3x_read_reg(dev, STPM3X_REG_DSP_CR3, &row2);
    row2  = (row2 | 0x00600000); // S/W latch1 + S/W latch2
    stpm3x_write_reg(dev, STPM3X_REG_DSP_CR3, &row2);
}

uint16_t stpm3x_read_current_rms_1(const stpm3x_t *dev)
{
    _stpm3x_sw_latch(dev);

    uint32_t value = 0;
    stpm3x_read_reg(dev, STPM3X_REG_DSP_REG14, &value);

    // SAUL works with int16_t. So through this interface, you can measure up to 32767 [mA].
    return ((uint16_t)(((value & STPM3X_MASK_C1_RMS_DATA) >> 15) * dev->params.currentRMSLSBValue)) & 0x7FFF;
}

uint16_t stpm3x_read_voltage_rms_1(const stpm3x_t *dev)
{
    _stpm3x_sw_latch(dev);

    uint32_t value = 0;
    stpm3x_read_reg(dev, STPM3X_REG_DSP_REG14, &value);

    return (value & STPM3X_MASK_V1_RMS_DATA) * dev->params.voltageRMSLSBValue;
}

uint16_t stpm3x_read_current_rms_2(const stpm3x_t *dev)
{
    _stpm3x_sw_latch(dev);

    uint32_t value = 0;
    stpm3x_read_reg(dev, STPM3X_REG_DSP_REG15, &value);

    // SAUL works with int16_t. So through this interface, you can measure up to 32767 [mA].
    return ((uint16_t)(((value & STPM3X_MASK_C2_RMS_DATA) >> 15) * dev->params.currentRMSLSBValue)) & 0x7FFF;
}

uint16_t stpm3x_read_voltage_rms_2(const stpm3x_t *dev)
{
    _stpm3x_sw_latch(dev);

    uint32_t value = 0;
    stpm3x_read_reg(dev, STPM3X_REG_DSP_REG15, &value);
    return (value & STPM3X_MASK_V2_RMS_DATA) * dev->params.voltageRMSLSBValue;
}

/*
 * Two functions taken from user manuel from ST "UM2066"- "Getting started with the STPM3x"
 */
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

    for (uint8_t i = 0; i < 8; i++)
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
