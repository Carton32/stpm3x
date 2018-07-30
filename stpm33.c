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
 * @brief       Driver for the ST STPM33 made for measurement of power and energies.
 *              You can adapt this driver to make it work with other STPM3x chips in the serie.
 *              Only SPI communication is implemented. The chips support UART too but it's not suported by this driver.
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
#include "stpm33.h"
#include "stpm33_registers.h"

#define ENABLE_DEBUG                                  (0)
#include "debug.h"
#include "log.h"

static uint8_t CalcCRC8(uint8_t *pBuf);
static void Crc8Calc(uint8_t u8Data, uint8_t *CRC_u8Checksum);

/*
 * \fn int8_t stpm3x_init(stpm3x_t *dev, const stpm3x_params_t *params)
 * \brief Initializing STPM3x
 *
 * \param stpm3x_t *dev Pointer to device structure
 * \param const stpm3x_params_t *params Constant pointer to parameters structure
 * \param
*/
int8_t stpm3x_init(stpm3x_t *dev, const stpm3x_params_t *params, uint32_t gain, uint8_t currentRMSLSBValue, uint8_t voltageRMSLSBValue)
{
    assert(dev && params);

    switch (gain)
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
            assert(false);
    }

    dev->currentRMSLSBValue = currentRMSLSBValue; // Values of LSB calculated with formulas at p.52 of datasheet
    dev->voltageRMSLSBValue = voltageRMSLSBValue;

    dev->spi = params->spi;
    dev->sclk = params->sclk;
    dev->scs = params->scs;
    dev->syn = params->syn;
    dev->nrst = params->nrst;

    if (spi_init_cs(dev->spi, dev->scs) != SPI_OK)
    {
        DEBUG("[STPM33] error while initializing CS pin\n");
        return -1;
    }

    gpio_clear(dev->nrst);
    gpio_clear(dev->scs);
    xtimer_usleep(50);

    gpio_set(dev->syn);
    gpio_set(dev->nrst);

    xtimer_usleep(100000);
    gpio_set(dev->scs);
    xtimer_usleep(100);

    uint8_t i;
    for (i = 0; i < 3; i++)
    {
        gpio_clear(dev->syn);
        xtimer_usleep(100);

        gpio_set(dev->syn);
        xtimer_usleep(100);
    }

    // init of registers
    uint32_t reg2  = 0x008004E1; // DSP control register : default value + bit23 at 1 (S/W Auto Latch)
    stpm3x_write_reg(&dev, STPM3X_REG_DSP_CR3, &reg2);
    stpm3x_write_reg(&dev, 0x18, &gain); // same gain on both channels
    stpm3x_write_reg(&dev, 0x1A, &gain);


    return 0;
}

uint8_t stpm3x_read_reg(const stpm3x_t *dev, const uint8_t reg, uint32_t *value)
{
    uint8_t data_out[10] = {reg, 0xff, 0, 0, 0, 0xff, 0xff, 0xff, 0xff, 0};
    data_out[4] = CalcCRC8(data_out);
    data_out[9] = CalcCRC8(data_out+5);
    uint8_t data_in[10];

    gpio_clear(dev->scs);
    xtimer_usleep(50);

    spi_acquire(dev->spi, dev->scs, STPM3X_SPI_MODE, dev->sclk);
    spi_transfer_bytes(dev->spi, dev->scs, true, data_out, data_in, 5);
    spi_release(dev->spi);

    xtimer_usleep(50);
    gpio_set(dev->scs);
    xtimer_usleep(50);
    gpio_clear(dev->scs);
    xtimer_usleep(50);

    spi_acquire(dev->spi, dev->scs, STPM3X_SPI_MODE, dev->sclk);
    spi_transfer_bytes(dev->spi, dev->scs, true, data_out+5, data_in, 5);
    spi_release(dev->spi);

    xtimer_usleep(50);
    gpio_set(dev->scs);

    *value = (data_in[0] | (data_in[1] << 8) | (data_in[2] << 16) | (data_in[3] << 24));

    return 0;
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
        data_out[4] = (uint8_t) CalcCRC8(data_out);

        xtimer_usleep(50);
        gpio_clear(dev->scs);
        xtimer_usleep(50);

        spi_acquire(dev->spi, dev->scs, STPM3X_SPI_MODE, dev->sclk);
        spi_transfer_bytes(dev->spi, dev->scs, true, data_out, data_in, 5);
        spi_release(dev->spi);

        xtimer_usleep(50);
        gpio_set(dev->scs);
     }


     return 0;
}

/*---Two functions taken from UM2066 User manual - "Getting started with the STPM3x" PDF---*/
static uint8_t CalcCRC8(uint8_t *pBuf)
{
    uint8_t i;
    uint8_t CRC_u8Checksum = 0x00;

    for (i = 0; i < STPM3X_FRAME_LEN - 1; i++)
    {
		Crc8Calc(pBuf[i], &CRC_u8Checksum);
    }

    return CRC_u8Checksum;
}

static void Crc8Calc(uint8_t u8Data, uint8_t *CRC_u8Checksum)
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
