#ifndef STPM3X_PARAMS_H
#define STPM3X_PARAMS_H

#include "board.h"
#include "stpm3x.h"
#include "saul_reg.h"
#include "stpm3x_registers.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for the STPM3X driver
 * @{
 */
#ifndef STPM3X_PARAM_SPI
#define STPM3X_PARAM_SPI                              (SPI_DEV(0))
#endif
#ifndef STPM3X_PARAM_SPI_CLK
#define STPM3X_PARAM_SPI_CLK                          (SPI_CLK_400KHZ)
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
#ifndef STPM3X_PARAM_NRST
#define STPM3X_PARAM_NRST                             (GPIO_PIN(0, 4))
#endif
#ifndef STPM3X_PARAM_CURRENTLSB
#define STPM3X_PARAM_CURRENTLSB                       (0.848)
#endif
#ifndef STPM3X_PARAM_VOLTAGELSB
#define STPM3X_PARAM_VOLTAGELSB                       (1)
#endif
#ifndef STPM3X_PARAM_GAIN
#define STPM3X_PARAM_GAIN                             (2)
#endif
/**@}*/

/**
 * @name    STPM3X configuration
 * @{
 */
#ifndef STPM3X_PARAMS_DEFAULT
#define STPM3X_PARAMS_DEFAULT                         { .spi    = STPM3X_PARAM_SPI,         \
                                                        .sclk   = STPM3X_PARAM_SPI_CLK,     \
                                                        .scs    = STPM3X_PARAM_SCS,         \
                                                        .syn    = STPM3X_PARAM_SYN,         \
                                                        .int1   = STPM3X_PARAM_INT1,        \
                                                        .int2   = STPM3X_PARAM_INT2,        \
                                                        .nrst   = STPM3X_PARAM_NRST         \
                                                        .currentRMSLSBValue = STPM3X_PARAM_CURRENTLSB \
                                                        .voltageRMSLSBValue = STPM3X_PARAM_VOLTAGELSB \
                                                        .gain = STPM3X_PARAM_GAIN \
                                                      }
#endif
/**@}*/

/**
 * @brief   STPM3X configuration
 */
static const stpm3x_params_t stpm3x_params[] =
{
#ifdef STPM3X_PARAMS_BOARD
    STPM3X_PARAMS_BOARD
#else
    STPM3X_PARAMS_DEFAULT
#endif
};

static const saul_reg_info_t stpm3x_saul_info[] =
{
    { .name = "stpm3x" }
};

#ifdef __cplusplus
}
#endif

#endif /* STPM3X_PARAMS_H */
/** @} */
