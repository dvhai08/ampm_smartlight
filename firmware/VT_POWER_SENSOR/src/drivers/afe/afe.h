/******************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      afe.h
 * @version   1.0.4.0
 * @date      Apr-27-2014
 * @brief     Analog Front End (AFE) driver header file.
 ******************************************************************************/
#ifndef __AFE_H
#define __AFE_H 

/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint32 CR, CKR;  } tAFE;
typedef struct { uint32 CFR;      } tAFE_CH;

/******************************************************************************
* AFE clock sources definitions used by AFE_Init() function
*
*//*! @addtogroup afe_clocks
* @{
*******************************************************************************/
#define AFE_IRC_CLK   (uint32)0x00  ///< MCG_IRC_CLK(2MHZ or 32KHz)
#define AFE_PLL_CLK   (uint32)0x01  ///< ICS_PLL(SIM_AFE_PLL_CLKSEL=00)\n
                                    ///  ICS_FLL(SIM_AFE_PLL_CLKSEL=01)\n
                                    ///  OSC_CLK(SIM_AFE_PLL_CLKSEL=10)\n
                                    ///  RESERVED(SIM_AFE_PLL_CLKSEL=10)
#define AFE_TMR0_CLK  (uint32)0x02  ///< TMR0_OUTPUT
#define AFE_EXT_CLK   (uint32)0x03  ///< AFE_CLK(routed from either PTB7 or PTE3 
                                    ///  according to pin muxing
/*! @} End of afe_clocks                                                      */

/******************************************************************************
* AFE clock divider definitions used by AFE_Init() function
*
*//*! @addtogroup afe_clkdiv
* @{
*******************************************************************************/
#define AFE_DIV1      (uint32)0x00   ///< divide by 1
#define AFE_DIV2      (uint32)0x01   ///< divide by 2 (default)
#define AFE_DIV4      (uint32)0x02   ///< divide by 4
#define AFE_DIV8      (uint32)0x03   ///< divide by 8
#define AFE_DIV16     (uint32)0x04   ///< divide by 16
#define AFE_DIV32     (uint32)0x05   ///< divide by 32
#define AFE_DIV64     (uint32)0x06   ///< divide by 64
#define AFE_DIV128    (uint32)0x07   ///< divide by 128
#define AFE_DIV256    (uint32)0x08   ///< divide by 256
/*! @} End of afe_clkdiv                                                      */

/******************************************************************************
* AFE clock divider definitions used by AFE_CH_Init() function. 
*
*//*! @addtogroup afe_osr
* @{
*******************************************************************************/
#define DEC_OSR64    (uint32)0x00  ///< f-3dB=25.15KHz,fn= 48KHz @ fs=6.144MHz
#define DEC_OSR128   (uint32)0x01  ///< f-3dB=12.58KHz,fn= 24KHz @ fs=6.144MHz
#define DEC_OSR256   (uint32)0x02  ///< f-3dB= 6.29KHz,fn= 12KHz @ fs=6.144MHz
#define DEC_OSR512   (uint32)0x03  ///< f-3dB= 3.14KHz,fn=  6KHz @ fs=6.144MHz
#define DEC_OSR1024  (uint32)0x04  ///< f-3dB= 1.75KHz,fn=  3KHz @ fs=6.144MHz
#define DEC_OSR2048  (uint32)0x05  ///< f-3dB= 0.79KHz,fn=1.5KHz @ fs=6.144MHz
/*! @} End of afe_osr                                                         */

/******************************************************************************
* AFE PGA gain definitions used by AFE_CH_Init() function. 
*
*//*! @addtogroup afe_gain
* @{
*******************************************************************************/
#define PGA_DISABLE   (uint32)0x07  ///< Disabled (off)
#define PGA_GAINLP    (uint32)0x00  ///< Gain 01x (low power operation)
#define PGA_GAIN1     (uint32)0x01  ///< Gain 01x 
#define PGA_GAIN2     (uint32)0x02  ///< Gain 02x
#define PGA_GAIN4     (uint32)0x03  ///< Gain 04x
#define PGA_GAIN8     (uint32)0x04  ///< Gain 08x
#define PGA_GAIN16    (uint32)0x05  ///< Gain 16x
#define PGA_GAIN32    (uint32)0x06  ///< Gain 32x
/*! @} End of afe_gain                                                        */

/******************************************************************************
* AFE channels mask definition used by AFE_SwTrigger() and AFE_Read() macros. 
*
*//*! @addtogroup afe_channel
* @{
*******************************************************************************/
#define CH0   (uint32)(1l << 1)     ///< Channel 0
#define CH1   (uint32)(1l << 2)     ///< Channel 1
#define CH2   (uint32)(1l << 3)     ///< Channel 2
#define CH3   (uint32)(1l << 4)     ///< Channel 3
/*! @} End of afe_channel                                                     */
                                                       
