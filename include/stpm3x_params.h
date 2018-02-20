#ifndef STPM3X_PARAMS_H
#define STPM3X_PARAMS_H

#include "board.h"
#include "stpm3x.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for the STPM3X driver
 * @{
 */

/**@}*/

/**
 * @brief   STPM3X configuration
 */
static const stpm3x_params_t stpm3x_params[] =
{
#ifdef STPM3X_PARAMS_BOARD
    STPM3X_PARAMS_BOARD,
#else
    STPM3X_PARAMS_DEFAULT,
#endif
};

#ifdef __cplusplus
}
#endif

#endif /* STPM3X_PARAMS_H */
/** @} */
