#ifndef __MAIN_H__
#define __MAIN_H__
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "appconfig.h"
#include "drivers.h"
#include "fraclib.h"
#include "meterlib.h"

#include "system_config.h"
#include "smarthome_meter.h"
#include "smarthome_cfg.h"
#include "i2ceeprom.h"
#include "comm.h"
#include "lib/data_cmp.h"
#include "lib/ampm_sprintf.h"
#include "lib/protocol.h"
#include "app_config_task.h"
#include "app_rtc.h"
#include "auto_calib.h"
// #include "relay.h"

#define ROLL_BACK 999999999
#define PHASE_CONVERT_PULSE_TO_ENERGY 100

/***************************************************************************//*!
 * @brief   Voltage reference switch setting.
 * @details Configures voltage reference switches:
 *          - Buffer enable,
 *          - Buffer drive voltage on PAD,   S1 = H,
 *          - Internal reference select,     S2 = L,
 *          - AFE Internal reference select, S3 = L,
 * @details The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 ******************************************************************************/
#define VREF_SWITCH_S1_H_S2_L_S3_L                                             \
(tVREF_S){                                                                     \
/* SIM_MISC  */  CLR(SIM_MISC_CTL_VREFBUFPD_MASK)|                             \
/* ......... */  CLR(SIM_MISC_CTL_VREFBUFINSEL_MASK)|                          \
/* ......... */  SET(SIM_MISC_CTL_VREFBUFOUTEN_MASK),                          \
/* VERFL_TRM */  CLR(VREF_VREFL_TRM_VREFL_SEL_MASK),                           \
}

/**************************************************************************/ /*!
 * @brief Selects 16-bit hardware triggered mode with internal voltage reference.
 *****************************************************************************/
#define ADC_MODULE_DIV4_16B_HWTRG_IREF_CONFIG                                 \
(tADC){                                                                       \
/* CFG1   */  CLR(ADC_CFG1_ADLPC_MASK)|SET(ADC_CFG1_ADIV(2))|                 \
              SET(ADC_CFG1_ADLSMP_MASK)|SET(ADC_CFG1_MODE(3))|                \
              SET(ADC_CFG1_ADICLK(0)),                                        \
/* CFG2   */  CLR(ADC_CFG2_MUXSEL_MASK)|CLR(ADC_CFG2_ADACKEN_MASK)|           \
              SET(ADC_CFG2_ADHSC_MASK)|SET(ADC_CFG2_ADLSTS(0)),               \
/* CV1    */  0l,                                                             \
/* CV2    */  0l,                                                             \
/* SC2    */  SET(ADC_SC2_ADTRG_MASK)|CLR(ADC_SC2_ACFE_MASK)|                 \
              CLR(ADC_SC2_ACFGT_MASK)|CLR(ADC_SC2_ACREN_MASK)|                \
              CLR(ADC_SC2_DMAEN_MASK)|SET(ADC_SC2_REFSEL(1)),                 \
/* SC3    */  CLR(ADC_SC3_CAL_MASK)|CLR(ADC_SC3_ADCO_MASK)|                   \
              CLR(ADC_SC3_AVGE_MASK)|SET(ADC_SC3_AVGS(0)),                    \
}

/***************************************************************************//*!
 * @brief   Timer Pulse Output Control One Shoot Mode
 * @details This use case will configure the Quad Timer channel in one shoot 
 * counting mode with the following initial characteristics:
 * - Counter operation disabled by default (will be controlled by 
 *   @ref TMR_SetCountMode() macro) 
 * - Primary count source = IP bus clock divide by 1 prescaler
 * - Secondary count source = passed by sec_clk_src input argument
 * - Count until compare and then stop
 * - Count until compare, then re-initialize
 * - Count up
 * - Co-channel counter/timers cannot force a Timer/counter re-initialization
 * - Output Mode = passed by output_mode input argument
 * - All interrupts disabled
 * - True polarity of input signal
 * - Capture function is disabled
 * - External OFLAG force enabled and logic 1 is forced to OFLAG output pin
 * - True output polarity
 * - OFLAG output signal is driven on the external pin.
 * - Continue with normal operation during debug mode (default)
 * - Fault function disabled
 * - Counter is re-initialized only with the LOAD register
 * - Do not reload the counter on a capture event
 * - Timer Compare 1 and Compare 2 interrupt disabled
 * - COMP1 and CMP2 are never preloaded
 * @param   sec_clk_src     Use one of the following @ref tmr_sec_src 
 * @param   output_mode     Use one of the following @ref tmr_out_mode 
 *****************************************************************************/
