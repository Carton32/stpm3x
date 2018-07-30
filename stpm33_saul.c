#include "saul.h"
#include "stpm33.h"

static uint32_t read_current_rms_1(const void* dev, phydat_t *res)
{
    uint32_t value;

    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG14, &value);

    res->val[0] = (uint16_t)(((value & 0xFFFF8000) >> 15) & 0xFFFF) * dev->currentRMSLSBValue;
    res->unit = UNIT_A;
    res->scale = -3;
    return 1;
}

static uint32_t read_voltage_rms_1(const void* dev, phydat_t *res)
{
    uint32_t value;

    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG14, &value);

    res->val[0] = (uint16_t)(value & 0x7FFF) * dev->voltageRMSSBValue;
    res->unit = UNIT_V;
    res->scale = -3;
    return 1;
}

static uint32_t read_current_rms_2(const void* dev, phydat_t *res)
{
    uint32_t value;

    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG15, &value);

    res->val[0] = (uint16_t)(((value & 0xFFFF8000) >> 15) & 0xFFFF) * dev->currentRMSLSBValue;
    res->unit = UNIT_A;
    res->scale = -3;
    return 1;
}

static uint32_t read_voltage_rms_2(const void* dev, phydat_t *res)
{
    uint32_t value;

    stpm3x_read_reg((const stpm3x_t *) dev, (const uint8_t) STPM3X_REG_DSP_REG15, &value);

    res->val[0] = (uint16_t)(value & 0x7FFF) * dev->voltageRMSSBValue;
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
    .read = read_voltage_rms_1,
    .write = saul_notsup,
    .type = SAUL_SENSE_ANALOG
};
