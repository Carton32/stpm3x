/*
 * Copyright (C) 2020 eeproperty Ltd.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_stpm3x
 * @{
 *
 * @file
 * @brief       SAUL registry of the STPM3x drivers
 *
 * @author      Joel Carron <jo.carron@cartondu.ch>
 *
 * @}
 */

#include "saul.h"
#include "stpm3x.h"

static int read_current_rms_1(const void *dev, phydat_t *res)
{
    const stpm3x_t *d = (stpm3x_t *) dev;

    res->val[0] = stpm3x_read_current_rms_1(d);
    res->unit = UNIT_A;
    res->scale = -3;
    return 1;
}

static int read_voltage_rms_1(const void *dev, phydat_t *res)
{
    const stpm3x_t *d = (stpm3x_t *) dev;

    res->val[0] = stpm3x_read_voltage_rms_1(d);
    res->unit = UNIT_V;
    res->scale = -3;
    return 1;
}

static int read_current_rms_2(const void *dev, phydat_t *res)
{
    const stpm3x_t *d = (stpm3x_t *) dev;

    res->val[0] = stpm3x_read_current_rms_2(d);
    res->unit = UNIT_A;
    res->scale = -3;
    return 1;
}

static int read_voltage_rms_2(const void *dev, phydat_t *res)
{
    const stpm3x_t *d = (stpm3x_t *) dev;

    res->val[0] = stpm3x_read_voltage_rms_2(d);
    res->unit = UNIT_V;
    res->scale = -3;
    return 1;
}

const saul_driver_t stpm3x_current1_saul_driver = {
    .read = read_current_rms_1,
    .write = saul_notsup,
    .type = SAUL_SENSE_ANALOG
};

const saul_driver_t stpm3x_voltage1_saul_driver = {
    .read = read_voltage_rms_1,
    .write = saul_notsup,
    .type = SAUL_SENSE_ANALOG
};

const saul_driver_t stpm3x_current2_saul_driver = {
    .read = read_current_rms_2,
    .write = saul_notsup,
    .type = SAUL_SENSE_ANALOG
};

const saul_driver_t stpm3x_voltage2_saul_driver = {
    .read = read_voltage_rms_2,
    .write = saul_notsup,
    .type = SAUL_SENSE_ANALOG
};