/******************************************************************************
* AFE default configurations used by AFE_Init() function
*
*//*! @addtogroup afe_module_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief Selects normal mode of operation with right justified 2's complement 
 * 32-bit output data format.
 * @details Selects normal mode of operation with right justified 2's complement 
 * 32-bit output data format.
 * @param   src       Select one of the following @ref afe_clocks
 * @param   div       Select one of the following @ref afe_clkdiv
 * @param   freq      Clock frequency in Hz (e.g. 12288000); this value is used 
 *                    by the precompiler to calculate value for the 
 *                    AFE_CR_STRTUP_CNT register field.
 * @showinitializer
 *****************************************************************************/
#define AFE_MODULE_RJFORMAT_CONFIG(src,div,freq)                              \
(tAFE){                                                                       \
/* CR   */  SET(AFE_CR_MSTR_EN_MASK)|CLR(AFE_CR_SOFT_TRG0_MASK)|              \
/* ..   */  CLR(AFE_CR_SOFT_TRG1_MASK)|CLR(AFE_CR_SOFT_TRG2_MASK)|            \
/* ..   */  CLR(AFE_CR_SOFT_TRG3_MASK)|CLR(AFE_CR_LPM_EN_MASK)|               \
/* ..   */  SET(AFE_CR_RST_B_MASK)|SET(AFE_CR_DLY_OK_MASK)|                   \
/* ..   */  SET(AFE_CR_RESULT_FORMAT_MASK)|                                   \
/* ..   */  SET(AFE_CR_STRTUP_CNT((uint8)((freq/(1<<div))*20e-6))),           \
/* CKR  */  AFE_CKR_DIV(div)|AFE_CKR_CLS(src),                                \
}
/***************************************************************************//*!
 * @brief Selects normal mode of operation with left justified 2's complement 
 * 32-bit output data format.
 * @details Selects normal mode of operation with left justified 2's complement 
 * 32-bit output data format.
 * @param   src       Select one of the following @ref afe_clocks
 * @param   div       Select one of the following @ref afe_clkdiv
 * @param   freq      Clock frequency in Hz (e.g. 12288000); this value is used 
 *                    by the precompiler to calculate value for the 
 *                    AFE_CR_STRTUP_CNT register field.
 * @showinitializer
 *****************************************************************************/
#define AFE_MODULE_LJFORMAT_CONFIG(src,div,freq)                              \
(tAFE){                                                                       \
/* CR   */  SET(AFE_CR_MSTR_EN_MASK)|CLR(AFE_CR_SOFT_TRG0_MASK)|              \
/* ..   */  CLR(AFE_CR_SOFT_TRG1_MASK)|CLR(AFE_CR_SOFT_TRG2_MASK)|            \
/* ..   */  CLR(AFE_CR_SOFT_TRG3_MASK)|CLR(AFE_CR_LPM_EN_MASK)|               \
/* ..   */  SET(AFE_CR_RST_B_MASK)|SET(AFE_CR_DLY_OK_MASK)|                   \
/* ..   */  CLR(AFE_CR_RESULT_FORMAT_MASK)|                                   \
/* ..   */  SET(AFE_CR_STRTUP_CNT((uint8)((freq/(1<<div))*20e-6))),           \
/* CKR  */  AFE_CKR_DIV(div)|AFE_CKR_CLS(src),                                \
}
/***************************************************************************//*!
 * @brief Selects low power mode of operation with right justified 2's complement 
 * 32-bit output data format.
 * @details Selects low power mode of operation with right justified 2's 
 * complement 32-bit output data format.
 * @param   src       Select one of the following @ref afe_clocks
 * @param   div       Select one of the following @ref afe_clkdiv
 * @param   freq      Clock frequency in Hz (e.g. 12288000); this value is used 
 *                    by the precompiler to calculate value for the 
 *                    AFE_CR_STRTUP_CNT register field.
 * @showinitializer
 *****************************************************************************/