#define TMR_PULSE_OUTPUT_ONE_SHOOT_MODE_CONFIG(sec_clk_src,output_mode)       \
(tTMR_CH){                                                                    \
/* CTRL     */  SET(TMR_CTRL_CM(COUNT_DISABLED))|                             \
                SET(TMR_CTRL_PCS(BUS_CLK_DIV1))|                              \
                SET(TMR_CTRL_SCS(sec_clk_src))|                               \
                CLR(TMR_CTRL_DIR_MASK)|SET(TMR_CTRL_LENGTH_MASK)|             \
                SET(TMR_CTRL_ONCE_MASK)|CLR(TMR_CTRL_COINIT_MASK)|            \
                SET(TMR_CTRL_OUTMODE(output_mode)),                           \
/* SCTRL    */  CLR(TMR_SCTRL_TCFIE_MASK)|CLR(TMR_SCTRL_TOFIE_MASK)|          \
                CLR(TMR_SCTRL_IEFIE_MASK)|CLR(TMR_SCTRL_IPS_MASK)|            \
                SET(TMR_SCTRL_CAPTURE_MODE(CAPTURE_DISABLED))|                \
                CLR(TMR_SCTRL_MSTR_MASK)|CLR(TMR_SCTRL_EEOF_MASK)|            \
                SET(TMR_SCTRL_VAL_MASK)|CLR(TMR_SCTRL_TOF_MASK)|              \
                CLR(TMR_SCTRL_OPS_MASK)|SET(TMR_SCTRL_OEN_MASK)|              \
                SET(TMR_SCTRL_FORCE_MASK),                                    \
 /* CSCTRL  */  SET(TMR_CSCTRL_DBG_EN(0))|CLR(TMR_CSCTRL_FAULT_MASK)|         \
                CLR(TMR_CSCTRL_ALT_LOAD_MASK)|CLR(TMR_CSCTRL_ROC_MASK)|       \
                CLR(TMR_CSCTRL_TCI_MASK)|CLR(TMR_CSCTRL_TCF2EN_MASK)|         \
                CLR(TMR_CSCTRL_TCF1EN_MASK)|SET(TMR_CSCTRL_CL2(0))|           \
                SET(TMR_CSCTRL_CL1(0)),                                       \
}

/**************************************************************************/ /*!
 * @brief   Input Capture Bus Clock Mode to Low Power Oscillator Clock Setting
 * Other characteristics:
 * - Count rising edges
 * - Rollover enabled
 * - One shot Timer mode disabled
 * - Overflow interrupt disabled
 * - Co-channel initialization disabled
 * - Compare interrupt disabled
 * - Input capture interrupt disabled
 * - Input signal polarity not inverted
 * - Continue with normal operation during debug mode (default)
 * - Reload counter on capture event
 * @param   prm_clk_src     Use one of the following @ref tmr_prm_src
 * @param   sec_clk_src     Use one of the following @ref tmr_sec_src 
 * @param   input_mode      Use one of the following @ref tmr_capt_mode
 *****************************************************************************/
#define TMR_PHASE_ERROR_COMPENSATION_MODE(prm_clk_src,sec_clk_src,            \
                                                          input_mode)         \
(tTMR_CH){                                                                    \
/* CTRL   */    SET(TMR_CTRL_CM(COUNT_TRIGGERED))|                            \
                SET(TMR_CTRL_PCS(prm_clk_src))|SET(TMR_CTRL_SCS(sec_clk_src))|\
                CLR(TMR_CTRL_DIR_MASK)|SET(TMR_CTRL_LENGTH_MASK)|             \
                CLR(TMR_CTRL_ONCE_MASK)|CLR(TMR_CTRL_COINIT_MASK)|            \
                SET(TMR_CTRL_OUTMODE(SET_ON_COMP_CLR_ON_SEC_EDGE)),           \
/* SCTRL    */  CLR(TMR_SCTRL_TCFIE_MASK)|CLR(TMR_SCTRL_TOFIE_MASK)|          \
                CLR(TMR_SCTRL_IEFIE_MASK)|CLR(TMR_SCTRL_IPS_MASK)|            \
                SET(TMR_SCTRL_CAPTURE_MODE(input_mode))|                      \
                CLR(TMR_SCTRL_MSTR_MASK)|CLR(TMR_SCTRL_EEOF_MASK)|            \
                CLR(TMR_SCTRL_VAL_MASK)|CLR(TMR_SCTRL_TOF_MASK)|              \
                CLR(TMR_SCTRL_OPS_MASK)|CLR(TMR_SCTRL_OEN_MASK)|              \
                CLR(TMR_SCTRL_FORCE_MASK),                                    \
 /* CSCTRL  */  SET(TMR_CSCTRL_DBG_EN(0))|CLR(TMR_CSCTRL_FAULT_MASK)|         \
                CLR(TMR_CSCTRL_ALT_LOAD_MASK)|SET(TMR_CSCTRL_ROC_MASK)|       \
                CLR(TMR_CSCTRL_TCI_MASK)|CLR(TMR_CSCTRL_TCF2EN_MASK)|         \
                CLR(TMR_CSCTRL_TCF1EN_MASK)|SET(TMR_CSCTRL_CL2(0))|           \
                SET(TMR_CSCTRL_CL1(0)),                                       \
}

