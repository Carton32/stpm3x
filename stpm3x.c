/*
 * Copyright (C) 2018 HEIG-VD, MNT Institute // eeproperty SA
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
#include "include/stpm3x_registers.h"

#define ENABLE_DEBUG                                  (0)
#include "debug.h"
#include "log.h"

/*---Two functions taken from UM2066 User manual - "Getting started with the STPM3x" PDF---*/
static uint8_t _CalcCRC8(uint8_t *pBuf);
static void _Crc8Calc(uint8_t u8Data, uint8_t *CRC_u8Checksum);

/*
 * \fn int8_t stpm3x_init(stpm3x_t *dev, const stpm3x_params_t *params)
 * \brief Initializing STPM3x
 *
 * \param stpm3x_t *dev Pointer to device structure
 * \param const stpm3x_params_t *params Constant pointer to parameters structure
 * \param
*/
int8_t stpm3x_init(stpm3x_t *dev, const stpm3x_params_t *params)
{
    assert(dev && params);

    //memcpy(&dev->params, params, sizeof(stpm3x_params_t));
    dev->params = *params;

    uint32_t gain;

    switch (dev->params.gain)
    {
        // Values of gain taken from p.88 & p.103-104 of datasheet
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
            gain = 0x3270327; // default value of 2
    }

    if (spi_init_cs(dev->params.spi, dev->params.scs) != SPI_OK)
    {
        DEBUG("[STPM33] error while initializing CS pin\n");
        return STPM3X_ERROR;
    }

    gpio_clear(dev->params.nrst);
    gpio_clear(dev->params.scs);
    xtimer_usleep(50);

    gpio_set(dev->params.syn);
    gpio_set(dev->params.nrst);

    xtimer_usleep(100000);
    gpio_set(dev->params.scs);
    xtimer_usleep(100);

    uint8_t i;
    for (i = 0; i < 3; i++)
    {
        gpio_clear(dev->params.syn);
        xtimer_usleep(100);

        gpio_set(dev->params.syn);
        xtimer_usleep(100);
    }

    // init of registers
    uint32_t reg2  = 0x008004E1; // DSP control register : default value + bit23 at 1 (S/W Auto Latch)
    stpm3x_write_reg(dev, STPM3X_REG_DSP_CR3, &reg2);
    stpm3x_write_reg(dev, STPM3X_REG_DFE_CR1, &gain); // same gain on both channels
    stpm3x_write_reg(dev, STPM3X_REG_DFE_CR2, &gain);


    return STPM3X_OK;
}

uint8_t stpm3x_read_reg(const stpm3x_t *dev, const uint8_t reg, uint32_t *value)
{
    uint8_t data_out[10] = {reg, 0xff, 0, 0, 0, 0xff, 0xff, 0xff, 0xff, 0};
    data_out[4] = _CalcCRC8(data_out);
    data_out[9] = _CalcCRC8(data_out+5);
    uint8_t data_in[10];

    gpio_clear(dev->params.scs);
    xtimer_usleep(50);

    spi_acquire(dev->params.spi, dev->params.scs, STPM3X_SPI_MODE, dev->params.sclk);
    spi_transfer_bytes(dev->params.spi, dev->params.scs, true, data_out, data_in, 5);
    spi_release(dev->params.spi);

    xtimer_usleep(50);
    gpio_set(dev->params.scs);
    xtimer_usleep(50);
    gpio_clear(dev->params.scs);
    xtimer_usleep(50);

    spi_acquire(dev->params.spi, dev->params.scs, STPM3X_SPI_MODE, dev->params.sclk);
    spi_transfer_bytes(dev->params.spi, dev->params.scs, true, data_out+5, data_in, 5);
    spi_release(dev->params.spi);

    xtimer_usleep(50);
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

	for(i = 0; i < 2; i++)
    {
		data_out[1] = reg + i; // address
		data_out[2] = (uint8_t)(*value >> (i * 16)) & 0xff;
		data_out[3] = (uint8_t)(*value >> (8 + (i * 16))) & 0xff;
        data_out[4] = (uint8_t) _CalcCRC8(data_out);

        xtimer_usleep(50);
        gpio_clear(dev->params.scs);
        xtimer_usleep(50);

        spi_acquire(dev->params.spi, dev->params.scs, STPM3X_SPI_MODE, dev->params.sclk);
        spi_transfer_bytes(dev->params.spi, dev->params.scs, true, data_out, data_in, 5);
        spi_release(dev->params.spi);

        xtimer_usleep(50);
        gpio_set(dev->params.scs);
     }

     return STPM3X_OK;
}

uint16_t stpm3x_read_current_rms_1(const stpm3x_t *dev)
{
    uint32_t value;
    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG14, &value);

    return (uint16_t)(((value & 0xFFFF8000) >> 15) & 0xFFFF) * dev->params.currentRMSLSBValue;
}

uint16_t stpm3x_read_voltage_rms_1(const stpm3x_t *dev)
{
    uint32_t value;
    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG14, &value);

    return (uint16_t)(value & 0x7FFF) * dev->params.voltageRMSLSBValue;
}

uint16_t stpm3x_read_current_rms_2(const stpm3x_t *dev)
{
    uint32_t value;
    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG15, &value);

    return (uint16_t)(((value & 0xFFFF8000) >> 15) & 0xFFFF) * dev->params.currentRMSLSBValue;
}

uint16_t stpm3x_read_voltage_rms_2(const stpm3x_t *dev)
{
    uint32_t value;
    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG15, &value);

    return (uint16_t)(value & 0x7FFF) * dev->params.voltageRMSLSBValue;
}

/*---Two functions taken from UM2066 User manual - "Getting started with the STPM3x" PDF (ST)---*/
static uint8_t _CalcCRC8(uint8_t *pBuf)
{
    uint8_t i;
    uint8_t CRC_u8Checksum = 0x00;

    for (i = 0; i < STPM3X_FRAME_LEN - 1; i++)
    {
		_Crc8Calc(pBuf[i], &CRC_u8Checksum);
    }

    return CRC_u8Checksum;
}

static void _Crc8Calc(uint8_t u8Data, uint8_t *CRC_u8Checksum)
{
    uint8_t loc_u8Idx = 0;
    uint8_t loc_u8Temp;

    while (loc_u8Idx < 8)
    {
		loc_u8Temp = u8Data ^ *CRC_u8Checksum;
        *CRC_u8Checksum <<= 1;

        if (loc_u8Temp & 0x80)
        {
            *CRC_u8Checksum ^= STPM3X_CRC_8;
		}

		u8Data <<= 1;
        loc_u8Idx++;
    }
}
/*-----------------------------------------------------------------------------------*/