#define AFE_MODULE_LPM_RJFORMAT_CONFIG(src,div,freq)                          \
(tAFE){                                                                       \
/* CR   */  SET(AFE_CR_MSTR_EN_MASK)|CLR(AFE_CR_SOFT_TRG0_MASK)|              \
/* ..   */  CLR(AFE_CR_SOFT_TRG1_MASK)|CLR(AFE_CR_SOFT_TRG2_MASK)|            \
/* ..   */  CLR(AFE_CR_SOFT_TRG3_MASK)|SET(AFE_CR_LPM_EN_MASK)|               \
/* ..   */  SET(AFE_CR_RST_B_MASK)|SET(AFE_CR_DLY_OK_MASK)|                   \
/* ..   */  SET(AFE_CR_RESULT_FORMAT_MASK)|                                   \
/* ..   */  SET(AFE_CR_STRTUP_CNT((uint8)((freq/(1<<div))*20e-6))),           \
/* CKR  */  AFE_CKR_DIV(div)|AFE_CKR_CLS(src),                                \
}
/***************************************************************************//*!
 * @brief Selects low power mode of operation with left justified 2's complement 
 * 32-bit output data format.
 * @details Selects low power mode of operation with left justified 2's 
 * complement 32-bit output data format.
 * @param   src       Select one of the following @ref afe_clocks
 * @param   div       Select one of the following @ref afe_clkdiv
 * @param   freq      Clock frequency in Hz (e.g. 12288000); this value is used 
 *                    by the precompiler to calculate value for the 
 *                    AFE_CR_STRTUP_CNT register field.
 * @showinitializer
 *****************************************************************************/
#define AFE_MODULE_LPM_LJFORMAT_CONFIG(src,div,freq)                          \
(tAFE){                                                                       \
/* CR   */  SET(AFE_CR_MSTR_EN_MASK)|CLR(AFE_CR_SOFT_TRG0_MASK)|              \
/* ..   */  CLR(AFE_CR_SOFT_TRG1_MASK)|CLR(AFE_CR_SOFT_TRG2_MASK)|            \
/* ..   */  CLR(AFE_CR_SOFT_TRG3_MASK)|SET(AFE_CR_LPM_EN_MASK)|               \
/* ..   */  SET(AFE_CR_RST_B_MASK)|SET(AFE_CR_DLY_OK_MASK)|                   \
/* ..   */  CLR(AFE_CR_RESULT_FORMAT_MASK)|                                   \
/* ..   */  SET(AFE_CR_STRTUP_CNT((uint8)((freq/(1<<div))*20e-6))),           \
/* CKR  */  AFE_CKR_DIV(div)|AFE_CKR_CLS(src),                                \
}
/*! @} End of afe_module_config                                               */

