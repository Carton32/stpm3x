/*
 * Copyright (C) 2020 eeproperty Ltd.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_stpm3x
 *
 * @{
 * @file
 * @brief       Default configuration for the STPM3x
 *
 * @author      Joel Carron <jo.carron@cartondu.ch>
 */

#ifndef STPM3X_PARAMS_H
#define STPM3X_PARAMS_H

#include "board.h"
#include "stpm3x.h"
#include "saul_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief SPI mode used by STPM3x
 * From Datasheet 8.6.2 p.69
 */
#define STPM3X_SPI_MODE             SPI_MODE_3

/**
 * @name    Set default configuration parameters for the STPM3X
 * @{
 */
#ifndef STPM3X_PARAM_SPI
#define STPM3X_PARAM_SPI                              (SPI_DEV(0))
#endif
#ifndef STPM3X_PARAM_SPI_CLK
#define STPM3X_PARAM_SPI_CLK                          (SPI_CLK_5MHZ)
#endif
#ifndef STPM3X_PARAM_SCS
#define STPM3X_PARAM_SCS                              (GPIO_PIN(0, 0))
#endif
#ifndef STPM3X_PARAM_SYN
#define STPM3X_PARAM_SYN                              (GPIO_PIN(0, 1))
#endif
#ifndef STPM3X_PARAM_INT1
#define STPM3X_PARAM_INT1                             (GPIO_PIN(0, 2))
#endif
#ifndef STPM3X_PARAM_INT2
#define STPM3X_PARAM_INT2                             (GPIO_PIN(0, 3))
#endif
#ifndef STPM3X_PARAM_EN
#define STPM3X_PARAM_EN                               (GPIO_PIN(0, 4))
#endif
#ifndef STPM3X_PARAM_CURRENTLSB
#define STPM3X_PARAM_CURRENTLSB                       (0.424)               /**< Calculated with formula in Table 15 p.52 of Datasheet */
#endif
#ifndef STPM3X_PARAM_VOLTAGELSB
#define STPM3X_PARAM_VOLTAGELSB                       (1)                   /**< Calculated with formula in Table 15 p.52 of Datasheet */
#endif
#ifndef STPM3X_PARAM_GAIN
#define STPM3X_PARAM_GAIN                             (2)                   /**< Values : 2, 4, 8 or 16 */
#endif

#ifndef STPM3X_PARAMS_DEFAULT
#define STPM3X_PARAMS_DEFAULT                         {                                     \
                                                        .spi    = STPM3X_PARAM_SPI,         \
                                                        .sclk   = STPM3X_PARAM_SPI_CLK,     \
                                                        .scs    = STPM3X_PARAM_SCS,         \
                                                        .syn    = STPM3X_PARAM_SYN,         \
                                                        .int1   = STPM3X_PARAM_INT1,        \
                                                        .int2   = STPM3X_PARAM_INT2,        \
                                                        .en   = STPM3X_PARAM_EN,            \
                                                        .currentRMSLSBValue = STPM3X_PARAM_CURRENTLSB, \
                                                        .voltageRMSLSBValue = STPM3X_PARAM_VOLTAGELSB, \
                                                        .gain = STPM3X_PARAM_GAIN \
                                                      }
#endif
/** @} */

/**
 * @brief Configure STPM3X
 */
static const stpm3x_params_t stpm3x_params[] =
{
#ifdef STPM3X_PARAMS_BOARD
    STPM3X_PARAMS_BOARD
#else
    STPM3X_PARAMS_DEFAULT
#endif
};

/**
 * @brief The number of configured STPM3X sensors
 */
#define STPM3X_NUMOF    (sizeof(stpm3x_params) / sizeof(stpm3x_params[0]))

/**
 * @brief   Allocate and configure entries to the SAUL registry
 * This two dimensional array contains static details of the sensors.
 * Please be aware that the indexes are used in
 * auto_init_stpm3x, so make sure the indexes match.
 */
static const saul_reg_info_t stpm3x_saul_info[STPM3X_NUMOF] =
{
    { .name = "stpm3x" }
};

#ifdef __cplusplus
}
#endif

#endif /* STPM3X_PARAMS_H */
/** @} */
