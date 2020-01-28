/*
 * Copyright (C) 2020 eeproperty Ltd.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_stpm3x
 * @brief       Internal addresses, registers, constants for the STPM3X family sensors
 * @{
 * @file
 * @brief       Internal addresses, registers, constants for the STPM3X family sensors
 *
 * @author      Joel Carron <jo.carron@cartondu.ch>
 */

#ifndef STPM3X_INTERNALS_H
#define STPM3X_INTERNALS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @name   DSP Control registers
  * @brief  32 bits registers, datasheet p.87-89, read-write
  * @{
  */
#define STPM3X_REG_DSP_CR1                            (0x00) /* DSP control register #1 */
#define STPM3X_MASK_CLRSS_TO1                         (0x0F)
#define STPM3X_MASK_CLEARSS                           (0x10)
#define STPM3X_MASK_ENVREF1                           (0x20)
#define STPM3X_MASK_TC1                               (0x1C0)
#define STPM3X_MASK_AEM1                              (0x20000)
#define STPM3X_MASK_APM1                              (0x40000)
#define STPM3X_MASK_BHPFV1                            (0x80000)
#define STPM3X_MASK_BHPFC1                            (0x100000)
#define STPM3X_MASK_ROC1                              (0x200000)
#define STPM3X_MASK_LPW1                              (0xF000000)
#define STPM3X_MASK_LPS1                              (0x30000000)
#define STPM3X_MASK_LCS1                              (0xC0000000)

#define STPM3X_REG_DSP_CR2                            (0x02) /* DSP control register #2 */
#define STPM3X_MASK_CLRSS_TO2                         (0x0F)
#define STPM3X_MASK_ENVREF2                           (0x20)
#define STPM3X_MASK_TC2                               (0x1C0)
#define STPM3X_MASK_AEM2                              (0x20000)
#define STPM3X_MASK_APM2                              (0x40000)
#define STPM3X_MASK_BHPFV2                            (0x80000)
#define STPM3X_MASK_BHPFC2                            (0x100000)
#define STPM3X_MASK_ROC2                              (0x200000)
#define STPM3X_MASK_LPW2                              (0xF000000)
#define STPM3X_MASK_LPS2                              (0x30000000)
#define STPM3X_MASK_LCS2                              (0xC0000000)

#define STPM3X_REG_DSP_CR3                            (0x04) /* DSP control register #3 */
#define STPM3X_MASK_SAG_TIME_THR                      (0x3FFF)
#define STPM3X_MASK_ZCR_SEL                           (0xC000)
#define STPM3X_MASK_ZCR_EN                            (0x10000)
#define STPM3X_MASK_TMP_TOL                           (0x60000)
#define STPM3X_MASK_TMP_EN                            (0x80000)
#define STPM3X_MASK_SW_RESET                          (0x100000)
#define STPM3X_MASK_SW_LATCH1                         (0x200000)
#define STPM3X_MASK_SW_LATCH2                         (0x400000)
#define STPM3X_MASK_SW_AUTOLATCH                      (0x800000)
#define STPM3X_MASK_LED_OFF1                          (0x1000000)
#define STPM3X_MASK_LED_OFF2                          (0x2000000)
#define STPM3X_MASK_EN_CUM                            (0x4000000)
#define STPM3X_MASK_REF_FREQ                          (0x8000000)

#define STPM3X_REG_DSP_CR4                            (0x06) /* DSP control register #4 */
#define STPM3X_MASK_PHC2                              (0x3FF)
#define STPM3X_MASK_PHV2                              (0xC00)
#define STPM3X_MASK_PHC1                              (0x3FF000)
#define STPM3X_MASK_PHV1                              (0xC00000)

#define STPM3X_REG_DSP_CR5                            (0x08) /* DSP control register #5 */
#define STPM3X_MASK_CHV1                              (0xFFF)
#define STPM3X_MASK_SWV_THR1                          (0x3FF000)
#define STPM3X_MASK_SAG_THR1                          (0xFFC00000)

#define STPM3X_REG_DSP_CR6                            (0x0A) /* DSP control register #6 */
#define STPM3X_MASK_CHC1                              (0xFFF)
#define STPM3X_MASK_SWC_THR1                          (0x3FF000)

#define STPM3X_REG_DSP_CR7                            (0x0C) /* DSP control register #7 */
#define STPM3X_MASK_CHC1                              (0xFFF)
#define STPM3X_MASK_SWC_THR1                          (0x3FF000)
#define STPM3X_MASK_SAG_THR2                          (0xFFC00000)

#define STPM3X_REG_DSP_CR8                            (0x0E) /* DSP control register #8 */
#define STPM3X_MASK_CHC2                              (0xFFF)
#define STPM3X_MASK_SWC_THR2                          (0x3FF000)

#define STPM3X_REG_DSP_CR9                            (0x10) /* DSP control register #9 */
#define STPM3X_MASK_AH_UP1                            (0xFFF)
#define STPM3X_MASK_OFA1                              (0x3FF000)
#define STPM3X_MASK_OFAF1                             (0xFFC00000)

#define STPM3X_REG_DSP_CR10                           (0x12) /* DSP control register #10 */
#define STPM3X_MASK_AH_DOWN1                          (0xFFF)
#define STPM3X_MASK_OFR1                              (0x3FF000)
#define STPM3X_MASK_OFS1                              (0xFFC00000)

#define STPM3X_REG_DSP_CR11                           (0x14) /* DSP control register #11 */
#define STPM3X_MASK_AH_UP2                            (0xFFF)
#define STPM3X_MASK_OFA2                              (0x3FF000)
#define STPM3X_MASK_OFAF2                             (0xFFC00000)

#define STPM3X_REG_DSP_CR12                           (0x16) /* DSP control register #12 */
#define STPM3X_MASK_AH_DOWN2                          (0xFFF)
#define STPM3X_MASK_OFR2                              (0x3FF000)
#define STPM3X_MASK_OFS2                              (0xFFC00000)
/** @} */

/**
  * @name   DFE Control registers
  * @brief  32 bits registers, datasheet p.89, read-write
  * @{
  */
#define STPM3X_REG_DFE_CR1                            (0x18) /* DFE control register #1 */
#define STPM3X_MASK_ENV1                              (0x1)
#define STPM3X_MASK_ENC1                              (0x10000)
#define STPM3X_MASK_GAIN1                             (0xC000000)

#define STPM3X_REG_DFE_CR2                            (0x1A) /* DFE control register #2 */
#define STPM3X_MASK_ENV2                              (0x1)
#define STPM3X_MASK_ENC2                              (0x10000)
#define STPM3X_MASK_GAIN2                             (0xC000000)
/** @} */

/**
  * @name   DSP IRQ Control registers
  * @brief  32 bits registers, datasheet p.90, read-write-latch
  * @{
  */
#define STPM3X_REG_DSP_IRQ1                           (0x1C) /* DSP IRQ (Interrupt Control Mask) register #1 */
#define STPM3X_MASK_IRQ1_PH1PH2_IRQ_CR                (0xF)
#define STPM3X_MASK_IRQ1_PH2_IRQ_CR                   (0xFF0)
#define STPM3X_MASK_IRQ1_PH1_IRQ_CR                   (0xFF000)
#define STPM3X_MASK_IRQ1_C1_IRQ_CR                    (0xF00000)
#define STPM3X_MASK_IRQ1_V1_IRQ_CR                    (0xFF000000)

#define STPM3X_REG_DSP_IRQ2                           (0x1E) /* DSP IRQ (Interrupt Control Mask) register #2 */
#define STPM3X_MASK_IRQ2_PH1PH2_IRQ_CR                (0xF)
#define STPM3X_MASK_IRQ2_PH2_IRQ_CR                   (0xFF0)
#define STPM3X_MASK_IRQ2_PH1_IRQ_CR                   (0xFF000)
#define STPM3X_MASK_IRQ2_C2_IRQ_CR                    (0xF00000)
#define STPM3X_MASK_IRQ2_V2_IRQ_CR                    (0xFF000000)
/** @} */

/**
  * @name   DSP Status registers
  * @brief  32 bits registers, datasheet p.91-92, read-write-latch
  * @brief  A = Active, F = Fundamental, R = Reactive, A = Apparent
  * @{
  */
#define STPM3X_REG_DSP_SR1                            (0x20) /* DSP Status Register #1 */
#define STPM3X_REG_DSP_SR2                            (0x22) /* DSP Status Register #2 */
#define STPM3X_MASK_SR_PH1PH2_POWER_SIGN_A            (0x1)  /* Same masks for both registers */
#define STPM3X_MASK_SR_PH1PH2_POWER_SIGN_R            (0x2)
#define STPM3X_MASK_SR_PH1PH2_ENERGY_OVERFLOW_A       (0x4)
#define STPM3X_MASK_SR_PH1PH2_ENERGY_OVERFLOW_R       (0x8)
#define STPM3X_MASK_SR_PH2_POWER_SIGN_A               (0x10)
#define STPM3X_MASK_SR_PH2_POWER_SIGN_F               (0x20)
#define STPM3X_MASK_SR_PH2_POWER_SIGN_R               (0x40)
#define STPM3X_MASK_SR_PH2_POWER_SIGN_S               (0x80)
#define STPM3X_MASK_SR_PH2_ENERGY_OVERFLOW_A          (0x100)
#define STPM3X_MASK_SR_PH2_ENERGY_OVERFLOW_F          (0x200)
#define STPM3X_MASK_SR_PH2_ENERGY_OVERFLOW_R          (0x400)
#define STPM3X_MASK_SR_PH2_ENERGY_OVERFLOW_S          (0x800)
#define STPM3X_MASK_SR_PH1_POWER_SIGN_A               (0x1000)
#define STPM3X_MASK_SR_PH1_POWER_SIGN_F               (0x2000)
#define STPM3X_MASK_SR_PH1_POWER_SIGN_R               (0x4000)
#define STPM3X_MASK_SR_PH1_POWER_SIGN_S               (0x8000)
#define STPM3X_MASK_SR_PH1_ENERGY_OVERFLOW_A          (0x10000)
#define STPM3X_MASK_SR_PH1_ENERGY_OVERFLOW_F          (0x20000)
#define STPM3X_MASK_SR_PH1_ENERGY_OVERFLOW_R          (0x40000)
#define STPM3X_MASK_SR_PH1_ENERGY_OVERFLOW_S          (0x80000)
#define STPM3X_MASK_SR_C1_SIGNAL_STUCK                (0x100000)
#define STPM3X_MASK_SR_C1_NAH                         (0x200000)
#define STPM3X_MASK_SR_C1_SWELL_START                 (0x400000)
#define STPM3X_MASK_SR_C1_SWELL_END                   (0x800000)
#define STPM3X_MASK_SR_V1_SIGNAL_STUCK                (0x1000000)
#define STPM3X_MASK_SR_V1_PER_ERR                     (0x2000000)
#define STPM3X_MASK_SR_V1_SAG_START                   (0x4000000)
#define STPM3X_MASK_SR_V1_SAG_END                     (0x8000000)
#define STPM3X_MASK_SR_V1_SWELL_START                 (0x10000000)
#define STPM3X_MASK_SR_V1_SWELL_END                   (0x20000000)
#define STPM3X_MASK_SR_V1_PH1_TAMPER                  (0x40000000)
#define STPM3X_MASK_SR_V1_TAMPER_OR_WRONG             (0x80000000)
/** @} */

/**
  * @name   UART & SPI Control registers
  * @brief  32 bits registers, datasheet p.92-93, read-write
  * @{
  */
#define STPM3X_REG_US_REG1                            (0x24) /* UART & SPI Control Register #1 */
#define STPM3X_MASK_CRC_POLYNOMIAL                    (0xFF)
#define STPM3X_MASK_NOISE_EN                          (0x100)
#define STPM3X_MASK_BREAK_ON_ERR                      (0x200)
#define STPM3X_MASK_CRC_EN                            (0x4000)
#define STPM3X_MASK_LSB_FIRST                         (0x8000)
#define STPM3X_MASK_TIME_OUT                          (0xFF0000)

#define STPM3X_REG_US_REG2                            (0x26) /* UART & SPI Control Register #2 */
#define STPM3X_MASK_BAUD_RATE                         (0xFFFF)
#define STPM3X_MASK_FRAME_DELAY                       (0xFF0000)

#define STPM3X_REG_US_REG3                            (0x28) /* UART & SPI IRQ Register */
#define STPM3X_MASK_CR_CRC_ERR1                       (0x2)
#define STPM3X_MASK_CR_TIMEOUT_ERR                    (0x4)
#define STPM3X_MASK_CR_FRAME_ERR                      (0x8)
#define STPM3X_MASK_CR_NOISE_ERR                      (0x10)
#define STPM3X_MASK_CR_RX_OVR                         (0x20)
#define STPM3X_MASK_CR_TX_OVR                         (0x40)
#define STPM3X_MASK_CR_UNDERRUN                       (0x400)
#define STPM3X_MASK_CR_READ_ERR                       (0x800)
#define STPM3X_MASK_CR_WRTIE_ERR                      (0x1000)
#define STPM3X_MASK_CR_CRC_ERR2                       (0x2000)
#define STPM3X_MASK_CR_OVERRUN                        (0x4000)
#define STPM3X_MASK_STATUS_BREAK                      (0x10000)
#define STPM3X_MASK_STATUS_CRC_ERR1                   (0x20000)
#define STPM3X_MASK_STATUS_TIMEOUT_ERR                (0x40000)
#define STPM3X_MASK_STATUS_FRAME_ERR                  (0x80000)
#define STPM3X_MASK_STATUS_NOISE_ERR                  (0x100000)
#define STPM3X_MASK_STATUS_RX_OVR                     (0x200000)
#define STPM3X_MASK_STATUS_TX_OVR                     (0x400000)
#define STPM3X_MASK_STATUS_RX_FULL                    (0x1000000)
#define STPM3X_MASK_STATUS_TX_EMPTY                   (0x2000000)
#define STPM3X_MASK_STATUS_READ_ERR                   (0x4000000)
#define STPM3X_MASK_STATUS_WRITE_ERR                  (0x8000000)
#define STPM3X_MASK_STATUS_CRC_ERR2                   (0x10000000)
#define STPM3X_MASK_STATUS_UNDERRUN                   (0x20000000)
#define STPM3X_MASK_STATUS_OVERRUN                    (0x40000000)
/** @} */

/**
  * @name   DSP live event registers
  * @brief  32 bits registers, datasheet p.94-95, read-latch
  * @{
  */
#define STPM3X_REG_DSP_EV1                            (0x2A) /* DSP Live Events #1 */
#define STPM3X_MASK_EV1_PH1PH2_POWER_SIGN_A           (0x1)
#define STPM3X_MASK_EV1_PH1PH2_POWER_SIGN_R           (0x2)
#define STPM3X_MASK_EV1_PH1PH2_ENERGY_OVERFLOW_A      (0x4)
#define STPM3X_MASK_EV1_PH1PH2_ENERGY_OVERFLOW_R      (0x8)
#define STPM3X_MASK_EV1_PH1_POWER_SIGN_A              (0x10)
#define STPM3X_MASK_EV1_PH1_POWER_SIGN_F              (0x20)
#define STPM3X_MASK_EV1_PH1_POWER_SIGN_R              (0x40)
#define STPM3X_MASK_EV1_PH1_POWER_SIGN_S              (0x80)
#define STPM3X_MASK_EV1_PH1_ENERGY_OVERFLOW_A         (0x100)
#define STPM3X_MASK_EV1_PH1_ENERGY_OVERFLOW_F         (0x200)
#define STPM3X_MASK_EV1_PH1_ENERGY_OVERFLOW_R         (0x400)
#define STPM3X_MASK_EV1_PH1_ENERGY_OVERFLOW_S         (0x800)
#define STPM3X_MASK_EV1_C1_ZCR                        (0x1000)
#define STPM3X_MASK_EV1_C1_SIGNAL_STUCK               (0x2000)
#define STPM3X_MASK_EV1_C1_NAH                        (0x4000)
#define STPM3X_MASK_EV1_C1_SWC1_EV                    (0x78000)
#define STPM3X_MASK_EV1_V1_ZCR                        (0x80000)
#define STPM3X_MASK_EV1_V1_SIGNAL_STUCK               (0x100000)
#define STPM3X_MASK_EV1_V1_PER_ERR                    (0x200000)
#define STPM3X_MASK_EV1_V1_SWV1_EV                    (0x3C00000)
#define STPM3X_MASK_EV1_V1_SAG1_EV                    (0x3C000000)

#define STPM3X_REG_DSP_EV2                            (0x2C) /* DSP Live Events #2 */
#define STPM3X_MASK_EV1_PH1PH2_POWER_SIGN_A           (0x1)
#define STPM3X_MASK_EV1_PH1PH2_POWER_SIGN_R           (0x2)
#define STPM3X_MASK_EV1_PH1PH2_ENERGY_OVERFLOW_A      (0x4)
#define STPM3X_MASK_EV1_PH1PH2_ENERGY_OVERFLOW_R      (0x8)
#define STPM3X_MASK_EV1_PH2_POWER_SIGN_A              (0x10)
#define STPM3X_MASK_EV1_PH2_POWER_SIGN_F              (0x20)
#define STPM3X_MASK_EV1_PH2_POWER_SIGN_R              (0x40)
#define STPM3X_MASK_EV1_PH2_POWER_SIGN_S              (0x80)
#define STPM3X_MASK_EV1_PH2_ENERGY_OVERFLOW_A         (0x100)
#define STPM3X_MASK_EV1_PH2_ENERGY_OVERFLOW_F         (0x200)
#define STPM3X_MASK_EV1_PH2_ENERGY_OVERFLOW_R         (0x400)
#define STPM3X_MASK_EV1_PH2_ENERGY_OVERFLOW_S         (0x800)
#define STPM3X_MASK_EV1_C2_ZCR                        (0x1000)
#define STPM3X_MASK_EV1_C2_SIGNAL_STUCK               (0x2000)
#define STPM3X_MASK_EV1_C2_NAH                        (0x4000)
#define STPM3X_MASK_EV1_C2_SWC2_EV                    (0x78000)
#define STPM3X_MASK_EV1_V2_ZCR                        (0x80000)
#define STPM3X_MASK_EV1_V2_SIGNAL_STUCK               (0x100000)
#define STPM3X_MASK_EV1_V2_PER_ERR                    (0x200000)
#define STPM3X_MASK_EV1_V2_SWV2_EV                    (0x3C00000)
#define STPM3X_MASK_EV1_V2_SAG2_EV                    (0x3C000000)
/** @} */

/**
  * @name   DSP Data
  * @brief  32 bits registers, datasheet p.95-99, read-latch
  * @{
  */
#define STPM3X_REG_DSP_REG1                           (0x2E) /* PH1 & PH2 Period */
#define STPM3X_MASK_PH1_PERIOD                        (0xFFF)
#define STPM3X_MASK_PH2_PERIOD                        (0xFFF0000)

#define STPM3X_REG_DSP_REG2                           (0x30) /* V1 Data */
#define STPM3X_MASK_V1_DATA                           (0xFFFFFF)

#define STPM3X_REG_DSP_REG3                           (0x32) /* C1 Data */
#define STPM3X_MASK_C1_DATA                           (0xFFFFFF)

#define STPM3X_REG_DSP_REG4                           (0x34) /* V2 Data */
#define STPM3X_MASK_V2_DATA                           (0xFFFFFF)

#define STPM3X_REG_DSP_REG5                           (0x36) /* C2 Data */
#define STPM3X_MASK_C2_DATA                           (0xFFFFFF)

#define STPM3X_REG_DSP_REG6                           (0x38) /* V1 Fund */
#define STPM3X_MASK_V1_FUND                           (0xFFFFFF)

#define STPM3X_REG_DSP_REG7                           (0x3A) /* C1 Fund */
#define STPM3X_MASK_C1_FUND                           (0xFFFFFF)

#define STPM3X_REG_DSP_REG8                           (0x3C) /* V2 Fund */
#define STPM3X_MASK_V2_FUND                           (0xFFFFFF)

#define STPM3X_REG_DSP_REG9                           (0x3E) /* C2 Fund */
#define STPM3X_MASK_C2_FUND                           (0xFFFFFF)

#define STPM3X_REG_DSP_REG14                          (0x48) /* V1 & C1 RMS Data */
#define STPM3X_MASK_V1_RMS_DATA                       (0x7FFF)
#define STPM3X_MASK_C1_RMS_DATA                       (0xFFFF8000)

#define STPM3X_REG_DSP_REG15                          (0x4A) /* V2 & C2 RMS Data */
#define STPM3X_MASK_V2_RMS_DATA                       (0x7FFF)
#define STPM3X_MASK_C2_RMS_DATA                       (0xFFFF8000)

#define STPM3X_REG_DSP_REG16                          (0x4C) /* SWV1 & SAG1 Time */
#define STPM3X_MASK_SWV1_TIME                         (0x7FFF)
#define STPM3X_MASK_SAG1_TIME                         (0x7FFF0000)

#define STPM3X_REG_DSP_REG17                          (0x4E) /* C1 Phase / SWC1 Time */
#define STPM3X_MASK_SWC1_TIME                         (0x7FFF)
#define STPM3X_MASK_C1_PHA                            (0xFFF0000)

#define STPM3X_REG_DSP_REG18                          (0x50) /* SWV2 & SAG2 Time */
#define STPM3X_MASK_SWV2_TIME                         (0x7FFF)
#define STPM3X_MASK_SAG2_TIME                         (0x7FFF0000)

#define STPM3X_REG_DSP_REG19                          (0x52) /* SWC2 Time / C2 Phase */
#define STPM3X_MASK_SWC2_TIME                         (0x7FFF)
#define STPM3X_MASK_C2_PHA                            (0xFFF0000)

#define STPM3X_REG_PH1_REG1                           (0x54) /* PH1 Active Energy */
#define STPM3X_MASK_PH1_ACTIVE_ENERGY                 (0xFFFFFFFF)

#define STPM3X_REG_PH1_REG2                           (0x56) /* PH1 Fundamental Energy */
#define STPM3X_MASK_PH1_FUNDAMENTAL_ENERGY            (0xFFFFFFFF)

#define STPM3X_REG_PH1_REG3                           (0x58) /* PH1 Reactie Energy */
#define STPM3X_MASK_PH1_REACTIVE_ENERGY               (0xFFFFFFFF)

#define STPM3X_REG_PH1_REG4                           (0x5A) /* PH1 Apparent Energy */
#define STPM3X_MASK_PH1_APPARENT_ENERGY               (0xFFFFFFFF)

#define STPM3X_REG_PH1_REG5                           (0x5C) /* PH1 Active Power */
#define STPM3X_MASK_PH1_ACTIVE_POWER                  (0x1FFFFFFF)

#define STPM3X_REG_PH1_REG6                           (0x5E) /* PH1 Fundamental Power */
#define STPM3X_MASK_PH1_FUNDAMENTAL_POWER             (0x1FFFFFFF)

#define STPM3X_REG_PH1_REG7                           (0x60) /* PH1 Reactive Power */
#define STPM3X_MASK_PH1_REACTIVE_POWER                (0x1FFFFFFF)

#define STPM3X_REG_PH1_REG8                           (0x62) /* PH1 Apparent RMS Power */
#define STPM3X_MASK_PH1_APPARENT_RMS_POWER            (0x1FFFFFFF)

#define STPM3X_REG_PH1_REG9                           (0x64) /* PH1 Apparent Vectorial Power */
#define STPM3X_MASK_PH1_APPARENT_VECTORIAL_POWER      (0x1FFFFFFF)

#define STPM3X_REG_PH1_REG10                          (0x66) /* PH1 Momentary Active Power */
#define STPM3X_MASK_PH1_MOMENTARY_ACTIVE_POWER        (0x1FFFFFFF)

#define STPM3X_REG_PH1_REG11                          (0x68) /* PH1 Momentary Fundamental Power */
#define STPM3X_MASK_PH1_MOMENTARY_FUNDAMENTAL_POWER   (0x1FFFFFFF)

#define STPM3X_REG_PH1_REG12                          (0x6A) /* PH1 AH_ACC */
#define STPM3X_MASK_PH1_AH_ACC                        (0xFFFFFFFF)

#define STPM3X_REG_PH2_REG1                           (0x6C) /* PH2 Active Energy */
#define STPM3X_MASK_PH2_ACTIVE_ENERGY                 (0xFFFFFFFF)

#define STPM3X_REG_PH2_REG2                           (0x6E) /* PH2 Fundamental Energy */
#define STPM3X_MASK_PH2_FUNDAMENTAL_ENERGY            (0xFFFFFFFF)

#define STPM3X_REG_PH2_REG3                           (0x70) /* PH2 Reactie Energy */
#define STPM3X_MASK_PH2_REACTIVE_ENERGY               (0xFFFFFFFF)

#define STPM3X_REG_PH2_REG4                           (0x72) /* PH2 Apparent Energy */
#define STPM3X_MASK_PH2_REACTIVE_ENERGY               (0xFFFFFFFF)

#define STPM3X_REG_PH2_REG5                           (0x74) /* PH2 Active Power */
#define STPM3X_MASK_PH2_ACTIVE_POWER                  (0x1FFFFFFF)

#define STPM3X_REG_PH2_REG6                           (0x76) /* PH2 Fundamental Power */
#define STPM3X_MASK_PH2_FUNDAMENTAL_POWER             (0x1FFFFFFF)

#define STPM3X_REG_PH2_REG7                           (0x78) /* PH2 Reactive Power */
#define STPM3X_MASK_PH2_REACTIVE_POWER                (0x1FFFFFFF)

#define STPM3X_REG_PH2_REG8                           (0x7A) /* PH2 Apparent RMS Power */
#define STPM3X_MASK_PH2_APPARENT_RMS_POWER            (0x1FFFFFFF)

#define STPM3X_REG_PH2_REG9                           (0x7C) /* PH2 Apparent Vectorial Power */
#define STPM3X_MASK_PH2_APPARENT_VECTORIAL_POWER      (0x1FFFFFFF)

#define STPM3X_REG_PH2_REG10                          (0x7E) /* PH2 Momentary Active Power */
#define STPM3X_MASK_PH2_MOMENTARY_ACTIVE_POWER        (0x1FFFFFFF)

#define STPM3X_REG_PH2_REG11                          (0x80) /* PH2 Momentary Fundamental Power */
#define STPM3X_MASK_PH2_MOMENTARY_FUNDAMENTAL_POWER   (0x1FFFFFFF)

#define STPM3X_REG_PH2_REG12                          (0x82) /* PH2 AH_ACC */
#define STPM3X_MASK_PH2_AH_ACC                        (0xFFFFFFFF)

#define STPM3X_REG_TOT_ACTIVE_ENERGY                  (0x84) /* Total Active Energy */
#define STPM3X_MASK_TOT_ACTIVE_ENERGY                 (0xFFFFFFFF)

#define STPM3X_REG_TOT_FUNDAMENTAL_ENERGY             (0x86) /* Total Fundamental Energy */
#define STPM3X_MASK_TOT_FUNDAMENTAL_ENERGY            (0xFFFFFFFF)

#define STPM3X_REG_TOT_REACTIVE_ENERGY                (0x88) /* Total Reactive Energy */
#define STPM3X_MASK_TOT_REACTIVE_ENERGY               (0xFFFFFFFF)

#define STPM3X_REG_TOT_APPARENT_ENERGY                (0x8A) /* Total Appartent Energy */
#define STPM3X_MASK_TOT_APPARENT_ENERGY               (0xFFFFFFFF)
/** @} */

/**
  * @brief   Timing definition for STPM3X
  *
  * From Datasheet p.19-20 of 'Getting started with STPM3x' p.6, in [us] unless specified
  *
  */
#define STPM3X_T_EN_MIN             (50U)     // ns
#define STPM3X_T_EN_CUST            (1U)
#define STPM3X_T_CLK_MIN            (50U)     // ns
#define STPM3X_T_CPW_MIN            (25U)     // ns
#define STPM3X_T_SETUP_MIN          (10U)     // ns
#define STPM3X_T_HOLD_MIN           (40U)     // ns
#define STPM3X_T_LTCH_MIN           (20U)     // ns
#define STPM3X_T_LPW_MIN            (4U)
#define STPM3X_T_W_MIN              (4U)
#define STPM3X_T_RPW_MIN            (4U)
#define STPM3X_T_RPW_TYP            (1000U)
#define STPM3X_T_REL_MIN            (40U)     // ns
#define STPM3X_T_STARTUP_MIN        (35000U)
#define STPM3X_T_STARTUP_TYP        (35000U)
#define STPM3X_T_IF_MIN             (4U)
#define STPM3X_T_IF_TYP             (10000U)
#define STPM3X_T_SCS_MIN            (4U)
#define STPM3X_T_SCS_CUST           (50U)
#define STPM3X_T_SCS_TYP            (1000U)
#define STPM3X_T_READY_WAIT         (1000000U)

/**
  * @brief   Constants for CRC generation
  *
  * From 'Getting started with the STPM3x', p.15
  */
#define STPM3X_CRC_8                (0x07)
#define STPM3X_FRAME_LEN            (5U)

/**
  * @brief   Size of data to write in the SPI bus
  *
  * From 'Getting started with the STPM3x', p.13
  */
#define STPM3X_DATA_SIZE            (10U)
#define STPM3X_DATA_SIZE_STEP       (STPM3X_DATA_SIZE / 2)

#ifdef __cplusplus
}
#endif

#endif /* STPM3X_INTERNALS_H */
/** @} */
