/*
 * Copyright (C) 2020 eeproperty Ltd.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/**
 * @defgroup    drivers_stpm3x STPM3X currents, voltages, active/reactive/apparent power and energies
 * @ingroup     drivers_sensors
 * @ingroup     drivers_saul
 * brief        Device driver interface for the ST STPM3x sensors
 *
 * For more information, see the datasheets and related documents:
 * * [STPM32](https://www.st.com/en/data-converters/stpm32.html)
 * * [STPM33](https://www.st.com/en/data-converters/stpm33.html)
 * * [STPM34](https://www.st.com/en/data-converters/stpm34.html)
 * * [Datasheet for all 3 sensors](https://www.st.com/resource/en/datasheet/stpm33.pdf)
 * * [UM2066 - Getting started with the STPM3x](https://www.st.com/resource/en/user_manual/dm00290866.pdf)
 *
 * This driver provides @ref drivers_saul capabilites.
 *
 * ## Usage
 *
 * Add the following to your application makefile:
 *
 * ```make
 * # For STPM3X
 * USEMODULE += stpm3x
 * ```
 *
 * This allows initialisation with the default parameters in `STPM3X_PARAMS_DEFAULT`
 * from "stpm3x_params.h". If you need other settings, you can specify them in the "board.h" file of your board.
 *
 * @{
 * @file
 * @brief       Device driver interface for the STPM3X sensors (STPM32, STPM33, STPM34) from ST.
 *
 * @details     This device driver allows to read intanteaneous current and voltage values on channels 1&2 given by a STPM3X.
 *              The latch of theses values is automatically done (S/W Auto-latch, DSP_REG3 - bit23)
 *              If you need other physical values which the STPM3X can measure, you need to implement the corresponding function.
 *              The device driver implements communication with the microcontroller only with SPI. For UART, you'll need to implement it.
 *
 * @author      Joël Carron <jo.carron@cartondu.ch>
 */

#ifndef STPM3X_H
#define STPM3X_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "board.h"
#include "periph/spi.h"
#include "periph/gpio.h"

/**
  * @brief Error codes
  */
enum {
    STPM3X_OK      =     0,           /**< all went as expected */
    STPM3X_ERROR   =    -1,           /**< generic error code */
    STPM3X_ERROR_GPIO = -2            /**< error code for GPIO */
 };

/**
 * @brief Parameters for the STPM3X sensor
 */
typedef struct {
    spi_t spi;                      /**< SPI bus */
    spi_clk_t sclk;                 /**< SPI clock */
    gpio_t scs;                     /**< Chip-select SPI/UART */
    gpio_t syn;                     /**< Synchronization pin */
    gpio_t int1;                    /**< Interrupt 1 */
    gpio_t int2;                    /**< Interrupt 2 */
    gpio_t en;                      /**< Enable pin */
    double currentRMSLSBValue;      /**< From formual p.52 Datasheet */
    double voltageRMSLSBValue;      /**< From formual p.52 Datasheet */
    uint32_t gain;                  /**< From Table 14 p.49 of Datasheet */
} stpm3x_params_t;

/**
 * @brief Device descriptor for the STPM3X sensor
 */
typedef struct {
    stpm3x_params_t params;         /**< STPM3X initialization parameters */
} stpm3x_t;

/**
 * @brief Initialize the given STPM3X device
 *
 * @param[in] dev           Initialized device descriptor of STPM3X device
 * @param[in]  params       The parameters for the STPM3X device (SPI bus, INT pins, RST pin, LSB values, Gain value)
 *
 * @return                  STPM3X_ERROR on init error
 * @return                  STPM3X_OK on success
 */
uint8_t stpm3x_init(stpm3x_t *dev, const stpm3x_params_t *params);

/**
 * @brief Lock SPI interface for the STPM3X device
 *
 * @param[in] dev           Initialized device descriptor of STPM3X device
 */
void stpm3x_lock_spi_interface(stpm3x_t *dev);

/**
 * @brief Performs reset of the DSP of the STPM3X device
 *
 * @param[in] dev           Initialized device descriptor of STPM3X device
 */
void stpm3x_reset_hw(stpm3x_t *dev);

/**
 * @brief Read a register of the STPM3X
 *
 * @param[in]  dev          Device descriptor of STPM3X device to read from
 * @param[in]  reg          Address of register to read from
 * @param[out] value        Value read from register
 *
 * @return                  STPM3X_OK in any case
 */
uint8_t stpm3x_read_reg(const stpm3x_t *dev, uint8_t reg, uint32_t *value);

/**
 * @brief Write one register to the STPM3X sensor
 *
 * @param[in]  dev          Device descriptor of STPM3X device to write
 * @param[in]  reg          Address of register to write
 * @param[out] value        Value to write in register
 *
 * @return                  STPM3X_OK in any case
 */
uint8_t stpm3x_write_reg(const stpm3x_t *dev, uint8_t reg, const uint32_t *value);

/**
 * @brief Read the instantaneous RMS current value from channel 1
 *
 * @param[in]  dev          Device descriptor of STPM3X device to read from
 *
 * @returns                 The instantaneous RMS current value read from channel 1
 */
uint16_t stpm3x_read_current_rms_1(const stpm3x_t *dev);

/**
 * @brief Read the instantaneous RMS voltage value from channel 1
 *
 * @param[in]  dev          Device descriptor of STPM3X device to read from
 *
 * @returns                 The instantaneous RMS voltage value in [mA] read from channel 1
 */
uint16_t stpm3x_read_voltage_rms_1(const stpm3x_t *dev);

/**
 * @brief Read the instantaneous RMS current value from channel 2
 *
 * @param[in]  dev          Device descriptor of STPM3X device to read from
 *
 * @returns                 The instantaneous RMS current value read from channel 2
 */
uint16_t stpm3x_read_current_rms_2(const stpm3x_t *dev);

/**
 * @brief Read the instantaneous RMS voltage value from channel 2
 *
 * @param[in]  dev          Device descriptor of STPM3X device to read from
 *
 * @returns                 The instantaneous RMS current value read [mA] from channel 2
 */
uint16_t stpm3x_read_voltage_rms_2(const stpm3x_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* STPM3X_H */
/** @} */