#define TMR_PHASE_RELAY_COMPENSATION_MODE(prm_clk_src,sec_clk_src,            \
                                                          input_mode)         \
(tTMR_CH){                                                                    \
/* CTRL   */    SET(TMR_CTRL_CM(COUNT_TRIGGERED))|                            \
                SET(TMR_CTRL_PCS(prm_clk_src))|SET(TMR_CTRL_SCS(sec_clk_src))|\
                CLR(TMR_CTRL_DIR_MASK)|SET(TMR_CTRL_LENGTH_MASK)|             \
                CLR(TMR_CTRL_ONCE_MASK)|CLR(TMR_CTRL_COINIT_MASK)|            \
                SET(TMR_CTRL_OUTMODE(SET_ON_COMP_CLR_ON_SEC_EDGE)),           \
/* SCTRL    */  SET(TMR_SCTRL_TCFIE_MASK)|CLR(TMR_SCTRL_TOFIE_MASK)|          \
                CLR(TMR_SCTRL_IEFIE_MASK)|CLR(TMR_SCTRL_IPS_MASK)|            \
                SET(TMR_SCTRL_CAPTURE_MODE(input_mode))|                      \
                CLR(TMR_SCTRL_MSTR_MASK)|CLR(TMR_SCTRL_EEOF_MASK)|            \
                CLR(TMR_SCTRL_VAL_MASK)|CLR(TMR_SCTRL_TOF_MASK)|              \
                CLR(TMR_SCTRL_OPS_MASK)|CLR(TMR_SCTRL_OEN_MASK)|              \
                CLR(TMR_SCTRL_FORCE_MASK),                                    \
 /* CSCTRL  */  SET(TMR_CSCTRL_DBG_EN(0))|CLR(TMR_CSCTRL_FAULT_MASK)|         \
                CLR(TMR_CSCTRL_ALT_LOAD_MASK)|SET(TMR_CSCTRL_ROC_MASK)|       \
                CLR(TMR_CSCTRL_TCI_MASK)|CLR(TMR_CSCTRL_TCF2EN_MASK)|         \
                CLR(TMR_CSCTRL_TCF1EN_MASK)|SET(TMR_CSCTRL_CL2(0))|           \
                SET(TMR_CSCTRL_CL1(0)),                                       \
}

/**************************************************************************/ /*!
  * @brief Comparator Frequency Detection Setting:
  * - filter disabled, 
  * - hysteresis 5mv,
  * - sample, windowing and trigger modes disabled,
  * - high speed mode,
  * - non-inverting polarity,
  * - unfiltered signal propagated to output (disabled),
  * - comparator enabled,
  * - CMP filter disabled when CR1[SE]=0, 
  * - DMA disabled,
  * - IRQ enabled
  * - DAC enabled.
  *****************************************************************************/
#define CMP_ZERO_DETECT_MODE_CONFIG_EN_DAC(dac)                               \
(tCMP){                                                                       \
/* CR0    */  SET(CMP_CR0_FILTER_CNT(0)) |SET(CMP_CR0_HYSTCTR(0)),            \
/* CR1    */  CLR(CMP_CR1_SE_MASK)       |CLR(CMP_CR1_WE_MASK)|               \
              CLR(CMP_CR1_TRIGM_MASK)    |CLR	(CMP_CR1_PMODE_MASK)|            \
              CLR(CMP_CR1_INV_MASK)      |CLR(CMP_CR1_COS_MASK)|              \
              CLR(CMP_CR1_OPE_MASK)      |SET(CMP_CR1_EN_MASK),               \
/* FPR    */  SET(CMP_FPR_FILT_PER(0x00)),                                    \
/* SCR    */  CLR(CMP_SCR_DMAEN_MASK)    |SET(CMP_SCR_IER_MASK)|              \
              CLR(CMP_SCR_IEF_MASK),                                          \
/* DACCR  */  SET(CMP_DACCR_DACEN_MASK)  |CLR(CMP_DACCR_VRSEL_MASK)| dac      \
}

#define DAILY_SAVE (32)
#define DAILY_BUFFER (DAILY_SAVE*NUM_CHAN+1)
#define ADCHAN AD8

/** @brief init peripheral in normal mode
 *  PLL 12.288Mhz -> AFE
 *  FLL 24Mhz
 *  VREF trim to 1.2V
 *  AFE 0-3 current input
 *  CMP0 zerocross detect
 *  AD8 Voltage measure
 */
void hw_norm_init(void);
/** @brief reset all daily data
 */
void reset_daily(void);
/** @brief read daily data from flash
 */
void read_daily(void);


/** @brief measurement callback
 */
void afeCallback(AFE_CH_CALLBACK_TYPE type, int32 result);
void displayCallback(void);
void dataSaveCallback(void);
void hardfault_isr(void);
/** @brief calculate callback
 */
void calcCallback(void);
void lptmr_callback(void);
void tmr_callback(TMR_CH_CALLBACK_TYPE type, int32 result);
#endif