/******************************************************************************
* AFE default configurations used by AFE_CH_init() function
*
*//*! @addtogroup afe_chan_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief Selects hardware trigger continuous conversion mode. PGA is enabled 
 * and set to the user gain selected by the user.
 * @details Selects hardware trigger continuous conversion mode. PGA is enabled 
 * and set to the user gain selected by the user.
 * @param   osr       Select @ref afe_osr
 * @param   gain      Select @ref afe_gain
 * @note              Use this configuration structure only for AFE channels 
 *                    with built-in programmable gain amplifier (PGA)
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_CCM_PGAON_CONFIG(osr,gain)                               \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|SET(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(gain))|CLR(AFE_CFR_BYP_MODE_MASK)|        \
/* ... */  SET(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Selects software trigger continuous conversion mode. PGA is enabled 
 * and set to the user gain selected by the user.
 * @details Selects software trigger continuous conversion mode. PGA is enabled 
 * and set to the user gain selected by the user.
 * @param   osr       Select @ref afe_osr
 * @param   gain      Select @ref afe_gain
 * @note              Use this configuration structure only for AFE channels 
 *                    with built-in programmable gain amplifier (PGA)
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_CCM_PGAON_CONFIG(osr,gain)                               \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|SET(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(gain))|CLR(AFE_CFR_BYP_MODE_MASK)|        \
/* ... */  SET(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Selects hardware trigger single conversion mode. PGA is enabled 
 * and set to the user gain selected by the user.
 * @details Selects hardware trigger single conversion mode. PGA is enabled 
 * and set to the user gain selected by the user.
 * @param   osr       Select @ref afe_osr
 * @param   gain      Select @ref afe_gain
 * @note              Use this configuration structure only for AFE channels 
 *                    with built-in programmable gain amplifier (PGA)
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_SCM_PGAON_CONFIG(osr,gain)                               \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|SET(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(gain))|CLR(AFE_CFR_BYP_MODE_MASK)|        \
/* ... */  SET(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Selects software trigger single conversion mode. PGA is enabled 
 * and set to the user gain selected by the user.
 * @details Selects software trigger single conversion mode. PGA is enabled 
 * and set to the user gain selected by the user.
 * @param   osr       Select @ref afe_osr
 * @param   gain      Select @ref afe_gain
 * @note              Use this configuration structure only for AFE channels 
 *                    with built-in programmable gain amplifier (PGA)
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_SCM_PGAON_CONFIG(osr,gain)                               \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|SET(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(gain))|CLR(AFE_CFR_BYP_MODE_MASK)|        \
/* ... */  SET(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Selects hardware trigger continuous conversion mode. PGA is disabled.
 * @details Selects hardware trigger continuous conversion mode. PGA is disabled.
 * @param   osr       Select @ref afe_osr
 * @note              If applied for AFE channel with built-in programmable gain 
 *                    amplifier (PGA) then PGA on the channel will be disabled.
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_CCM_PGAOFF_CONFIG(osr)                                   \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|CLR(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  SET(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Selects software trigger continuous conversion mode. PGA is disabled.
 * @details Selects software trigger continuous conversion mode. PGA is disabled.
 * @param   osr       Select @ref afe_osr
 * @note              If applied for AFE channel with built-in programmable gain 
 *                    amplifier (PGA) then PGA on the channel will be disabled.
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(osr)                                   \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|CLR(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  SET(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Selects hardware trigger single conversion mode. PGA is disabled.
 * @details Selects hardware trigger single conversion mode. PGA is disabled.
 * @param   osr       Select @ref afe_osr
 * @note              If applied for AFE channel with built-in programmable gain 
 *                    amplifier (PGA) then PGA on the channel will be disabled.
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_SCM_PGAOFF_CONFIG(osr)                                   \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|CLR(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  SET(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Selects software trigger single conversion mode. PGA is disabled.
 * @details Selects software trigger single conversion mode. PGA is disabled. 
 * @param   osr       Select @ref afe_osr
 * @note              If applied for AFE channel with built-in programmable gain 
 *                    amplifier (PGA) then PGA on the channel will be disabled.
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_SCM_PGAOFF_CONFIG(osr)                                   \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|CLR(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  SET(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger single conversion mode. Decimator is 
 * clocked internally while registering input data on the rising (positive) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger single conversion mode. Decimator is 
 * clocked internally while registering input data on the rising (positive) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_SCM_DEC_INT_PE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger single conversion mode. Decimator is 
 * clocked internally while registering input data on the rising (positive) 
 * clock edge. 
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger single conversion mode. Decimator is 
 * clocked internally while registering input data on the rising (positive) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_SCM_DEC_INT_PE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger single conversion mode. Decimator is 
 * clocked externally while registering input data on the rising (positive) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger single conversion mode. Decimator is 
 * clocked externally while registering input data on the rising (positive) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_SCM_DEC_EXT_PE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  SET(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger single conversion mode. Decimator is 
 * clocked externally while registering input data on the rising (positive)
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger single conversion mode. Decimator is 
 * clocked externally while registering input data on the rising (positive)
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_SCM_DEC_EXT_PE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  SET(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger single conversion mode. Decimator is 
 * clocked internally while registering input data on the falling (negative) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger single conversion mode. Decimator is 
 * clocked internally while registering input data on the falling (negative) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_SCM_DEC_INT_NE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|SET(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger single conversion mode. Decimator is 
 * clocked internally while registering input data on the falling (negative) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger single conversion mode. Decimator is 
 * clocked internally while registering input data on the falling (negative) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_SCM_DEC_INT_NE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|SET(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger single conversion mode. Decimator is 
 * clocked externally while registering input data on the falling (negative) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger single conversion mode. Decimator is 
 * clocked externally while registering input data on the falling (negative) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_SCM_DEC_EXT_NE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|SET(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  SET(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger single conversion mode. Decimator is 
 * clocked externally while registering input data on the falling (negative) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger single conversion mode. Decimator is 
 * clocked externally while registering input data on the falling (negative) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_SCM_DEC_EXT_NE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  CLR(AFE_CFR_CC_MASK)|SET(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  SET(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger continuous conversion mode. Decimator 
 * is clocked internally while registering input data on the rising (positive) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger continuous conversion mode. Decimator 
 * is clocked internally while registering input data on the rising (positive) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_CCM_DEC_INT_PE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger continuous conversion mode. Decimator 
 * is clocked internally while registering input data on the rising (positive) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger continuous conversion mode. Decimator 
 * is clocked internally while registering input data on the rising (positive) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_CCM_DEC_INT_PE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger continuous conversion mode. Decimator 
 * is clocked externally while registering input data on the rising (positive) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger continuous conversion mode. Decimator 
 * is clocked externally while registering input data on the rising (positive) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_CCM_DEC_EXT_PE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  SET(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger continuous conversion mode. Decimator 
 * is clocked externally while registering input data on the rising (positive) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger continuous conversion mode. Decimator 
 * is clocked externally while registering input data on the rising (positive) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_CCM_DEC_EXT_PE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|CLR(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  SET(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger continuous conversion mode. Decimator 
 * is clocked internally while registering input data on the falling (negative) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger continuous conversion mode. Decimator 
 * is clocked internally while registering input data on the falling (negative) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_CCM_DEC_INT_NE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|SET(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger continuous conversion mode. Decimator 
 * is clocked internally while registering input data on the falling (negative) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger continuous conversion mode. Decimator 
 * is clocked internally while registering input data on the falling (negative) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_CCM_DEC_INT_NE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|SET(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  CLR(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger continuous conversion mode. Decimator 
 * is clocked externally while registering input data on the falling (negative) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to software trigger continuous conversion mode. Decimator 
 * is clocked externally while registering input data on the falling (negative) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_SWTRG_CCM_DEC_EXT_NE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|SET(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  SET(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  CLR(AFE_CFR_HW_TRG_MASK),                                          \
}
/***************************************************************************//*!
 * @brief Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger continuous conversion mode. Decimator 
 * is clocked externally while registering input data on the falling (negative) 
 * clock edge.
 * @details Modulator bypass mode with SD modulator and PGA blocks disabled. 
 * The wrapper is set to hardware trigger continuous conversion mode. Decimator 
 * is clocked externally while registering input data on the falling (negative) 
 * clock edge.
 * @param   osr       Select @ref afe_osr
 * @showinitializer
 *****************************************************************************/
#define AFE_CH_HWTRG_CCM_DEC_EXT_NE_CLK_BYPASS_CONFIG(osr)                    \
(tAFE_CH){                                                                    \
/* CFR */  SET(AFE_CFR_DEC_OSR(osr))|CLR(AFE_CFR_PGA_EN_MASK)|                \
/* ... */  SET(AFE_CFR_PGA_GAIN_SEL(1))|SET(AFE_CFR_BYP_MODE_MASK)|           \
/* ... */  CLR(AFE_CFR_SD_MOD_EN_MASK)|SET(AFE_CFR_DEC_EN_MASK)|              \
/* ... */  SET(AFE_CFR_CC_MASK)|SET(AFE_CFR_DEC_CLK_EDGE_SEL_MASK)|           \
/* ... */  SET(AFE_CFR_DEC_CLK_INP_SEL_MASK)|                                 \
/* ... */  SET(AFE_CFR_HW_TRG_MASK),                                          \
}
/*! @} End of afe_chan_config                                                 */

/******************************************************************************
* AFE callback registered by AFE_CH_Init() function
*
*//*! @addtogroup afe_ch_callback
* @{
*******************************************************************************/
/*! AFE_CH_CALLBACK type declaration                                          */
typedef enum 
{
  COC_CALLBACK=1, ///< Conversion Complete (COC) interrupt
  OVR_CALLBACK=2  ///< Overflow (OVR) interrupt
} AFE_CH_CALLBACK_TYPE;

/*! @brief AFE_CH_CALLBACK function declaration                               */
typedef void (*AFE_CH_CALLBACK)(AFE_CH_CALLBACK_TYPE type, int32 result);
/*! @} End of afe_ch_callback                                                 */

/******************************************************************************
* AFE function and macro definitions
*
*//*! @addtogroup afe_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Selects AFE clock divider value. 
 * @details This macro selects AFE clock divider value.
 * @param   div       Select one of the following @ref afe_clkdiv
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define AFE_SelClkDiv(div)      iobfiw (AFE_CKR,AFE_CKR_DIV_SHIFT,4,(div))

/***************************************************************************//*!
 * @brief   Selects AFE clock source. 
 * @details This macro selects AFE clock source.
 * @param   src       Select one of the following @ref afe_clocks  
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define AFE_SelClkSrc(src)      iobfiw (AFE_CKR,AFE_CKR_CLS_SHIFT,2,(src))

/***************************************************************************//*!
 * @brief   Triggers conversion on selected AFE channel(s).
 * @details This macro triggers conversion by software on AFE channels selected 
 *          by the channel mask.
 * @param   chanmask  Select one or more OR'ed @ref afe_channel
 * @note    Implemented as an inline macro.
 * @warning Once the trigger capture logic receives a trigger it will ignore 
 *          input trigger for next 2 mod_clk cycles. If some trigger comes in 
 *          between it will be ignored. If triggers are more than 2 mod_clk 
 *          cycles apart then it will cause re-sync operation. This is valid for 
 *          both hardware and software trigger.
 ******************************************************************************/
#define AFE_SwTrigger(chanmask)                                               \
{                                                                             \
  register uint32 tmp = 0l;                                                   \
  if ((chanmask) & CH0) { tmp |= AFE_CR_SOFT_TRG0_MASK; }                     \
  if ((chanmask) & CH1) { tmp |= AFE_CR_SOFT_TRG1_MASK; }                     \
  if ((chanmask) & CH2) { tmp |= AFE_CR_SOFT_TRG2_MASK; }                     \
  if ((chanmask) & CH3) { tmp |= AFE_CR_SOFT_TRG3_MASK; }                     \
  iobfiw (AFE_CR,AFE_CR_SOFT_TRG3_SHIFT,4,(tmp>>AFE_CR_SOFT_TRG3_SHIFT));     \
}

/***************************************************************************//*!
 * @brief   Enables DMA conversion on selected AFE channel(s).
 * @details This macro enables DMA conversion on selected AFE channels selected 
 *          by the channel mask.
 * @param   chanmask  Select one or more OR'ed @ref afe_channel
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define AFE_EnableDMA(chanmask)                                                    \
{                                                                                  \
  register uint32 tmp = AFE_DI;                                                    \
  if ((chanmask) & CH0) { tmp |= AFE_DI_DMAEN0_MASK; tmp &= ~AFE_DI_INTEN0_MASK; } \
  if ((chanmask) & CH1) { tmp |= AFE_DI_DMAEN1_MASK; tmp &= ~AFE_DI_INTEN1_MASK; } \
  if ((chanmask) & CH2) { tmp |= AFE_DI_DMAEN2_MASK; tmp &= ~AFE_DI_INTEN2_MASK; } \
  if ((chanmask) & CH3) { tmp |= AFE_DI_DMAEN3_MASK; tmp &= ~AFE_DI_INTEN3_MASK; } \
  AFE_DI = tmp;                                                                    \
}

/***************************************************************************//*!
 * @brief   Resets SD ADC, PGA and Decimator filters.
 * @details This macro resets SD ADC, PGA and Decimator filters. It also de-asserts 
 *          any pending interrupts. When called any internal state machine will 
 *          come to its reset state.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define AFE_SwReset()           {                                             \
                                  ioclrw (AFE_CR,AFE_CR_RST_B_MASK);          \
                                  iosetw (AFE_CR,AFE_CR_RST_B_MASK);          \
                                }

/***************************************************************************//*!
 * @brief   Asserts DLY_OK bit.
 * @details This macro asserts DLY_OK bit so written phase delay values on all 
 *          AFE channel will become active when a new conversion starts.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define AFE_DlyOk()             iosetw (AFE_CR,AFE_CR_DLY_OK_MASK)

/***************************************************************************//*!
 * @brief   Writes new phase delay value to the selected AFE channel.
 * @details This macro writes new phase delay value to the selected AFE channel. 
 *          The new phase delay value will become active after <c>AFE_DlyOk()</c> 
 *          macro is called and a new conversion starts. Since channel phase 
 *          delay can only increase the zero value is written to delay register 
 *          instead of negative phase delay value.
 * @param   chan    CH0,CH1,CH2,CH3
 * @param   dly     Channel phase delay (signed integer value -2047 to 2047). 
 * @note    Implemented as an inline macro. 
 * @see     @ref AFE_DlyOk
 ******************************************************************************/
#define AFE_DlyWrite(chan,dly)  { AFE_##chan##_DR = ((dly>0)?dly:0);           }

/***************************************************************************//*!
 * @brief   Updates all AFE channels with new delay values.
 * @details This macro updates all AFE channels with new delay values 
 *          provided as the input arguments of this macro. The macro also sets
 *          DLY_OK bit so all new delay values will become active after new 
 *          conversion. Since channel phase delay can only increase the zero 
 *          values are written to delay registers instead of any negative phase 
 *          delay values.
 * @param   dly0    Channel 0 phase delay (signed integer value -2047 to 2047)
 * @param   dly1    Channel 1 phase delay (signed integer value -2047 to 2047)
 * @param   dly2    Channel 2 phase delay (signed integer value -2047 to 2047)
 * @param   dly3    Channel 3 phase delay (signed integer value -2047 to 2047)
 * @note    Implemented as an inline macro.
 * @see     @ref AFE_DlyWrite
 ******************************************************************************/
#define AFE_DlyUpdate(dly0,dly1,dly2,dly3)  {                                 \
                                              AFE_CH0_DR = ((dly0>0)?dly0:0); \
                                              AFE_CH1_DR = ((dly1>0)?dly1:0); \
                                              AFE_CH2_DR = ((dly2>0)?dly2:0); \
                                              AFE_CH3_DR = ((dly3>0)?dly3:0); \
                                              AFE_DlyOk();                    \
                                            }                        

#define CH0_GainCtrl(gain)                                                    \
{                                                                             \
  if (gain == PGA_DISABLE)                                                    \
  {                                                                           \
    ioclrw (AFE_CH0_CFR,AFE_CFR_PGA_EN_MASK);                                 \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    iosetw (AFE_CH0_CFR,AFE_CFR_PGA_EN_MASK);                                 \
    iobfiw (AFE_CH0_CFR,AFE_CFR_PGA_GAIN_SEL_SHIFT,3,gain);                   \
  }                                                                           \
}    
#define CH1_GainCtrl(gain)                                                    \
{                                                                             \
  if (gain == PGA_DISABLE)                                                    \
  {                                                                           \
    ioclrw (AFE_CH1_CFR,AFE_CFR_PGA_EN_MASK);                                 \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    iosetw (AFE_CH1_CFR,AFE_CFR_PGA_EN_MASK);                                 \
    iobfiw (AFE_CH1_CFR,AFE_CFR_PGA_GAIN_SEL_SHIFT,3,gain);                   \
  }                                                                           \
}    
/***************************************************************************//*!
 * @brief   Controls PGA gain stage for given channels.
 * @details This macro controls PGA gain stage for channels with built-in 
 *          Programmable Gain Amplifier (PGA). 
 * @param   chan    CH0,CH1
 * @param   gain    Select one of the following @ref afe_gain. Note that 
 *                  PGA_DISABLE argument disables PGA completely.                 
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define AFE_GainCtrl(chan,gain) chan##_GainCtrl(gain)  

#define CH0_ChanReady           (AFE_SR & AFE_SR_COC0_MASK)
#define CH1_ChanReady           (AFE_SR & AFE_SR_COC1_MASK)
#define CH2_ChanReady           (AFE_SR & AFE_SR_COC2_MASK)
#define CH3_ChanReady           (AFE_SR & AFE_SR_COC3_MASK)
/***************************************************************************//*!
 * @brief   Returns conversion status (COCx flag) for given AFE channel.
 * @details This macro returns conversion status (COCx flag) for given AFE 
 *          channel. 
 * @param   chan    CH0,CH1,CH2,CH3
 * @return  true (non-zero) conversion completed (COC flag is asserted),
 * @return  false           conversion pending (COC flag is deasserted).
 * @note    Implemented as an inline macro.
 * @see     @ref AFE_ChanRead, @ref AFE_Read, @ref AFE_ChanOverflow
 ******************************************************************************/
#define AFE_ChanReady(chan)       chan##_ChanReady

#define CH0_ChanOverflow        (AFE_SR & AFE_SR_OVR0_MASK)
#define CH1_ChanOverflow        (AFE_SR & AFE_SR_OVR1_MASK)
#define CH2_ChanOverflow        (AFE_SR & AFE_SR_OVR2_MASK)
#define CH3_ChanOverflow        (AFE_SR & AFE_SR_OVR3_MASK)
/***************************************************************************//*!
 * @brief   Returns overflow status (OVRx flag) for given AFE channel.
 * @details This macro returns overflow status (OVRx flag) for given AFE 
 *          channel. 
 * @param   chan    CH0,CH1,CH2,CH3
 * @return  true (non-zero) data overwritten (OVR flag is asserted),
 * @return  false           data not overwritten (OVR flag is deasserted).
 * @note    Implemented as an inline macro.
 * @see     @ref AFE_ChanRead, @ref AFE_Read, @ref AFE_ChanReady
 ******************************************************************************/
#define AFE_ChanOverflow(chan)       chan##_ChanOverflow

/***************************************************************************//*!
 * @brief   Returns 2's complement 32-bit value of the selected channel result 
 *          register.
 * @details This macro returns 2's complement 32-bit value of the selected 
 *          channel result register. The alignment of the data format is either
 *          left or right justified based on the configuration structure 
 *          @ref afe_module_config passed to the <c>AFE_Init()</c> initialization 
 *          function.
 * @param   chan    CH0,CH1,CH2,CH3
 * @return  @ref int32 2's complement result register value.
 * @note    Implemented as an inline macro.
 * @see     @ref AFE_ChanReady, @ref AFE_ChanOverflow
 ******************************************************************************/
#define AFE_ChanRead(chan)        (int32)AFE_##chan##_RR

/***************************************************************************//*!
 * @brief   Returns 2's complement 32-bit values of the selected channel result 
 *          registers.
 * @details This macro returns 2's complement 32-bit value of the selected 
 *          channel result registers. The alignment of the data format is either
 *          left or right justified based on the configuration structure 
 *          @ref afe_module_config passed to the <c>AFE_Init()</c> initialization 
 *          function.
 * @param   chanmask  Select one or more OR'ed @ref afe_channel
 * @param   ptr       Pointer to the int32 array of the size that equals to
 *                    number of channels processed by the function. The values 
 *                    of lower channels are stored first.
 * @note    Implemented as an inline macro.
 * @see     @ref AFE_ChanReady, @ref AFE_ChanOverflow
 ******************************************************************************/
#define AFE_Read(chanmask,ptr)                                                \
{                                                                             \
  register int32 volatile *pTmp32 = (int32*)ptr;                              \
  if ((chanmask) & CH0) { *((int32*)pTmp32++) = AFE_CH0_RR; }                 \
  if ((chanmask) & CH1) { *((int32*)pTmp32++) = AFE_CH1_RR; }                 \
  if ((chanmask) & CH2) { *((int32*)pTmp32++) = AFE_CH2_RR; }                 \
  if ((chanmask) & CH3) { *((int32*)pTmp32++) = AFE_CH3_RR; }                 \
}

/***************************************************************************//*!
* @brief    Disables AFE operation.
* @details  This macro disables AFE operation. It also disables interrupts, 
*           DMA transfers and initializes control registers.
* @note     Implemented as an inline macro. Call this macro to disable AFE operation 
*           prior changing its configuration.
* @see     @ref AFE_Init, @ref AFE_ChanInit
******************************************************************************/
#define AFE_Disable()                                                               \
{                                                                                   \
  ioclrw (AFE_CR,AFE_CR_MSTR_EN_MASK|AFE_CR_LPM_EN_MASK|AFE_CR_RESULT_FORMAT_MASK); \
  AFE_DI      = 0x00000000; /* disable all interrupt and DMA requests       */      \
  AFE_CKR     = 0x10000000; /* clock divider set to divide by 2 (default)   */      \
  AFE_CH0_CFR = 0x00080000; /* gain set to 1x (default)                     */      \
  AFE_CH1_CFR = 0x00080000; /* gain set to 1x (default)                     */      \
  AFE_CH2_CFR = 0x00080000; /* gain set to 1x (default)                     */      \
  AFE_CH3_CFR = 0x00080000; /* gain set to 1x (default)                     */      \
  AFE_CH0_DR  = 0l;                                                                 \
  AFE_CH1_DR  = 0l;                                                                 \
  AFE_CH2_DR  = 0l;                                                                 \
  AFE_CH3_DR  = 0l;                                                                 \
}

/***************************************************************************//*!
 * @brief   AFE channel initialization.
 * @details This function initializes channel of the Analogue Front End (AFE)
 *          selected by the input argument. It also installs callback function 
 *          for interrupt vectors associated with initialized AFE channels: 
 *          vector 37(CH0), vector 38(CH1), vector 39(CH2) and vector 40(CH3).
 * @param   chan      CH0,CH1,CH2,CH3 
 * @param   cfg       Select one of the following @ref afe_chan_config
 * @param   dly       Channel phase delay (signed integer value -2047 to 2047).
 *                    Since channel phase delay can only increases the zero value
 *                    is written to the delay register instead of any negative 
 *                    phase delay value.
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to @ref AFE_CH_CALLBACK
 * @note    Implemented as a function call.
 * @warning Register interrupt handler for AFE channels:
 *|Channel|Appconfig.h                                                          |
 *|:-----:|:--------------------------------------------------------------------|
 *|CH0    |<c>extern void AFE_CH0_isr(void);\n#define VECTOR_037 AFE_CH0_isr</c>|
 *|CH1    |<c>extern void AFE_CH1_isr(void);\n#define VECTOR_038 AFE_CH1_isr</c>|
 *|CH2    |<c>extern void AFE_CH2_isr(void);\n#define VECTOR_039 AFE_CH2_isr</c>|
 *|CH3    |<c>extern void AFE_CH3_isr(void);\n#define VECTOR_040 AFE_CH3_isr</c>|
 * @see     @ref AFE_Init
 ******************************************************************************/ 
#define AFE_ChanInit(chan,cfg,dly,ip,callback) AFE_##chan##_Init(cfg,dly,ip,callback)

/***************************************************************************//*!
 * @brief   AFE module initialization.
 * @details This function initializes Analogue Front End (AFE) module. The function 
 *          can also trigger conversions by software on the respective channels 
 *          provided those channels have been already initialized using 
 *          <c>AFE_ChanInit()</c> function call.
 * @param   cfg     Select one of the following @ref afe_module_config
 * @note    Implemented as a function call.
 * @see     @ref AFE_ChanInit, @ref AFE_Disable  
 ******************************************************************************/ 
#define AFE_Init(cfg)                                                         \
{                                                                             \
  AFE_CKR = cfg.CKR;                                                          \
  AFE_CR  = cfg.CR;                                                           \
}
/*! @} End of afe_macro                                                      */
    
/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    
extern void AFE_CH0_Init (tAFE_CH ch, int16 dly, uint8 ip, AFE_CH_CALLBACK pCallback);
extern void AFE_CH1_Init (tAFE_CH ch, int16 dly, uint8 ip, AFE_CH_CALLBACK pCallback);
extern void AFE_CH2_Init (tAFE_CH ch, int16 dly, uint8 ip, AFE_CH_CALLBACK pCallback);
extern void AFE_CH3_Init (tAFE_CH ch, int16 dly, uint8 ip, AFE_CH_CALLBACK pCallback);

#endif /* __AFE_H */ 