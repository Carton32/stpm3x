#include "saul.h"
#include "stpm3x.h"

static int read_current_rms_1(const void* dev, phydat_t *res)
{
    res->val[0] = stpm3x_read_current_rms_1(dev);
    res->unit = UNIT_A;
    res->scale = -3;
    return 1;
}

static int read_voltage_rms_1(const void* dev, phydat_t *res)
{
    res->val[0] = stpm3x_read_voltage_rms_1(dev);
    res->unit = UNIT_V;
    res->scale = -3;
    return 1;
}

static int read_current_rms_2(const void* dev, phydat_t *res)
{
    res->val[0] = stpm3x_read_current_rms_2(dev);
    res->unit = UNIT_A;
    res->scale = -3;
    return 1;
}

static int read_voltage_rms_2(const void* dev, phydat_t *res)
{
    res->val[0] = stpm3x_read_voltage_rms_2(dev);
    res->unit = UNIT_V;
    res->scale = -3;
    return 1;
}

const saul_driver_t stpm33_current1_saul_driver = {
    .read = read_current_rms_1,
    .write = saul_notsup,
    .type = SAUL_SENSE_ANALOG
};

const saul_driver_t stpm33_voltage1_saul_driver = {
    .read = read_voltage_rms_1,
    .write = saul_notsup,
    .type = SAUL_SENSE_ANALOG
};

const saul_driver_t stpm33_current2_saul_driver = {
    .read = read_current_rms_2,
    .write = saul_notsup,
    .type = SAUL_SENSE_ANALOG
};

const saul_driver_t stpm33_voltage2_saul_driver = {
    .read = read_voltage_rms_2,
    .write = saul_notsup,
    .type = SAUL_SENSE_ANALOG
};
