/*
 * Copyright (C) 2020 eeproperty
 *
 */
/*
 * @ingroup     sys_auto_init_saul
 * @{
 *
 * @file
 * @brief       Auto initialization of STPM3X current/voltage sensors
 *
 * @author      Joël Carron <jo.carron@cartondu.ch>
 *
 * @}
 */
#ifdef MODULE_STPM3X
#include "assert.h"
#include "log.h"
#include "saul_reg.h"
#include "stpm3x.h"
#include "stpm3x_params.h"

/**
 * @brief   Allocate memory for the device descriptors
 */
static stpm3x_t stpm3x_devs[STPM3X_NUMOF];

/**
 * @brief   Memory for the SAUL registry entries
 */
static saul_reg_t saul_entries[STPM3X_NUMOF * 4];

/**
 * @brief   Define the number of saul info
 */
#define STPM3X_INFO_NUM    (sizeof(stpm3x_saul_info) / sizeof(stpm3x_saul_info[0]))

/**
 * @brief   Reference the driver structs.
 * @{
 */
extern const saul_driver_t stpm3x_current1_saul_driver;
extern const saul_driver_t stpm3x_voltage1_saul_driver;
extern const saul_driver_t stpm3x_current2_saul_driver;
extern const saul_driver_t stpm3x_voltage2_saul_driver;
/** @} */

void auto_init_stpm3x(void)
{
    assert(STPM3X_NUMOF == STPM3X_INFO_NUM);
    for (unsigned i = 0; i < STPM3X_NUMOF; i++) {
        LOG_DEBUG("[auto_init_saul] initializing stpm3x #%u\n", i);
        if (stpm3x_init(&stpm3x_devs[i],
                          &stpm3x_params[i]) != STPM3X_OK) {
            LOG_ERROR("[auto_init_saul] error initializing stpm3x #%u\n", i);
            continue;
        }
        /* current 1 */
        saul_entries[(i * 4)].dev = &(stpm3x_devs[i]);
        saul_entries[(i * 4)].name = stpm3x_saul_info[i].name;
        saul_entries[(i * 4)].driver = &stpm3x_current1_saul_driver;
        /* voltage 1 */
        saul_entries[(i * 4) + 1].dev = &(stpm3x_devs[i]);
        saul_entries[(i * 4) + 1].name = stpm3x_saul_info[i].name;
        saul_entries[(i * 4) + 1].driver = &stpm3x_voltage1_saul_driver;
        /* current 2 */
        saul_entries[(i * 4) + 2].dev = &(stpm3x_devs[i]);
        saul_entries[(i * 4) + 2].name = stpm3x_saul_info[i].name;
        saul_entries[(i * 4) + 2].driver = &stpm3x_current2_saul_driver;
        /* voltage 2 */
        saul_entries[(i * 4) + 3].dev = &(stpm3x_devs[i]);
        saul_entries[(i * 4) + 3].name = stpm3x_saul_info[i].name;
        saul_entries[(i * 4) + 3].driver = &stpm3x_voltage2_saul_driver;
        /* register to saul */
        saul_reg_add(&(saul_entries[(i * 4)]));
        saul_reg_add(&(saul_entries[(i * 4) + 1]));
        saul_reg_add(&(saul_entries[(i * 4) + 2]));
        saul_reg_add(&(saul_entries[(i * 4) + 3]));
    }
}
#else
typedef int dont_be_pedantic;
#endif /* MODULE_STPM3X */
