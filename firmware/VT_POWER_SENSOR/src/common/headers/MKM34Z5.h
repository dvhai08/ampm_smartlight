/*
** ###################################################################
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    MKMxxZxxCxx5RM Rev. 4, June 2013
**     Version:             rev. 1.5, 2015-07-23
**
**     Abstract:
**         This header file implements peripheral memory map for MKM34Z5
**         processor.
**
**     Copyright: 1997 - 2015 Freescale, Inc. All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2012-06-21)
**         Initial version.
**     - rev. 1.1 (2012-09-04)
**         Update according to reference manual rev. 2, draft A.
**     - rev. 1.2 (2012-10-18)
**         Update according to reference manual rev. 2.
**     - rev. 1.3 (2013-04-05)
**         Changed start of doxygen comment.
**     - rev. 1.4 (2015-05-06)
**         Update according to reference manual rev. 4.
**     - rev. 1.5 (2015-07-23)
**         Update according to final version of reference manual rev. 4.
**
** ###################################################################
*/

/*!
 * @file MKM34Z5.h
 * @version 1.5
 * @date 2015-07-23
 * @brief Peripheral memory map for MKM34Z5
 *
 * This header file implements peripheral memory map for MKM34Z5 processor.
 */


/* ----------------------------------------------------------------------------
   -- MCU activation
   ---------------------------------------------------------------------------- */

/* Prevention from multiple including the same memory map */
#if !defined(MCU_MKM34Z5) && !defined(MCU_MKM38Z5)  /* Check if memory map has not been already included */
#define MCU_MKM34Z5
#define MCU_MKM38Z5

/* Check if another memory map has not been also included */
#if (defined(MCU_ACTIVE))
  #error MKM34Z5 memory map: There is already included another memory map. Only one memory map can be included.
#endif /* (defined(MCU_ACTIVE)) */
#define MCU_ACTIVE

#include <stdint.h>

/** Memory map major version (memory maps with equal major version number are
 * compatible) */
#define MCU_MEM_MAP_VERSION 0x0100u
/** Memory map minor version */
#define MCU_MEM_MAP_VERSION_MINOR 0x0005u


/* ----------------------------------------------------------------------------
   -- Interrupt vector numbers
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Interrupt_vector_numbers Interrupt vector numbers
 * @{
 */

/** Interrupt Number Definitions */
typedef enum {
  INT_Initial_Stack_Pointer    = 0,                /**< Initial Stack Pointer */
  INT_Initial_Program_Counter  = 1,                /**< Initial Program Counter */
  INT_NMI                      = 2,                /**< Non-maskable Interrupt (NMI) */
  INT_Hard_Fault               = 3,                /**< Hard Fault */
  INT_Reserved4                = 4,                /**< Reserved interrupt 4 */
  INT_Reserved5                = 5,                /**< Reserved interrupt 5 */
  INT_Reserved6                = 6,                /**< Reserved interrupt 6 */
  INT_Reserved7                = 7,                /**< Reserved interrupt 7 */
  INT_Reserved8                = 8,                /**< Reserved interrupt 8 */
  INT_Reserved9                = 9,                /**< Reserved interrupt 9 */
  INT_Reserved10               = 10,               /**< Reserved interrupt 10 */
  INT_SVCall                   = 11,               /**< A supervisor call exception */
  INT_Reserved12               = 12,               /**< Reserved interrupt 12 */
  INT_Reserved13               = 13,               /**< Reserved interrupt 13 */
  INT_PendableSrvReq           = 14,               /**< Pendable request for system service (PendableSrvReq) */
  INT_SysTick                  = 15,               /**< SysTick Interrupt */
  INT_DMA0                     = 16,               /**< DMA Channel 0 Transfer Complete */
  INT_DMA1                     = 17,               /**< DMA Channel 1 Transfer Complete */
  INT_DMA2                     = 18,               /**< DMA Channel 2 Transfer Complete */
  INT_DMA3                     = 19,               /**< DMA Channel 3 Transfer Complete */
  INT_SPI0                     = 20,               /**< ORed Interrupt SPI0 */
  INT_SPI1                     = 21,               /**< ORed Interrupt SPI1 */
  INT_PMC                      = 22,               /**< Low-voltage detect, low-voltage warning */
  INT_TMR0                     = 23,               /**< Quad Timer Counter0 */
  INT_TMR1                     = 24,               /**< Quad Timer Counter1 */
  INT_TMR2                     = 25,               /**< Quad Timer Counter2 */
  INT_TMR3                     = 26,               /**< Quad Timer Counter3 */
  INT_PIT0_PIT1                = 27,               /**< ORed Interrupt of PITs */
  INT_LLWU                     = 28,               /**< Low Leakage Wakeup */
  INT_FTFA                     = 29,               /**< ORed interrupt for Flash Command Complete/Read collision */
  INT_CMP0_CMP1                = 30,               /**< ORed Interrupt of PRACMP */
  INT_LCD                      = 31,               /**< ORed Interrupt of SLCD */
  INT_ADC                      = 32,               /**< ORed Interrupt from SAR */
  INT_PTx                      = 33,               /**< ORed Interrupt from all GPIO (PTx) ports */
  INT_RNGA                     = 34,               /**< ORed Interrupt from RNGA */
  INT_SCI0_SCI1                = 35,               /**< ORed Interrupt SCI0 and SCI1 */
  INT_SCI2_SCI3                = 36,               /**< ORed Interrupt SCI2 and SCI3 */
  INT_AFE0                     = 37,               /**< AFE Channel 0 ORed Interrupt */
  INT_AFE1                     = 38,               /**< AFE Channel 1 ORed Interrupt */
  INT_AFE2                     = 39,               /**< AFE Channel 2 ORed Interrupt */
  INT_AFE3                     = 40,               /**< AFE Channel 3 ORed Interrupt */
  INT_RTC                      = 41,               /**< iRTC Interrupt */
  INT_I2C0_I2C1                = 42,               /**< ORed I2C interrupt */
  INT_EWM                      = 43,               /**< External Watchdog Monitor */
  INT_MCG                      = 44,               /**< MCG Loss of Clock, Loss of Lock */
  INT_Watchdog                 = 45,               /**< ORed WDOG Interrupt */
  INT_LPTMR                    = 46,               /**< ORed LPTIMER Interrupt */
  INT_PXBAR                    = 47                /**< Peripheral XBAR ORed Interrupt */
} IRQInterruptIndex;

/*!
 * @}
 */ /* end of group Interrupt_vector_numbers */


/* ----------------------------------------------------------------------------
   -- Peripheral type defines
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Peripheral_defines Peripheral type defines
 * @{
 */


/*
** Start of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #pragma push
  #pragma anon_unions
#elif defined(__CWCC__)
  #pragma push
  #pragma cpp_extensions on
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/* ----------------------------------------------------------------------------
   -- ADC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Peripheral ADC
 * @{
 */

/** ADC - Peripheral register structure */
typedef struct ADC_MemMap {
  uint32_t SC1[4];                                 /**< ADC Status and Control Registers 1, array offset: 0x0, array step: 0x4 */
  uint32_t CFG1;                                   /**< ADC Configuration Register 1, offset: 0x10 */
  uint32_t CFG2;                                   /**< ADC Configuration Register 2, offset: 0x14 */
  uint32_t R[4];                                   /**< ADC Data Result Register, array offset: 0x18, array step: 0x4 */
  uint32_t CV1;                                    /**< Compare Value Registers, offset: 0x28 */
  uint32_t CV2;                                    /**< Compare Value Registers, offset: 0x2C */
  uint32_t SC2;                                    /**< Status and Control Register 2, offset: 0x30 */
  uint32_t SC3;                                    /**< Status and Control Register 3, offset: 0x34 */
  uint32_t OFS;                                    /**< ADC Offset Correction Register, offset: 0x38 */
  uint32_t PG;                                     /**< ADC Plus-Side Gain Register, offset: 0x3C */
  uint8_t RESERVED_0[4];
  uint32_t CLPD;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x44 */
  uint32_t CLPS;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x48 */
  uint32_t CLP4;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x4C */
  uint32_t CLP3;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x50 */
  uint32_t CLP2;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x54 */
  uint32_t CLP1;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x58 */
  uint32_t CLP0;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x5C */
} volatile *ADC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- ADC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Register_Accessor_Macros ADC - Register accessor macros
 * @{
 */


/* ADC - Register accessors */
#define ADC_SC1_REG(base,index)                  ((base)->SC1[index])
#define ADC_CFG1_REG(base)                       ((base)->CFG1)
#define ADC_CFG2_REG(base)                       ((base)->CFG2)
#define ADC_R_REG(base,index)                    ((base)->R[index])
#define ADC_CV1_REG(base)                        ((base)->CV1)
#define ADC_CV2_REG(base)                        ((base)->CV2)
#define ADC_SC2_REG(base)                        ((base)->SC2)
#define ADC_SC3_REG(base)                        ((base)->SC3)
#define ADC_OFS_REG(base)                        ((base)->OFS)
#define ADC_PG_REG(base)                         ((base)->PG)
#define ADC_CLPD_REG(base)                       ((base)->CLPD)
#define ADC_CLPS_REG(base)                       ((base)->CLPS)
#define ADC_CLP4_REG(base)                       ((base)->CLP4)
#define ADC_CLP3_REG(base)                       ((base)->CLP3)
#define ADC_CLP2_REG(base)                       ((base)->CLP2)
#define ADC_CLP1_REG(base)                       ((base)->CLP1)
#define ADC_CLP0_REG(base)                       ((base)->CLP0)

/*!
 * @}
 */ /* end of group ADC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- ADC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Register_Masks ADC Register Masks
 * @{
 */

/* SC1 Bit Fields */
#define ADC_SC1_ADCH_MASK                        0x1Fu
#define ADC_SC1_ADCH_SHIFT                       0
#define ADC_SC1_ADCH(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC1_ADCH_SHIFT))&ADC_SC1_ADCH_MASK)
#define ADC_SC1_AIEN_MASK                        0x40u
#define ADC_SC1_AIEN_SHIFT                       6
#define ADC_SC1_COCO_MASK                        0x80u
#define ADC_SC1_COCO_SHIFT                       7
/* CFG1 Bit Fields */
#define ADC_CFG1_ADICLK_MASK                     0x3u
#define ADC_CFG1_ADICLK_SHIFT                    0
#define ADC_CFG1_ADICLK(x)                       (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_ADICLK_SHIFT))&ADC_CFG1_ADICLK_MASK)
#define ADC_CFG1_MODE_MASK                       0xCu
#define ADC_CFG1_MODE_SHIFT                      2
#define ADC_CFG1_MODE(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_MODE_SHIFT))&ADC_CFG1_MODE_MASK)
#define ADC_CFG1_ADLSMP_MASK                     0x10u
#define ADC_CFG1_ADLSMP_SHIFT                    4
#define ADC_CFG1_ADIV_MASK                       0x60u
#define ADC_CFG1_ADIV_SHIFT                      5
#define ADC_CFG1_ADIV(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_ADIV_SHIFT))&ADC_CFG1_ADIV_MASK)
#define ADC_CFG1_ADLPC_MASK                      0x80u
#define ADC_CFG1_ADLPC_SHIFT                     7
/* CFG2 Bit Fields */
#define ADC_CFG2_ADLSTS_MASK                     0x3u
#define ADC_CFG2_ADLSTS_SHIFT                    0
#define ADC_CFG2_ADLSTS(x)                       (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_ADLSTS_SHIFT))&ADC_CFG2_ADLSTS_MASK)
#define ADC_CFG2_ADHSC_MASK                      0x4u
#define ADC_CFG2_ADHSC_SHIFT                     2
#define ADC_CFG2_ADACKEN_MASK                    0x8u
#define ADC_CFG2_ADACKEN_SHIFT                   3
#define ADC_CFG2_MUXSEL_MASK                     0x10u
#define ADC_CFG2_MUXSEL_SHIFT                    4
/* R Bit Fields */
#define ADC_R_D_MASK                             0xFFFFu
#define ADC_R_D_SHIFT                            0
#define ADC_R_D(x)                               (((uint32_t)(((uint32_t)(x))<<ADC_R_D_SHIFT))&ADC_R_D_MASK)
/* CV1 Bit Fields */
#define ADC_CV1_CV_MASK                          0xFFFFu
#define ADC_CV1_CV_SHIFT                         0
#define ADC_CV1_CV(x)                            (((uint32_t)(((uint32_t)(x))<<ADC_CV1_CV_SHIFT))&ADC_CV1_CV_MASK)
/* CV2 Bit Fields */
#define ADC_CV2_CV_MASK                          0xFFFFu
#define ADC_CV2_CV_SHIFT                         0
#define ADC_CV2_CV(x)                            (((uint32_t)(((uint32_t)(x))<<ADC_CV2_CV_SHIFT))&ADC_CV2_CV_MASK)
/* SC2 Bit Fields */
#define ADC_SC2_REFSEL_MASK                      0x3u
#define ADC_SC2_REFSEL_SHIFT                     0
#define ADC_SC2_REFSEL(x)                        (((uint32_t)(((uint32_t)(x))<<ADC_SC2_REFSEL_SHIFT))&ADC_SC2_REFSEL_MASK)
#define ADC_SC2_DMAEN_MASK                       0x4u
#define ADC_SC2_DMAEN_SHIFT                      2
#define ADC_SC2_ACREN_MASK                       0x8u
#define ADC_SC2_ACREN_SHIFT                      3
#define ADC_SC2_ACFGT_MASK                       0x10u
#define ADC_SC2_ACFGT_SHIFT                      4
#define ADC_SC2_ACFE_MASK                        0x20u
#define ADC_SC2_ACFE_SHIFT                       5
#define ADC_SC2_ADTRG_MASK                       0x40u
#define ADC_SC2_ADTRG_SHIFT                      6
#define ADC_SC2_ADACT_MASK                       0x80u
#define ADC_SC2_ADACT_SHIFT                      7
/* SC3 Bit Fields */
#define ADC_SC3_AVGS_MASK                        0x3u
#define ADC_SC3_AVGS_SHIFT                       0
#define ADC_SC3_AVGS(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC3_AVGS_SHIFT))&ADC_SC3_AVGS_MASK)
#define ADC_SC3_AVGE_MASK                        0x4u
#define ADC_SC3_AVGE_SHIFT                       2
#define ADC_SC3_ADCO_MASK                        0x8u
#define ADC_SC3_ADCO_SHIFT                       3
#define ADC_SC3_CALF_MASK                        0x40u
#define ADC_SC3_CALF_SHIFT                       6
#define ADC_SC3_CAL_MASK                         0x80u
#define ADC_SC3_CAL_SHIFT                        7
/* OFS Bit Fields */
#define ADC_OFS_OFS_MASK                         0xFFFFu
#define ADC_OFS_OFS_SHIFT                        0
#define ADC_OFS_OFS(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_OFS_OFS_SHIFT))&ADC_OFS_OFS_MASK)
/* PG Bit Fields */
#define ADC_PG_PG_MASK                           0xFFFFu
#define ADC_PG_PG_SHIFT                          0
#define ADC_PG_PG(x)                             (((uint32_t)(((uint32_t)(x))<<ADC_PG_PG_SHIFT))&ADC_PG_PG_MASK)
/* CLPD Bit Fields */
#define ADC_CLPD_CLPD_MASK                       0x3Fu
#define ADC_CLPD_CLPD_SHIFT                      0
#define ADC_CLPD_CLPD(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLPD_CLPD_SHIFT))&ADC_CLPD_CLPD_MASK)
/* CLPS Bit Fields */
#define ADC_CLPS_CLPS_MASK                       0x3Fu
#define ADC_CLPS_CLPS_SHIFT                      0
#define ADC_CLPS_CLPS(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLPS_CLPS_SHIFT))&ADC_CLPS_CLPS_MASK)
/* CLP4 Bit Fields */
#define ADC_CLP4_CLP4_MASK                       0x3FFu
#define ADC_CLP4_CLP4_SHIFT                      0
#define ADC_CLP4_CLP4(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP4_CLP4_SHIFT))&ADC_CLP4_CLP4_MASK)
/* CLP3 Bit Fields */
#define ADC_CLP3_CLP3_MASK                       0x1FFu
#define ADC_CLP3_CLP3_SHIFT                      0
#define ADC_CLP3_CLP3(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP3_CLP3_SHIFT))&ADC_CLP3_CLP3_MASK)
/* CLP2 Bit Fields */
#define ADC_CLP2_CLP2_MASK                       0xFFu
#define ADC_CLP2_CLP2_SHIFT                      0
#define ADC_CLP2_CLP2(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP2_CLP2_SHIFT))&ADC_CLP2_CLP2_MASK)
/* CLP1 Bit Fields */
#define ADC_CLP1_CLP1_MASK                       0x7Fu
#define ADC_CLP1_CLP1_SHIFT                      0
#define ADC_CLP1_CLP1(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP1_CLP1_SHIFT))&ADC_CLP1_CLP1_MASK)
/* CLP0 Bit Fields */
#define ADC_CLP0_CLP0_MASK                       0x3Fu
#define ADC_CLP0_CLP0_SHIFT                      0
#define ADC_CLP0_CLP0(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP0_CLP0_SHIFT))&ADC_CLP0_CLP0_MASK)

/*!
 * @}
 */ /* end of group ADC_Register_Masks */


/* ADC - Peripheral instance base addresses */
/** Peripheral ADC base pointer */
#define ADC_BASE_PTR                             ((ADC_MemMapPtr)0x4002B000u)
/** Array initializer of ADC peripheral base pointers */
#define ADC_BASE_PTRS                            { ADC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- ADC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Register_Accessor_Macros ADC - Register accessor macros
 * @{
 */


/* ADC - Register instance definitions */
/* ADC */
#define ADC_SC1A                                 ADC_SC1_REG(ADC_BASE_PTR,0)
#define ADC_SC1B                                 ADC_SC1_REG(ADC_BASE_PTR,1)
#define ADC_SC1C                                 ADC_SC1_REG(ADC_BASE_PTR,2)
#define ADC_SC1D                                 ADC_SC1_REG(ADC_BASE_PTR,3)
#define ADC_CFG1                                 ADC_CFG1_REG(ADC_BASE_PTR)
#define ADC_CFG2                                 ADC_CFG2_REG(ADC_BASE_PTR)
#define ADC_RA                                   ADC_R_REG(ADC_BASE_PTR,0)
#define ADC_RB                                   ADC_R_REG(ADC_BASE_PTR,1)
#define ADC_RC                                   ADC_R_REG(ADC_BASE_PTR,2)
#define ADC_RD                                   ADC_R_REG(ADC_BASE_PTR,3)
#define ADC_CV1                                  ADC_CV1_REG(ADC_BASE_PTR)
#define ADC_CV2                                  ADC_CV2_REG(ADC_BASE_PTR)
#define ADC_SC2                                  ADC_SC2_REG(ADC_BASE_PTR)
#define ADC_SC3                                  ADC_SC3_REG(ADC_BASE_PTR)
#define ADC_OFS                                  ADC_OFS_REG(ADC_BASE_PTR)
#define ADC_PG                                   ADC_PG_REG(ADC_BASE_PTR)
#define ADC_CLPD                                 ADC_CLPD_REG(ADC_BASE_PTR)
#define ADC_CLPS                                 ADC_CLPS_REG(ADC_BASE_PTR)
#define ADC_CLP4                                 ADC_CLP4_REG(ADC_BASE_PTR)
#define ADC_CLP3                                 ADC_CLP3_REG(ADC_BASE_PTR)
#define ADC_CLP2                                 ADC_CLP2_REG(ADC_BASE_PTR)
#define ADC_CLP1                                 ADC_CLP1_REG(ADC_BASE_PTR)
#define ADC_CLP0                                 ADC_CLP0_REG(ADC_BASE_PTR)

/* ADC - Register array accessors */
#define ADC_SC1(index)                           ADC_SC1_REG(ADC_BASE_PTR,index)
#define ADC_R(index)                             ADC_R_REG(ADC_BASE_PTR,index)

/*!
 * @}
 */ /* end of group ADC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group ADC_Peripheral */


/* ----------------------------------------------------------------------------
   -- AFE
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AFE_Peripheral AFE
 * @{
 */

/** AFE - Peripheral register structure */
typedef struct AFE_MemMap {
  uint32_t CFR[4];                                 /**< Channel0 Configuration Register..Channel3 Configuration Register, array offset: 0x0, array step: 0x4 */
  uint8_t RESERVED_0[8];
  uint32_t CR;                                     /**< Control Register, offset: 0x18 */
  uint32_t CKR;                                    /**< Clock Configuration Register, offset: 0x1C */
  uint32_t DI;                                     /**< DMA and Interrupt Register, offset: 0x20 */
  uint8_t RESERVED_1[8];
  uint32_t DR[4];                                  /**< Channel0 Delay Register..Channel3 Delay Register, array offset: 0x2C, array step: 0x4 */
  uint8_t RESERVED_2[8];
  uint32_t RR[4];                                  /**< Channel0 Result Register..Channel3 Result Register, array offset: 0x44, array step: 0x4 */
  uint8_t RESERVED_3[8];
  uint32_t SR;                                     /**< Status Register, offset: 0x5C */
} volatile *AFE_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- AFE - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AFE_Register_Accessor_Macros AFE - Register accessor macros
 * @{
 */


/* AFE - Register accessors */
#define AFE_CFR_REG(base,index)                  ((base)->CFR[index])
#define AFE_CR_REG(base)                         ((base)->CR)
#define AFE_CKR_REG(base)                        ((base)->CKR)
#define AFE_DI_REG(base)                         ((base)->DI)
#define AFE_DR_REG(base,index)                   ((base)->DR[index])
#define AFE_RR_REG(base,index)                   ((base)->RR[index])
#define AFE_SR_REG(base)                         ((base)->SR)

/*!
 * @}
 */ /* end of group AFE_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- AFE Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AFE_Register_Masks AFE Register Masks
 * @{
 */

/* CFR Bit Fields */
#define AFE_CFR_HW_TRG_MASK                      0x200u
#define AFE_CFR_HW_TRG_SHIFT                     9
#define AFE_CFR_DEC_CLK_INP_SEL_MASK             0x400u
#define AFE_CFR_DEC_CLK_INP_SEL_SHIFT            10
#define AFE_CFR_DEC_CLK_EDGE_SEL_MASK            0x800u
#define AFE_CFR_DEC_CLK_EDGE_SEL_SHIFT           11
#define AFE_CFR_CC_MASK                          0x1000u
#define AFE_CFR_CC_SHIFT                         12
#define AFE_CFR_DEC_EN_MASK                      0x2000u
#define AFE_CFR_DEC_EN_SHIFT                     13
#define AFE_CFR_SD_MOD_EN_MASK                   0x4000u
#define AFE_CFR_SD_MOD_EN_SHIFT                  14
#define AFE_CFR_BYP_MODE_MASK                    0x20000u
#define AFE_CFR_BYP_MODE_SHIFT                   17
#define AFE_CFR_PGA_GAIN_SEL_MASK                0x380000u
#define AFE_CFR_PGA_GAIN_SEL_SHIFT               19
#define AFE_CFR_PGA_GAIN_SEL(x)                  (((uint32_t)(((uint32_t)(x))<<AFE_CFR_PGA_GAIN_SEL_SHIFT))&AFE_CFR_PGA_GAIN_SEL_MASK)
#define AFE_CFR_PGA_EN_MASK                      0x1000000u
#define AFE_CFR_PGA_EN_SHIFT                     24
#define AFE_CFR_DEC_OSR_MASK                     0xE0000000u
#define AFE_CFR_DEC_OSR_SHIFT                    29
#define AFE_CFR_DEC_OSR(x)                       (((uint32_t)(((uint32_t)(x))<<AFE_CFR_DEC_OSR_SHIFT))&AFE_CFR_DEC_OSR_MASK)
/* CR Bit Fields */
#define AFE_CR_STRTUP_CNT_MASK                   0xFE00u
#define AFE_CR_STRTUP_CNT_SHIFT                  9
#define AFE_CR_STRTUP_CNT(x)                     (((uint32_t)(((uint32_t)(x))<<AFE_CR_STRTUP_CNT_SHIFT))&AFE_CR_STRTUP_CNT_MASK)
#define AFE_CR_RESULT_FORMAT_MASK                0x40000u
#define AFE_CR_RESULT_FORMAT_SHIFT               18
#define AFE_CR_DLY_OK_MASK                       0x200000u
#define AFE_CR_DLY_OK_SHIFT                      21
#define AFE_CR_RST_B_MASK                        0x400000u
#define AFE_CR_RST_B_SHIFT                       22
#define AFE_CR_LPM_EN_MASK                       0x2000000u
#define AFE_CR_LPM_EN_SHIFT                      25
#define AFE_CR_SOFT_TRG3_MASK                    0x8000000u
#define AFE_CR_SOFT_TRG3_SHIFT                   27
#define AFE_CR_SOFT_TRG2_MASK                    0x10000000u
#define AFE_CR_SOFT_TRG2_SHIFT                   28
#define AFE_CR_SOFT_TRG1_MASK                    0x20000000u
#define AFE_CR_SOFT_TRG1_SHIFT                   29
#define AFE_CR_SOFT_TRG0_MASK                    0x40000000u
#define AFE_CR_SOFT_TRG0_SHIFT                   30
#define AFE_CR_MSTR_EN_MASK                      0x80000000u
#define AFE_CR_MSTR_EN_SHIFT                     31
/* CKR Bit Fields */
#define AFE_CKR_CLS_MASK                         0x600000u
#define AFE_CKR_CLS_SHIFT                        21
#define AFE_CKR_CLS(x)                           (((uint32_t)(((uint32_t)(x))<<AFE_CKR_CLS_SHIFT))&AFE_CKR_CLS_MASK)
#define AFE_CKR_DIV_MASK                         0xF0000000u
#define AFE_CKR_DIV_SHIFT                        28
#define AFE_CKR_DIV(x)                           (((uint32_t)(((uint32_t)(x))<<AFE_CKR_DIV_SHIFT))&AFE_CKR_DIV_MASK)
/* DI Bit Fields */
#define AFE_DI_INTEN3_MASK                       0x800000u
#define AFE_DI_INTEN3_SHIFT                      23
#define AFE_DI_INTEN2_MASK                       0x1000000u
#define AFE_DI_INTEN2_SHIFT                      24
#define AFE_DI_INTEN1_MASK                       0x2000000u
#define AFE_DI_INTEN1_SHIFT                      25
#define AFE_DI_INTEN0_MASK                       0x4000000u
#define AFE_DI_INTEN0_SHIFT                      26
#define AFE_DI_DMAEN3_MASK                       0x10000000u
#define AFE_DI_DMAEN3_SHIFT                      28
#define AFE_DI_DMAEN2_MASK                       0x20000000u
#define AFE_DI_DMAEN2_SHIFT                      29
#define AFE_DI_DMAEN1_MASK                       0x40000000u
#define AFE_DI_DMAEN1_SHIFT                      30
#define AFE_DI_DMAEN0_MASK                       0x80000000u
#define AFE_DI_DMAEN0_SHIFT                      31
/* DR Bit Fields */
#define AFE_DR_DLY_MASK                          0x7FFu
#define AFE_DR_DLY_SHIFT                         0
#define AFE_DR_DLY(x)                            (((uint32_t)(((uint32_t)(x))<<AFE_DR_DLY_SHIFT))&AFE_DR_DLY_MASK)
/* RR Bit Fields */
#define AFE_RR_SDR_MASK                          0x7FFFFFu
#define AFE_RR_SDR_SHIFT                         0
#define AFE_RR_SDR(x)                            (((uint32_t)(((uint32_t)(x))<<AFE_RR_SDR_SHIFT))&AFE_RR_SDR_MASK)
#define AFE_RR_SIGN_BITS_MASK                    0xFF800000u
#define AFE_RR_SIGN_BITS_SHIFT                   23
#define AFE_RR_SIGN_BITS(x)                      (((uint32_t)(((uint32_t)(x))<<AFE_RR_SIGN_BITS_SHIFT))&AFE_RR_SIGN_BITS_MASK)
/* SR Bit Fields */
#define AFE_SR_RDY3_MASK                         0x10000u
#define AFE_SR_RDY3_SHIFT                        16
#define AFE_SR_RDY2_MASK                         0x20000u
#define AFE_SR_RDY2_SHIFT                        17
#define AFE_SR_RDY1_MASK                         0x40000u
#define AFE_SR_RDY1_SHIFT                        18
#define AFE_SR_RDY0_MASK                         0x80000u
#define AFE_SR_RDY0_SHIFT                        19
#define AFE_SR_OVR3_MASK                         0x200000u
#define AFE_SR_OVR3_SHIFT                        21
#define AFE_SR_OVR2_MASK                         0x400000u
#define AFE_SR_OVR2_SHIFT                        22
#define AFE_SR_OVR1_MASK                         0x800000u
#define AFE_SR_OVR1_SHIFT                        23
#define AFE_SR_OVR0_MASK                         0x1000000u
#define AFE_SR_OVR0_SHIFT                        24
#define AFE_SR_COC3_MASK                         0x10000000u
#define AFE_SR_COC3_SHIFT                        28
#define AFE_SR_COC2_MASK                         0x20000000u
#define AFE_SR_COC2_SHIFT                        29
#define AFE_SR_COC1_MASK                         0x40000000u
#define AFE_SR_COC1_SHIFT                        30
#define AFE_SR_COC0_MASK                         0x80000000u
#define AFE_SR_COC0_SHIFT                        31

/*!
 * @}
 */ /* end of group AFE_Register_Masks */


/* AFE - Peripheral instance base addresses */
/** Peripheral AFE base pointer */
#define AFE_BASE_PTR                             ((AFE_MemMapPtr)0x40030000u)
/** Array initializer of AFE peripheral base pointers */
#define AFE_BASE_PTRS                            { AFE_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- AFE - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AFE_Register_Accessor_Macros AFE - Register accessor macros
 * @{
 */


/* AFE - Register instance definitions */
/* AFE */
#define AFE_CH0_CFR                              AFE_CFR_REG(AFE_BASE_PTR,0)
#define AFE_CH1_CFR                              AFE_CFR_REG(AFE_BASE_PTR,1)
#define AFE_CH2_CFR                              AFE_CFR_REG(AFE_BASE_PTR,2)
#define AFE_CH3_CFR                              AFE_CFR_REG(AFE_BASE_PTR,3)
#define AFE_CR                                   AFE_CR_REG(AFE_BASE_PTR)
#define AFE_CKR                                  AFE_CKR_REG(AFE_BASE_PTR)
#define AFE_DI                                   AFE_DI_REG(AFE_BASE_PTR)
#define AFE_CH0_DR                               AFE_DR_REG(AFE_BASE_PTR,0)
#define AFE_CH1_DR                               AFE_DR_REG(AFE_BASE_PTR,1)
#define AFE_CH2_DR                               AFE_DR_REG(AFE_BASE_PTR,2)
#define AFE_CH3_DR                               AFE_DR_REG(AFE_BASE_PTR,3)
#define AFE_CH0_RR                               AFE_RR_REG(AFE_BASE_PTR,0)
#define AFE_CH1_RR                               AFE_RR_REG(AFE_BASE_PTR,1)
#define AFE_CH2_RR                               AFE_RR_REG(AFE_BASE_PTR,2)
#define AFE_CH3_RR                               AFE_RR_REG(AFE_BASE_PTR,3)
#define AFE_SR                                   AFE_SR_REG(AFE_BASE_PTR)

/* AFE - Register array accessors */
#define AFE_CFR(index)                           AFE_CFR_REG(AFE_BASE_PTR,index)
#define AFE_DR(index)                            AFE_DR_REG(AFE_BASE_PTR,index)
#define AFE_RR(index)                            AFE_RR_REG(AFE_BASE_PTR,index)

/*!
 * @}
 */ /* end of group AFE_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group AFE_Peripheral */


/* ----------------------------------------------------------------------------
   -- AIPS
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AIPS_Peripheral AIPS
 * @{
 */

/** AIPS - Peripheral register structure */
typedef struct AIPS_MemMap {
  uint8_t RESERVED_0[32];
  uint32_t PACRA;                                  /**< Peripheral Access Control Register, offset: 0x20 */
  uint32_t PACRB;                                  /**< Peripheral Access Control Register, offset: 0x24 */
  uint8_t RESERVED_1[24];
  uint32_t PACRE;                                  /**< Peripheral Access Control Register, offset: 0x40 */
  uint32_t PACRF;                                  /**< Peripheral Access Control Register, offset: 0x44 */
  uint32_t PACRG;                                  /**< Peripheral Access Control Register, offset: 0x48 */
  uint32_t PACRH;                                  /**< Peripheral Access Control Register, offset: 0x4C */
  uint32_t PACRI;                                  /**< Peripheral Access Control Register, offset: 0x50 */
  uint32_t PACRJ;                                  /**< Peripheral Access Control Register, offset: 0x54 */
  uint32_t PACRK;                                  /**< Peripheral Access Control Register, offset: 0x58 */
  uint32_t PACRL;                                  /**< Peripheral Access Control Register, offset: 0x5C */
  uint32_t PACRM;                                  /**< Peripheral Access Control Register, offset: 0x60 */
  uint32_t PACRN;                                  /**< Peripheral Access Control Register, offset: 0x64 */
  uint32_t PACRO;                                  /**< Peripheral Access Control Register, offset: 0x68 */
  uint32_t PACRP;                                  /**< Peripheral Access Control Register, offset: 0x6C */
} volatile *AIPS_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- AIPS - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AIPS_Register_Accessor_Macros AIPS - Register accessor macros
 * @{
 */


/* AIPS - Register accessors */
#define AIPS_PACRA_REG(base)                     ((base)->PACRA)
#define AIPS_PACRB_REG(base)                     ((base)->PACRB)
#define AIPS_PACRE_REG(base)                     ((base)->PACRE)
#define AIPS_PACRF_REG(base)                     ((base)->PACRF)
#define AIPS_PACRG_REG(base)                     ((base)->PACRG)
#define AIPS_PACRH_REG(base)                     ((base)->PACRH)
#define AIPS_PACRI_REG(base)                     ((base)->PACRI)
#define AIPS_PACRJ_REG(base)                     ((base)->PACRJ)
#define AIPS_PACRK_REG(base)                     ((base)->PACRK)
#define AIPS_PACRL_REG(base)                     ((base)->PACRL)
#define AIPS_PACRM_REG(base)                     ((base)->PACRM)
#define AIPS_PACRN_REG(base)                     ((base)->PACRN)
#define AIPS_PACRO_REG(base)                     ((base)->PACRO)
#define AIPS_PACRP_REG(base)                     ((base)->PACRP)

/*!
 * @}
 */ /* end of group AIPS_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- AIPS Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AIPS_Register_Masks AIPS Register Masks
 * @{
 */

/* PACRA Bit Fields */
#define AIPS_PACRA_AC7_MASK                      0x7u
#define AIPS_PACRA_AC7_SHIFT                     0
#define AIPS_PACRA_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRA_AC7_SHIFT))&AIPS_PACRA_AC7_MASK)
#define AIPS_PACRA_RO7_MASK                      0x8u
#define AIPS_PACRA_RO7_SHIFT                     3
#define AIPS_PACRA_AC6_MASK                      0x70u
#define AIPS_PACRA_AC6_SHIFT                     4
#define AIPS_PACRA_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRA_AC6_SHIFT))&AIPS_PACRA_AC6_MASK)
#define AIPS_PACRA_RO6_MASK                      0x80u
#define AIPS_PACRA_RO6_SHIFT                     7
#define AIPS_PACRA_AC5_MASK                      0x700u
#define AIPS_PACRA_AC5_SHIFT                     8
#define AIPS_PACRA_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRA_AC5_SHIFT))&AIPS_PACRA_AC5_MASK)
#define AIPS_PACRA_RO5_MASK                      0x800u
#define AIPS_PACRA_RO5_SHIFT                     11
#define AIPS_PACRA_AC4_MASK                      0x7000u
#define AIPS_PACRA_AC4_SHIFT                     12
#define AIPS_PACRA_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRA_AC4_SHIFT))&AIPS_PACRA_AC4_MASK)
#define AIPS_PACRA_RO4_MASK                      0x8000u
#define AIPS_PACRA_RO4_SHIFT                     15
#define AIPS_PACRA_AC3_MASK                      0x70000u
#define AIPS_PACRA_AC3_SHIFT                     16
#define AIPS_PACRA_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRA_AC3_SHIFT))&AIPS_PACRA_AC3_MASK)
#define AIPS_PACRA_RO3_MASK                      0x80000u
#define AIPS_PACRA_RO3_SHIFT                     19
#define AIPS_PACRA_AC2_MASK                      0x700000u
#define AIPS_PACRA_AC2_SHIFT                     20
#define AIPS_PACRA_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRA_AC2_SHIFT))&AIPS_PACRA_AC2_MASK)
#define AIPS_PACRA_RO2_MASK                      0x800000u
#define AIPS_PACRA_RO2_SHIFT                     23
#define AIPS_PACRA_AC1_MASK                      0x7000000u
#define AIPS_PACRA_AC1_SHIFT                     24
#define AIPS_PACRA_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRA_AC1_SHIFT))&AIPS_PACRA_AC1_MASK)
#define AIPS_PACRA_RO1_MASK                      0x8000000u
#define AIPS_PACRA_RO1_SHIFT                     27
#define AIPS_PACRA_AC0_MASK                      0x70000000u
#define AIPS_PACRA_AC0_SHIFT                     28
#define AIPS_PACRA_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRA_AC0_SHIFT))&AIPS_PACRA_AC0_MASK)
#define AIPS_PACRA_RO0_MASK                      0x80000000u
#define AIPS_PACRA_RO0_SHIFT                     31
/* PACRB Bit Fields */
#define AIPS_PACRB_AC7_MASK                      0x7u
#define AIPS_PACRB_AC7_SHIFT                     0
#define AIPS_PACRB_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRB_AC7_SHIFT))&AIPS_PACRB_AC7_MASK)
#define AIPS_PACRB_RO7_MASK                      0x8u
#define AIPS_PACRB_RO7_SHIFT                     3
#define AIPS_PACRB_AC6_MASK                      0x70u
#define AIPS_PACRB_AC6_SHIFT                     4
#define AIPS_PACRB_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRB_AC6_SHIFT))&AIPS_PACRB_AC6_MASK)
#define AIPS_PACRB_RO6_MASK                      0x80u
#define AIPS_PACRB_RO6_SHIFT                     7
#define AIPS_PACRB_AC5_MASK                      0x700u
#define AIPS_PACRB_AC5_SHIFT                     8
#define AIPS_PACRB_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRB_AC5_SHIFT))&AIPS_PACRB_AC5_MASK)
#define AIPS_PACRB_RO5_MASK                      0x800u
#define AIPS_PACRB_RO5_SHIFT                     11
#define AIPS_PACRB_AC4_MASK                      0x7000u
#define AIPS_PACRB_AC4_SHIFT                     12
#define AIPS_PACRB_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRB_AC4_SHIFT))&AIPS_PACRB_AC4_MASK)
#define AIPS_PACRB_RO4_MASK                      0x8000u
#define AIPS_PACRB_RO4_SHIFT                     15
#define AIPS_PACRB_AC3_MASK                      0x70000u
#define AIPS_PACRB_AC3_SHIFT                     16
#define AIPS_PACRB_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRB_AC3_SHIFT))&AIPS_PACRB_AC3_MASK)
#define AIPS_PACRB_RO3_MASK                      0x80000u
#define AIPS_PACRB_RO3_SHIFT                     19
#define AIPS_PACRB_AC2_MASK                      0x700000u
#define AIPS_PACRB_AC2_SHIFT                     20
#define AIPS_PACRB_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRB_AC2_SHIFT))&AIPS_PACRB_AC2_MASK)
#define AIPS_PACRB_RO2_MASK                      0x800000u
#define AIPS_PACRB_RO2_SHIFT                     23
#define AIPS_PACRB_AC1_MASK                      0x7000000u
#define AIPS_PACRB_AC1_SHIFT                     24
#define AIPS_PACRB_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRB_AC1_SHIFT))&AIPS_PACRB_AC1_MASK)
#define AIPS_PACRB_RO1_MASK                      0x8000000u
#define AIPS_PACRB_RO1_SHIFT                     27
#define AIPS_PACRB_AC0_MASK                      0x70000000u
#define AIPS_PACRB_AC0_SHIFT                     28
#define AIPS_PACRB_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRB_AC0_SHIFT))&AIPS_PACRB_AC0_MASK)
#define AIPS_PACRB_RO0_MASK                      0x80000000u
#define AIPS_PACRB_RO0_SHIFT                     31
/* PACRE Bit Fields */
#define AIPS_PACRE_AC7_MASK                      0x7u
#define AIPS_PACRE_AC7_SHIFT                     0
#define AIPS_PACRE_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRE_AC7_SHIFT))&AIPS_PACRE_AC7_MASK)
#define AIPS_PACRE_RO7_MASK                      0x8u
#define AIPS_PACRE_RO7_SHIFT                     3
#define AIPS_PACRE_AC6_MASK                      0x70u
#define AIPS_PACRE_AC6_SHIFT                     4
#define AIPS_PACRE_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRE_AC6_SHIFT))&AIPS_PACRE_AC6_MASK)
#define AIPS_PACRE_RO6_MASK                      0x80u
#define AIPS_PACRE_RO6_SHIFT                     7
#define AIPS_PACRE_AC5_MASK                      0x700u
#define AIPS_PACRE_AC5_SHIFT                     8
#define AIPS_PACRE_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRE_AC5_SHIFT))&AIPS_PACRE_AC5_MASK)
#define AIPS_PACRE_RO5_MASK                      0x800u
#define AIPS_PACRE_RO5_SHIFT                     11
#define AIPS_PACRE_AC4_MASK                      0x7000u
#define AIPS_PACRE_AC4_SHIFT                     12
#define AIPS_PACRE_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRE_AC4_SHIFT))&AIPS_PACRE_AC4_MASK)
#define AIPS_PACRE_RO4_MASK                      0x8000u
#define AIPS_PACRE_RO4_SHIFT                     15
#define AIPS_PACRE_AC3_MASK                      0x70000u
#define AIPS_PACRE_AC3_SHIFT                     16
#define AIPS_PACRE_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRE_AC3_SHIFT))&AIPS_PACRE_AC3_MASK)
#define AIPS_PACRE_RO3_MASK                      0x80000u
#define AIPS_PACRE_RO3_SHIFT                     19
#define AIPS_PACRE_AC2_MASK                      0x700000u
#define AIPS_PACRE_AC2_SHIFT                     20
#define AIPS_PACRE_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRE_AC2_SHIFT))&AIPS_PACRE_AC2_MASK)
#define AIPS_PACRE_RO2_MASK                      0x800000u
#define AIPS_PACRE_RO2_SHIFT                     23
#define AIPS_PACRE_AC1_MASK                      0x7000000u
#define AIPS_PACRE_AC1_SHIFT                     24
#define AIPS_PACRE_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRE_AC1_SHIFT))&AIPS_PACRE_AC1_MASK)
#define AIPS_PACRE_RO1_MASK                      0x8000000u
#define AIPS_PACRE_RO1_SHIFT                     27
#define AIPS_PACRE_AC0_MASK                      0x70000000u
#define AIPS_PACRE_AC0_SHIFT                     28
#define AIPS_PACRE_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRE_AC0_SHIFT))&AIPS_PACRE_AC0_MASK)
#define AIPS_PACRE_RO0_MASK                      0x80000000u
#define AIPS_PACRE_RO0_SHIFT                     31
/* PACRF Bit Fields */
#define AIPS_PACRF_AC7_MASK                      0x7u
#define AIPS_PACRF_AC7_SHIFT                     0
#define AIPS_PACRF_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRF_AC7_SHIFT))&AIPS_PACRF_AC7_MASK)
#define AIPS_PACRF_RO7_MASK                      0x8u
#define AIPS_PACRF_RO7_SHIFT                     3
#define AIPS_PACRF_AC6_MASK                      0x70u
#define AIPS_PACRF_AC6_SHIFT                     4
#define AIPS_PACRF_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRF_AC6_SHIFT))&AIPS_PACRF_AC6_MASK)
#define AIPS_PACRF_RO6_MASK                      0x80u
#define AIPS_PACRF_RO6_SHIFT                     7
#define AIPS_PACRF_AC5_MASK                      0x700u
#define AIPS_PACRF_AC5_SHIFT                     8
#define AIPS_PACRF_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRF_AC5_SHIFT))&AIPS_PACRF_AC5_MASK)
#define AIPS_PACRF_RO5_MASK                      0x800u
#define AIPS_PACRF_RO5_SHIFT                     11
#define AIPS_PACRF_AC4_MASK                      0x7000u
#define AIPS_PACRF_AC4_SHIFT                     12
#define AIPS_PACRF_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRF_AC4_SHIFT))&AIPS_PACRF_AC4_MASK)
#define AIPS_PACRF_RO4_MASK                      0x8000u
#define AIPS_PACRF_RO4_SHIFT                     15
#define AIPS_PACRF_AC3_MASK                      0x70000u
#define AIPS_PACRF_AC3_SHIFT                     16
#define AIPS_PACRF_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRF_AC3_SHIFT))&AIPS_PACRF_AC3_MASK)
#define AIPS_PACRF_RO3_MASK                      0x80000u
#define AIPS_PACRF_RO3_SHIFT                     19
#define AIPS_PACRF_AC2_MASK                      0x700000u
#define AIPS_PACRF_AC2_SHIFT                     20
#define AIPS_PACRF_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRF_AC2_SHIFT))&AIPS_PACRF_AC2_MASK)
#define AIPS_PACRF_RO2_MASK                      0x800000u
#define AIPS_PACRF_RO2_SHIFT                     23
#define AIPS_PACRF_AC1_MASK                      0x7000000u
#define AIPS_PACRF_AC1_SHIFT                     24
#define AIPS_PACRF_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRF_AC1_SHIFT))&AIPS_PACRF_AC1_MASK)
#define AIPS_PACRF_RO1_MASK                      0x8000000u
#define AIPS_PACRF_RO1_SHIFT                     27
#define AIPS_PACRF_AC0_MASK                      0x70000000u
#define AIPS_PACRF_AC0_SHIFT                     28
#define AIPS_PACRF_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRF_AC0_SHIFT))&AIPS_PACRF_AC0_MASK)
#define AIPS_PACRF_RO0_MASK                      0x80000000u
#define AIPS_PACRF_RO0_SHIFT                     31
/* PACRG Bit Fields */
#define AIPS_PACRG_AC7_MASK                      0x7u
#define AIPS_PACRG_AC7_SHIFT                     0
#define AIPS_PACRG_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRG_AC7_SHIFT))&AIPS_PACRG_AC7_MASK)
#define AIPS_PACRG_RO7_MASK                      0x8u
#define AIPS_PACRG_RO7_SHIFT                     3
#define AIPS_PACRG_AC6_MASK                      0x70u
#define AIPS_PACRG_AC6_SHIFT                     4
#define AIPS_PACRG_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRG_AC6_SHIFT))&AIPS_PACRG_AC6_MASK)
#define AIPS_PACRG_RO6_MASK                      0x80u
#define AIPS_PACRG_RO6_SHIFT                     7
#define AIPS_PACRG_AC5_MASK                      0x700u
#define AIPS_PACRG_AC5_SHIFT                     8
#define AIPS_PACRG_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRG_AC5_SHIFT))&AIPS_PACRG_AC5_MASK)
#define AIPS_PACRG_RO5_MASK                      0x800u
#define AIPS_PACRG_RO5_SHIFT                     11
#define AIPS_PACRG_AC4_MASK                      0x7000u
#define AIPS_PACRG_AC4_SHIFT                     12
#define AIPS_PACRG_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRG_AC4_SHIFT))&AIPS_PACRG_AC4_MASK)
#define AIPS_PACRG_RO4_MASK                      0x8000u
#define AIPS_PACRG_RO4_SHIFT                     15
#define AIPS_PACRG_AC3_MASK                      0x70000u
#define AIPS_PACRG_AC3_SHIFT                     16
#define AIPS_PACRG_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRG_AC3_SHIFT))&AIPS_PACRG_AC3_MASK)
#define AIPS_PACRG_RO3_MASK                      0x80000u
#define AIPS_PACRG_RO3_SHIFT                     19
#define AIPS_PACRG_AC2_MASK                      0x700000u
#define AIPS_PACRG_AC2_SHIFT                     20
#define AIPS_PACRG_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRG_AC2_SHIFT))&AIPS_PACRG_AC2_MASK)
#define AIPS_PACRG_RO2_MASK                      0x800000u
#define AIPS_PACRG_RO2_SHIFT                     23
#define AIPS_PACRG_AC1_MASK                      0x7000000u
#define AIPS_PACRG_AC1_SHIFT                     24
#define AIPS_PACRG_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRG_AC1_SHIFT))&AIPS_PACRG_AC1_MASK)
#define AIPS_PACRG_RO1_MASK                      0x8000000u
#define AIPS_PACRG_RO1_SHIFT                     27
#define AIPS_PACRG_AC0_MASK                      0x70000000u
#define AIPS_PACRG_AC0_SHIFT                     28
#define AIPS_PACRG_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRG_AC0_SHIFT))&AIPS_PACRG_AC0_MASK)
#define AIPS_PACRG_RO0_MASK                      0x80000000u
#define AIPS_PACRG_RO0_SHIFT                     31
/* PACRH Bit Fields */
#define AIPS_PACRH_AC7_MASK                      0x7u
#define AIPS_PACRH_AC7_SHIFT                     0
#define AIPS_PACRH_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRH_AC7_SHIFT))&AIPS_PACRH_AC7_MASK)
#define AIPS_PACRH_RO7_MASK                      0x8u
#define AIPS_PACRH_RO7_SHIFT                     3
#define AIPS_PACRH_AC6_MASK                      0x70u
#define AIPS_PACRH_AC6_SHIFT                     4
#define AIPS_PACRH_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRH_AC6_SHIFT))&AIPS_PACRH_AC6_MASK)
#define AIPS_PACRH_RO6_MASK                      0x80u
#define AIPS_PACRH_RO6_SHIFT                     7
#define AIPS_PACRH_AC5_MASK                      0x700u
#define AIPS_PACRH_AC5_SHIFT                     8
#define AIPS_PACRH_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRH_AC5_SHIFT))&AIPS_PACRH_AC5_MASK)
#define AIPS_PACRH_RO5_MASK                      0x800u
#define AIPS_PACRH_RO5_SHIFT                     11
#define AIPS_PACRH_AC4_MASK                      0x7000u
#define AIPS_PACRH_AC4_SHIFT                     12
#define AIPS_PACRH_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRH_AC4_SHIFT))&AIPS_PACRH_AC4_MASK)
#define AIPS_PACRH_RO4_MASK                      0x8000u
#define AIPS_PACRH_RO4_SHIFT                     15
#define AIPS_PACRH_AC3_MASK                      0x70000u
#define AIPS_PACRH_AC3_SHIFT                     16
#define AIPS_PACRH_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRH_AC3_SHIFT))&AIPS_PACRH_AC3_MASK)
#define AIPS_PACRH_RO3_MASK                      0x80000u
#define AIPS_PACRH_RO3_SHIFT                     19
#define AIPS_PACRH_AC2_MASK                      0x700000u
#define AIPS_PACRH_AC2_SHIFT                     20
#define AIPS_PACRH_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRH_AC2_SHIFT))&AIPS_PACRH_AC2_MASK)
#define AIPS_PACRH_RO2_MASK                      0x800000u
#define AIPS_PACRH_RO2_SHIFT                     23
#define AIPS_PACRH_AC1_MASK                      0x7000000u
#define AIPS_PACRH_AC1_SHIFT                     24
#define AIPS_PACRH_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRH_AC1_SHIFT))&AIPS_PACRH_AC1_MASK)
#define AIPS_PACRH_RO1_MASK                      0x8000000u
#define AIPS_PACRH_RO1_SHIFT                     27
#define AIPS_PACRH_AC0_MASK                      0x70000000u
#define AIPS_PACRH_AC0_SHIFT                     28
#define AIPS_PACRH_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRH_AC0_SHIFT))&AIPS_PACRH_AC0_MASK)
#define AIPS_PACRH_RO0_MASK                      0x80000000u
#define AIPS_PACRH_RO0_SHIFT                     31
/* PACRI Bit Fields */
#define AIPS_PACRI_AC7_MASK                      0x7u
#define AIPS_PACRI_AC7_SHIFT                     0
#define AIPS_PACRI_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRI_AC7_SHIFT))&AIPS_PACRI_AC7_MASK)
#define AIPS_PACRI_RO7_MASK                      0x8u
#define AIPS_PACRI_RO7_SHIFT                     3
#define AIPS_PACRI_AC6_MASK                      0x70u
#define AIPS_PACRI_AC6_SHIFT                     4
#define AIPS_PACRI_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRI_AC6_SHIFT))&AIPS_PACRI_AC6_MASK)
#define AIPS_PACRI_RO6_MASK                      0x80u
#define AIPS_PACRI_RO6_SHIFT                     7
#define AIPS_PACRI_AC5_MASK                      0x700u
#define AIPS_PACRI_AC5_SHIFT                     8
#define AIPS_PACRI_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRI_AC5_SHIFT))&AIPS_PACRI_AC5_MASK)
#define AIPS_PACRI_RO5_MASK                      0x800u
#define AIPS_PACRI_RO5_SHIFT                     11
#define AIPS_PACRI_AC4_MASK                      0x7000u
#define AIPS_PACRI_AC4_SHIFT                     12
#define AIPS_PACRI_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRI_AC4_SHIFT))&AIPS_PACRI_AC4_MASK)
#define AIPS_PACRI_RO4_MASK                      0x8000u
#define AIPS_PACRI_RO4_SHIFT                     15
#define AIPS_PACRI_AC3_MASK                      0x70000u
#define AIPS_PACRI_AC3_SHIFT                     16
#define AIPS_PACRI_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRI_AC3_SHIFT))&AIPS_PACRI_AC3_MASK)
#define AIPS_PACRI_RO3_MASK                      0x80000u
#define AIPS_PACRI_RO3_SHIFT                     19
#define AIPS_PACRI_AC2_MASK                      0x700000u
#define AIPS_PACRI_AC2_SHIFT                     20
#define AIPS_PACRI_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRI_AC2_SHIFT))&AIPS_PACRI_AC2_MASK)
#define AIPS_PACRI_RO2_MASK                      0x800000u
#define AIPS_PACRI_RO2_SHIFT                     23
#define AIPS_PACRI_AC1_MASK                      0x7000000u
#define AIPS_PACRI_AC1_SHIFT                     24
#define AIPS_PACRI_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRI_AC1_SHIFT))&AIPS_PACRI_AC1_MASK)
#define AIPS_PACRI_RO1_MASK                      0x8000000u
#define AIPS_PACRI_RO1_SHIFT                     27
#define AIPS_PACRI_AC0_MASK                      0x70000000u
#define AIPS_PACRI_AC0_SHIFT                     28
#define AIPS_PACRI_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRI_AC0_SHIFT))&AIPS_PACRI_AC0_MASK)
#define AIPS_PACRI_RO0_MASK                      0x80000000u
#define AIPS_PACRI_RO0_SHIFT                     31
/* PACRJ Bit Fields */
#define AIPS_PACRJ_AC7_MASK                      0x7u
#define AIPS_PACRJ_AC7_SHIFT                     0
#define AIPS_PACRJ_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRJ_AC7_SHIFT))&AIPS_PACRJ_AC7_MASK)
#define AIPS_PACRJ_RO7_MASK                      0x8u
#define AIPS_PACRJ_RO7_SHIFT                     3
#define AIPS_PACRJ_AC6_MASK                      0x70u
#define AIPS_PACRJ_AC6_SHIFT                     4
#define AIPS_PACRJ_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRJ_AC6_SHIFT))&AIPS_PACRJ_AC6_MASK)
#define AIPS_PACRJ_RO6_MASK                      0x80u
#define AIPS_PACRJ_RO6_SHIFT                     7
#define AIPS_PACRJ_AC5_MASK                      0x700u
#define AIPS_PACRJ_AC5_SHIFT                     8
#define AIPS_PACRJ_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRJ_AC5_SHIFT))&AIPS_PACRJ_AC5_MASK)
#define AIPS_PACRJ_RO5_MASK                      0x800u
#define AIPS_PACRJ_RO5_SHIFT                     11
#define AIPS_PACRJ_AC4_MASK                      0x7000u
#define AIPS_PACRJ_AC4_SHIFT                     12
#define AIPS_PACRJ_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRJ_AC4_SHIFT))&AIPS_PACRJ_AC4_MASK)
#define AIPS_PACRJ_RO4_MASK                      0x8000u
#define AIPS_PACRJ_RO4_SHIFT                     15
#define AIPS_PACRJ_AC3_MASK                      0x70000u
#define AIPS_PACRJ_AC3_SHIFT                     16
#define AIPS_PACRJ_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRJ_AC3_SHIFT))&AIPS_PACRJ_AC3_MASK)
#define AIPS_PACRJ_RO3_MASK                      0x80000u
#define AIPS_PACRJ_RO3_SHIFT                     19
#define AIPS_PACRJ_AC2_MASK                      0x700000u
#define AIPS_PACRJ_AC2_SHIFT                     20
#define AIPS_PACRJ_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRJ_AC2_SHIFT))&AIPS_PACRJ_AC2_MASK)
#define AIPS_PACRJ_RO2_MASK                      0x800000u
#define AIPS_PACRJ_RO2_SHIFT                     23
#define AIPS_PACRJ_AC1_MASK                      0x7000000u
#define AIPS_PACRJ_AC1_SHIFT                     24
#define AIPS_PACRJ_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRJ_AC1_SHIFT))&AIPS_PACRJ_AC1_MASK)
#define AIPS_PACRJ_RO1_MASK                      0x8000000u
#define AIPS_PACRJ_RO1_SHIFT                     27
#define AIPS_PACRJ_AC0_MASK                      0x70000000u
#define AIPS_PACRJ_AC0_SHIFT                     28
#define AIPS_PACRJ_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRJ_AC0_SHIFT))&AIPS_PACRJ_AC0_MASK)
#define AIPS_PACRJ_RO0_MASK                      0x80000000u
#define AIPS_PACRJ_RO0_SHIFT                     31
/* PACRK Bit Fields */
#define AIPS_PACRK_AC7_MASK                      0x7u
#define AIPS_PACRK_AC7_SHIFT                     0
#define AIPS_PACRK_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRK_AC7_SHIFT))&AIPS_PACRK_AC7_MASK)
#define AIPS_PACRK_RO7_MASK                      0x8u
#define AIPS_PACRK_RO7_SHIFT                     3
#define AIPS_PACRK_AC6_MASK                      0x70u
#define AIPS_PACRK_AC6_SHIFT                     4
#define AIPS_PACRK_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRK_AC6_SHIFT))&AIPS_PACRK_AC6_MASK)
#define AIPS_PACRK_RO6_MASK                      0x80u
#define AIPS_PACRK_RO6_SHIFT                     7
#define AIPS_PACRK_AC5_MASK                      0x700u
#define AIPS_PACRK_AC5_SHIFT                     8
#define AIPS_PACRK_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRK_AC5_SHIFT))&AIPS_PACRK_AC5_MASK)
#define AIPS_PACRK_RO5_MASK                      0x800u
#define AIPS_PACRK_RO5_SHIFT                     11
#define AIPS_PACRK_AC4_MASK                      0x7000u
#define AIPS_PACRK_AC4_SHIFT                     12
#define AIPS_PACRK_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRK_AC4_SHIFT))&AIPS_PACRK_AC4_MASK)
#define AIPS_PACRK_RO4_MASK                      0x8000u
#define AIPS_PACRK_RO4_SHIFT                     15
#define AIPS_PACRK_AC3_MASK                      0x70000u
#define AIPS_PACRK_AC3_SHIFT                     16
#define AIPS_PACRK_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRK_AC3_SHIFT))&AIPS_PACRK_AC3_MASK)
#define AIPS_PACRK_RO3_MASK                      0x80000u
#define AIPS_PACRK_RO3_SHIFT                     19
#define AIPS_PACRK_AC2_MASK                      0x700000u
#define AIPS_PACRK_AC2_SHIFT                     20
#define AIPS_PACRK_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRK_AC2_SHIFT))&AIPS_PACRK_AC2_MASK)
#define AIPS_PACRK_RO2_MASK                      0x800000u
#define AIPS_PACRK_RO2_SHIFT                     23
#define AIPS_PACRK_AC1_MASK                      0x7000000u
#define AIPS_PACRK_AC1_SHIFT                     24
#define AIPS_PACRK_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRK_AC1_SHIFT))&AIPS_PACRK_AC1_MASK)
#define AIPS_PACRK_RO1_MASK                      0x8000000u
#define AIPS_PACRK_RO1_SHIFT                     27
#define AIPS_PACRK_AC0_MASK                      0x70000000u
#define AIPS_PACRK_AC0_SHIFT                     28
#define AIPS_PACRK_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRK_AC0_SHIFT))&AIPS_PACRK_AC0_MASK)
#define AIPS_PACRK_RO0_MASK                      0x80000000u
#define AIPS_PACRK_RO0_SHIFT                     31
/* PACRL Bit Fields */
#define AIPS_PACRL_AC7_MASK                      0x7u
#define AIPS_PACRL_AC7_SHIFT                     0
#define AIPS_PACRL_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRL_AC7_SHIFT))&AIPS_PACRL_AC7_MASK)
#define AIPS_PACRL_RO7_MASK                      0x8u
#define AIPS_PACRL_RO7_SHIFT                     3
#define AIPS_PACRL_AC6_MASK                      0x70u
#define AIPS_PACRL_AC6_SHIFT                     4
#define AIPS_PACRL_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRL_AC6_SHIFT))&AIPS_PACRL_AC6_MASK)
#define AIPS_PACRL_RO6_MASK                      0x80u
#define AIPS_PACRL_RO6_SHIFT                     7
#define AIPS_PACRL_AC5_MASK                      0x700u
#define AIPS_PACRL_AC5_SHIFT                     8
#define AIPS_PACRL_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRL_AC5_SHIFT))&AIPS_PACRL_AC5_MASK)
#define AIPS_PACRL_RO5_MASK                      0x800u
#define AIPS_PACRL_RO5_SHIFT                     11
#define AIPS_PACRL_AC4_MASK                      0x7000u
#define AIPS_PACRL_AC4_SHIFT                     12
#define AIPS_PACRL_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRL_AC4_SHIFT))&AIPS_PACRL_AC4_MASK)
#define AIPS_PACRL_RO4_MASK                      0x8000u
#define AIPS_PACRL_RO4_SHIFT                     15
#define AIPS_PACRL_AC3_MASK                      0x70000u
#define AIPS_PACRL_AC3_SHIFT                     16
#define AIPS_PACRL_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRL_AC3_SHIFT))&AIPS_PACRL_AC3_MASK)
#define AIPS_PACRL_RO3_MASK                      0x80000u
#define AIPS_PACRL_RO3_SHIFT                     19
#define AIPS_PACRL_AC2_MASK                      0x700000u
#define AIPS_PACRL_AC2_SHIFT                     20
#define AIPS_PACRL_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRL_AC2_SHIFT))&AIPS_PACRL_AC2_MASK)
#define AIPS_PACRL_RO2_MASK                      0x800000u
#define AIPS_PACRL_RO2_SHIFT                     23
#define AIPS_PACRL_AC1_MASK                      0x7000000u
#define AIPS_PACRL_AC1_SHIFT                     24
#define AIPS_PACRL_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRL_AC1_SHIFT))&AIPS_PACRL_AC1_MASK)
#define AIPS_PACRL_RO1_MASK                      0x8000000u
#define AIPS_PACRL_RO1_SHIFT                     27
#define AIPS_PACRL_AC0_MASK                      0x70000000u
#define AIPS_PACRL_AC0_SHIFT                     28
#define AIPS_PACRL_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRL_AC0_SHIFT))&AIPS_PACRL_AC0_MASK)
#define AIPS_PACRL_RO0_MASK                      0x80000000u
#define AIPS_PACRL_RO0_SHIFT                     31
/* PACRM Bit Fields */
#define AIPS_PACRM_AC7_MASK                      0x7u
#define AIPS_PACRM_AC7_SHIFT                     0
#define AIPS_PACRM_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRM_AC7_SHIFT))&AIPS_PACRM_AC7_MASK)
#define AIPS_PACRM_RO7_MASK                      0x8u
#define AIPS_PACRM_RO7_SHIFT                     3
#define AIPS_PACRM_AC6_MASK                      0x70u
#define AIPS_PACRM_AC6_SHIFT                     4
#define AIPS_PACRM_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRM_AC6_SHIFT))&AIPS_PACRM_AC6_MASK)
#define AIPS_PACRM_RO6_MASK                      0x80u
#define AIPS_PACRM_RO6_SHIFT                     7
#define AIPS_PACRM_AC5_MASK                      0x700u
#define AIPS_PACRM_AC5_SHIFT                     8
#define AIPS_PACRM_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRM_AC5_SHIFT))&AIPS_PACRM_AC5_MASK)
#define AIPS_PACRM_RO5_MASK                      0x800u
#define AIPS_PACRM_RO5_SHIFT                     11
#define AIPS_PACRM_AC4_MASK                      0x7000u
#define AIPS_PACRM_AC4_SHIFT                     12
#define AIPS_PACRM_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRM_AC4_SHIFT))&AIPS_PACRM_AC4_MASK)
#define AIPS_PACRM_RO4_MASK                      0x8000u
#define AIPS_PACRM_RO4_SHIFT                     15
#define AIPS_PACRM_AC3_MASK                      0x70000u
#define AIPS_PACRM_AC3_SHIFT                     16
#define AIPS_PACRM_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRM_AC3_SHIFT))&AIPS_PACRM_AC3_MASK)
#define AIPS_PACRM_RO3_MASK                      0x80000u
#define AIPS_PACRM_RO3_SHIFT                     19
#define AIPS_PACRM_AC2_MASK                      0x700000u
#define AIPS_PACRM_AC2_SHIFT                     20
#define AIPS_PACRM_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRM_AC2_SHIFT))&AIPS_PACRM_AC2_MASK)
#define AIPS_PACRM_RO2_MASK                      0x800000u
#define AIPS_PACRM_RO2_SHIFT                     23
#define AIPS_PACRM_AC1_MASK                      0x7000000u
#define AIPS_PACRM_AC1_SHIFT                     24
#define AIPS_PACRM_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRM_AC1_SHIFT))&AIPS_PACRM_AC1_MASK)
#define AIPS_PACRM_RO1_MASK                      0x8000000u
#define AIPS_PACRM_RO1_SHIFT                     27
#define AIPS_PACRM_AC0_MASK                      0x70000000u
#define AIPS_PACRM_AC0_SHIFT                     28
#define AIPS_PACRM_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRM_AC0_SHIFT))&AIPS_PACRM_AC0_MASK)
#define AIPS_PACRM_RO0_MASK                      0x80000000u
#define AIPS_PACRM_RO0_SHIFT                     31
/* PACRN Bit Fields */
#define AIPS_PACRN_AC7_MASK                      0x7u
#define AIPS_PACRN_AC7_SHIFT                     0
#define AIPS_PACRN_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRN_AC7_SHIFT))&AIPS_PACRN_AC7_MASK)
#define AIPS_PACRN_RO7_MASK                      0x8u
#define AIPS_PACRN_RO7_SHIFT                     3
#define AIPS_PACRN_AC6_MASK                      0x70u
#define AIPS_PACRN_AC6_SHIFT                     4
#define AIPS_PACRN_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRN_AC6_SHIFT))&AIPS_PACRN_AC6_MASK)
#define AIPS_PACRN_RO6_MASK                      0x80u
#define AIPS_PACRN_RO6_SHIFT                     7
#define AIPS_PACRN_AC5_MASK                      0x700u
#define AIPS_PACRN_AC5_SHIFT                     8
#define AIPS_PACRN_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRN_AC5_SHIFT))&AIPS_PACRN_AC5_MASK)
#define AIPS_PACRN_RO5_MASK                      0x800u
#define AIPS_PACRN_RO5_SHIFT                     11
#define AIPS_PACRN_AC4_MASK                      0x7000u
#define AIPS_PACRN_AC4_SHIFT                     12
#define AIPS_PACRN_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRN_AC4_SHIFT))&AIPS_PACRN_AC4_MASK)
#define AIPS_PACRN_RO4_MASK                      0x8000u
#define AIPS_PACRN_RO4_SHIFT                     15
#define AIPS_PACRN_AC3_MASK                      0x70000u
#define AIPS_PACRN_AC3_SHIFT                     16
#define AIPS_PACRN_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRN_AC3_SHIFT))&AIPS_PACRN_AC3_MASK)
#define AIPS_PACRN_RO3_MASK                      0x80000u
#define AIPS_PACRN_RO3_SHIFT                     19
#define AIPS_PACRN_AC2_MASK                      0x700000u
#define AIPS_PACRN_AC2_SHIFT                     20
#define AIPS_PACRN_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRN_AC2_SHIFT))&AIPS_PACRN_AC2_MASK)
#define AIPS_PACRN_RO2_MASK                      0x800000u
#define AIPS_PACRN_RO2_SHIFT                     23
#define AIPS_PACRN_AC1_MASK                      0x7000000u
#define AIPS_PACRN_AC1_SHIFT                     24
#define AIPS_PACRN_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRN_AC1_SHIFT))&AIPS_PACRN_AC1_MASK)
#define AIPS_PACRN_RO1_MASK                      0x8000000u
#define AIPS_PACRN_RO1_SHIFT                     27
#define AIPS_PACRN_AC0_MASK                      0x70000000u
#define AIPS_PACRN_AC0_SHIFT                     28
#define AIPS_PACRN_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRN_AC0_SHIFT))&AIPS_PACRN_AC0_MASK)
#define AIPS_PACRN_RO0_MASK                      0x80000000u
#define AIPS_PACRN_RO0_SHIFT                     31
/* PACRO Bit Fields */
#define AIPS_PACRO_AC7_MASK                      0x7u
#define AIPS_PACRO_AC7_SHIFT                     0
#define AIPS_PACRO_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRO_AC7_SHIFT))&AIPS_PACRO_AC7_MASK)
#define AIPS_PACRO_RO7_MASK                      0x8u
#define AIPS_PACRO_RO7_SHIFT                     3
#define AIPS_PACRO_AC6_MASK                      0x70u
#define AIPS_PACRO_AC6_SHIFT                     4
#define AIPS_PACRO_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRO_AC6_SHIFT))&AIPS_PACRO_AC6_MASK)
#define AIPS_PACRO_RO6_MASK                      0x80u
#define AIPS_PACRO_RO6_SHIFT                     7
#define AIPS_PACRO_AC5_MASK                      0x700u
#define AIPS_PACRO_AC5_SHIFT                     8
#define AIPS_PACRO_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRO_AC5_SHIFT))&AIPS_PACRO_AC5_MASK)
#define AIPS_PACRO_RO5_MASK                      0x800u
#define AIPS_PACRO_RO5_SHIFT                     11
#define AIPS_PACRO_AC4_MASK                      0x7000u
#define AIPS_PACRO_AC4_SHIFT                     12
#define AIPS_PACRO_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRO_AC4_SHIFT))&AIPS_PACRO_AC4_MASK)
#define AIPS_PACRO_RO4_MASK                      0x8000u
#define AIPS_PACRO_RO4_SHIFT                     15
#define AIPS_PACRO_AC3_MASK                      0x70000u
#define AIPS_PACRO_AC3_SHIFT                     16
#define AIPS_PACRO_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRO_AC3_SHIFT))&AIPS_PACRO_AC3_MASK)
#define AIPS_PACRO_RO3_MASK                      0x80000u
#define AIPS_PACRO_RO3_SHIFT                     19
#define AIPS_PACRO_AC2_MASK                      0x700000u
#define AIPS_PACRO_AC2_SHIFT                     20
#define AIPS_PACRO_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRO_AC2_SHIFT))&AIPS_PACRO_AC2_MASK)
#define AIPS_PACRO_RO2_MASK                      0x800000u
#define AIPS_PACRO_RO2_SHIFT                     23
#define AIPS_PACRO_AC1_MASK                      0x7000000u
#define AIPS_PACRO_AC1_SHIFT                     24
#define AIPS_PACRO_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRO_AC1_SHIFT))&AIPS_PACRO_AC1_MASK)
#define AIPS_PACRO_RO1_MASK                      0x8000000u
#define AIPS_PACRO_RO1_SHIFT                     27
#define AIPS_PACRO_AC0_MASK                      0x70000000u
#define AIPS_PACRO_AC0_SHIFT                     28
#define AIPS_PACRO_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRO_AC0_SHIFT))&AIPS_PACRO_AC0_MASK)
#define AIPS_PACRO_RO0_MASK                      0x80000000u
#define AIPS_PACRO_RO0_SHIFT                     31
/* PACRP Bit Fields */
#define AIPS_PACRP_AC7_MASK                      0x7u
#define AIPS_PACRP_AC7_SHIFT                     0
#define AIPS_PACRP_AC7(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRP_AC7_SHIFT))&AIPS_PACRP_AC7_MASK)
#define AIPS_PACRP_RO7_MASK                      0x8u
#define AIPS_PACRP_RO7_SHIFT                     3
#define AIPS_PACRP_AC6_MASK                      0x70u
#define AIPS_PACRP_AC6_SHIFT                     4
#define AIPS_PACRP_AC6(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRP_AC6_SHIFT))&AIPS_PACRP_AC6_MASK)
#define AIPS_PACRP_RO6_MASK                      0x80u
#define AIPS_PACRP_RO6_SHIFT                     7
#define AIPS_PACRP_AC5_MASK                      0x700u
#define AIPS_PACRP_AC5_SHIFT                     8
#define AIPS_PACRP_AC5(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRP_AC5_SHIFT))&AIPS_PACRP_AC5_MASK)
#define AIPS_PACRP_RO5_MASK                      0x800u
#define AIPS_PACRP_RO5_SHIFT                     11
#define AIPS_PACRP_AC4_MASK                      0x7000u
#define AIPS_PACRP_AC4_SHIFT                     12
#define AIPS_PACRP_AC4(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRP_AC4_SHIFT))&AIPS_PACRP_AC4_MASK)
#define AIPS_PACRP_RO4_MASK                      0x8000u
#define AIPS_PACRP_RO4_SHIFT                     15
#define AIPS_PACRP_AC3_MASK                      0x70000u
#define AIPS_PACRP_AC3_SHIFT                     16
#define AIPS_PACRP_AC3(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRP_AC3_SHIFT))&AIPS_PACRP_AC3_MASK)
#define AIPS_PACRP_RO3_MASK                      0x80000u
#define AIPS_PACRP_RO3_SHIFT                     19
#define AIPS_PACRP_AC2_MASK                      0x700000u
#define AIPS_PACRP_AC2_SHIFT                     20
#define AIPS_PACRP_AC2(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRP_AC2_SHIFT))&AIPS_PACRP_AC2_MASK)
#define AIPS_PACRP_RO2_MASK                      0x800000u
#define AIPS_PACRP_RO2_SHIFT                     23
#define AIPS_PACRP_AC1_MASK                      0x7000000u
#define AIPS_PACRP_AC1_SHIFT                     24
#define AIPS_PACRP_AC1(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRP_AC1_SHIFT))&AIPS_PACRP_AC1_MASK)
#define AIPS_PACRP_RO1_MASK                      0x8000000u
#define AIPS_PACRP_RO1_SHIFT                     27
#define AIPS_PACRP_AC0_MASK                      0x70000000u
#define AIPS_PACRP_AC0_SHIFT                     28
#define AIPS_PACRP_AC0(x)                        (((uint32_t)(((uint32_t)(x))<<AIPS_PACRP_AC0_SHIFT))&AIPS_PACRP_AC0_MASK)
#define AIPS_PACRP_RO0_MASK                      0x80000000u
#define AIPS_PACRP_RO0_SHIFT                     31

/*!
 * @}
 */ /* end of group AIPS_Register_Masks */


/* AIPS - Peripheral instance base addresses */
/** Peripheral AIPS base pointer */
#define AIPS_BASE_PTR                            ((AIPS_MemMapPtr)0x40000000u)
/** Array initializer of AIPS peripheral base pointers */
#define AIPS_BASE_PTRS                           { AIPS_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- AIPS - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AIPS_Register_Accessor_Macros AIPS - Register accessor macros
 * @{
 */


/* AIPS - Register instance definitions */
/* AIPS */
#define AIPS_PACRA                               AIPS_PACRA_REG(AIPS_BASE_PTR)
#define AIPS_PACRB                               AIPS_PACRB_REG(AIPS_BASE_PTR)
#define AIPS_PACRE                               AIPS_PACRE_REG(AIPS_BASE_PTR)
#define AIPS_PACRF                               AIPS_PACRF_REG(AIPS_BASE_PTR)
#define AIPS_PACRG                               AIPS_PACRG_REG(AIPS_BASE_PTR)
#define AIPS_PACRH                               AIPS_PACRH_REG(AIPS_BASE_PTR)
#define AIPS_PACRI                               AIPS_PACRI_REG(AIPS_BASE_PTR)
#define AIPS_PACRJ                               AIPS_PACRJ_REG(AIPS_BASE_PTR)
#define AIPS_PACRK                               AIPS_PACRK_REG(AIPS_BASE_PTR)
#define AIPS_PACRL                               AIPS_PACRL_REG(AIPS_BASE_PTR)
#define AIPS_PACRM                               AIPS_PACRM_REG(AIPS_BASE_PTR)
#define AIPS_PACRN                               AIPS_PACRN_REG(AIPS_BASE_PTR)
#define AIPS_PACRO                               AIPS_PACRO_REG(AIPS_BASE_PTR)
#define AIPS_PACRP                               AIPS_PACRP_REG(AIPS_BASE_PTR)

/*!
 * @}
 */ /* end of group AIPS_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group AIPS_Peripheral */


/* ----------------------------------------------------------------------------
   -- BP
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BP_Peripheral BP
 * @{
 */

/** BP - Peripheral register structure */
typedef struct BP_MemMap {
  uint32_t CTRL;                                   /**< FlashPatch Control Register, offset: 0x0 */
  uint8_t RESERVED_0[4];
  uint32_t COMP[2];                                /**< FlashPatch Comparator Register 0..FlashPatch Comparator Register 1, array offset: 0x8, array step: 0x4 */
  uint8_t RESERVED_1[4032];
  uint32_t PID4;                                   /**< Peripheral Identification Register 4., offset: 0xFD0 */
  uint32_t PID5;                                   /**< Peripheral Identification Register 5., offset: 0xFD4 */
  uint32_t PID6;                                   /**< Peripheral Identification Register 6., offset: 0xFD8 */
  uint32_t PID7;                                   /**< Peripheral Identification Register 7., offset: 0xFDC */
  uint32_t PID0;                                   /**< Peripheral Identification Register 0., offset: 0xFE0 */
  uint32_t PID1;                                   /**< Peripheral Identification Register 1., offset: 0xFE4 */
  uint32_t PID2;                                   /**< Peripheral Identification Register 2., offset: 0xFE8 */
  uint32_t PID3;                                   /**< Peripheral Identification Register 3., offset: 0xFEC */
  uint32_t CID0;                                   /**< Component Identification Register 0., offset: 0xFF0 */
  uint32_t CID1;                                   /**< Component Identification Register 1., offset: 0xFF4 */
  uint32_t CID2;                                   /**< Component Identification Register 2., offset: 0xFF8 */
  uint32_t CID3;                                   /**< Component Identification Register 3., offset: 0xFFC */
} volatile *BP_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- BP - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BP_Register_Accessor_Macros BP - Register accessor macros
 * @{
 */


/* BP - Register accessors */
#define BP_CTRL_REG(base)                        ((base)->CTRL)
#define BP_COMP_REG(base,index)                  ((base)->COMP[index])
#define BP_PID4_REG(base)                        ((base)->PID4)
#define BP_PID5_REG(base)                        ((base)->PID5)
#define BP_PID6_REG(base)                        ((base)->PID6)
#define BP_PID7_REG(base)                        ((base)->PID7)
#define BP_PID0_REG(base)                        ((base)->PID0)
#define BP_PID1_REG(base)                        ((base)->PID1)
#define BP_PID2_REG(base)                        ((base)->PID2)
#define BP_PID3_REG(base)                        ((base)->PID3)
#define BP_CID0_REG(base)                        ((base)->CID0)
#define BP_CID1_REG(base)                        ((base)->CID1)
#define BP_CID2_REG(base)                        ((base)->CID2)
#define BP_CID3_REG(base)                        ((base)->CID3)

/*!
 * @}
 */ /* end of group BP_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- BP Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BP_Register_Masks BP Register Masks
 * @{
 */


/*!
 * @}
 */ /* end of group BP_Register_Masks */


/* BP - Peripheral instance base addresses */
/** Peripheral BP base pointer */
#define BP_BASE_PTR                              ((BP_MemMapPtr)0xE0002000u)
/** Array initializer of BP peripheral base pointers */
#define BP_BASE_PTRS                             { BP_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- BP - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BP_Register_Accessor_Macros BP - Register accessor macros
 * @{
 */


/* BP - Register instance definitions */
/* BP */
#define BP_CTRL                                  BP_CTRL_REG(BP_BASE_PTR)
#define BP_COMP0                                 BP_COMP_REG(BP_BASE_PTR,0)
#define BP_COMP1                                 BP_COMP_REG(BP_BASE_PTR,1)
#define BP_PID4                                  BP_PID4_REG(BP_BASE_PTR)
#define BP_PID5                                  BP_PID5_REG(BP_BASE_PTR)
#define BP_PID6                                  BP_PID6_REG(BP_BASE_PTR)
#define BP_PID7                                  BP_PID7_REG(BP_BASE_PTR)
#define BP_PID0                                  BP_PID0_REG(BP_BASE_PTR)
#define BP_PID1                                  BP_PID1_REG(BP_BASE_PTR)
#define BP_PID2                                  BP_PID2_REG(BP_BASE_PTR)
#define BP_PID3                                  BP_PID3_REG(BP_BASE_PTR)
#define BP_CID0                                  BP_CID0_REG(BP_BASE_PTR)
#define BP_CID1                                  BP_CID1_REG(BP_BASE_PTR)
#define BP_CID2                                  BP_CID2_REG(BP_BASE_PTR)
#define BP_CID3                                  BP_CID3_REG(BP_BASE_PTR)

/* BP - Register array accessors */
#define BP_COMP(index)                           BP_COMP_REG(BP_BASE_PTR,index)

/*!
 * @}
 */ /* end of group BP_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group BP_Peripheral */


/* ----------------------------------------------------------------------------
   -- CMP
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Peripheral CMP
 * @{
 */

/** CMP - Peripheral register structure */
typedef struct CMP_MemMap {
  uint8_t CR0;                                     /**< CMP Control Register 0, offset: 0x0 */
  uint8_t CR1;                                     /**< CMP Control Register 1, offset: 0x1 */
  uint8_t FPR;                                     /**< CMP Filter Period Register, offset: 0x2 */
  uint8_t SCR;                                     /**< CMP Status and Control Register, offset: 0x3 */
  uint8_t DACCR;                                   /**< DAC Control Register, offset: 0x4 */
  uint8_t MUXCR;                                   /**< MUX Control Register, offset: 0x5 */
} volatile *CMP_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- CMP - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Register_Accessor_Macros CMP - Register accessor macros
 * @{
 */


/* CMP - Register accessors */
#define CMP_CR0_REG(base)                        ((base)->CR0)
#define CMP_CR1_REG(base)                        ((base)->CR1)
#define CMP_FPR_REG(base)                        ((base)->FPR)
#define CMP_SCR_REG(base)                        ((base)->SCR)
#define CMP_DACCR_REG(base)                      ((base)->DACCR)
#define CMP_MUXCR_REG(base)                      ((base)->MUXCR)

/*!
 * @}
 */ /* end of group CMP_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- CMP Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Register_Masks CMP Register Masks
 * @{
 */

/* CR0 Bit Fields */
#define CMP_CR0_HYSTCTR_MASK                     0x3u
#define CMP_CR0_HYSTCTR_SHIFT                    0
#define CMP_CR0_HYSTCTR(x)                       (((uint8_t)(((uint8_t)(x))<<CMP_CR0_HYSTCTR_SHIFT))&CMP_CR0_HYSTCTR_MASK)
#define CMP_CR0_FILTER_CNT_MASK                  0x70u
#define CMP_CR0_FILTER_CNT_SHIFT                 4
#define CMP_CR0_FILTER_CNT(x)                    (((uint8_t)(((uint8_t)(x))<<CMP_CR0_FILTER_CNT_SHIFT))&CMP_CR0_FILTER_CNT_MASK)
/* CR1 Bit Fields */
#define CMP_CR1_EN_MASK                          0x1u
#define CMP_CR1_EN_SHIFT                         0
#define CMP_CR1_OPE_MASK                         0x2u
#define CMP_CR1_OPE_SHIFT                        1
#define CMP_CR1_COS_MASK                         0x4u
#define CMP_CR1_COS_SHIFT                        2
#define CMP_CR1_INV_MASK                         0x8u
#define CMP_CR1_INV_SHIFT                        3
#define CMP_CR1_PMODE_MASK                       0x10u
#define CMP_CR1_PMODE_SHIFT                      4
#define CMP_CR1_TRIGM_MASK                       0x20u
#define CMP_CR1_TRIGM_SHIFT                      5
#define CMP_CR1_WE_MASK                          0x40u
#define CMP_CR1_WE_SHIFT                         6
#define CMP_CR1_SE_MASK                          0x80u
#define CMP_CR1_SE_SHIFT                         7
/* FPR Bit Fields */
#define CMP_FPR_FILT_PER_MASK                    0xFFu
#define CMP_FPR_FILT_PER_SHIFT                   0
#define CMP_FPR_FILT_PER(x)                      (((uint8_t)(((uint8_t)(x))<<CMP_FPR_FILT_PER_SHIFT))&CMP_FPR_FILT_PER_MASK)
/* SCR Bit Fields */
#define CMP_SCR_COUT_MASK                        0x1u
#define CMP_SCR_COUT_SHIFT                       0
#define CMP_SCR_CFF_MASK                         0x2u
#define CMP_SCR_CFF_SHIFT                        1
#define CMP_SCR_CFR_MASK                         0x4u
#define CMP_SCR_CFR_SHIFT                        2
#define CMP_SCR_IEF_MASK                         0x8u
#define CMP_SCR_IEF_SHIFT                        3
#define CMP_SCR_IER_MASK                         0x10u
#define CMP_SCR_IER_SHIFT                        4
#define CMP_SCR_DMAEN_MASK                       0x40u
#define CMP_SCR_DMAEN_SHIFT                      6
/* DACCR Bit Fields */
#define CMP_DACCR_VOSEL_MASK                     0x3Fu
#define CMP_DACCR_VOSEL_SHIFT                    0
#define CMP_DACCR_VOSEL(x)                       (((uint8_t)(((uint8_t)(x))<<CMP_DACCR_VOSEL_SHIFT))&CMP_DACCR_VOSEL_MASK)
#define CMP_DACCR_VRSEL_MASK                     0x40u
#define CMP_DACCR_VRSEL_SHIFT                    6
#define CMP_DACCR_DACEN_MASK                     0x80u
#define CMP_DACCR_DACEN_SHIFT                    7
/* MUXCR Bit Fields */
#define CMP_MUXCR_MSEL_MASK                      0x7u
#define CMP_MUXCR_MSEL_SHIFT                     0
#define CMP_MUXCR_MSEL(x)                        (((uint8_t)(((uint8_t)(x))<<CMP_MUXCR_MSEL_SHIFT))&CMP_MUXCR_MSEL_MASK)
#define CMP_MUXCR_PSEL_MASK                      0x38u
#define CMP_MUXCR_PSEL_SHIFT                     3
#define CMP_MUXCR_PSEL(x)                        (((uint8_t)(((uint8_t)(x))<<CMP_MUXCR_PSEL_SHIFT))&CMP_MUXCR_PSEL_MASK)

/*!
 * @}
 */ /* end of group CMP_Register_Masks */


/* CMP - Peripheral instance base addresses */
/** Peripheral CMP0 base pointer */
#define CMP0_BASE_PTR                            ((CMP_MemMapPtr)0x40072000u)
/** Peripheral CMP1 base pointer */
#define CMP1_BASE_PTR                            ((CMP_MemMapPtr)0x40072008u)
/** Array initializer of CMP peripheral base pointers */
#define CMP_BASE_PTRS                            { CMP0_BASE_PTR, CMP1_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- CMP - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Register_Accessor_Macros CMP - Register accessor macros
 * @{
 */


/* CMP - Register instance definitions */
/* CMP0 */
#define CMP0_CR0                                 CMP_CR0_REG(CMP0_BASE_PTR)
#define CMP0_CR1                                 CMP_CR1_REG(CMP0_BASE_PTR)
#define CMP0_FPR                                 CMP_FPR_REG(CMP0_BASE_PTR)
#define CMP0_SCR                                 CMP_SCR_REG(CMP0_BASE_PTR)
#define CMP0_DACCR                               CMP_DACCR_REG(CMP0_BASE_PTR)
#define CMP0_MUXCR                               CMP_MUXCR_REG(CMP0_BASE_PTR)
/* CMP1 */
#define CMP1_CR0                                 CMP_CR0_REG(CMP1_BASE_PTR)
#define CMP1_CR1                                 CMP_CR1_REG(CMP1_BASE_PTR)
#define CMP1_FPR                                 CMP_FPR_REG(CMP1_BASE_PTR)
#define CMP1_SCR                                 CMP_SCR_REG(CMP1_BASE_PTR)
#define CMP1_DACCR                               CMP_DACCR_REG(CMP1_BASE_PTR)
#define CMP1_MUXCR                               CMP_MUXCR_REG(CMP1_BASE_PTR)

/*!
 * @}
 */ /* end of group CMP_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group CMP_Peripheral */


/* ----------------------------------------------------------------------------
   -- CRC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CRC_Peripheral CRC
 * @{
 */

/** CRC - Peripheral register structure */
typedef struct CRC_MemMap {
  union {                                          /* offset: 0x0 */
    struct {                                         /* offset: 0x0 */
      uint16_t CRCL;                                   /**< CRC_CRCL register., offset: 0x0 */
      uint16_t CRCH;                                   /**< CRC_CRCH register., offset: 0x2 */
    } ACCESS16BIT;
    uint32_t CRC;                                    /**< CRC Data register, offset: 0x0 */
    struct {                                         /* offset: 0x0 */
      uint8_t CRCLL;                                   /**< CRC_CRCLL register., offset: 0x0 */
      uint8_t CRCLU;                                   /**< CRC_CRCLU register., offset: 0x1 */
      uint8_t CRCHL;                                   /**< CRC_CRCHL register., offset: 0x2 */
      uint8_t CRCHU;                                   /**< CRC_CRCHU register., offset: 0x3 */
    } ACCESS8BIT;
  };
  union {                                          /* offset: 0x4 */
    struct {                                         /* offset: 0x4 */
      uint16_t GPOLYL;                                 /**< CRC_GPOLYL register., offset: 0x4 */
      uint16_t GPOLYH;                                 /**< CRC_GPOLYH register., offset: 0x6 */
    } GPOLY_ACCESS16BIT;
    uint32_t GPOLY;                                  /**< CRC Polynomial register, offset: 0x4 */
    struct {                                         /* offset: 0x4 */
      uint8_t GPOLYLL;                                 /**< CRC_GPOLYLL register., offset: 0x4 */
      uint8_t GPOLYLU;                                 /**< CRC_GPOLYLU register., offset: 0x5 */
      uint8_t GPOLYHL;                                 /**< CRC_GPOLYHL register., offset: 0x6 */
      uint8_t GPOLYHU;                                 /**< CRC_GPOLYHU register., offset: 0x7 */
    } GPOLY_ACCESS8BIT;
  };
  union {                                          /* offset: 0x8 */
    uint32_t CTRL;                                   /**< CRC Control register, offset: 0x8 */
    struct {                                         /* offset: 0x8 */
      uint8_t RESERVED_0[3];
      uint8_t CTRLHU;                                  /**< CRC_CTRLHU register., offset: 0xB */
    } CTRL_ACCESS8BIT;
  };
} volatile *CRC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- CRC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CRC_Register_Accessor_Macros CRC - Register accessor macros
 * @{
 */


/* CRC - Register accessors */
#define CRC_CRCL_REG(base)                       ((base)->ACCESS16BIT.CRCL)
#define CRC_CRCH_REG(base)                       ((base)->ACCESS16BIT.CRCH)
#define CRC_CRC_REG(base)                        ((base)->CRC)
#define CRC_CRCLL_REG(base)                      ((base)->ACCESS8BIT.CRCLL)
#define CRC_CRCLU_REG(base)                      ((base)->ACCESS8BIT.CRCLU)
#define CRC_CRCHL_REG(base)                      ((base)->ACCESS8BIT.CRCHL)
#define CRC_CRCHU_REG(base)                      ((base)->ACCESS8BIT.CRCHU)
#define CRC_GPOLYL_REG(base)                     ((base)->GPOLY_ACCESS16BIT.GPOLYL)
#define CRC_GPOLYH_REG(base)                     ((base)->GPOLY_ACCESS16BIT.GPOLYH)
#define CRC_GPOLY_REG(base)                      ((base)->GPOLY)
#define CRC_GPOLYLL_REG(base)                    ((base)->GPOLY_ACCESS8BIT.GPOLYLL)
#define CRC_GPOLYLU_REG(base)                    ((base)->GPOLY_ACCESS8BIT.GPOLYLU)
#define CRC_GPOLYHL_REG(base)                    ((base)->GPOLY_ACCESS8BIT.GPOLYHL)
#define CRC_GPOLYHU_REG(base)                    ((base)->GPOLY_ACCESS8BIT.GPOLYHU)
#define CRC_CTRL_REG(base)                       ((base)->CTRL)
#define CRC_CTRLHU_REG(base)                     ((base)->CTRL_ACCESS8BIT.CTRLHU)

/*!
 * @}
 */ /* end of group CRC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- CRC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CRC_Register_Masks CRC Register Masks
 * @{
 */

/* CRCL Bit Fields */
#define CRC_CRCL_CRCL_MASK                       0xFFFFu
#define CRC_CRCL_CRCL_SHIFT                      0
#define CRC_CRCL_CRCL(x)                         (((uint16_t)(((uint16_t)(x))<<CRC_CRCL_CRCL_SHIFT))&CRC_CRCL_CRCL_MASK)
/* CRCH Bit Fields */
#define CRC_CRCH_CRCH_MASK                       0xFFFFu
#define CRC_CRCH_CRCH_SHIFT                      0
#define CRC_CRCH_CRCH(x)                         (((uint16_t)(((uint16_t)(x))<<CRC_CRCH_CRCH_SHIFT))&CRC_CRCH_CRCH_MASK)
/* CRC Bit Fields */
#define CRC_CRC_LL_MASK                          0xFFu
#define CRC_CRC_LL_SHIFT                         0
#define CRC_CRC_LL(x)                            (((uint32_t)(((uint32_t)(x))<<CRC_CRC_LL_SHIFT))&CRC_CRC_LL_MASK)
#define CRC_CRC_LU_MASK                          0xFF00u
#define CRC_CRC_LU_SHIFT                         8
#define CRC_CRC_LU(x)                            (((uint32_t)(((uint32_t)(x))<<CRC_CRC_LU_SHIFT))&CRC_CRC_LU_MASK)
#define CRC_CRC_HL_MASK                          0xFF0000u
#define CRC_CRC_HL_SHIFT                         16
#define CRC_CRC_HL(x)                            (((uint32_t)(((uint32_t)(x))<<CRC_CRC_HL_SHIFT))&CRC_CRC_HL_MASK)
#define CRC_CRC_HU_MASK                          0xFF000000u
#define CRC_CRC_HU_SHIFT                         24
#define CRC_CRC_HU(x)                            (((uint32_t)(((uint32_t)(x))<<CRC_CRC_HU_SHIFT))&CRC_CRC_HU_MASK)
/* CRCLL Bit Fields */
#define CRC_CRCLL_CRCLL_MASK                     0xFFu
#define CRC_CRCLL_CRCLL_SHIFT                    0
#define CRC_CRCLL_CRCLL(x)                       (((uint8_t)(((uint8_t)(x))<<CRC_CRCLL_CRCLL_SHIFT))&CRC_CRCLL_CRCLL_MASK)
/* CRCLU Bit Fields */
#define CRC_CRCLU_CRCLU_MASK                     0xFFu
#define CRC_CRCLU_CRCLU_SHIFT                    0
#define CRC_CRCLU_CRCLU(x)                       (((uint8_t)(((uint8_t)(x))<<CRC_CRCLU_CRCLU_SHIFT))&CRC_CRCLU_CRCLU_MASK)
/* CRCHL Bit Fields */
#define CRC_CRCHL_CRCHL_MASK                     0xFFu
#define CRC_CRCHL_CRCHL_SHIFT                    0
#define CRC_CRCHL_CRCHL(x)                       (((uint8_t)(((uint8_t)(x))<<CRC_CRCHL_CRCHL_SHIFT))&CRC_CRCHL_CRCHL_MASK)
/* CRCHU Bit Fields */
#define CRC_CRCHU_CRCHU_MASK                     0xFFu
#define CRC_CRCHU_CRCHU_SHIFT                    0
#define CRC_CRCHU_CRCHU(x)                       (((uint8_t)(((uint8_t)(x))<<CRC_CRCHU_CRCHU_SHIFT))&CRC_CRCHU_CRCHU_MASK)
/* GPOLYL Bit Fields */
#define CRC_GPOLYL_GPOLYL_MASK                   0xFFFFu
#define CRC_GPOLYL_GPOLYL_SHIFT                  0
#define CRC_GPOLYL_GPOLYL(x)                     (((uint16_t)(((uint16_t)(x))<<CRC_GPOLYL_GPOLYL_SHIFT))&CRC_GPOLYL_GPOLYL_MASK)
/* GPOLYH Bit Fields */
#define CRC_GPOLYH_GPOLYH_MASK                   0xFFFFu
#define CRC_GPOLYH_GPOLYH_SHIFT                  0
#define CRC_GPOLYH_GPOLYH(x)                     (((uint16_t)(((uint16_t)(x))<<CRC_GPOLYH_GPOLYH_SHIFT))&CRC_GPOLYH_GPOLYH_MASK)
/* GPOLY Bit Fields */
#define CRC_GPOLY_LOW_MASK                       0xFFFFu
#define CRC_GPOLY_LOW_SHIFT                      0
#define CRC_GPOLY_LOW(x)                         (((uint32_t)(((uint32_t)(x))<<CRC_GPOLY_LOW_SHIFT))&CRC_GPOLY_LOW_MASK)
#define CRC_GPOLY_HIGH_MASK                      0xFFFF0000u
#define CRC_GPOLY_HIGH_SHIFT                     16
#define CRC_GPOLY_HIGH(x)                        (((uint32_t)(((uint32_t)(x))<<CRC_GPOLY_HIGH_SHIFT))&CRC_GPOLY_HIGH_MASK)
/* GPOLYLL Bit Fields */
#define CRC_GPOLYLL_GPOLYLL_MASK                 0xFFu
#define CRC_GPOLYLL_GPOLYLL_SHIFT                0
#define CRC_GPOLYLL_GPOLYLL(x)                   (((uint8_t)(((uint8_t)(x))<<CRC_GPOLYLL_GPOLYLL_SHIFT))&CRC_GPOLYLL_GPOLYLL_MASK)
/* GPOLYLU Bit Fields */
#define CRC_GPOLYLU_GPOLYLU_MASK                 0xFFu
#define CRC_GPOLYLU_GPOLYLU_SHIFT                0
#define CRC_GPOLYLU_GPOLYLU(x)                   (((uint8_t)(((uint8_t)(x))<<CRC_GPOLYLU_GPOLYLU_SHIFT))&CRC_GPOLYLU_GPOLYLU_MASK)
/* GPOLYHL Bit Fields */
#define CRC_GPOLYHL_GPOLYHL_MASK                 0xFFu
#define CRC_GPOLYHL_GPOLYHL_SHIFT                0
#define CRC_GPOLYHL_GPOLYHL(x)                   (((uint8_t)(((uint8_t)(x))<<CRC_GPOLYHL_GPOLYHL_SHIFT))&CRC_GPOLYHL_GPOLYHL_MASK)
/* GPOLYHU Bit Fields */
#define CRC_GPOLYHU_GPOLYHU_MASK                 0xFFu
#define CRC_GPOLYHU_GPOLYHU_SHIFT                0
#define CRC_GPOLYHU_GPOLYHU(x)                   (((uint8_t)(((uint8_t)(x))<<CRC_GPOLYHU_GPOLYHU_SHIFT))&CRC_GPOLYHU_GPOLYHU_MASK)
/* CTRL Bit Fields */
#define CRC_CTRL_TCRC_MASK                       0x1000000u
#define CRC_CTRL_TCRC_SHIFT                      24
#define CRC_CTRL_WAS_MASK                        0x2000000u
#define CRC_CTRL_WAS_SHIFT                       25
#define CRC_CTRL_FXOR_MASK                       0x4000000u
#define CRC_CTRL_FXOR_SHIFT                      26
#define CRC_CTRL_TOTR_MASK                       0x30000000u
#define CRC_CTRL_TOTR_SHIFT                      28
#define CRC_CTRL_TOTR(x)                         (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_TOTR_SHIFT))&CRC_CTRL_TOTR_MASK)
#define CRC_CTRL_TOT_MASK                        0xC0000000u
#define CRC_CTRL_TOT_SHIFT                       30
#define CRC_CTRL_TOT(x)                          (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_TOT_SHIFT))&CRC_CTRL_TOT_MASK)
/* CTRLHU Bit Fields */
#define CRC_CTRLHU_TCRC_MASK                     0x1u
#define CRC_CTRLHU_TCRC_SHIFT                    0
#define CRC_CTRLHU_WAS_MASK                      0x2u
#define CRC_CTRLHU_WAS_SHIFT                     1
#define CRC_CTRLHU_FXOR_MASK                     0x4u
#define CRC_CTRLHU_FXOR_SHIFT                    2
#define CRC_CTRLHU_TOTR_MASK                     0x30u
#define CRC_CTRLHU_TOTR_SHIFT                    4
#define CRC_CTRLHU_TOTR(x)                       (((uint8_t)(((uint8_t)(x))<<CRC_CTRLHU_TOTR_SHIFT))&CRC_CTRLHU_TOTR_MASK)
#define CRC_CTRLHU_TOT_MASK                      0xC0u
#define CRC_CTRLHU_TOT_SHIFT                     6
#define CRC_CTRLHU_TOT(x)                        (((uint8_t)(((uint8_t)(x))<<CRC_CTRLHU_TOT_SHIFT))&CRC_CTRLHU_TOT_MASK)

/*!
 * @}
 */ /* end of group CRC_Register_Masks */


/* CRC - Peripheral instance base addresses */
/** Peripheral CRC base pointer */
#define CRC_BASE_PTR                             ((CRC_MemMapPtr)0x40034000u)
/** Array initializer of CRC peripheral base pointers */
#define CRC_BASE_PTRS                            { CRC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- CRC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CRC_Register_Accessor_Macros CRC - Register accessor macros
 * @{
 */


/* CRC - Register instance definitions */
/* CRC */
#define CRC_CRCL                                 CRC_CRCL_REG(CRC_BASE_PTR)
#define CRC_CRCLL                                CRC_CRCLL_REG(CRC_BASE_PTR)
#define CRC_DATA                                 CRC_CRC_REG(CRC_BASE_PTR)
#define CRC_CRCLU                                CRC_CRCLU_REG(CRC_BASE_PTR)
#define CRC_CRCH                                 CRC_CRCH_REG(CRC_BASE_PTR)
#define CRC_CRCHL                                CRC_CRCHL_REG(CRC_BASE_PTR)
#define CRC_CRCHU                                CRC_CRCHU_REG(CRC_BASE_PTR)
#define CRC_GPOLY                                CRC_GPOLY_REG(CRC_BASE_PTR)
#define CRC_GPOLYL                               CRC_GPOLYL_REG(CRC_BASE_PTR)
#define CRC_GPOLYLL                              CRC_GPOLYLL_REG(CRC_BASE_PTR)
#define CRC_GPOLYLU                              CRC_GPOLYLU_REG(CRC_BASE_PTR)
#define CRC_GPOLYH                               CRC_GPOLYH_REG(CRC_BASE_PTR)
#define CRC_GPOLYHL                              CRC_GPOLYHL_REG(CRC_BASE_PTR)
#define CRC_GPOLYHU                              CRC_GPOLYHU_REG(CRC_BASE_PTR)
#define CRC_CTRL                                 CRC_CTRL_REG(CRC_BASE_PTR)
#define CRC_CTRLHU                               CRC_CTRLHU_REG(CRC_BASE_PTR)

/*!
 * @}
 */ /* end of group CRC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group CRC_Peripheral */


/* ----------------------------------------------------------------------------
   -- CoreDebug
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CoreDebug_Peripheral CoreDebug
 * @{
 */

/** CoreDebug - Peripheral register structure */
typedef struct CoreDebug_MemMap {
  union {                                          /* offset: 0x0 */
    uint32_t base_DHCSR_Read;                        /**< Debug Halting Control and Status Register, offset: 0x0 */
    uint32_t base_DHCSR_Write;                       /**< Debug Halting Control and Status Register, offset: 0x0 */
  };
  uint32_t base_DCRSR;                             /**< Debug Core Register Selector Register, offset: 0x4 */
  uint32_t base_DCRDR;                             /**< Debug Core Register Data Register, offset: 0x8 */
  uint32_t base_DEMCR;                             /**< Debug Exception and Monitor Control Register, offset: 0xC */
} volatile *CoreDebug_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- CoreDebug - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CoreDebug_Register_Accessor_Macros CoreDebug - Register accessor macros
 * @{
 */


/* CoreDebug - Register accessors */
#define CoreDebug_base_DHCSR_Read_REG(base)      ((base)->base_DHCSR_Read)
#define CoreDebug_base_DHCSR_Write_REG(base)     ((base)->base_DHCSR_Write)
#define CoreDebug_base_DCRSR_REG(base)           ((base)->base_DCRSR)
#define CoreDebug_base_DCRDR_REG(base)           ((base)->base_DCRDR)
#define CoreDebug_base_DEMCR_REG(base)           ((base)->base_DEMCR)

/*!
 * @}
 */ /* end of group CoreDebug_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- CoreDebug Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CoreDebug_Register_Masks CoreDebug Register Masks
 * @{
 */


/*!
 * @}
 */ /* end of group CoreDebug_Register_Masks */


/* CoreDebug - Peripheral instance base addresses */
/** Peripheral CoreDebug base pointer */
#define CoreDebug_BASE_PTR                       ((CoreDebug_MemMapPtr)0xE000EDF0u)
/** Array initializer of CoreDebug peripheral base pointers */
#define CoreDebug_BASE_PTRS                      { CoreDebug_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- CoreDebug - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CoreDebug_Register_Accessor_Macros CoreDebug - Register accessor macros
 * @{
 */


/* CoreDebug - Register instance definitions */
/* CoreDebug */
#define DHCSR_Read                               CoreDebug_base_DHCSR_Read_REG(CoreDebug_BASE_PTR)
#define DHCSR_Write                              CoreDebug_base_DHCSR_Write_REG(CoreDebug_BASE_PTR)
#define DCRSR                                    CoreDebug_base_DCRSR_REG(CoreDebug_BASE_PTR)
#define DCRDR                                    CoreDebug_base_DCRDR_REG(CoreDebug_BASE_PTR)
#define DEMCR                                    CoreDebug_base_DEMCR_REG(CoreDebug_BASE_PTR)

/*!
 * @}
 */ /* end of group CoreDebug_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group CoreDebug_Peripheral */


/* ----------------------------------------------------------------------------
   -- DMA
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMA_Peripheral DMA
 * @{
 */

/** DMA - Peripheral register structure */
typedef struct DMA_MemMap {
  uint8_t RESERVED_0[256];
  struct {                                         /* offset: 0x100, array step: 0x10 */
    uint32_t SAR;                                    /**< Source Address Register, array offset: 0x100, array step: 0x10 */
    uint32_t DAR;                                    /**< Destination Address Register, array offset: 0x104, array step: 0x10 */
    union {                                          /* offset: 0x108, array step: 0x10 */
      uint32_t DSR_BCR;                                /**< DMA Status Register / Byte Count Register, array offset: 0x108, array step: 0x10 */
      struct {                                         /* offset: 0x108, array step: 0x10 */
        uint8_t RESERVED_0[3];
        uint8_t DSR;                                     /**< DMA_DSR0 register...DMA_DSR3 register., array offset: 0x10B, array step: 0x10 */
      } DMA_DSR_ACCESS8BIT;
    };
    uint32_t DCR;                                    /**< DMA Control Register, array offset: 0x10C, array step: 0x10 */
  } DMA[4];
} volatile *DMA_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- DMA - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMA_Register_Accessor_Macros DMA - Register accessor macros
 * @{
 */


/* DMA - Register accessors */
#define DMA_SAR_REG(base,index)                  ((base)->DMA[index].SAR)
#define DMA_DAR_REG(base,index)                  ((base)->DMA[index].DAR)
#define DMA_DSR_BCR_REG(base,index)              ((base)->DMA[index].DSR_BCR)
#define DMA_DSR_REG(base,index)                  ((base)->DMA[index].DMA_DSR_ACCESS8BIT.DSR)
#define DMA_DCR_REG(base,index)                  ((base)->DMA[index].DCR)

/*!
 * @}
 */ /* end of group DMA_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- DMA Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMA_Register_Masks DMA Register Masks
 * @{
 */

/* SAR Bit Fields */
#define DMA_SAR_SAR_MASK                         0xFFFFFFFFu
#define DMA_SAR_SAR_SHIFT                        0
#define DMA_SAR_SAR(x)                           (((uint32_t)(((uint32_t)(x))<<DMA_SAR_SAR_SHIFT))&DMA_SAR_SAR_MASK)
/* DAR Bit Fields */
#define DMA_DAR_DAR_MASK                         0xFFFFFFFFu
#define DMA_DAR_DAR_SHIFT                        0
#define DMA_DAR_DAR(x)                           (((uint32_t)(((uint32_t)(x))<<DMA_DAR_DAR_SHIFT))&DMA_DAR_DAR_MASK)
/* DSR_BCR Bit Fields */
#define DMA_DSR_BCR_BCR_MASK                     0xFFFFFFu
#define DMA_DSR_BCR_BCR_SHIFT                    0
#define DMA_DSR_BCR_BCR(x)                       (((uint32_t)(((uint32_t)(x))<<DMA_DSR_BCR_BCR_SHIFT))&DMA_DSR_BCR_BCR_MASK)
#define DMA_DSR_BCR_DONE_MASK                    0x1000000u
#define DMA_DSR_BCR_DONE_SHIFT                   24
#define DMA_DSR_BCR_BSY_MASK                     0x2000000u
#define DMA_DSR_BCR_BSY_SHIFT                    25
#define DMA_DSR_BCR_REQ_MASK                     0x4000000u
#define DMA_DSR_BCR_REQ_SHIFT                    26
#define DMA_DSR_BCR_BED_MASK                     0x10000000u
#define DMA_DSR_BCR_BED_SHIFT                    28
#define DMA_DSR_BCR_BES_MASK                     0x20000000u
#define DMA_DSR_BCR_BES_SHIFT                    29
#define DMA_DSR_BCR_CE_MASK                      0x40000000u
#define DMA_DSR_BCR_CE_SHIFT                     30
/* DCR Bit Fields */
#define DMA_DCR_LCH2_MASK                        0x3u
#define DMA_DCR_LCH2_SHIFT                       0
#define DMA_DCR_LCH2(x)                          (((uint32_t)(((uint32_t)(x))<<DMA_DCR_LCH2_SHIFT))&DMA_DCR_LCH2_MASK)
#define DMA_DCR_LCH1_MASK                        0xCu
#define DMA_DCR_LCH1_SHIFT                       2
#define DMA_DCR_LCH1(x)                          (((uint32_t)(((uint32_t)(x))<<DMA_DCR_LCH1_SHIFT))&DMA_DCR_LCH1_MASK)
#define DMA_DCR_LINKCC_MASK                      0x30u
#define DMA_DCR_LINKCC_SHIFT                     4
#define DMA_DCR_LINKCC(x)                        (((uint32_t)(((uint32_t)(x))<<DMA_DCR_LINKCC_SHIFT))&DMA_DCR_LINKCC_MASK)
#define DMA_DCR_D_REQ_MASK                       0x80u
#define DMA_DCR_D_REQ_SHIFT                      7
#define DMA_DCR_DMOD_MASK                        0xF00u
#define DMA_DCR_DMOD_SHIFT                       8
#define DMA_DCR_DMOD(x)                          (((uint32_t)(((uint32_t)(x))<<DMA_DCR_DMOD_SHIFT))&DMA_DCR_DMOD_MASK)
#define DMA_DCR_SMOD_MASK                        0xF000u
#define DMA_DCR_SMOD_SHIFT                       12
#define DMA_DCR_SMOD(x)                          (((uint32_t)(((uint32_t)(x))<<DMA_DCR_SMOD_SHIFT))&DMA_DCR_SMOD_MASK)
#define DMA_DCR_START_MASK                       0x10000u
#define DMA_DCR_START_SHIFT                      16
#define DMA_DCR_DSIZE_MASK                       0x60000u
#define DMA_DCR_DSIZE_SHIFT                      17
#define DMA_DCR_DSIZE(x)                         (((uint32_t)(((uint32_t)(x))<<DMA_DCR_DSIZE_SHIFT))&DMA_DCR_DSIZE_MASK)
#define DMA_DCR_DINC_MASK                        0x80000u
#define DMA_DCR_DINC_SHIFT                       19
#define DMA_DCR_SSIZE_MASK                       0x300000u
#define DMA_DCR_SSIZE_SHIFT                      20
#define DMA_DCR_SSIZE(x)                         (((uint32_t)(((uint32_t)(x))<<DMA_DCR_SSIZE_SHIFT))&DMA_DCR_SSIZE_MASK)
#define DMA_DCR_SINC_MASK                        0x400000u
#define DMA_DCR_SINC_SHIFT                       22
#define DMA_DCR_EADREQ_MASK                      0x800000u
#define DMA_DCR_EADREQ_SHIFT                     23
#define DMA_DCR_UMNSM_MASK                       0x3000000u
#define DMA_DCR_UMNSM_SHIFT                      24
#define DMA_DCR_UMNSM(x)                         (((uint32_t)(((uint32_t)(x))<<DMA_DCR_UMNSM_SHIFT))&DMA_DCR_UMNSM_MASK)
#define DMA_DCR_CHACR_MASK                       0xC000000u
#define DMA_DCR_CHACR_SHIFT                      26
#define DMA_DCR_CHACR(x)                         (((uint32_t)(((uint32_t)(x))<<DMA_DCR_CHACR_SHIFT))&DMA_DCR_CHACR_MASK)
#define DMA_DCR_AA_MASK                          0x10000000u
#define DMA_DCR_AA_SHIFT                         28
#define DMA_DCR_CS_MASK                          0x20000000u
#define DMA_DCR_CS_SHIFT                         29
#define DMA_DCR_ERQ_MASK                         0x40000000u
#define DMA_DCR_ERQ_SHIFT                        30
#define DMA_DCR_EINT_MASK                        0x80000000u
#define DMA_DCR_EINT_SHIFT                       31

/*!
 * @}
 */ /* end of group DMA_Register_Masks */


/* DMA - Peripheral instance base addresses */
/** Peripheral DMA base pointer */
#define DMA_BASE_PTR                             ((DMA_MemMapPtr)0x40008000u)
/** Array initializer of DMA peripheral base pointers */
#define DMA_BASE_PTRS                            { DMA_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- DMA - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMA_Register_Accessor_Macros DMA - Register accessor macros
 * @{
 */


/* DMA - Register instance definitions */
/* DMA */
#define DMA_SAR0                                 DMA_SAR_REG(DMA_BASE_PTR,0)
#define DMA_DAR0                                 DMA_DAR_REG(DMA_BASE_PTR,0)
#define DMA_DSR_BCR0                             DMA_DSR_BCR_REG(DMA_BASE_PTR,0)
#define DMA_DSR0                                 DMA_DSR_REG(DMA_BASE_PTR,0)
#define DMA_DCR0                                 DMA_DCR_REG(DMA_BASE_PTR,0)
#define DMA_SAR1                                 DMA_SAR_REG(DMA_BASE_PTR,1)
#define DMA_DAR1                                 DMA_DAR_REG(DMA_BASE_PTR,1)
#define DMA_DSR_BCR1                             DMA_DSR_BCR_REG(DMA_BASE_PTR,1)
#define DMA_DSR1                                 DMA_DSR_REG(DMA_BASE_PTR,1)
#define DMA_DCR1                                 DMA_DCR_REG(DMA_BASE_PTR,1)
#define DMA_SAR2                                 DMA_SAR_REG(DMA_BASE_PTR,2)
#define DMA_DAR2                                 DMA_DAR_REG(DMA_BASE_PTR,2)
#define DMA_DSR_BCR2                             DMA_DSR_BCR_REG(DMA_BASE_PTR,2)
#define DMA_DSR2                                 DMA_DSR_REG(DMA_BASE_PTR,2)
#define DMA_DCR2                                 DMA_DCR_REG(DMA_BASE_PTR,2)
#define DMA_SAR3                                 DMA_SAR_REG(DMA_BASE_PTR,3)
#define DMA_DAR3                                 DMA_DAR_REG(DMA_BASE_PTR,3)
#define DMA_DSR_BCR3                             DMA_DSR_BCR_REG(DMA_BASE_PTR,3)
#define DMA_DSR3                                 DMA_DSR_REG(DMA_BASE_PTR,3)
#define DMA_DCR3                                 DMA_DCR_REG(DMA_BASE_PTR,3)

/* DMA - Register array accessors */
#define DMA_SAR(index)                           DMA_SAR_REG(DMA_BASE_PTR,index)
#define DMA_DAR(index)                           DMA_DAR_REG(DMA_BASE_PTR,index)
#define DMA_DSR_BCR(index)                       DMA_DSR_BCR_REG(DMA_BASE_PTR,index)
#define DMA_DSR(index)                           DMA_DSR_REG(DMA_BASE_PTR,index)
#define DMA_DCR(index)                           DMA_DCR_REG(DMA_BASE_PTR,index)

/*!
 * @}
 */ /* end of group DMA_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group DMA_Peripheral */


/* ----------------------------------------------------------------------------
   -- DMAMUX
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMAMUX_Peripheral DMAMUX
 * @{
 */

/** DMAMUX - Peripheral register structure */
typedef struct DMAMUX_MemMap {
  uint8_t CHCFG;                                   /**< Channel Configuration register, offset: 0x0 */
} volatile *DMAMUX_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- DMAMUX - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMAMUX_Register_Accessor_Macros DMAMUX - Register accessor macros
 * @{
 */


/* DMAMUX - Register accessors */
#define DMAMUX_CHCFG_REG(base)                   ((base)->CHCFG)

/*!
 * @}
 */ /* end of group DMAMUX_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- DMAMUX Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMAMUX_Register_Masks DMAMUX Register Masks
 * @{
 */

/* CHCFG Bit Fields */
#define DMAMUX_CHCFG_SOURCE_MASK                 0x3Fu
#define DMAMUX_CHCFG_SOURCE_SHIFT                0
#define DMAMUX_CHCFG_SOURCE(x)                   (((uint8_t)(((uint8_t)(x))<<DMAMUX_CHCFG_SOURCE_SHIFT))&DMAMUX_CHCFG_SOURCE_MASK)
#define DMAMUX_CHCFG_TRIG_MASK                   0x40u
#define DMAMUX_CHCFG_TRIG_SHIFT                  6
#define DMAMUX_CHCFG_ENBL_MASK                   0x80u
#define DMAMUX_CHCFG_ENBL_SHIFT                  7

/*!
 * @}
 */ /* end of group DMAMUX_Register_Masks */


/* DMAMUX - Peripheral instance base addresses */
/** Peripheral DMAMUX0 base pointer */
#define DMAMUX0_BASE_PTR                         ((DMAMUX_MemMapPtr)0x40021000u)
/** Peripheral DMAMUX1 base pointer */
#define DMAMUX1_BASE_PTR                         ((DMAMUX_MemMapPtr)0x40022000u)
/** Peripheral DMAMUX2 base pointer */
#define DMAMUX2_BASE_PTR                         ((DMAMUX_MemMapPtr)0x40023000u)
/** Peripheral DMAMUX3 base pointer */
#define DMAMUX3_BASE_PTR                         ((DMAMUX_MemMapPtr)0x40024000u)
/** Array initializer of DMAMUX peripheral base pointers */
#define DMAMUX_BASE_PTRS                         { DMAMUX0_BASE_PTR, DMAMUX1_BASE_PTR, DMAMUX2_BASE_PTR, DMAMUX3_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- DMAMUX - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMAMUX_Register_Accessor_Macros DMAMUX - Register accessor macros
 * @{
 */


/* DMAMUX - Register instance definitions */
/* DMAMUX0 */
#define DMAMUX0_CHCFG                            DMAMUX_CHCFG_REG(DMAMUX0_BASE_PTR)
/* DMAMUX1 */
#define DMAMUX1_CHCFG                            DMAMUX_CHCFG_REG(DMAMUX1_BASE_PTR)
/* DMAMUX2 */
#define DMAMUX2_CHCFG                            DMAMUX_CHCFG_REG(DMAMUX2_BASE_PTR)
/* DMAMUX3 */
#define DMAMUX3_CHCFG                            DMAMUX_CHCFG_REG(DMAMUX3_BASE_PTR)

/*!
 * @}
 */ /* end of group DMAMUX_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group DMAMUX_Peripheral */


/* ----------------------------------------------------------------------------
   -- DWT
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DWT_Peripheral DWT
 * @{
 */

/** DWT - Peripheral register structure */
typedef struct DWT_MemMap {
  uint32_t CTRL;                                   /**< Control Register, offset: 0x0 */
  uint8_t RESERVED_0[24];
  uint32_t PCSR;                                   /**< Program Counter Sample Register, offset: 0x1C */
  struct {                                         /* offset: 0x20, array step: 0x10 */
    uint32_t COMP;                                   /**< Comparator Register 0..Comparator Register 1, array offset: 0x20, array step: 0x10 */
    uint32_t MASK;                                   /**< Mask Register 0..Mask Register 1, array offset: 0x24, array step: 0x10 */
    uint32_t FUNCTION;                               /**< Function Register 0..Function Register 1, array offset: 0x28, array step: 0x10 */
    uint8_t RESERVED_0[4];
  } COMPARATOR[2];
} volatile *DWT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- DWT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DWT_Register_Accessor_Macros DWT - Register accessor macros
 * @{
 */


/* DWT - Register accessors */
#define DWT_CTRL_REG(base)                       ((base)->CTRL)
#define DWT_PCSR_REG(base)                       ((base)->PCSR)
#define DWT_COMP_REG(base,index)                 ((base)->COMPARATOR[index].COMP)
#define DWT_MASK_REG(base,index)                 ((base)->COMPARATOR[index].MASK)
#define DWT_FUNCTION_REG(base,index)             ((base)->COMPARATOR[index].FUNCTION)

/*!
 * @}
 */ /* end of group DWT_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- DWT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DWT_Register_Masks DWT Register Masks
 * @{
 */


/*!
 * @}
 */ /* end of group DWT_Register_Masks */


/* DWT - Peripheral instance base addresses */
/** Peripheral DWT base pointer */
#define DWT_BASE_PTR                             ((DWT_MemMapPtr)0xE0001000u)
/** Array initializer of DWT peripheral base pointers */
#define DWT_BASE_PTRS                            { DWT_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- DWT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DWT_Register_Accessor_Macros DWT - Register accessor macros
 * @{
 */


/* DWT - Register instance definitions */
/* DWT */
#define DWT_CTRL                                 DWT_CTRL_REG(DWT_BASE_PTR)
#define DWT_PCSR                                 DWT_PCSR_REG(DWT_BASE_PTR)
#define DWT_COMP0                                DWT_COMP_REG(DWT_BASE_PTR,0)
#define DWT_MASK0                                DWT_MASK_REG(DWT_BASE_PTR,0)
#define DWT_FUNCTION0                            DWT_FUNCTION_REG(DWT_BASE_PTR,0)
#define DWT_COMP1                                DWT_COMP_REG(DWT_BASE_PTR,1)
#define DWT_MASK1                                DWT_MASK_REG(DWT_BASE_PTR,1)
#define DWT_FUNCTION1                            DWT_FUNCTION_REG(DWT_BASE_PTR,1)

/* DWT - Register array accessors */
#define DWT_COMP(index)                          DWT_COMP_REG(DWT_BASE_PTR,index)
#define DWT_MASK(index)                          DWT_MASK_REG(DWT_BASE_PTR,index)
#define DWT_FUNCTION(index)                      DWT_FUNCTION_REG(DWT_BASE_PTR,index)

/*!
 * @}
 */ /* end of group DWT_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group DWT_Peripheral */


/* ----------------------------------------------------------------------------
   -- EWM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup EWM_Peripheral EWM
 * @{
 */

/** EWM - Peripheral register structure */
typedef struct EWM_MemMap {
  uint8_t CTRL;                                    /**< Control Register, offset: 0x0 */
  uint8_t SERV;                                    /**< Service Register, offset: 0x1 */
  uint8_t CMPL;                                    /**< Compare Low Register, offset: 0x2 */
  uint8_t CMPH;                                    /**< Compare High Register, offset: 0x3 */
} volatile *EWM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- EWM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup EWM_Register_Accessor_Macros EWM - Register accessor macros
 * @{
 */


/* EWM - Register accessors */
#define EWM_CTRL_REG(base)                       ((base)->CTRL)
#define EWM_SERV_REG(base)                       ((base)->SERV)
#define EWM_CMPL_REG(base)                       ((base)->CMPL)
#define EWM_CMPH_REG(base)                       ((base)->CMPH)

/*!
 * @}
 */ /* end of group EWM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- EWM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup EWM_Register_Masks EWM Register Masks
 * @{
 */

/* CTRL Bit Fields */
#define EWM_CTRL_EWMEN_MASK                      0x1u
#define EWM_CTRL_EWMEN_SHIFT                     0
#define EWM_CTRL_ASSIN_MASK                      0x2u
#define EWM_CTRL_ASSIN_SHIFT                     1
#define EWM_CTRL_INEN_MASK                       0x4u
#define EWM_CTRL_INEN_SHIFT                      2
#define EWM_CTRL_INTEN_MASK                      0x8u
#define EWM_CTRL_INTEN_SHIFT                     3
/* SERV Bit Fields */
#define EWM_SERV_SERVICE_MASK                    0xFFu
#define EWM_SERV_SERVICE_SHIFT                   0
#define EWM_SERV_SERVICE(x)                      (((uint8_t)(((uint8_t)(x))<<EWM_SERV_SERVICE_SHIFT))&EWM_SERV_SERVICE_MASK)
/* CMPL Bit Fields */
#define EWM_CMPL_COMPAREL_MASK                   0xFFu
#define EWM_CMPL_COMPAREL_SHIFT                  0
#define EWM_CMPL_COMPAREL(x)                     (((uint8_t)(((uint8_t)(x))<<EWM_CMPL_COMPAREL_SHIFT))&EWM_CMPL_COMPAREL_MASK)
/* CMPH Bit Fields */
#define EWM_CMPH_COMPAREH_MASK                   0xFFu
#define EWM_CMPH_COMPAREH_SHIFT                  0
#define EWM_CMPH_COMPAREH(x)                     (((uint8_t)(((uint8_t)(x))<<EWM_CMPH_COMPAREH_SHIFT))&EWM_CMPH_COMPAREH_MASK)

/*!
 * @}
 */ /* end of group EWM_Register_Masks */


/* EWM - Peripheral instance base addresses */
/** Peripheral EWM base pointer */
#define EWM_BASE_PTR                             ((EWM_MemMapPtr)0x40061000u)
/** Array initializer of EWM peripheral base pointers */
#define EWM_BASE_PTRS                            { EWM_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- EWM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup EWM_Register_Accessor_Macros EWM - Register accessor macros
 * @{
 */


/* EWM - Register instance definitions */
/* EWM */
#define EWM_CTRL                                 EWM_CTRL_REG(EWM_BASE_PTR)
#define EWM_SERV                                 EWM_SERV_REG(EWM_BASE_PTR)
#define EWM_CMPL                                 EWM_CMPL_REG(EWM_BASE_PTR)
#define EWM_CMPH                                 EWM_CMPH_REG(EWM_BASE_PTR)

/*!
 * @}
 */ /* end of group EWM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group EWM_Peripheral */


/* ----------------------------------------------------------------------------
   -- FTFA
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Peripheral FTFA
 * @{
 */

/** FTFA - Peripheral register structure */
typedef struct FTFA_MemMap {
  uint8_t FSTAT;                                   /**< Flash Status Register, offset: 0x0 */
  uint8_t FCNFG;                                   /**< Flash Configuration Register, offset: 0x1 */
  uint8_t FSEC;                                    /**< Flash Security Register, offset: 0x2 */
  uint8_t FOPT;                                    /**< Flash Option Register, offset: 0x3 */
  uint8_t FCCOB3;                                  /**< Flash Common Command Object Registers, offset: 0x4 */
  uint8_t FCCOB2;                                  /**< Flash Common Command Object Registers, offset: 0x5 */
  uint8_t FCCOB1;                                  /**< Flash Common Command Object Registers, offset: 0x6 */
  uint8_t FCCOB0;                                  /**< Flash Common Command Object Registers, offset: 0x7 */
  uint8_t FCCOB7;                                  /**< Flash Common Command Object Registers, offset: 0x8 */
  uint8_t FCCOB6;                                  /**< Flash Common Command Object Registers, offset: 0x9 */
  uint8_t FCCOB5;                                  /**< Flash Common Command Object Registers, offset: 0xA */
  uint8_t FCCOB4;                                  /**< Flash Common Command Object Registers, offset: 0xB */
  uint8_t FCCOBB;                                  /**< Flash Common Command Object Registers, offset: 0xC */
  uint8_t FCCOBA;                                  /**< Flash Common Command Object Registers, offset: 0xD */
  uint8_t FCCOB9;                                  /**< Flash Common Command Object Registers, offset: 0xE */
  uint8_t FCCOB8;                                  /**< Flash Common Command Object Registers, offset: 0xF */
  uint8_t FPROT3;                                  /**< Program Flash Protection Registers, offset: 0x10 */
  uint8_t FPROT2;                                  /**< Program Flash Protection Registers, offset: 0x11 */
  uint8_t FPROT1;                                  /**< Program Flash Protection Registers, offset: 0x12 */
  uint8_t FPROT0;                                  /**< Program Flash Protection Registers, offset: 0x13 */
} volatile *FTFA_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- FTFA - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Register_Accessor_Macros FTFA - Register accessor macros
 * @{
 */


/* FTFA - Register accessors */
#define FTFA_FSTAT_REG(base)                     ((base)->FSTAT)
#define FTFA_FCNFG_REG(base)                     ((base)->FCNFG)
#define FTFA_FSEC_REG(base)                      ((base)->FSEC)
#define FTFA_FOPT_REG(base)                      ((base)->FOPT)
#define FTFA_FCCOB3_REG(base)                    ((base)->FCCOB3)
#define FTFA_FCCOB2_REG(base)                    ((base)->FCCOB2)
#define FTFA_FCCOB1_REG(base)                    ((base)->FCCOB1)
#define FTFA_FCCOB0_REG(base)                    ((base)->FCCOB0)
#define FTFA_FCCOB7_REG(base)                    ((base)->FCCOB7)
#define FTFA_FCCOB6_REG(base)                    ((base)->FCCOB6)
#define FTFA_FCCOB5_REG(base)                    ((base)->FCCOB5)
#define FTFA_FCCOB4_REG(base)                    ((base)->FCCOB4)
#define FTFA_FCCOBB_REG(base)                    ((base)->FCCOBB)
#define FTFA_FCCOBA_REG(base)                    ((base)->FCCOBA)
#define FTFA_FCCOB9_REG(base)                    ((base)->FCCOB9)
#define FTFA_FCCOB8_REG(base)                    ((base)->FCCOB8)
#define FTFA_FPROT3_REG(base)                    ((base)->FPROT3)
#define FTFA_FPROT2_REG(base)                    ((base)->FPROT2)
#define FTFA_FPROT1_REG(base)                    ((base)->FPROT1)
#define FTFA_FPROT0_REG(base)                    ((base)->FPROT0)

/*!
 * @}
 */ /* end of group FTFA_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- FTFA Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Register_Masks FTFA Register Masks
 * @{
 */

/* FSTAT Bit Fields */
#define FTFA_FSTAT_MGSTAT0_MASK                  0x1u
#define FTFA_FSTAT_MGSTAT0_SHIFT                 0
#define FTFA_FSTAT_FPVIOL_MASK                   0x10u
#define FTFA_FSTAT_FPVIOL_SHIFT                  4
#define FTFA_FSTAT_ACCERR_MASK                   0x20u
#define FTFA_FSTAT_ACCERR_SHIFT                  5
#define FTFA_FSTAT_RDCOLERR_MASK                 0x40u
#define FTFA_FSTAT_RDCOLERR_SHIFT                6
#define FTFA_FSTAT_CCIF_MASK                     0x80u
#define FTFA_FSTAT_CCIF_SHIFT                    7
/* FCNFG Bit Fields */
#define FTFA_FCNFG_ERSSUSP_MASK                  0x10u
#define FTFA_FCNFG_ERSSUSP_SHIFT                 4
#define FTFA_FCNFG_ERSAREQ_MASK                  0x20u
#define FTFA_FCNFG_ERSAREQ_SHIFT                 5
#define FTFA_FCNFG_RDCOLLIE_MASK                 0x40u
#define FTFA_FCNFG_RDCOLLIE_SHIFT                6
#define FTFA_FCNFG_CCIE_MASK                     0x80u
#define FTFA_FCNFG_CCIE_SHIFT                    7
/* FSEC Bit Fields */
#define FTFA_FSEC_SEC_MASK                       0x3u
#define FTFA_FSEC_SEC_SHIFT                      0
#define FTFA_FSEC_SEC(x)                         (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_SEC_SHIFT))&FTFA_FSEC_SEC_MASK)
#define FTFA_FSEC_FSLACC_MASK                    0xCu
#define FTFA_FSEC_FSLACC_SHIFT                   2
#define FTFA_FSEC_FSLACC(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_FSLACC_SHIFT))&FTFA_FSEC_FSLACC_MASK)
#define FTFA_FSEC_MEEN_MASK                      0x30u
#define FTFA_FSEC_MEEN_SHIFT                     4
#define FTFA_FSEC_MEEN(x)                        (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_MEEN_SHIFT))&FTFA_FSEC_MEEN_MASK)
#define FTFA_FSEC_KEYEN_MASK                     0xC0u
#define FTFA_FSEC_KEYEN_SHIFT                    6
#define FTFA_FSEC_KEYEN(x)                       (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_KEYEN_SHIFT))&FTFA_FSEC_KEYEN_MASK)
/* FOPT Bit Fields */
#define FTFA_FOPT_OPT_MASK                       0xFFu
#define FTFA_FOPT_OPT_SHIFT                      0
#define FTFA_FOPT_OPT(x)                         (((uint8_t)(((uint8_t)(x))<<FTFA_FOPT_OPT_SHIFT))&FTFA_FOPT_OPT_MASK)
/* FCCOB3 Bit Fields */
#define FTFA_FCCOB3_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB3_CCOBn_SHIFT                  0
#define FTFA_FCCOB3_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB3_CCOBn_SHIFT))&FTFA_FCCOB3_CCOBn_MASK)
/* FCCOB2 Bit Fields */
#define FTFA_FCCOB2_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB2_CCOBn_SHIFT                  0
#define FTFA_FCCOB2_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB2_CCOBn_SHIFT))&FTFA_FCCOB2_CCOBn_MASK)
/* FCCOB1 Bit Fields */
#define FTFA_FCCOB1_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB1_CCOBn_SHIFT                  0
#define FTFA_FCCOB1_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB1_CCOBn_SHIFT))&FTFA_FCCOB1_CCOBn_MASK)
/* FCCOB0 Bit Fields */
#define FTFA_FCCOB0_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB0_CCOBn_SHIFT                  0
#define FTFA_FCCOB0_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB0_CCOBn_SHIFT))&FTFA_FCCOB0_CCOBn_MASK)
/* FCCOB7 Bit Fields */
#define FTFA_FCCOB7_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB7_CCOBn_SHIFT                  0
#define FTFA_FCCOB7_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB7_CCOBn_SHIFT))&FTFA_FCCOB7_CCOBn_MASK)
/* FCCOB6 Bit Fields */
#define FTFA_FCCOB6_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB6_CCOBn_SHIFT                  0
#define FTFA_FCCOB6_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB6_CCOBn_SHIFT))&FTFA_FCCOB6_CCOBn_MASK)
/* FCCOB5 Bit Fields */
#define FTFA_FCCOB5_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB5_CCOBn_SHIFT                  0
#define FTFA_FCCOB5_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB5_CCOBn_SHIFT))&FTFA_FCCOB5_CCOBn_MASK)
/* FCCOB4 Bit Fields */
#define FTFA_FCCOB4_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB4_CCOBn_SHIFT                  0
#define FTFA_FCCOB4_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB4_CCOBn_SHIFT))&FTFA_FCCOB4_CCOBn_MASK)
/* FCCOBB Bit Fields */
#define FTFA_FCCOBB_CCOBn_MASK                   0xFFu
#define FTFA_FCCOBB_CCOBn_SHIFT                  0
#define FTFA_FCCOBB_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOBB_CCOBn_SHIFT))&FTFA_FCCOBB_CCOBn_MASK)
/* FCCOBA Bit Fields */
#define FTFA_FCCOBA_CCOBn_MASK                   0xFFu
#define FTFA_FCCOBA_CCOBn_SHIFT                  0
#define FTFA_FCCOBA_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOBA_CCOBn_SHIFT))&FTFA_FCCOBA_CCOBn_MASK)
/* FCCOB9 Bit Fields */
#define FTFA_FCCOB9_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB9_CCOBn_SHIFT                  0
#define FTFA_FCCOB9_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB9_CCOBn_SHIFT))&FTFA_FCCOB9_CCOBn_MASK)
/* FCCOB8 Bit Fields */
#define FTFA_FCCOB8_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB8_CCOBn_SHIFT                  0
#define FTFA_FCCOB8_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB8_CCOBn_SHIFT))&FTFA_FCCOB8_CCOBn_MASK)
/* FPROT3 Bit Fields */
#define FTFA_FPROT3_PROT_MASK                    0xFFu
#define FTFA_FPROT3_PROT_SHIFT                   0
#define FTFA_FPROT3_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT3_PROT_SHIFT))&FTFA_FPROT3_PROT_MASK)
/* FPROT2 Bit Fields */
#define FTFA_FPROT2_PROT_MASK                    0xFFu
#define FTFA_FPROT2_PROT_SHIFT                   0
#define FTFA_FPROT2_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT2_PROT_SHIFT))&FTFA_FPROT2_PROT_MASK)
/* FPROT1 Bit Fields */
#define FTFA_FPROT1_PROT_MASK                    0xFFu
#define FTFA_FPROT1_PROT_SHIFT                   0
#define FTFA_FPROT1_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT1_PROT_SHIFT))&FTFA_FPROT1_PROT_MASK)
/* FPROT0 Bit Fields */
#define FTFA_FPROT0_PROT_MASK                    0xFFu
#define FTFA_FPROT0_PROT_SHIFT                   0
#define FTFA_FPROT0_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT0_PROT_SHIFT))&FTFA_FPROT0_PROT_MASK)

/*!
 * @}
 */ /* end of group FTFA_Register_Masks */


/* FTFA - Peripheral instance base addresses */
/** Peripheral FTFA base pointer */
#define FTFA_BASE_PTR                            ((FTFA_MemMapPtr)0x40020000u)
/** Array initializer of FTFA peripheral base pointers */
#define FTFA_BASE_PTRS                           { FTFA_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- FTFA - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Register_Accessor_Macros FTFA - Register accessor macros
 * @{
 */


/* FTFA - Register instance definitions */
/* FTFA */
#define FTFA_FSTAT                               FTFA_FSTAT_REG(FTFA_BASE_PTR)
#define FTFA_FCNFG                               FTFA_FCNFG_REG(FTFA_BASE_PTR)
#define FTFA_FSEC                                FTFA_FSEC_REG(FTFA_BASE_PTR)
#define FTFA_FOPT                                FTFA_FOPT_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB3                              FTFA_FCCOB3_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB2                              FTFA_FCCOB2_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB1                              FTFA_FCCOB1_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB0                              FTFA_FCCOB0_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB7                              FTFA_FCCOB7_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB6                              FTFA_FCCOB6_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB5                              FTFA_FCCOB5_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB4                              FTFA_FCCOB4_REG(FTFA_BASE_PTR)
#define FTFA_FCCOBB                              FTFA_FCCOBB_REG(FTFA_BASE_PTR)
#define FTFA_FCCOBA                              FTFA_FCCOBA_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB9                              FTFA_FCCOB9_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB8                              FTFA_FCCOB8_REG(FTFA_BASE_PTR)
#define FTFA_FPROT3                              FTFA_FPROT3_REG(FTFA_BASE_PTR)
#define FTFA_FPROT2                              FTFA_FPROT2_REG(FTFA_BASE_PTR)
#define FTFA_FPROT1                              FTFA_FPROT1_REG(FTFA_BASE_PTR)
#define FTFA_FPROT0                              FTFA_FPROT0_REG(FTFA_BASE_PTR)

/*!
 * @}
 */ /* end of group FTFA_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group FTFA_Peripheral */


/* ----------------------------------------------------------------------------
   -- GPIO
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Peripheral GPIO
 * @{
 */

/** GPIO - Peripheral register structure */
typedef struct GPIO_MemMap {
  uint8_t PDOR;                                    /**< Port Data Output Register, offset: 0x0 */
  uint8_t RESERVED_0[15];
  uint8_t PDIR;                                    /**< Port Data Input Register, offset: 0x10 */
  uint8_t RESERVED_1[3];
  uint8_t PDDR;                                    /**< Port Data Direction Register, offset: 0x14 */
  uint8_t RESERVED_2[7];
  uint8_t GACR;                                    /**< GPIO Attribute Checker Register, offset: 0x1C */
} volatile *GPIO_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- GPIO - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Register_Accessor_Macros GPIO - Register accessor macros
 * @{
 */


/* GPIO - Register accessors */
#define GPIO_PDOR_REG(base)                      ((base)->PDOR)
#define GPIO_PDIR_REG(base)                      ((base)->PDIR)
#define GPIO_PDDR_REG(base)                      ((base)->PDDR)
#define GPIO_GACR_REG(base)                      ((base)->GACR)

/*!
 * @}
 */ /* end of group GPIO_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- GPIO Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Register_Masks GPIO Register Masks
 * @{
 */

/* PDOR Bit Fields */
#define GPIO_PDOR_PDO_MASK                       0xFFu
#define GPIO_PDOR_PDO_SHIFT                      0
#define GPIO_PDOR_PDO(x)                         (((uint8_t)(((uint8_t)(x))<<GPIO_PDOR_PDO_SHIFT))&GPIO_PDOR_PDO_MASK)
/* PDIR Bit Fields */
#define GPIO_PDIR_PDI_MASK                       0xFFu
#define GPIO_PDIR_PDI_SHIFT                      0
#define GPIO_PDIR_PDI(x)                         (((uint8_t)(((uint8_t)(x))<<GPIO_PDIR_PDI_SHIFT))&GPIO_PDIR_PDI_MASK)
/* PDDR Bit Fields */
#define GPIO_PDDR_PDD_MASK                       0xFFu
#define GPIO_PDDR_PDD_SHIFT                      0
#define GPIO_PDDR_PDD(x)                         (((uint8_t)(((uint8_t)(x))<<GPIO_PDDR_PDD_SHIFT))&GPIO_PDDR_PDD_MASK)
/* GACR Bit Fields */
#define GPIO_GACR_ACB_MASK                       0x7u
#define GPIO_GACR_ACB_SHIFT                      0
#define GPIO_GACR_ACB(x)                         (((uint8_t)(((uint8_t)(x))<<GPIO_GACR_ACB_SHIFT))&GPIO_GACR_ACB_MASK)
#define GPIO_GACR_ROB_MASK                       0x80u
#define GPIO_GACR_ROB_SHIFT                      7

/*!
 * @}
 */ /* end of group GPIO_Register_Masks */


/* GPIO - Peripheral instance base addresses */
/** Peripheral GPIOA base pointer */
#define GPIOA_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF000u)
/** Peripheral GPIOB base pointer */
#define GPIOB_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF001u)
/** Peripheral GPIOC base pointer */
#define GPIOC_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF002u)
/** Peripheral GPIOD base pointer */
#define GPIOD_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF003u)
/** Peripheral GPIOE base pointer */
#define GPIOE_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF040u)
/** Peripheral GPIOF base pointer */
#define GPIOF_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF041u)
/** Peripheral GPIOG base pointer */
#define GPIOG_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF042u)
/** Peripheral GPIOH base pointer */
#define GPIOH_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF043u)
/** Peripheral GPIOI base pointer */
#define GPIOI_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF080u)
/** Array initializer of GPIO peripheral base pointers */
#define GPIO_BASE_PTRS                           { GPIOA_BASE_PTR, GPIOB_BASE_PTR, GPIOC_BASE_PTR, GPIOD_BASE_PTR, GPIOE_BASE_PTR, GPIOF_BASE_PTR, GPIOG_BASE_PTR, GPIOH_BASE_PTR, GPIOI_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- GPIO - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Register_Accessor_Macros GPIO - Register accessor macros
 * @{
 */


/* GPIO - Register instance definitions */
/* GPIOA */
#define GPIOA_PDOR                               GPIO_PDOR_REG(GPIOA_BASE_PTR)
#define GPIOA_PDIR                               GPIO_PDIR_REG(GPIOA_BASE_PTR)
#define GPIOA_PDDR                               GPIO_PDDR_REG(GPIOA_BASE_PTR)
#define GPIOA_GACR                               GPIO_GACR_REG(GPIOA_BASE_PTR)
/* GPIOB */
#define GPIOB_PDOR                               GPIO_PDOR_REG(GPIOB_BASE_PTR)
#define GPIOB_PDIR                               GPIO_PDIR_REG(GPIOB_BASE_PTR)
#define GPIOB_PDDR                               GPIO_PDDR_REG(GPIOB_BASE_PTR)
#define GPIOB_GACR                               GPIO_GACR_REG(GPIOB_BASE_PTR)
/* GPIOC */
#define GPIOC_PDOR                               GPIO_PDOR_REG(GPIOC_BASE_PTR)
#define GPIOC_PDIR                               GPIO_PDIR_REG(GPIOC_BASE_PTR)
#define GPIOC_PDDR                               GPIO_PDDR_REG(GPIOC_BASE_PTR)
#define GPIOC_GACR                               GPIO_GACR_REG(GPIOC_BASE_PTR)
/* GPIOD */
#define GPIOD_PDOR                               GPIO_PDOR_REG(GPIOD_BASE_PTR)
#define GPIOD_PDIR                               GPIO_PDIR_REG(GPIOD_BASE_PTR)
#define GPIOD_PDDR                               GPIO_PDDR_REG(GPIOD_BASE_PTR)
#define GPIOD_GACR                               GPIO_GACR_REG(GPIOD_BASE_PTR)
/* GPIOE */
#define GPIOE_PDOR                               GPIO_PDOR_REG(GPIOE_BASE_PTR)
#define GPIOE_PDIR                               GPIO_PDIR_REG(GPIOE_BASE_PTR)
#define GPIOE_PDDR                               GPIO_PDDR_REG(GPIOE_BASE_PTR)
#define GPIOE_GACR                               GPIO_GACR_REG(GPIOE_BASE_PTR)
/* GPIOF */
#define GPIOF_PDOR                               GPIO_PDOR_REG(GPIOF_BASE_PTR)
#define GPIOF_PDIR                               GPIO_PDIR_REG(GPIOF_BASE_PTR)
#define GPIOF_PDDR                               GPIO_PDDR_REG(GPIOF_BASE_PTR)
#define GPIOF_GACR                               GPIO_GACR_REG(GPIOF_BASE_PTR)
/* GPIOG */
#define GPIOG_PDOR                               GPIO_PDOR_REG(GPIOG_BASE_PTR)
#define GPIOG_PDIR                               GPIO_PDIR_REG(GPIOG_BASE_PTR)
#define GPIOG_PDDR                               GPIO_PDDR_REG(GPIOG_BASE_PTR)
#define GPIOG_GACR                               GPIO_GACR_REG(GPIOG_BASE_PTR)
/* GPIOH */
#define GPIOH_PDOR                               GPIO_PDOR_REG(GPIOH_BASE_PTR)
#define GPIOH_PDIR                               GPIO_PDIR_REG(GPIOH_BASE_PTR)
#define GPIOH_PDDR                               GPIO_PDDR_REG(GPIOH_BASE_PTR)
#define GPIOH_GACR                               GPIO_GACR_REG(GPIOH_BASE_PTR)
/* GPIOI */
#define GPIOI_PDOR                               GPIO_PDOR_REG(GPIOI_BASE_PTR)
#define GPIOI_PDIR                               GPIO_PDIR_REG(GPIOI_BASE_PTR)
#define GPIOI_PDDR                               GPIO_PDDR_REG(GPIOI_BASE_PTR)
#define GPIOI_GACR                               GPIO_GACR_REG(GPIOI_BASE_PTR)

/*!
 * @}
 */ /* end of group GPIO_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group GPIO_Peripheral */


/* ----------------------------------------------------------------------------
   -- I2C
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Peripheral I2C
 * @{
 */

/** I2C - Peripheral register structure */
typedef struct I2C_MemMap {
  uint8_t A1;                                      /**< I2C Address Register 1, offset: 0x0 */
  uint8_t F;                                       /**< I2C Frequency Divider register, offset: 0x1 */
  uint8_t C1;                                      /**< I2C Control Register 1, offset: 0x2 */
  uint8_t S;                                       /**< I2C Status register, offset: 0x3 */
  uint8_t D;                                       /**< I2C Data I/O register, offset: 0x4 */
  uint8_t C2;                                      /**< I2C Control Register 2, offset: 0x5 */
  uint8_t FLT;                                     /**< I2C Programmable Input Glitch Filter register, offset: 0x6 */
  uint8_t RA;                                      /**< I2C Range Address register, offset: 0x7 */
  uint8_t SMB;                                     /**< I2C SMBus Control and Status register, offset: 0x8 */
  uint8_t A2;                                      /**< I2C Address Register 2, offset: 0x9 */
  uint8_t SLTH;                                    /**< I2C SCL Low Timeout Register High, offset: 0xA */
  uint8_t SLTL;                                    /**< I2C SCL Low Timeout Register Low, offset: 0xB */
} volatile *I2C_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- I2C - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Register_Accessor_Macros I2C - Register accessor macros
 * @{
 */


/* I2C - Register accessors */
#define I2C_A1_REG(base)                         ((base)->A1)
#define I2C_F_REG(base)                          ((base)->F)
#define I2C_C1_REG(base)                         ((base)->C1)
#define I2C_S_REG(base)                          ((base)->S)
#define I2C_D_REG(base)                          ((base)->D)
#define I2C_C2_REG(base)                         ((base)->C2)
#define I2C_FLT_REG(base)                        ((base)->FLT)
#define I2C_RA_REG(base)                         ((base)->RA)
#define I2C_SMB_REG(base)                        ((base)->SMB)
#define I2C_A2_REG(base)                         ((base)->A2)
#define I2C_SLTH_REG(base)                       ((base)->SLTH)
#define I2C_SLTL_REG(base)                       ((base)->SLTL)

/*!
 * @}
 */ /* end of group I2C_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- I2C Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Register_Masks I2C Register Masks
 * @{
 */

/* A1 Bit Fields */
#define I2C_A1_AD_MASK                           0xFEu
#define I2C_A1_AD_SHIFT                          1
#define I2C_A1_AD(x)                             (((uint8_t)(((uint8_t)(x))<<I2C_A1_AD_SHIFT))&I2C_A1_AD_MASK)
/* F Bit Fields */
#define I2C_F_ICR_MASK                           0x3Fu
#define I2C_F_ICR_SHIFT                          0
#define I2C_F_ICR(x)                             (((uint8_t)(((uint8_t)(x))<<I2C_F_ICR_SHIFT))&I2C_F_ICR_MASK)
#define I2C_F_MULT_MASK                          0xC0u
#define I2C_F_MULT_SHIFT                         6
#define I2C_F_MULT(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_F_MULT_SHIFT))&I2C_F_MULT_MASK)
/* C1 Bit Fields */
#define I2C_C1_DMAEN_MASK                        0x1u
#define I2C_C1_DMAEN_SHIFT                       0
#define I2C_C1_WUEN_MASK                         0x2u
#define I2C_C1_WUEN_SHIFT                        1
#define I2C_C1_RSTA_MASK                         0x4u
#define I2C_C1_RSTA_SHIFT                        2
#define I2C_C1_TXAK_MASK                         0x8u
#define I2C_C1_TXAK_SHIFT                        3
#define I2C_C1_TX_MASK                           0x10u
#define I2C_C1_TX_SHIFT                          4
#define I2C_C1_MST_MASK                          0x20u
#define I2C_C1_MST_SHIFT                         5
#define I2C_C1_IICIE_MASK                        0x40u
#define I2C_C1_IICIE_SHIFT                       6
#define I2C_C1_IICEN_MASK                        0x80u
#define I2C_C1_IICEN_SHIFT                       7
/* S Bit Fields */
#define I2C_S_RXAK_MASK                          0x1u
#define I2C_S_RXAK_SHIFT                         0
#define I2C_S_IICIF_MASK                         0x2u
#define I2C_S_IICIF_SHIFT                        1
#define I2C_S_SRW_MASK                           0x4u
#define I2C_S_SRW_SHIFT                          2
#define I2C_S_RAM_MASK                           0x8u
#define I2C_S_RAM_SHIFT                          3
#define I2C_S_ARBL_MASK                          0x10u
#define I2C_S_ARBL_SHIFT                         4
#define I2C_S_BUSY_MASK                          0x20u
#define I2C_S_BUSY_SHIFT                         5
#define I2C_S_IAAS_MASK                          0x40u
#define I2C_S_IAAS_SHIFT                         6
#define I2C_S_TCF_MASK                           0x80u
#define I2C_S_TCF_SHIFT                          7
/* D Bit Fields */
#define I2C_D_DATA_MASK                          0xFFu
#define I2C_D_DATA_SHIFT                         0
#define I2C_D_DATA(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_D_DATA_SHIFT))&I2C_D_DATA_MASK)
/* C2 Bit Fields */
#define I2C_C2_AD_MASK                           0x7u
#define I2C_C2_AD_SHIFT                          0
#define I2C_C2_AD(x)                             (((uint8_t)(((uint8_t)(x))<<I2C_C2_AD_SHIFT))&I2C_C2_AD_MASK)
#define I2C_C2_RMEN_MASK                         0x8u
#define I2C_C2_RMEN_SHIFT                        3
#define I2C_C2_SBRC_MASK                         0x10u
#define I2C_C2_SBRC_SHIFT                        4
#define I2C_C2_HDRS_MASK                         0x20u
#define I2C_C2_HDRS_SHIFT                        5
#define I2C_C2_ADEXT_MASK                        0x40u
#define I2C_C2_ADEXT_SHIFT                       6
#define I2C_C2_GCAEN_MASK                        0x80u
#define I2C_C2_GCAEN_SHIFT                       7
/* FLT Bit Fields */
#define I2C_FLT_FLT_MASK                         0xFu
#define I2C_FLT_FLT_SHIFT                        0
#define I2C_FLT_FLT(x)                           (((uint8_t)(((uint8_t)(x))<<I2C_FLT_FLT_SHIFT))&I2C_FLT_FLT_MASK)
#define I2C_FLT_STARTF_MASK                      0x10u
#define I2C_FLT_STARTF_SHIFT                     4
#define I2C_FLT_SSIE_MASK                        0x20u
#define I2C_FLT_SSIE_SHIFT                       5
#define I2C_FLT_STOPF_MASK                       0x40u
#define I2C_FLT_STOPF_SHIFT                      6
#define I2C_FLT_SHEN_MASK                        0x80u
#define I2C_FLT_SHEN_SHIFT                       7
/* RA Bit Fields */
#define I2C_RA_RAD_MASK                          0xFEu
#define I2C_RA_RAD_SHIFT                         1
#define I2C_RA_RAD(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_RA_RAD_SHIFT))&I2C_RA_RAD_MASK)
/* SMB Bit Fields */
#define I2C_SMB_SHTF2IE_MASK                     0x1u
#define I2C_SMB_SHTF2IE_SHIFT                    0
#define I2C_SMB_SHTF2_MASK                       0x2u
#define I2C_SMB_SHTF2_SHIFT                      1
#define I2C_SMB_SHTF1_MASK                       0x4u
#define I2C_SMB_SHTF1_SHIFT                      2
#define I2C_SMB_SLTF_MASK                        0x8u
#define I2C_SMB_SLTF_SHIFT                       3
#define I2C_SMB_TCKSEL_MASK                      0x10u
#define I2C_SMB_TCKSEL_SHIFT                     4
#define I2C_SMB_SIICAEN_MASK                     0x20u
#define I2C_SMB_SIICAEN_SHIFT                    5
#define I2C_SMB_ALERTEN_MASK                     0x40u
#define I2C_SMB_ALERTEN_SHIFT                    6
#define I2C_SMB_FACK_MASK                        0x80u
#define I2C_SMB_FACK_SHIFT                       7
/* A2 Bit Fields */
#define I2C_A2_SAD_MASK                          0xFEu
#define I2C_A2_SAD_SHIFT                         1
#define I2C_A2_SAD(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_A2_SAD_SHIFT))&I2C_A2_SAD_MASK)
/* SLTH Bit Fields */
#define I2C_SLTH_SSLT_MASK                       0xFFu
#define I2C_SLTH_SSLT_SHIFT                      0
#define I2C_SLTH_SSLT(x)                         (((uint8_t)(((uint8_t)(x))<<I2C_SLTH_SSLT_SHIFT))&I2C_SLTH_SSLT_MASK)
/* SLTL Bit Fields */
#define I2C_SLTL_SSLT_MASK                       0xFFu
#define I2C_SLTL_SSLT_SHIFT                      0
#define I2C_SLTL_SSLT(x)                         (((uint8_t)(((uint8_t)(x))<<I2C_SLTL_SSLT_SHIFT))&I2C_SLTL_SSLT_MASK)

/*!
 * @}
 */ /* end of group I2C_Register_Masks */


/* I2C - Peripheral instance base addresses */
/** Peripheral I2C0 base pointer */
#define I2C0_BASE_PTR                            ((I2C_MemMapPtr)0x40067000u)
/** Peripheral I2C1 base pointer */
#define I2C1_BASE_PTR                            ((I2C_MemMapPtr)0x40068000u)
/** Array initializer of I2C peripheral base pointers */
#define I2C_BASE_PTRS                            { I2C0_BASE_PTR, I2C1_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- I2C - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Register_Accessor_Macros I2C - Register accessor macros
 * @{
 */


/* I2C - Register instance definitions */
/* I2C0 */
#define I2C0_A1                                  I2C_A1_REG(I2C0_BASE_PTR)
#define I2C0_F                                   I2C_F_REG(I2C0_BASE_PTR)
#define I2C0_C1                                  I2C_C1_REG(I2C0_BASE_PTR)
#define I2C0_S                                   I2C_S_REG(I2C0_BASE_PTR)
#define I2C0_D                                   I2C_D_REG(I2C0_BASE_PTR)
#define I2C0_C2                                  I2C_C2_REG(I2C0_BASE_PTR)
#define I2C0_FLT                                 I2C_FLT_REG(I2C0_BASE_PTR)
#define I2C0_RA                                  I2C_RA_REG(I2C0_BASE_PTR)
#define I2C0_SMB                                 I2C_SMB_REG(I2C0_BASE_PTR)
#define I2C0_A2                                  I2C_A2_REG(I2C0_BASE_PTR)
#define I2C0_SLTH                                I2C_SLTH_REG(I2C0_BASE_PTR)
#define I2C0_SLTL                                I2C_SLTL_REG(I2C0_BASE_PTR)
/* I2C1 */
#define I2C1_A1                                  I2C_A1_REG(I2C1_BASE_PTR)
#define I2C1_F                                   I2C_F_REG(I2C1_BASE_PTR)
#define I2C1_C1                                  I2C_C1_REG(I2C1_BASE_PTR)
#define I2C1_S                                   I2C_S_REG(I2C1_BASE_PTR)
#define I2C1_D                                   I2C_D_REG(I2C1_BASE_PTR)
#define I2C1_C2                                  I2C_C2_REG(I2C1_BASE_PTR)
#define I2C1_FLT                                 I2C_FLT_REG(I2C1_BASE_PTR)
#define I2C1_RA                                  I2C_RA_REG(I2C1_BASE_PTR)
#define I2C1_SMB                                 I2C_SMB_REG(I2C1_BASE_PTR)
#define I2C1_A2                                  I2C_A2_REG(I2C1_BASE_PTR)
#define I2C1_SLTH                                I2C_SLTH_REG(I2C1_BASE_PTR)
#define I2C1_SLTL                                I2C_SLTL_REG(I2C1_BASE_PTR)

/*!
 * @}
 */ /* end of group I2C_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group I2C_Peripheral */


/* ----------------------------------------------------------------------------
   -- LCD
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LCD_Peripheral LCD
 * @{
 */

/** LCD - Peripheral register structure */
typedef struct LCD_MemMap {
  uint32_t GCR;                                    /**< LCD General Control Register, offset: 0x0 */
  uint32_t AR;                                     /**< LCD Auxiliary Register, offset: 0x4 */
  uint32_t FDCR;                                   /**< LCD Fault Detect Control Register, offset: 0x8 */
  uint32_t FDSR;                                   /**< LCD Fault Detect Status Register, offset: 0xC */
  uint32_t PEN[2];                                 /**< LCD Pin Enable register, array offset: 0x10, array step: 0x4 */
  uint32_t BPEN[2];                                /**< LCD Back Plane Enable register, array offset: 0x18, array step: 0x4 */
  union {                                          /* offset: 0x20 */
    uint32_t WF[16];                                 /**< LCD Waveform register, array offset: 0x20, array step: 0x4 */
    uint8_t WF8B[64];                                /**< LCD Waveform Register 0...LCD Waveform Register 63., array offset: 0x20, array step: 0x1 */
  };
} volatile *LCD_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- LCD - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LCD_Register_Accessor_Macros LCD - Register accessor macros
 * @{
 */


/* LCD - Register accessors */
#define LCD_GCR_REG(base)                        ((base)->GCR)
#define LCD_AR_REG(base)                         ((base)->AR)
#define LCD_FDCR_REG(base)                       ((base)->FDCR)
#define LCD_FDSR_REG(base)                       ((base)->FDSR)
#define LCD_PEN_REG(base,index)                  ((base)->PEN[index])
#define LCD_BPEN_REG(base,index)                 ((base)->BPEN[index])
#define LCD_WF_REG(base,index2)                  ((base)->WF[index2])
#define LCD_WF8B_REG(base,index2)                ((base)->WF8B[index2])

/*!
 * @}
 */ /* end of group LCD_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- LCD Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LCD_Register_Masks LCD Register Masks
 * @{
 */

/* GCR Bit Fields */
#define LCD_GCR_DUTY_MASK                        0x7u
#define LCD_GCR_DUTY_SHIFT                       0
#define LCD_GCR_DUTY(x)                          (((uint32_t)(((uint32_t)(x))<<LCD_GCR_DUTY_SHIFT))&LCD_GCR_DUTY_MASK)
#define LCD_GCR_LCLK_MASK                        0x38u
#define LCD_GCR_LCLK_SHIFT                       3
#define LCD_GCR_LCLK(x)                          (((uint32_t)(((uint32_t)(x))<<LCD_GCR_LCLK_SHIFT))&LCD_GCR_LCLK_MASK)
#define LCD_GCR_SOURCE_MASK                      0x40u
#define LCD_GCR_SOURCE_SHIFT                     6
#define LCD_GCR_LCDEN_MASK                       0x80u
#define LCD_GCR_LCDEN_SHIFT                      7
#define LCD_GCR_LCDSTP_MASK                      0x100u
#define LCD_GCR_LCDSTP_SHIFT                     8
#define LCD_GCR_LCDDOZE_MASK                     0x200u
#define LCD_GCR_LCDDOZE_SHIFT                    9
#define LCD_GCR_ALTDIV_MASK                      0x3000u
#define LCD_GCR_ALTDIV_SHIFT                     12
#define LCD_GCR_ALTDIV(x)                        (((uint32_t)(((uint32_t)(x))<<LCD_GCR_ALTDIV_SHIFT))&LCD_GCR_ALTDIV_MASK)
#define LCD_GCR_FDCIEN_MASK                      0x4000u
#define LCD_GCR_FDCIEN_SHIFT                     14
#define LCD_GCR_LCDIEN_MASK                      0x8000u
#define LCD_GCR_LCDIEN_SHIFT                     15
#define LCD_GCR_VSUPPLY_MASK                     0x20000u
#define LCD_GCR_VSUPPLY_SHIFT                    17
#define LCD_GCR_LADJ_MASK                        0x300000u
#define LCD_GCR_LADJ_SHIFT                       20
#define LCD_GCR_LADJ(x)                          (((uint32_t)(((uint32_t)(x))<<LCD_GCR_LADJ_SHIFT))&LCD_GCR_LADJ_MASK)
#define LCD_GCR_CPSEL_MASK                       0x800000u
#define LCD_GCR_CPSEL_SHIFT                      23
#define LCD_GCR_RVTRIM_MASK                      0xF000000u
#define LCD_GCR_RVTRIM_SHIFT                     24
#define LCD_GCR_RVTRIM(x)                        (((uint32_t)(((uint32_t)(x))<<LCD_GCR_RVTRIM_SHIFT))&LCD_GCR_RVTRIM_MASK)
#define LCD_GCR_RVEN_MASK                        0x80000000u
#define LCD_GCR_RVEN_SHIFT                       31
/* AR Bit Fields */
#define LCD_AR_BRATE_MASK                        0x7u
#define LCD_AR_BRATE_SHIFT                       0
#define LCD_AR_BRATE(x)                          (((uint32_t)(((uint32_t)(x))<<LCD_AR_BRATE_SHIFT))&LCD_AR_BRATE_MASK)
#define LCD_AR_BMODE_MASK                        0x8u
#define LCD_AR_BMODE_SHIFT                       3
#define LCD_AR_BLANK_MASK                        0x20u
#define LCD_AR_BLANK_SHIFT                       5
#define LCD_AR_ALT_MASK                          0x40u
#define LCD_AR_ALT_SHIFT                         6
#define LCD_AR_BLINK_MASK                        0x80u
#define LCD_AR_BLINK_SHIFT                       7
#define LCD_AR_LCDIF_MASK                        0x8000u
#define LCD_AR_LCDIF_SHIFT                       15
/* FDCR Bit Fields */
#define LCD_FDCR_FDPINID_MASK                    0x3Fu
#define LCD_FDCR_FDPINID_SHIFT                   0
#define LCD_FDCR_FDPINID(x)                      (((uint32_t)(((uint32_t)(x))<<LCD_FDCR_FDPINID_SHIFT))&LCD_FDCR_FDPINID_MASK)
#define LCD_FDCR_FDBPEN_MASK                     0x40u
#define LCD_FDCR_FDBPEN_SHIFT                    6
#define LCD_FDCR_FDEN_MASK                       0x80u
#define LCD_FDCR_FDEN_SHIFT                      7
#define LCD_FDCR_FDSWW_MASK                      0xE00u
#define LCD_FDCR_FDSWW_SHIFT                     9
#define LCD_FDCR_FDSWW(x)                        (((uint32_t)(((uint32_t)(x))<<LCD_FDCR_FDSWW_SHIFT))&LCD_FDCR_FDSWW_MASK)
#define LCD_FDCR_FDPRS_MASK                      0x7000u
#define LCD_FDCR_FDPRS_SHIFT                     12
#define LCD_FDCR_FDPRS(x)                        (((uint32_t)(((uint32_t)(x))<<LCD_FDCR_FDPRS_SHIFT))&LCD_FDCR_FDPRS_MASK)
/* FDSR Bit Fields */
#define LCD_FDSR_FDCNT_MASK                      0xFFu
#define LCD_FDSR_FDCNT_SHIFT                     0
#define LCD_FDSR_FDCNT(x)                        (((uint32_t)(((uint32_t)(x))<<LCD_FDSR_FDCNT_SHIFT))&LCD_FDSR_FDCNT_MASK)
#define LCD_FDSR_FDCF_MASK                       0x8000u
#define LCD_FDSR_FDCF_SHIFT                      15
/* PEN Bit Fields */
#define LCD_PEN_PEN_MASK                         0xFFFFFFFFu
#define LCD_PEN_PEN_SHIFT                        0
#define LCD_PEN_PEN(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_PEN_PEN_SHIFT))&LCD_PEN_PEN_MASK)
/* BPEN Bit Fields */
#define LCD_BPEN_BPEN_MASK                       0xFFFFFFFFu
#define LCD_BPEN_BPEN_SHIFT                      0
#define LCD_BPEN_BPEN(x)                         (((uint32_t)(((uint32_t)(x))<<LCD_BPEN_BPEN_SHIFT))&LCD_BPEN_BPEN_MASK)
/* WF Bit Fields */
#define LCD_WF_WF0_MASK                          0xFFu
#define LCD_WF_WF0_SHIFT                         0
#define LCD_WF_WF0(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF0_SHIFT))&LCD_WF_WF0_MASK)
#define LCD_WF_WF60_MASK                         0xFFu
#define LCD_WF_WF60_SHIFT                        0
#define LCD_WF_WF60(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF60_SHIFT))&LCD_WF_WF60_MASK)
#define LCD_WF_WF56_MASK                         0xFFu
#define LCD_WF_WF56_SHIFT                        0
#define LCD_WF_WF56(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF56_SHIFT))&LCD_WF_WF56_MASK)
#define LCD_WF_WF52_MASK                         0xFFu
#define LCD_WF_WF52_SHIFT                        0
#define LCD_WF_WF52(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF52_SHIFT))&LCD_WF_WF52_MASK)
#define LCD_WF_WF4_MASK                          0xFFu
#define LCD_WF_WF4_SHIFT                         0
#define LCD_WF_WF4(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF4_SHIFT))&LCD_WF_WF4_MASK)
#define LCD_WF_WF48_MASK                         0xFFu
#define LCD_WF_WF48_SHIFT                        0
#define LCD_WF_WF48(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF48_SHIFT))&LCD_WF_WF48_MASK)
#define LCD_WF_WF44_MASK                         0xFFu
#define LCD_WF_WF44_SHIFT                        0
#define LCD_WF_WF44(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF44_SHIFT))&LCD_WF_WF44_MASK)
#define LCD_WF_WF40_MASK                         0xFFu
#define LCD_WF_WF40_SHIFT                        0
#define LCD_WF_WF40(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF40_SHIFT))&LCD_WF_WF40_MASK)
#define LCD_WF_WF8_MASK                          0xFFu
#define LCD_WF_WF8_SHIFT                         0
#define LCD_WF_WF8(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF8_SHIFT))&LCD_WF_WF8_MASK)
#define LCD_WF_WF36_MASK                         0xFFu
#define LCD_WF_WF36_SHIFT                        0
#define LCD_WF_WF36(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF36_SHIFT))&LCD_WF_WF36_MASK)
#define LCD_WF_WF32_MASK                         0xFFu
#define LCD_WF_WF32_SHIFT                        0
#define LCD_WF_WF32(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF32_SHIFT))&LCD_WF_WF32_MASK)
#define LCD_WF_WF28_MASK                         0xFFu
#define LCD_WF_WF28_SHIFT                        0
#define LCD_WF_WF28(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF28_SHIFT))&LCD_WF_WF28_MASK)
#define LCD_WF_WF12_MASK                         0xFFu
#define LCD_WF_WF12_SHIFT                        0
#define LCD_WF_WF12(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF12_SHIFT))&LCD_WF_WF12_MASK)
#define LCD_WF_WF24_MASK                         0xFFu
#define LCD_WF_WF24_SHIFT                        0
#define LCD_WF_WF24(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF24_SHIFT))&LCD_WF_WF24_MASK)
#define LCD_WF_WF20_MASK                         0xFFu
#define LCD_WF_WF20_SHIFT                        0
#define LCD_WF_WF20(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF20_SHIFT))&LCD_WF_WF20_MASK)
#define LCD_WF_WF16_MASK                         0xFFu
#define LCD_WF_WF16_SHIFT                        0
#define LCD_WF_WF16(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF16_SHIFT))&LCD_WF_WF16_MASK)
#define LCD_WF_WF5_MASK                          0xFF00u
#define LCD_WF_WF5_SHIFT                         8
#define LCD_WF_WF5(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF5_SHIFT))&LCD_WF_WF5_MASK)
#define LCD_WF_WF49_MASK                         0xFF00u
#define LCD_WF_WF49_SHIFT                        8
#define LCD_WF_WF49(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF49_SHIFT))&LCD_WF_WF49_MASK)
#define LCD_WF_WF45_MASK                         0xFF00u
#define LCD_WF_WF45_SHIFT                        8
#define LCD_WF_WF45(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF45_SHIFT))&LCD_WF_WF45_MASK)
#define LCD_WF_WF61_MASK                         0xFF00u
#define LCD_WF_WF61_SHIFT                        8
#define LCD_WF_WF61(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF61_SHIFT))&LCD_WF_WF61_MASK)
#define LCD_WF_WF25_MASK                         0xFF00u
#define LCD_WF_WF25_SHIFT                        8
#define LCD_WF_WF25(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF25_SHIFT))&LCD_WF_WF25_MASK)
#define LCD_WF_WF17_MASK                         0xFF00u
#define LCD_WF_WF17_SHIFT                        8
#define LCD_WF_WF17(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF17_SHIFT))&LCD_WF_WF17_MASK)
#define LCD_WF_WF41_MASK                         0xFF00u
#define LCD_WF_WF41_SHIFT                        8
#define LCD_WF_WF41(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF41_SHIFT))&LCD_WF_WF41_MASK)
#define LCD_WF_WF13_MASK                         0xFF00u
#define LCD_WF_WF13_SHIFT                        8
#define LCD_WF_WF13(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF13_SHIFT))&LCD_WF_WF13_MASK)
#define LCD_WF_WF57_MASK                         0xFF00u
#define LCD_WF_WF57_SHIFT                        8
#define LCD_WF_WF57(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF57_SHIFT))&LCD_WF_WF57_MASK)
#define LCD_WF_WF53_MASK                         0xFF00u
#define LCD_WF_WF53_SHIFT                        8
#define LCD_WF_WF53(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF53_SHIFT))&LCD_WF_WF53_MASK)
#define LCD_WF_WF37_MASK                         0xFF00u
#define LCD_WF_WF37_SHIFT                        8
#define LCD_WF_WF37(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF37_SHIFT))&LCD_WF_WF37_MASK)
#define LCD_WF_WF9_MASK                          0xFF00u
#define LCD_WF_WF9_SHIFT                         8
#define LCD_WF_WF9(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF9_SHIFT))&LCD_WF_WF9_MASK)
#define LCD_WF_WF1_MASK                          0xFF00u
#define LCD_WF_WF1_SHIFT                         8
#define LCD_WF_WF1(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF1_SHIFT))&LCD_WF_WF1_MASK)
#define LCD_WF_WF29_MASK                         0xFF00u
#define LCD_WF_WF29_SHIFT                        8
#define LCD_WF_WF29(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF29_SHIFT))&LCD_WF_WF29_MASK)
#define LCD_WF_WF33_MASK                         0xFF00u
#define LCD_WF_WF33_SHIFT                        8
#define LCD_WF_WF33(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF33_SHIFT))&LCD_WF_WF33_MASK)
#define LCD_WF_WF21_MASK                         0xFF00u
#define LCD_WF_WF21_SHIFT                        8
#define LCD_WF_WF21(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF21_SHIFT))&LCD_WF_WF21_MASK)
#define LCD_WF_WF26_MASK                         0xFF0000u
#define LCD_WF_WF26_SHIFT                        16
#define LCD_WF_WF26(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF26_SHIFT))&LCD_WF_WF26_MASK)
#define LCD_WF_WF46_MASK                         0xFF0000u
#define LCD_WF_WF46_SHIFT                        16
#define LCD_WF_WF46(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF46_SHIFT))&LCD_WF_WF46_MASK)
#define LCD_WF_WF6_MASK                          0xFF0000u
#define LCD_WF_WF6_SHIFT                         16
#define LCD_WF_WF6(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF6_SHIFT))&LCD_WF_WF6_MASK)
#define LCD_WF_WF42_MASK                         0xFF0000u
#define LCD_WF_WF42_SHIFT                        16
#define LCD_WF_WF42(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF42_SHIFT))&LCD_WF_WF42_MASK)
#define LCD_WF_WF18_MASK                         0xFF0000u
#define LCD_WF_WF18_SHIFT                        16
#define LCD_WF_WF18(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF18_SHIFT))&LCD_WF_WF18_MASK)
#define LCD_WF_WF38_MASK                         0xFF0000u
#define LCD_WF_WF38_SHIFT                        16
#define LCD_WF_WF38(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF38_SHIFT))&LCD_WF_WF38_MASK)
#define LCD_WF_WF22_MASK                         0xFF0000u
#define LCD_WF_WF22_SHIFT                        16
#define LCD_WF_WF22(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF22_SHIFT))&LCD_WF_WF22_MASK)
#define LCD_WF_WF34_MASK                         0xFF0000u
#define LCD_WF_WF34_SHIFT                        16
#define LCD_WF_WF34(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF34_SHIFT))&LCD_WF_WF34_MASK)
#define LCD_WF_WF50_MASK                         0xFF0000u
#define LCD_WF_WF50_SHIFT                        16
#define LCD_WF_WF50(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF50_SHIFT))&LCD_WF_WF50_MASK)
#define LCD_WF_WF14_MASK                         0xFF0000u
#define LCD_WF_WF14_SHIFT                        16
#define LCD_WF_WF14(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF14_SHIFT))&LCD_WF_WF14_MASK)
#define LCD_WF_WF54_MASK                         0xFF0000u
#define LCD_WF_WF54_SHIFT                        16
#define LCD_WF_WF54(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF54_SHIFT))&LCD_WF_WF54_MASK)
#define LCD_WF_WF2_MASK                          0xFF0000u
#define LCD_WF_WF2_SHIFT                         16
#define LCD_WF_WF2(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF2_SHIFT))&LCD_WF_WF2_MASK)
#define LCD_WF_WF58_MASK                         0xFF0000u
#define LCD_WF_WF58_SHIFT                        16
#define LCD_WF_WF58(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF58_SHIFT))&LCD_WF_WF58_MASK)
#define LCD_WF_WF30_MASK                         0xFF0000u
#define LCD_WF_WF30_SHIFT                        16
#define LCD_WF_WF30(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF30_SHIFT))&LCD_WF_WF30_MASK)
#define LCD_WF_WF62_MASK                         0xFF0000u
#define LCD_WF_WF62_SHIFT                        16
#define LCD_WF_WF62(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF62_SHIFT))&LCD_WF_WF62_MASK)
#define LCD_WF_WF10_MASK                         0xFF0000u
#define LCD_WF_WF10_SHIFT                        16
#define LCD_WF_WF10(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF10_SHIFT))&LCD_WF_WF10_MASK)
#define LCD_WF_WF63_MASK                         0xFF000000u
#define LCD_WF_WF63_SHIFT                        24
#define LCD_WF_WF63(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF63_SHIFT))&LCD_WF_WF63_MASK)
#define LCD_WF_WF59_MASK                         0xFF000000u
#define LCD_WF_WF59_SHIFT                        24
#define LCD_WF_WF59(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF59_SHIFT))&LCD_WF_WF59_MASK)
#define LCD_WF_WF55_MASK                         0xFF000000u
#define LCD_WF_WF55_SHIFT                        24
#define LCD_WF_WF55(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF55_SHIFT))&LCD_WF_WF55_MASK)
#define LCD_WF_WF3_MASK                          0xFF000000u
#define LCD_WF_WF3_SHIFT                         24
#define LCD_WF_WF3(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF3_SHIFT))&LCD_WF_WF3_MASK)
#define LCD_WF_WF51_MASK                         0xFF000000u
#define LCD_WF_WF51_SHIFT                        24
#define LCD_WF_WF51(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF51_SHIFT))&LCD_WF_WF51_MASK)
#define LCD_WF_WF47_MASK                         0xFF000000u
#define LCD_WF_WF47_SHIFT                        24
#define LCD_WF_WF47(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF47_SHIFT))&LCD_WF_WF47_MASK)
#define LCD_WF_WF43_MASK                         0xFF000000u
#define LCD_WF_WF43_SHIFT                        24
#define LCD_WF_WF43(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF43_SHIFT))&LCD_WF_WF43_MASK)
#define LCD_WF_WF7_MASK                          0xFF000000u
#define LCD_WF_WF7_SHIFT                         24
#define LCD_WF_WF7(x)                            (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF7_SHIFT))&LCD_WF_WF7_MASK)
#define LCD_WF_WF39_MASK                         0xFF000000u
#define LCD_WF_WF39_SHIFT                        24
#define LCD_WF_WF39(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF39_SHIFT))&LCD_WF_WF39_MASK)
#define LCD_WF_WF35_MASK                         0xFF000000u
#define LCD_WF_WF35_SHIFT                        24
#define LCD_WF_WF35(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF35_SHIFT))&LCD_WF_WF35_MASK)
#define LCD_WF_WF31_MASK                         0xFF000000u
#define LCD_WF_WF31_SHIFT                        24
#define LCD_WF_WF31(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF31_SHIFT))&LCD_WF_WF31_MASK)
#define LCD_WF_WF11_MASK                         0xFF000000u
#define LCD_WF_WF11_SHIFT                        24
#define LCD_WF_WF11(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF11_SHIFT))&LCD_WF_WF11_MASK)
#define LCD_WF_WF27_MASK                         0xFF000000u
#define LCD_WF_WF27_SHIFT                        24
#define LCD_WF_WF27(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF27_SHIFT))&LCD_WF_WF27_MASK)
#define LCD_WF_WF23_MASK                         0xFF000000u
#define LCD_WF_WF23_SHIFT                        24
#define LCD_WF_WF23(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF23_SHIFT))&LCD_WF_WF23_MASK)
#define LCD_WF_WF19_MASK                         0xFF000000u
#define LCD_WF_WF19_SHIFT                        24
#define LCD_WF_WF19(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF19_SHIFT))&LCD_WF_WF19_MASK)
#define LCD_WF_WF15_MASK                         0xFF000000u
#define LCD_WF_WF15_SHIFT                        24
#define LCD_WF_WF15(x)                           (((uint32_t)(((uint32_t)(x))<<LCD_WF_WF15_SHIFT))&LCD_WF_WF15_MASK)
/* WF8B Bit Fields */
#define LCD_WF8B_BPALCD0_MASK                    0x1u
#define LCD_WF8B_BPALCD0_SHIFT                   0
#define LCD_WF8B_BPALCD63_MASK                   0x1u
#define LCD_WF8B_BPALCD63_SHIFT                  0
#define LCD_WF8B_BPALCD62_MASK                   0x1u
#define LCD_WF8B_BPALCD62_SHIFT                  0
#define LCD_WF8B_BPALCD61_MASK                   0x1u
#define LCD_WF8B_BPALCD61_SHIFT                  0
#define LCD_WF8B_BPALCD60_MASK                   0x1u
#define LCD_WF8B_BPALCD60_SHIFT                  0
#define LCD_WF8B_BPALCD59_MASK                   0x1u
#define LCD_WF8B_BPALCD59_SHIFT                  0
#define LCD_WF8B_BPALCD58_MASK                   0x1u
#define LCD_WF8B_BPALCD58_SHIFT                  0
#define LCD_WF8B_BPALCD57_MASK                   0x1u
#define LCD_WF8B_BPALCD57_SHIFT                  0
#define LCD_WF8B_BPALCD1_MASK                    0x1u
#define LCD_WF8B_BPALCD1_SHIFT                   0
#define LCD_WF8B_BPALCD56_MASK                   0x1u
#define LCD_WF8B_BPALCD56_SHIFT                  0
#define LCD_WF8B_BPALCD55_MASK                   0x1u
#define LCD_WF8B_BPALCD55_SHIFT                  0
#define LCD_WF8B_BPALCD54_MASK                   0x1u
#define LCD_WF8B_BPALCD54_SHIFT                  0
#define LCD_WF8B_BPALCD53_MASK                   0x1u
#define LCD_WF8B_BPALCD53_SHIFT                  0
#define LCD_WF8B_BPALCD52_MASK                   0x1u
#define LCD_WF8B_BPALCD52_SHIFT                  0
#define LCD_WF8B_BPALCD51_MASK                   0x1u
#define LCD_WF8B_BPALCD51_SHIFT                  0
#define LCD_WF8B_BPALCD50_MASK                   0x1u
#define LCD_WF8B_BPALCD50_SHIFT                  0
#define LCD_WF8B_BPALCD2_MASK                    0x1u
#define LCD_WF8B_BPALCD2_SHIFT                   0
#define LCD_WF8B_BPALCD49_MASK                   0x1u
#define LCD_WF8B_BPALCD49_SHIFT                  0
#define LCD_WF8B_BPALCD48_MASK                   0x1u
#define LCD_WF8B_BPALCD48_SHIFT                  0
#define LCD_WF8B_BPALCD47_MASK                   0x1u
#define LCD_WF8B_BPALCD47_SHIFT                  0
#define LCD_WF8B_BPALCD46_MASK                   0x1u
#define LCD_WF8B_BPALCD46_SHIFT                  0
#define LCD_WF8B_BPALCD45_MASK                   0x1u
#define LCD_WF8B_BPALCD45_SHIFT                  0
#define LCD_WF8B_BPALCD44_MASK                   0x1u
#define LCD_WF8B_BPALCD44_SHIFT                  0
#define LCD_WF8B_BPALCD43_MASK                   0x1u
#define LCD_WF8B_BPALCD43_SHIFT                  0
#define LCD_WF8B_BPALCD3_MASK                    0x1u
#define LCD_WF8B_BPALCD3_SHIFT                   0
#define LCD_WF8B_BPALCD42_MASK                   0x1u
#define LCD_WF8B_BPALCD42_SHIFT                  0
#define LCD_WF8B_BPALCD41_MASK                   0x1u
#define LCD_WF8B_BPALCD41_SHIFT                  0
#define LCD_WF8B_BPALCD40_MASK                   0x1u
#define LCD_WF8B_BPALCD40_SHIFT                  0
#define LCD_WF8B_BPALCD39_MASK                   0x1u
#define LCD_WF8B_BPALCD39_SHIFT                  0
#define LCD_WF8B_BPALCD38_MASK                   0x1u
#define LCD_WF8B_BPALCD38_SHIFT                  0
#define LCD_WF8B_BPALCD37_MASK                   0x1u
#define LCD_WF8B_BPALCD37_SHIFT                  0
#define LCD_WF8B_BPALCD36_MASK                   0x1u
#define LCD_WF8B_BPALCD36_SHIFT                  0
#define LCD_WF8B_BPALCD4_MASK                    0x1u
#define LCD_WF8B_BPALCD4_SHIFT                   0
#define LCD_WF8B_BPALCD35_MASK                   0x1u
#define LCD_WF8B_BPALCD35_SHIFT                  0
#define LCD_WF8B_BPALCD34_MASK                   0x1u
#define LCD_WF8B_BPALCD34_SHIFT                  0
#define LCD_WF8B_BPALCD33_MASK                   0x1u
#define LCD_WF8B_BPALCD33_SHIFT                  0
#define LCD_WF8B_BPALCD32_MASK                   0x1u
#define LCD_WF8B_BPALCD32_SHIFT                  0
#define LCD_WF8B_BPALCD31_MASK                   0x1u
#define LCD_WF8B_BPALCD31_SHIFT                  0
#define LCD_WF8B_BPALCD30_MASK                   0x1u
#define LCD_WF8B_BPALCD30_SHIFT                  0
#define LCD_WF8B_BPALCD29_MASK                   0x1u
#define LCD_WF8B_BPALCD29_SHIFT                  0
#define LCD_WF8B_BPALCD5_MASK                    0x1u
#define LCD_WF8B_BPALCD5_SHIFT                   0
#define LCD_WF8B_BPALCD28_MASK                   0x1u
#define LCD_WF8B_BPALCD28_SHIFT                  0
#define LCD_WF8B_BPALCD27_MASK                   0x1u
#define LCD_WF8B_BPALCD27_SHIFT                  0
#define LCD_WF8B_BPALCD26_MASK                   0x1u
#define LCD_WF8B_BPALCD26_SHIFT                  0
#define LCD_WF8B_BPALCD25_MASK                   0x1u
#define LCD_WF8B_BPALCD25_SHIFT                  0
#define LCD_WF8B_BPALCD24_MASK                   0x1u
#define LCD_WF8B_BPALCD24_SHIFT                  0
#define LCD_WF8B_BPALCD23_MASK                   0x1u
#define LCD_WF8B_BPALCD23_SHIFT                  0
#define LCD_WF8B_BPALCD22_MASK                   0x1u
#define LCD_WF8B_BPALCD22_SHIFT                  0
#define LCD_WF8B_BPALCD6_MASK                    0x1u
#define LCD_WF8B_BPALCD6_SHIFT                   0
#define LCD_WF8B_BPALCD21_MASK                   0x1u
#define LCD_WF8B_BPALCD21_SHIFT                  0
#define LCD_WF8B_BPALCD20_MASK                   0x1u
#define LCD_WF8B_BPALCD20_SHIFT                  0
#define LCD_WF8B_BPALCD19_MASK                   0x1u
#define LCD_WF8B_BPALCD19_SHIFT                  0
#define LCD_WF8B_BPALCD18_MASK                   0x1u
#define LCD_WF8B_BPALCD18_SHIFT                  0
#define LCD_WF8B_BPALCD17_MASK                   0x1u
#define LCD_WF8B_BPALCD17_SHIFT                  0
#define LCD_WF8B_BPALCD16_MASK                   0x1u
#define LCD_WF8B_BPALCD16_SHIFT                  0
#define LCD_WF8B_BPALCD15_MASK                   0x1u
#define LCD_WF8B_BPALCD15_SHIFT                  0
#define LCD_WF8B_BPALCD7_MASK                    0x1u
#define LCD_WF8B_BPALCD7_SHIFT                   0
#define LCD_WF8B_BPALCD14_MASK                   0x1u
#define LCD_WF8B_BPALCD14_SHIFT                  0
#define LCD_WF8B_BPALCD13_MASK                   0x1u
#define LCD_WF8B_BPALCD13_SHIFT                  0
#define LCD_WF8B_BPALCD12_MASK                   0x1u
#define LCD_WF8B_BPALCD12_SHIFT                  0
#define LCD_WF8B_BPALCD11_MASK                   0x1u
#define LCD_WF8B_BPALCD11_SHIFT                  0
#define LCD_WF8B_BPALCD10_MASK                   0x1u
#define LCD_WF8B_BPALCD10_SHIFT                  0
#define LCD_WF8B_BPALCD9_MASK                    0x1u
#define LCD_WF8B_BPALCD9_SHIFT                   0
#define LCD_WF8B_BPALCD8_MASK                    0x1u
#define LCD_WF8B_BPALCD8_SHIFT                   0
#define LCD_WF8B_BPBLCD1_MASK                    0x2u
#define LCD_WF8B_BPBLCD1_SHIFT                   1
#define LCD_WF8B_BPBLCD32_MASK                   0x2u
#define LCD_WF8B_BPBLCD32_SHIFT                  1
#define LCD_WF8B_BPBLCD30_MASK                   0x2u
#define LCD_WF8B_BPBLCD30_SHIFT                  1
#define LCD_WF8B_BPBLCD60_MASK                   0x2u
#define LCD_WF8B_BPBLCD60_SHIFT                  1
#define LCD_WF8B_BPBLCD24_MASK                   0x2u
#define LCD_WF8B_BPBLCD24_SHIFT                  1
#define LCD_WF8B_BPBLCD28_MASK                   0x2u
#define LCD_WF8B_BPBLCD28_SHIFT                  1
#define LCD_WF8B_BPBLCD23_MASK                   0x2u
#define LCD_WF8B_BPBLCD23_SHIFT                  1
#define LCD_WF8B_BPBLCD48_MASK                   0x2u
#define LCD_WF8B_BPBLCD48_SHIFT                  1
#define LCD_WF8B_BPBLCD10_MASK                   0x2u
#define LCD_WF8B_BPBLCD10_SHIFT                  1
#define LCD_WF8B_BPBLCD15_MASK                   0x2u
#define LCD_WF8B_BPBLCD15_SHIFT                  1
#define LCD_WF8B_BPBLCD36_MASK                   0x2u
#define LCD_WF8B_BPBLCD36_SHIFT                  1
#define LCD_WF8B_BPBLCD44_MASK                   0x2u
#define LCD_WF8B_BPBLCD44_SHIFT                  1
#define LCD_WF8B_BPBLCD62_MASK                   0x2u
#define LCD_WF8B_BPBLCD62_SHIFT                  1
#define LCD_WF8B_BPBLCD53_MASK                   0x2u
#define LCD_WF8B_BPBLCD53_SHIFT                  1
#define LCD_WF8B_BPBLCD22_MASK                   0x2u
#define LCD_WF8B_BPBLCD22_SHIFT                  1
#define LCD_WF8B_BPBLCD47_MASK                   0x2u
#define LCD_WF8B_BPBLCD47_SHIFT                  1
#define LCD_WF8B_BPBLCD33_MASK                   0x2u
#define LCD_WF8B_BPBLCD33_SHIFT                  1
#define LCD_WF8B_BPBLCD2_MASK                    0x2u
#define LCD_WF8B_BPBLCD2_SHIFT                   1
#define LCD_WF8B_BPBLCD49_MASK                   0x2u
#define LCD_WF8B_BPBLCD49_SHIFT                  1
#define LCD_WF8B_BPBLCD0_MASK                    0x2u
#define LCD_WF8B_BPBLCD0_SHIFT                   1
#define LCD_WF8B_BPBLCD55_MASK                   0x2u
#define LCD_WF8B_BPBLCD55_SHIFT                  1
#define LCD_WF8B_BPBLCD56_MASK                   0x2u
#define LCD_WF8B_BPBLCD56_SHIFT                  1
#define LCD_WF8B_BPBLCD21_MASK                   0x2u
#define LCD_WF8B_BPBLCD21_SHIFT                  1
#define LCD_WF8B_BPBLCD6_MASK                    0x2u
#define LCD_WF8B_BPBLCD6_SHIFT                   1
#define LCD_WF8B_BPBLCD29_MASK                   0x2u
#define LCD_WF8B_BPBLCD29_SHIFT                  1
#define LCD_WF8B_BPBLCD25_MASK                   0x2u
#define LCD_WF8B_BPBLCD25_SHIFT                  1
#define LCD_WF8B_BPBLCD8_MASK                    0x2u
#define LCD_WF8B_BPBLCD8_SHIFT                   1
#define LCD_WF8B_BPBLCD54_MASK                   0x2u
#define LCD_WF8B_BPBLCD54_SHIFT                  1
#define LCD_WF8B_BPBLCD38_MASK                   0x2u
#define LCD_WF8B_BPBLCD38_SHIFT                  1
#define LCD_WF8B_BPBLCD43_MASK                   0x2u
#define LCD_WF8B_BPBLCD43_SHIFT                  1
#define LCD_WF8B_BPBLCD20_MASK                   0x2u
#define LCD_WF8B_BPBLCD20_SHIFT                  1
#define LCD_WF8B_BPBLCD9_MASK                    0x2u
#define LCD_WF8B_BPBLCD9_SHIFT                   1
#define LCD_WF8B_BPBLCD7_MASK                    0x2u
#define LCD_WF8B_BPBLCD7_SHIFT                   1
#define LCD_WF8B_BPBLCD50_MASK                   0x2u
#define LCD_WF8B_BPBLCD50_SHIFT                  1
#define LCD_WF8B_BPBLCD40_MASK                   0x2u
#define LCD_WF8B_BPBLCD40_SHIFT                  1
#define LCD_WF8B_BPBLCD63_MASK                   0x2u
#define LCD_WF8B_BPBLCD63_SHIFT                  1
#define LCD_WF8B_BPBLCD26_MASK                   0x2u
#define LCD_WF8B_BPBLCD26_SHIFT                  1
#define LCD_WF8B_BPBLCD12_MASK                   0x2u
#define LCD_WF8B_BPBLCD12_SHIFT                  1
#define LCD_WF8B_BPBLCD19_MASK                   0x2u
#define LCD_WF8B_BPBLCD19_SHIFT                  1
#define LCD_WF8B_BPBLCD34_MASK                   0x2u
#define LCD_WF8B_BPBLCD34_SHIFT                  1
#define LCD_WF8B_BPBLCD39_MASK                   0x2u
#define LCD_WF8B_BPBLCD39_SHIFT                  1
#define LCD_WF8B_BPBLCD59_MASK                   0x2u
#define LCD_WF8B_BPBLCD59_SHIFT                  1
#define LCD_WF8B_BPBLCD61_MASK                   0x2u
#define LCD_WF8B_BPBLCD61_SHIFT                  1
#define LCD_WF8B_BPBLCD37_MASK                   0x2u
#define LCD_WF8B_BPBLCD37_SHIFT                  1
#define LCD_WF8B_BPBLCD31_MASK                   0x2u
#define LCD_WF8B_BPBLCD31_SHIFT                  1
#define LCD_WF8B_BPBLCD58_MASK                   0x2u
#define LCD_WF8B_BPBLCD58_SHIFT                  1
#define LCD_WF8B_BPBLCD18_MASK                   0x2u
#define LCD_WF8B_BPBLCD18_SHIFT                  1
#define LCD_WF8B_BPBLCD45_MASK                   0x2u
#define LCD_WF8B_BPBLCD45_SHIFT                  1
#define LCD_WF8B_BPBLCD27_MASK                   0x2u
#define LCD_WF8B_BPBLCD27_SHIFT                  1
#define LCD_WF8B_BPBLCD14_MASK                   0x2u
#define LCD_WF8B_BPBLCD14_SHIFT                  1
#define LCD_WF8B_BPBLCD51_MASK                   0x2u
#define LCD_WF8B_BPBLCD51_SHIFT                  1
#define LCD_WF8B_BPBLCD52_MASK                   0x2u
#define LCD_WF8B_BPBLCD52_SHIFT                  1
#define LCD_WF8B_BPBLCD4_MASK                    0x2u
#define LCD_WF8B_BPBLCD4_SHIFT                   1
#define LCD_WF8B_BPBLCD35_MASK                   0x2u
#define LCD_WF8B_BPBLCD35_SHIFT                  1
#define LCD_WF8B_BPBLCD17_MASK                   0x2u
#define LCD_WF8B_BPBLCD17_SHIFT                  1
#define LCD_WF8B_BPBLCD41_MASK                   0x2u
#define LCD_WF8B_BPBLCD41_SHIFT                  1
#define LCD_WF8B_BPBLCD11_MASK                   0x2u
#define LCD_WF8B_BPBLCD11_SHIFT                  1
#define LCD_WF8B_BPBLCD46_MASK                   0x2u
#define LCD_WF8B_BPBLCD46_SHIFT                  1
#define LCD_WF8B_BPBLCD57_MASK                   0x2u
#define LCD_WF8B_BPBLCD57_SHIFT                  1
#define LCD_WF8B_BPBLCD42_MASK                   0x2u
#define LCD_WF8B_BPBLCD42_SHIFT                  1
#define LCD_WF8B_BPBLCD5_MASK                    0x2u
#define LCD_WF8B_BPBLCD5_SHIFT                   1
#define LCD_WF8B_BPBLCD3_MASK                    0x2u
#define LCD_WF8B_BPBLCD3_SHIFT                   1
#define LCD_WF8B_BPBLCD16_MASK                   0x2u
#define LCD_WF8B_BPBLCD16_SHIFT                  1
#define LCD_WF8B_BPBLCD13_MASK                   0x2u
#define LCD_WF8B_BPBLCD13_SHIFT                  1
#define LCD_WF8B_BPCLCD10_MASK                   0x4u
#define LCD_WF8B_BPCLCD10_SHIFT                  2
#define LCD_WF8B_BPCLCD55_MASK                   0x4u
#define LCD_WF8B_BPCLCD55_SHIFT                  2
#define LCD_WF8B_BPCLCD2_MASK                    0x4u
#define LCD_WF8B_BPCLCD2_SHIFT                   2
#define LCD_WF8B_BPCLCD23_MASK                   0x4u
#define LCD_WF8B_BPCLCD23_SHIFT                  2
#define LCD_WF8B_BPCLCD48_MASK                   0x4u
#define LCD_WF8B_BPCLCD48_SHIFT                  2
#define LCD_WF8B_BPCLCD24_MASK                   0x4u
#define LCD_WF8B_BPCLCD24_SHIFT                  2
#define LCD_WF8B_BPCLCD60_MASK                   0x4u
#define LCD_WF8B_BPCLCD60_SHIFT                  2
#define LCD_WF8B_BPCLCD47_MASK                   0x4u
#define LCD_WF8B_BPCLCD47_SHIFT                  2
#define LCD_WF8B_BPCLCD22_MASK                   0x4u
#define LCD_WF8B_BPCLCD22_SHIFT                  2
#define LCD_WF8B_BPCLCD8_MASK                    0x4u
#define LCD_WF8B_BPCLCD8_SHIFT                   2
#define LCD_WF8B_BPCLCD21_MASK                   0x4u
#define LCD_WF8B_BPCLCD21_SHIFT                  2
#define LCD_WF8B_BPCLCD49_MASK                   0x4u
#define LCD_WF8B_BPCLCD49_SHIFT                  2
#define LCD_WF8B_BPCLCD25_MASK                   0x4u
#define LCD_WF8B_BPCLCD25_SHIFT                  2
#define LCD_WF8B_BPCLCD1_MASK                    0x4u
#define LCD_WF8B_BPCLCD1_SHIFT                   2
#define LCD_WF8B_BPCLCD20_MASK                   0x4u
#define LCD_WF8B_BPCLCD20_SHIFT                  2
#define LCD_WF8B_BPCLCD50_MASK                   0x4u
#define LCD_WF8B_BPCLCD50_SHIFT                  2
#define LCD_WF8B_BPCLCD19_MASK                   0x4u
#define LCD_WF8B_BPCLCD19_SHIFT                  2
#define LCD_WF8B_BPCLCD26_MASK                   0x4u
#define LCD_WF8B_BPCLCD26_SHIFT                  2
#define LCD_WF8B_BPCLCD59_MASK                   0x4u
#define LCD_WF8B_BPCLCD59_SHIFT                  2
#define LCD_WF8B_BPCLCD61_MASK                   0x4u
#define LCD_WF8B_BPCLCD61_SHIFT                  2
#define LCD_WF8B_BPCLCD46_MASK                   0x4u
#define LCD_WF8B_BPCLCD46_SHIFT                  2
#define LCD_WF8B_BPCLCD18_MASK                   0x4u
#define LCD_WF8B_BPCLCD18_SHIFT                  2
#define LCD_WF8B_BPCLCD5_MASK                    0x4u
#define LCD_WF8B_BPCLCD5_SHIFT                   2
#define LCD_WF8B_BPCLCD63_MASK                   0x4u
#define LCD_WF8B_BPCLCD63_SHIFT                  2
#define LCD_WF8B_BPCLCD27_MASK                   0x4u
#define LCD_WF8B_BPCLCD27_SHIFT                  2
#define LCD_WF8B_BPCLCD17_MASK                   0x4u
#define LCD_WF8B_BPCLCD17_SHIFT                  2
#define LCD_WF8B_BPCLCD51_MASK                   0x4u
#define LCD_WF8B_BPCLCD51_SHIFT                  2
#define LCD_WF8B_BPCLCD9_MASK                    0x4u
#define LCD_WF8B_BPCLCD9_SHIFT                   2
#define LCD_WF8B_BPCLCD54_MASK                   0x4u
#define LCD_WF8B_BPCLCD54_SHIFT                  2
#define LCD_WF8B_BPCLCD15_MASK                   0x4u
#define LCD_WF8B_BPCLCD15_SHIFT                  2
#define LCD_WF8B_BPCLCD16_MASK                   0x4u
#define LCD_WF8B_BPCLCD16_SHIFT                  2
#define LCD_WF8B_BPCLCD14_MASK                   0x4u
#define LCD_WF8B_BPCLCD14_SHIFT                  2
#define LCD_WF8B_BPCLCD32_MASK                   0x4u
#define LCD_WF8B_BPCLCD32_SHIFT                  2
#define LCD_WF8B_BPCLCD28_MASK                   0x4u
#define LCD_WF8B_BPCLCD28_SHIFT                  2
#define LCD_WF8B_BPCLCD53_MASK                   0x4u
#define LCD_WF8B_BPCLCD53_SHIFT                  2
#define LCD_WF8B_BPCLCD33_MASK                   0x4u
#define LCD_WF8B_BPCLCD33_SHIFT                  2
#define LCD_WF8B_BPCLCD0_MASK                    0x4u
#define LCD_WF8B_BPCLCD0_SHIFT                   2
#define LCD_WF8B_BPCLCD43_MASK                   0x4u
#define LCD_WF8B_BPCLCD43_SHIFT                  2
#define LCD_WF8B_BPCLCD7_MASK                    0x4u
#define LCD_WF8B_BPCLCD7_SHIFT                   2
#define LCD_WF8B_BPCLCD4_MASK                    0x4u
#define LCD_WF8B_BPCLCD4_SHIFT                   2
#define LCD_WF8B_BPCLCD34_MASK                   0x4u
#define LCD_WF8B_BPCLCD34_SHIFT                  2
#define LCD_WF8B_BPCLCD29_MASK                   0x4u
#define LCD_WF8B_BPCLCD29_SHIFT                  2
#define LCD_WF8B_BPCLCD45_MASK                   0x4u
#define LCD_WF8B_BPCLCD45_SHIFT                  2
#define LCD_WF8B_BPCLCD57_MASK                   0x4u
#define LCD_WF8B_BPCLCD57_SHIFT                  2
#define LCD_WF8B_BPCLCD42_MASK                   0x4u
#define LCD_WF8B_BPCLCD42_SHIFT                  2
#define LCD_WF8B_BPCLCD35_MASK                   0x4u
#define LCD_WF8B_BPCLCD35_SHIFT                  2
#define LCD_WF8B_BPCLCD13_MASK                   0x4u
#define LCD_WF8B_BPCLCD13_SHIFT                  2
#define LCD_WF8B_BPCLCD36_MASK                   0x4u
#define LCD_WF8B_BPCLCD36_SHIFT                  2
#define LCD_WF8B_BPCLCD30_MASK                   0x4u
#define LCD_WF8B_BPCLCD30_SHIFT                  2
#define LCD_WF8B_BPCLCD52_MASK                   0x4u
#define LCD_WF8B_BPCLCD52_SHIFT                  2
#define LCD_WF8B_BPCLCD58_MASK                   0x4u
#define LCD_WF8B_BPCLCD58_SHIFT                  2
#define LCD_WF8B_BPCLCD41_MASK                   0x4u
#define LCD_WF8B_BPCLCD41_SHIFT                  2
#define LCD_WF8B_BPCLCD37_MASK                   0x4u
#define LCD_WF8B_BPCLCD37_SHIFT                  2
#define LCD_WF8B_BPCLCD3_MASK                    0x4u
#define LCD_WF8B_BPCLCD3_SHIFT                   2
#define LCD_WF8B_BPCLCD12_MASK                   0x4u
#define LCD_WF8B_BPCLCD12_SHIFT                  2
#define LCD_WF8B_BPCLCD11_MASK                   0x4u
#define LCD_WF8B_BPCLCD11_SHIFT                  2
#define LCD_WF8B_BPCLCD38_MASK                   0x4u
#define LCD_WF8B_BPCLCD38_SHIFT                  2
#define LCD_WF8B_BPCLCD44_MASK                   0x4u
#define LCD_WF8B_BPCLCD44_SHIFT                  2
#define LCD_WF8B_BPCLCD31_MASK                   0x4u
#define LCD_WF8B_BPCLCD31_SHIFT                  2
#define LCD_WF8B_BPCLCD40_MASK                   0x4u
#define LCD_WF8B_BPCLCD40_SHIFT                  2
#define LCD_WF8B_BPCLCD62_MASK                   0x4u
#define LCD_WF8B_BPCLCD62_SHIFT                  2
#define LCD_WF8B_BPCLCD56_MASK                   0x4u
#define LCD_WF8B_BPCLCD56_SHIFT                  2
#define LCD_WF8B_BPCLCD39_MASK                   0x4u
#define LCD_WF8B_BPCLCD39_SHIFT                  2
#define LCD_WF8B_BPCLCD6_MASK                    0x4u
#define LCD_WF8B_BPCLCD6_SHIFT                   2
#define LCD_WF8B_BPDLCD47_MASK                   0x8u
#define LCD_WF8B_BPDLCD47_SHIFT                  3
#define LCD_WF8B_BPDLCD23_MASK                   0x8u
#define LCD_WF8B_BPDLCD23_SHIFT                  3
#define LCD_WF8B_BPDLCD48_MASK                   0x8u
#define LCD_WF8B_BPDLCD48_SHIFT                  3
#define LCD_WF8B_BPDLCD24_MASK                   0x8u
#define LCD_WF8B_BPDLCD24_SHIFT                  3
#define LCD_WF8B_BPDLCD15_MASK                   0x8u
#define LCD_WF8B_BPDLCD15_SHIFT                  3
#define LCD_WF8B_BPDLCD22_MASK                   0x8u
#define LCD_WF8B_BPDLCD22_SHIFT                  3
#define LCD_WF8B_BPDLCD60_MASK                   0x8u
#define LCD_WF8B_BPDLCD60_SHIFT                  3
#define LCD_WF8B_BPDLCD10_MASK                   0x8u
#define LCD_WF8B_BPDLCD10_SHIFT                  3
#define LCD_WF8B_BPDLCD21_MASK                   0x8u
#define LCD_WF8B_BPDLCD21_SHIFT                  3
#define LCD_WF8B_BPDLCD49_MASK                   0x8u
#define LCD_WF8B_BPDLCD49_SHIFT                  3
#define LCD_WF8B_BPDLCD1_MASK                    0x8u
#define LCD_WF8B_BPDLCD1_SHIFT                   3
#define LCD_WF8B_BPDLCD25_MASK                   0x8u
#define LCD_WF8B_BPDLCD25_SHIFT                  3
#define LCD_WF8B_BPDLCD20_MASK                   0x8u
#define LCD_WF8B_BPDLCD20_SHIFT                  3
#define LCD_WF8B_BPDLCD2_MASK                    0x8u
#define LCD_WF8B_BPDLCD2_SHIFT                   3
#define LCD_WF8B_BPDLCD55_MASK                   0x8u
#define LCD_WF8B_BPDLCD55_SHIFT                  3
#define LCD_WF8B_BPDLCD59_MASK                   0x8u
#define LCD_WF8B_BPDLCD59_SHIFT                  3
#define LCD_WF8B_BPDLCD5_MASK                    0x8u
#define LCD_WF8B_BPDLCD5_SHIFT                   3
#define LCD_WF8B_BPDLCD19_MASK                   0x8u
#define LCD_WF8B_BPDLCD19_SHIFT                  3
#define LCD_WF8B_BPDLCD6_MASK                    0x8u
#define LCD_WF8B_BPDLCD6_SHIFT                   3
#define LCD_WF8B_BPDLCD26_MASK                   0x8u
#define LCD_WF8B_BPDLCD26_SHIFT                  3
#define LCD_WF8B_BPDLCD0_MASK                    0x8u
#define LCD_WF8B_BPDLCD0_SHIFT                   3
#define LCD_WF8B_BPDLCD50_MASK                   0x8u
#define LCD_WF8B_BPDLCD50_SHIFT                  3
#define LCD_WF8B_BPDLCD46_MASK                   0x8u
#define LCD_WF8B_BPDLCD46_SHIFT                  3
#define LCD_WF8B_BPDLCD18_MASK                   0x8u
#define LCD_WF8B_BPDLCD18_SHIFT                  3
#define LCD_WF8B_BPDLCD61_MASK                   0x8u
#define LCD_WF8B_BPDLCD61_SHIFT                  3
#define LCD_WF8B_BPDLCD9_MASK                    0x8u
#define LCD_WF8B_BPDLCD9_SHIFT                   3
#define LCD_WF8B_BPDLCD17_MASK                   0x8u
#define LCD_WF8B_BPDLCD17_SHIFT                  3
#define LCD_WF8B_BPDLCD27_MASK                   0x8u
#define LCD_WF8B_BPDLCD27_SHIFT                  3
#define LCD_WF8B_BPDLCD53_MASK                   0x8u
#define LCD_WF8B_BPDLCD53_SHIFT                  3
#define LCD_WF8B_BPDLCD51_MASK                   0x8u
#define LCD_WF8B_BPDLCD51_SHIFT                  3
#define LCD_WF8B_BPDLCD54_MASK                   0x8u
#define LCD_WF8B_BPDLCD54_SHIFT                  3
#define LCD_WF8B_BPDLCD13_MASK                   0x8u
#define LCD_WF8B_BPDLCD13_SHIFT                  3
#define LCD_WF8B_BPDLCD16_MASK                   0x8u
#define LCD_WF8B_BPDLCD16_SHIFT                  3
#define LCD_WF8B_BPDLCD32_MASK                   0x8u
#define LCD_WF8B_BPDLCD32_SHIFT                  3
#define LCD_WF8B_BPDLCD14_MASK                   0x8u
#define LCD_WF8B_BPDLCD14_SHIFT                  3
#define LCD_WF8B_BPDLCD28_MASK                   0x8u
#define LCD_WF8B_BPDLCD28_SHIFT                  3
#define LCD_WF8B_BPDLCD43_MASK                   0x8u
#define LCD_WF8B_BPDLCD43_SHIFT                  3
#define LCD_WF8B_BPDLCD4_MASK                    0x8u
#define LCD_WF8B_BPDLCD4_SHIFT                   3
#define LCD_WF8B_BPDLCD45_MASK                   0x8u
#define LCD_WF8B_BPDLCD45_SHIFT                  3
#define LCD_WF8B_BPDLCD8_MASK                    0x8u
#define LCD_WF8B_BPDLCD8_SHIFT                   3
#define LCD_WF8B_BPDLCD62_MASK                   0x8u
#define LCD_WF8B_BPDLCD62_SHIFT                  3
#define LCD_WF8B_BPDLCD33_MASK                   0x8u
#define LCD_WF8B_BPDLCD33_SHIFT                  3
#define LCD_WF8B_BPDLCD34_MASK                   0x8u
#define LCD_WF8B_BPDLCD34_SHIFT                  3
#define LCD_WF8B_BPDLCD29_MASK                   0x8u
#define LCD_WF8B_BPDLCD29_SHIFT                  3
#define LCD_WF8B_BPDLCD58_MASK                   0x8u
#define LCD_WF8B_BPDLCD58_SHIFT                  3
#define LCD_WF8B_BPDLCD57_MASK                   0x8u
#define LCD_WF8B_BPDLCD57_SHIFT                  3
#define LCD_WF8B_BPDLCD42_MASK                   0x8u
#define LCD_WF8B_BPDLCD42_SHIFT                  3
#define LCD_WF8B_BPDLCD35_MASK                   0x8u
#define LCD_WF8B_BPDLCD35_SHIFT                  3
#define LCD_WF8B_BPDLCD52_MASK                   0x8u
#define LCD_WF8B_BPDLCD52_SHIFT                  3
#define LCD_WF8B_BPDLCD7_MASK                    0x8u
#define LCD_WF8B_BPDLCD7_SHIFT                   3
#define LCD_WF8B_BPDLCD36_MASK                   0x8u
#define LCD_WF8B_BPDLCD36_SHIFT                  3
#define LCD_WF8B_BPDLCD30_MASK                   0x8u
#define LCD_WF8B_BPDLCD30_SHIFT                  3
#define LCD_WF8B_BPDLCD41_MASK                   0x8u
#define LCD_WF8B_BPDLCD41_SHIFT                  3
#define LCD_WF8B_BPDLCD37_MASK                   0x8u
#define LCD_WF8B_BPDLCD37_SHIFT                  3
#define LCD_WF8B_BPDLCD44_MASK                   0x8u
#define LCD_WF8B_BPDLCD44_SHIFT                  3
#define LCD_WF8B_BPDLCD63_MASK                   0x8u
#define LCD_WF8B_BPDLCD63_SHIFT                  3
#define LCD_WF8B_BPDLCD38_MASK                   0x8u
#define LCD_WF8B_BPDLCD38_SHIFT                  3
#define LCD_WF8B_BPDLCD56_MASK                   0x8u
#define LCD_WF8B_BPDLCD56_SHIFT                  3
#define LCD_WF8B_BPDLCD40_MASK                   0x8u
#define LCD_WF8B_BPDLCD40_SHIFT                  3
#define LCD_WF8B_BPDLCD31_MASK                   0x8u
#define LCD_WF8B_BPDLCD31_SHIFT                  3
#define LCD_WF8B_BPDLCD12_MASK                   0x8u
#define LCD_WF8B_BPDLCD12_SHIFT                  3
#define LCD_WF8B_BPDLCD39_MASK                   0x8u
#define LCD_WF8B_BPDLCD39_SHIFT                  3
#define LCD_WF8B_BPDLCD3_MASK                    0x8u
#define LCD_WF8B_BPDLCD3_SHIFT                   3
#define LCD_WF8B_BPDLCD11_MASK                   0x8u
#define LCD_WF8B_BPDLCD11_SHIFT                  3
#define LCD_WF8B_BPELCD12_MASK                   0x10u
#define LCD_WF8B_BPELCD12_SHIFT                  4
#define LCD_WF8B_BPELCD39_MASK                   0x10u
#define LCD_WF8B_BPELCD39_SHIFT                  4
#define LCD_WF8B_BPELCD3_MASK                    0x10u
#define LCD_WF8B_BPELCD3_SHIFT                   4
#define LCD_WF8B_BPELCD38_MASK                   0x10u
#define LCD_WF8B_BPELCD38_SHIFT                  4
#define LCD_WF8B_BPELCD40_MASK                   0x10u
#define LCD_WF8B_BPELCD40_SHIFT                  4
#define LCD_WF8B_BPELCD37_MASK                   0x10u
#define LCD_WF8B_BPELCD37_SHIFT                  4
#define LCD_WF8B_BPELCD41_MASK                   0x10u
#define LCD_WF8B_BPELCD41_SHIFT                  4
#define LCD_WF8B_BPELCD36_MASK                   0x10u
#define LCD_WF8B_BPELCD36_SHIFT                  4
#define LCD_WF8B_BPELCD8_MASK                    0x10u
#define LCD_WF8B_BPELCD8_SHIFT                   4
#define LCD_WF8B_BPELCD35_MASK                   0x10u
#define LCD_WF8B_BPELCD35_SHIFT                  4
#define LCD_WF8B_BPELCD42_MASK                   0x10u
#define LCD_WF8B_BPELCD42_SHIFT                  4
#define LCD_WF8B_BPELCD34_MASK                   0x10u
#define LCD_WF8B_BPELCD34_SHIFT                  4
#define LCD_WF8B_BPELCD33_MASK                   0x10u
#define LCD_WF8B_BPELCD33_SHIFT                  4
#define LCD_WF8B_BPELCD11_MASK                   0x10u
#define LCD_WF8B_BPELCD11_SHIFT                  4
#define LCD_WF8B_BPELCD43_MASK                   0x10u
#define LCD_WF8B_BPELCD43_SHIFT                  4
#define LCD_WF8B_BPELCD32_MASK                   0x10u
#define LCD_WF8B_BPELCD32_SHIFT                  4
#define LCD_WF8B_BPELCD31_MASK                   0x10u
#define LCD_WF8B_BPELCD31_SHIFT                  4
#define LCD_WF8B_BPELCD44_MASK                   0x10u
#define LCD_WF8B_BPELCD44_SHIFT                  4
#define LCD_WF8B_BPELCD30_MASK                   0x10u
#define LCD_WF8B_BPELCD30_SHIFT                  4
#define LCD_WF8B_BPELCD29_MASK                   0x10u
#define LCD_WF8B_BPELCD29_SHIFT                  4
#define LCD_WF8B_BPELCD7_MASK                    0x10u
#define LCD_WF8B_BPELCD7_SHIFT                   4
#define LCD_WF8B_BPELCD45_MASK                   0x10u
#define LCD_WF8B_BPELCD45_SHIFT                  4
#define LCD_WF8B_BPELCD28_MASK                   0x10u
#define LCD_WF8B_BPELCD28_SHIFT                  4
#define LCD_WF8B_BPELCD2_MASK                    0x10u
#define LCD_WF8B_BPELCD2_SHIFT                   4
#define LCD_WF8B_BPELCD27_MASK                   0x10u
#define LCD_WF8B_BPELCD27_SHIFT                  4
#define LCD_WF8B_BPELCD46_MASK                   0x10u
#define LCD_WF8B_BPELCD46_SHIFT                  4
#define LCD_WF8B_BPELCD26_MASK                   0x10u
#define LCD_WF8B_BPELCD26_SHIFT                  4
#define LCD_WF8B_BPELCD10_MASK                   0x10u
#define LCD_WF8B_BPELCD10_SHIFT                  4
#define LCD_WF8B_BPELCD13_MASK                   0x10u
#define LCD_WF8B_BPELCD13_SHIFT                  4
#define LCD_WF8B_BPELCD25_MASK                   0x10u
#define LCD_WF8B_BPELCD25_SHIFT                  4
#define LCD_WF8B_BPELCD5_MASK                    0x10u
#define LCD_WF8B_BPELCD5_SHIFT                   4
#define LCD_WF8B_BPELCD24_MASK                   0x10u
#define LCD_WF8B_BPELCD24_SHIFT                  4
#define LCD_WF8B_BPELCD47_MASK                   0x10u
#define LCD_WF8B_BPELCD47_SHIFT                  4
#define LCD_WF8B_BPELCD23_MASK                   0x10u
#define LCD_WF8B_BPELCD23_SHIFT                  4
#define LCD_WF8B_BPELCD22_MASK                   0x10u
#define LCD_WF8B_BPELCD22_SHIFT                  4
#define LCD_WF8B_BPELCD48_MASK                   0x10u
#define LCD_WF8B_BPELCD48_SHIFT                  4
#define LCD_WF8B_BPELCD21_MASK                   0x10u
#define LCD_WF8B_BPELCD21_SHIFT                  4
#define LCD_WF8B_BPELCD49_MASK                   0x10u
#define LCD_WF8B_BPELCD49_SHIFT                  4
#define LCD_WF8B_BPELCD20_MASK                   0x10u
#define LCD_WF8B_BPELCD20_SHIFT                  4
#define LCD_WF8B_BPELCD19_MASK                   0x10u
#define LCD_WF8B_BPELCD19_SHIFT                  4
#define LCD_WF8B_BPELCD9_MASK                    0x10u
#define LCD_WF8B_BPELCD9_SHIFT                   4
#define LCD_WF8B_BPELCD50_MASK                   0x10u
#define LCD_WF8B_BPELCD50_SHIFT                  4
#define LCD_WF8B_BPELCD18_MASK                   0x10u
#define LCD_WF8B_BPELCD18_SHIFT                  4
#define LCD_WF8B_BPELCD6_MASK                    0x10u
#define LCD_WF8B_BPELCD6_SHIFT                   4
#define LCD_WF8B_BPELCD17_MASK                   0x10u
#define LCD_WF8B_BPELCD17_SHIFT                  4
#define LCD_WF8B_BPELCD51_MASK                   0x10u
#define LCD_WF8B_BPELCD51_SHIFT                  4
#define LCD_WF8B_BPELCD16_MASK                   0x10u
#define LCD_WF8B_BPELCD16_SHIFT                  4
#define LCD_WF8B_BPELCD56_MASK                   0x10u
#define LCD_WF8B_BPELCD56_SHIFT                  4
#define LCD_WF8B_BPELCD57_MASK                   0x10u
#define LCD_WF8B_BPELCD57_SHIFT                  4
#define LCD_WF8B_BPELCD52_MASK                   0x10u
#define LCD_WF8B_BPELCD52_SHIFT                  4
#define LCD_WF8B_BPELCD1_MASK                    0x10u
#define LCD_WF8B_BPELCD1_SHIFT                   4
#define LCD_WF8B_BPELCD58_MASK                   0x10u
#define LCD_WF8B_BPELCD58_SHIFT                  4
#define LCD_WF8B_BPELCD59_MASK                   0x10u
#define LCD_WF8B_BPELCD59_SHIFT                  4
#define LCD_WF8B_BPELCD53_MASK                   0x10u
#define LCD_WF8B_BPELCD53_SHIFT                  4
#define LCD_WF8B_BPELCD14_MASK                   0x10u
#define LCD_WF8B_BPELCD14_SHIFT                  4
#define LCD_WF8B_BPELCD0_MASK                    0x10u
#define LCD_WF8B_BPELCD0_SHIFT                   4
#define LCD_WF8B_BPELCD60_MASK                   0x10u
#define LCD_WF8B_BPELCD60_SHIFT                  4
#define LCD_WF8B_BPELCD15_MASK                   0x10u
#define LCD_WF8B_BPELCD15_SHIFT                  4
#define LCD_WF8B_BPELCD61_MASK                   0x10u
#define LCD_WF8B_BPELCD61_SHIFT                  4
#define LCD_WF8B_BPELCD54_MASK                   0x10u
#define LCD_WF8B_BPELCD54_SHIFT                  4
#define LCD_WF8B_BPELCD62_MASK                   0x10u
#define LCD_WF8B_BPELCD62_SHIFT                  4
#define LCD_WF8B_BPELCD63_MASK                   0x10u
#define LCD_WF8B_BPELCD63_SHIFT                  4
#define LCD_WF8B_BPELCD55_MASK                   0x10u
#define LCD_WF8B_BPELCD55_SHIFT                  4
#define LCD_WF8B_BPELCD4_MASK                    0x10u
#define LCD_WF8B_BPELCD4_SHIFT                   4
#define LCD_WF8B_BPFLCD13_MASK                   0x20u
#define LCD_WF8B_BPFLCD13_SHIFT                  5
#define LCD_WF8B_BPFLCD39_MASK                   0x20u
#define LCD_WF8B_BPFLCD39_SHIFT                  5
#define LCD_WF8B_BPFLCD55_MASK                   0x20u
#define LCD_WF8B_BPFLCD55_SHIFT                  5
#define LCD_WF8B_BPFLCD47_MASK                   0x20u
#define LCD_WF8B_BPFLCD47_SHIFT                  5
#define LCD_WF8B_BPFLCD63_MASK                   0x20u
#define LCD_WF8B_BPFLCD63_SHIFT                  5
#define LCD_WF8B_BPFLCD43_MASK                   0x20u
#define LCD_WF8B_BPFLCD43_SHIFT                  5
#define LCD_WF8B_BPFLCD5_MASK                    0x20u
#define LCD_WF8B_BPFLCD5_SHIFT                   5
#define LCD_WF8B_BPFLCD62_MASK                   0x20u
#define LCD_WF8B_BPFLCD62_SHIFT                  5
#define LCD_WF8B_BPFLCD14_MASK                   0x20u
#define LCD_WF8B_BPFLCD14_SHIFT                  5
#define LCD_WF8B_BPFLCD24_MASK                   0x20u
#define LCD_WF8B_BPFLCD24_SHIFT                  5
#define LCD_WF8B_BPFLCD54_MASK                   0x20u
#define LCD_WF8B_BPFLCD54_SHIFT                  5
#define LCD_WF8B_BPFLCD15_MASK                   0x20u
#define LCD_WF8B_BPFLCD15_SHIFT                  5
#define LCD_WF8B_BPFLCD32_MASK                   0x20u
#define LCD_WF8B_BPFLCD32_SHIFT                  5
#define LCD_WF8B_BPFLCD61_MASK                   0x20u
#define LCD_WF8B_BPFLCD61_SHIFT                  5
#define LCD_WF8B_BPFLCD25_MASK                   0x20u
#define LCD_WF8B_BPFLCD25_SHIFT                  5
#define LCD_WF8B_BPFLCD60_MASK                   0x20u
#define LCD_WF8B_BPFLCD60_SHIFT                  5
#define LCD_WF8B_BPFLCD41_MASK                   0x20u
#define LCD_WF8B_BPFLCD41_SHIFT                  5
#define LCD_WF8B_BPFLCD33_MASK                   0x20u
#define LCD_WF8B_BPFLCD33_SHIFT                  5
#define LCD_WF8B_BPFLCD53_MASK                   0x20u
#define LCD_WF8B_BPFLCD53_SHIFT                  5
#define LCD_WF8B_BPFLCD59_MASK                   0x20u
#define LCD_WF8B_BPFLCD59_SHIFT                  5
#define LCD_WF8B_BPFLCD0_MASK                    0x20u
#define LCD_WF8B_BPFLCD0_SHIFT                   5
#define LCD_WF8B_BPFLCD46_MASK                   0x20u
#define LCD_WF8B_BPFLCD46_SHIFT                  5
#define LCD_WF8B_BPFLCD58_MASK                   0x20u
#define LCD_WF8B_BPFLCD58_SHIFT                  5
#define LCD_WF8B_BPFLCD26_MASK                   0x20u
#define LCD_WF8B_BPFLCD26_SHIFT                  5
#define LCD_WF8B_BPFLCD36_MASK                   0x20u
#define LCD_WF8B_BPFLCD36_SHIFT                  5
#define LCD_WF8B_BPFLCD10_MASK                   0x20u
#define LCD_WF8B_BPFLCD10_SHIFT                  5
#define LCD_WF8B_BPFLCD52_MASK                   0x20u
#define LCD_WF8B_BPFLCD52_SHIFT                  5
#define LCD_WF8B_BPFLCD57_MASK                   0x20u
#define LCD_WF8B_BPFLCD57_SHIFT                  5
#define LCD_WF8B_BPFLCD27_MASK                   0x20u
#define LCD_WF8B_BPFLCD27_SHIFT                  5
#define LCD_WF8B_BPFLCD11_MASK                   0x20u
#define LCD_WF8B_BPFLCD11_SHIFT                  5
#define LCD_WF8B_BPFLCD56_MASK                   0x20u
#define LCD_WF8B_BPFLCD56_SHIFT                  5
#define LCD_WF8B_BPFLCD1_MASK                    0x20u
#define LCD_WF8B_BPFLCD1_SHIFT                   5
#define LCD_WF8B_BPFLCD8_MASK                    0x20u
#define LCD_WF8B_BPFLCD8_SHIFT                   5
#define LCD_WF8B_BPFLCD40_MASK                   0x20u
#define LCD_WF8B_BPFLCD40_SHIFT                  5
#define LCD_WF8B_BPFLCD51_MASK                   0x20u
#define LCD_WF8B_BPFLCD51_SHIFT                  5
#define LCD_WF8B_BPFLCD16_MASK                   0x20u
#define LCD_WF8B_BPFLCD16_SHIFT                  5
#define LCD_WF8B_BPFLCD45_MASK                   0x20u
#define LCD_WF8B_BPFLCD45_SHIFT                  5
#define LCD_WF8B_BPFLCD6_MASK                    0x20u
#define LCD_WF8B_BPFLCD6_SHIFT                   5
#define LCD_WF8B_BPFLCD17_MASK                   0x20u
#define LCD_WF8B_BPFLCD17_SHIFT                  5
#define LCD_WF8B_BPFLCD28_MASK                   0x20u
#define LCD_WF8B_BPFLCD28_SHIFT                  5
#define LCD_WF8B_BPFLCD42_MASK                   0x20u
#define LCD_WF8B_BPFLCD42_SHIFT                  5
#define LCD_WF8B_BPFLCD29_MASK                   0x20u
#define LCD_WF8B_BPFLCD29_SHIFT                  5
#define LCD_WF8B_BPFLCD50_MASK                   0x20u
#define LCD_WF8B_BPFLCD50_SHIFT                  5
#define LCD_WF8B_BPFLCD18_MASK                   0x20u
#define LCD_WF8B_BPFLCD18_SHIFT                  5
#define LCD_WF8B_BPFLCD34_MASK                   0x20u
#define LCD_WF8B_BPFLCD34_SHIFT                  5
#define LCD_WF8B_BPFLCD19_MASK                   0x20u
#define LCD_WF8B_BPFLCD19_SHIFT                  5
#define LCD_WF8B_BPFLCD2_MASK                    0x20u
#define LCD_WF8B_BPFLCD2_SHIFT                   5
#define LCD_WF8B_BPFLCD9_MASK                    0x20u
#define LCD_WF8B_BPFLCD9_SHIFT                   5
#define LCD_WF8B_BPFLCD3_MASK                    0x20u
#define LCD_WF8B_BPFLCD3_SHIFT                   5
#define LCD_WF8B_BPFLCD37_MASK                   0x20u
#define LCD_WF8B_BPFLCD37_SHIFT                  5
#define LCD_WF8B_BPFLCD49_MASK                   0x20u
#define LCD_WF8B_BPFLCD49_SHIFT                  5
#define LCD_WF8B_BPFLCD20_MASK                   0x20u
#define LCD_WF8B_BPFLCD20_SHIFT                  5
#define LCD_WF8B_BPFLCD44_MASK                   0x20u
#define LCD_WF8B_BPFLCD44_SHIFT                  5
#define LCD_WF8B_BPFLCD30_MASK                   0x20u
#define LCD_WF8B_BPFLCD30_SHIFT                  5
#define LCD_WF8B_BPFLCD21_MASK                   0x20u
#define LCD_WF8B_BPFLCD21_SHIFT                  5
#define LCD_WF8B_BPFLCD35_MASK                   0x20u
#define LCD_WF8B_BPFLCD35_SHIFT                  5
#define LCD_WF8B_BPFLCD4_MASK                    0x20u
#define LCD_WF8B_BPFLCD4_SHIFT                   5
#define LCD_WF8B_BPFLCD31_MASK                   0x20u
#define LCD_WF8B_BPFLCD31_SHIFT                  5
#define LCD_WF8B_BPFLCD48_MASK                   0x20u
#define LCD_WF8B_BPFLCD48_SHIFT                  5
#define LCD_WF8B_BPFLCD7_MASK                    0x20u
#define LCD_WF8B_BPFLCD7_SHIFT                   5
#define LCD_WF8B_BPFLCD22_MASK                   0x20u
#define LCD_WF8B_BPFLCD22_SHIFT                  5
#define LCD_WF8B_BPFLCD38_MASK                   0x20u
#define LCD_WF8B_BPFLCD38_SHIFT                  5
#define LCD_WF8B_BPFLCD12_MASK                   0x20u
#define LCD_WF8B_BPFLCD12_SHIFT                  5
#define LCD_WF8B_BPFLCD23_MASK                   0x20u
#define LCD_WF8B_BPFLCD23_SHIFT                  5
#define LCD_WF8B_BPGLCD14_MASK                   0x40u
#define LCD_WF8B_BPGLCD14_SHIFT                  6
#define LCD_WF8B_BPGLCD55_MASK                   0x40u
#define LCD_WF8B_BPGLCD55_SHIFT                  6
#define LCD_WF8B_BPGLCD63_MASK                   0x40u
#define LCD_WF8B_BPGLCD63_SHIFT                  6
#define LCD_WF8B_BPGLCD15_MASK                   0x40u
#define LCD_WF8B_BPGLCD15_SHIFT                  6
#define LCD_WF8B_BPGLCD62_MASK                   0x40u
#define LCD_WF8B_BPGLCD62_SHIFT                  6
#define LCD_WF8B_BPGLCD54_MASK                   0x40u
#define LCD_WF8B_BPGLCD54_SHIFT                  6
#define LCD_WF8B_BPGLCD61_MASK                   0x40u
#define LCD_WF8B_BPGLCD61_SHIFT                  6
#define LCD_WF8B_BPGLCD60_MASK                   0x40u
#define LCD_WF8B_BPGLCD60_SHIFT                  6
#define LCD_WF8B_BPGLCD59_MASK                   0x40u
#define LCD_WF8B_BPGLCD59_SHIFT                  6
#define LCD_WF8B_BPGLCD53_MASK                   0x40u
#define LCD_WF8B_BPGLCD53_SHIFT                  6
#define LCD_WF8B_BPGLCD58_MASK                   0x40u
#define LCD_WF8B_BPGLCD58_SHIFT                  6
#define LCD_WF8B_BPGLCD0_MASK                    0x40u
#define LCD_WF8B_BPGLCD0_SHIFT                   6
#define LCD_WF8B_BPGLCD57_MASK                   0x40u
#define LCD_WF8B_BPGLCD57_SHIFT                  6
#define LCD_WF8B_BPGLCD52_MASK                   0x40u
#define LCD_WF8B_BPGLCD52_SHIFT                  6
#define LCD_WF8B_BPGLCD7_MASK                    0x40u
#define LCD_WF8B_BPGLCD7_SHIFT                   6
#define LCD_WF8B_BPGLCD56_MASK                   0x40u
#define LCD_WF8B_BPGLCD56_SHIFT                  6
#define LCD_WF8B_BPGLCD6_MASK                    0x40u
#define LCD_WF8B_BPGLCD6_SHIFT                   6
#define LCD_WF8B_BPGLCD51_MASK                   0x40u
#define LCD_WF8B_BPGLCD51_SHIFT                  6
#define LCD_WF8B_BPGLCD16_MASK                   0x40u
#define LCD_WF8B_BPGLCD16_SHIFT                  6
#define LCD_WF8B_BPGLCD1_MASK                    0x40u
#define LCD_WF8B_BPGLCD1_SHIFT                   6
#define LCD_WF8B_BPGLCD17_MASK                   0x40u
#define LCD_WF8B_BPGLCD17_SHIFT                  6
#define LCD_WF8B_BPGLCD50_MASK                   0x40u
#define LCD_WF8B_BPGLCD50_SHIFT                  6
#define LCD_WF8B_BPGLCD18_MASK                   0x40u
#define LCD_WF8B_BPGLCD18_SHIFT                  6
#define LCD_WF8B_BPGLCD19_MASK                   0x40u
#define LCD_WF8B_BPGLCD19_SHIFT                  6
#define LCD_WF8B_BPGLCD8_MASK                    0x40u
#define LCD_WF8B_BPGLCD8_SHIFT                   6
#define LCD_WF8B_BPGLCD49_MASK                   0x40u
#define LCD_WF8B_BPGLCD49_SHIFT                  6
#define LCD_WF8B_BPGLCD20_MASK                   0x40u
#define LCD_WF8B_BPGLCD20_SHIFT                  6
#define LCD_WF8B_BPGLCD9_MASK                    0x40u
#define LCD_WF8B_BPGLCD9_SHIFT                   6
#define LCD_WF8B_BPGLCD21_MASK                   0x40u
#define LCD_WF8B_BPGLCD21_SHIFT                  6
#define LCD_WF8B_BPGLCD13_MASK                   0x40u
#define LCD_WF8B_BPGLCD13_SHIFT                  6
#define LCD_WF8B_BPGLCD48_MASK                   0x40u
#define LCD_WF8B_BPGLCD48_SHIFT                  6
#define LCD_WF8B_BPGLCD22_MASK                   0x40u
#define LCD_WF8B_BPGLCD22_SHIFT                  6
#define LCD_WF8B_BPGLCD5_MASK                    0x40u
#define LCD_WF8B_BPGLCD5_SHIFT                   6
#define LCD_WF8B_BPGLCD47_MASK                   0x40u
#define LCD_WF8B_BPGLCD47_SHIFT                  6
#define LCD_WF8B_BPGLCD23_MASK                   0x40u
#define LCD_WF8B_BPGLCD23_SHIFT                  6
#define LCD_WF8B_BPGLCD24_MASK                   0x40u
#define LCD_WF8B_BPGLCD24_SHIFT                  6
#define LCD_WF8B_BPGLCD25_MASK                   0x40u
#define LCD_WF8B_BPGLCD25_SHIFT                  6
#define LCD_WF8B_BPGLCD46_MASK                   0x40u
#define LCD_WF8B_BPGLCD46_SHIFT                  6
#define LCD_WF8B_BPGLCD26_MASK                   0x40u
#define LCD_WF8B_BPGLCD26_SHIFT                  6
#define LCD_WF8B_BPGLCD27_MASK                   0x40u
#define LCD_WF8B_BPGLCD27_SHIFT                  6
#define LCD_WF8B_BPGLCD10_MASK                   0x40u
#define LCD_WF8B_BPGLCD10_SHIFT                  6
#define LCD_WF8B_BPGLCD45_MASK                   0x40u
#define LCD_WF8B_BPGLCD45_SHIFT                  6
#define LCD_WF8B_BPGLCD28_MASK                   0x40u
#define LCD_WF8B_BPGLCD28_SHIFT                  6
#define LCD_WF8B_BPGLCD29_MASK                   0x40u
#define LCD_WF8B_BPGLCD29_SHIFT                  6
#define LCD_WF8B_BPGLCD4_MASK                    0x40u
#define LCD_WF8B_BPGLCD4_SHIFT                   6
#define LCD_WF8B_BPGLCD44_MASK                   0x40u
#define LCD_WF8B_BPGLCD44_SHIFT                  6
#define LCD_WF8B_BPGLCD30_MASK                   0x40u
#define LCD_WF8B_BPGLCD30_SHIFT                  6
#define LCD_WF8B_BPGLCD2_MASK                    0x40u
#define LCD_WF8B_BPGLCD2_SHIFT                   6
#define LCD_WF8B_BPGLCD31_MASK                   0x40u
#define LCD_WF8B_BPGLCD31_SHIFT                  6
#define LCD_WF8B_BPGLCD43_MASK                   0x40u
#define LCD_WF8B_BPGLCD43_SHIFT                  6
#define LCD_WF8B_BPGLCD32_MASK                   0x40u
#define LCD_WF8B_BPGLCD32_SHIFT                  6
#define LCD_WF8B_BPGLCD33_MASK                   0x40u
#define LCD_WF8B_BPGLCD33_SHIFT                  6
#define LCD_WF8B_BPGLCD42_MASK                   0x40u
#define LCD_WF8B_BPGLCD42_SHIFT                  6
#define LCD_WF8B_BPGLCD34_MASK                   0x40u
#define LCD_WF8B_BPGLCD34_SHIFT                  6
#define LCD_WF8B_BPGLCD11_MASK                   0x40u
#define LCD_WF8B_BPGLCD11_SHIFT                  6
#define LCD_WF8B_BPGLCD35_MASK                   0x40u
#define LCD_WF8B_BPGLCD35_SHIFT                  6
#define LCD_WF8B_BPGLCD12_MASK                   0x40u
#define LCD_WF8B_BPGLCD12_SHIFT                  6
#define LCD_WF8B_BPGLCD41_MASK                   0x40u
#define LCD_WF8B_BPGLCD41_SHIFT                  6
#define LCD_WF8B_BPGLCD36_MASK                   0x40u
#define LCD_WF8B_BPGLCD36_SHIFT                  6
#define LCD_WF8B_BPGLCD3_MASK                    0x40u
#define LCD_WF8B_BPGLCD3_SHIFT                   6
#define LCD_WF8B_BPGLCD37_MASK                   0x40u
#define LCD_WF8B_BPGLCD37_SHIFT                  6
#define LCD_WF8B_BPGLCD40_MASK                   0x40u
#define LCD_WF8B_BPGLCD40_SHIFT                  6
#define LCD_WF8B_BPGLCD38_MASK                   0x40u
#define LCD_WF8B_BPGLCD38_SHIFT                  6
#define LCD_WF8B_BPGLCD39_MASK                   0x40u
#define LCD_WF8B_BPGLCD39_SHIFT                  6
#define LCD_WF8B_BPHLCD63_MASK                   0x80u
#define LCD_WF8B_BPHLCD63_SHIFT                  7
#define LCD_WF8B_BPHLCD62_MASK                   0x80u
#define LCD_WF8B_BPHLCD62_SHIFT                  7
#define LCD_WF8B_BPHLCD61_MASK                   0x80u
#define LCD_WF8B_BPHLCD61_SHIFT                  7
#define LCD_WF8B_BPHLCD60_MASK                   0x80u
#define LCD_WF8B_BPHLCD60_SHIFT                  7
#define LCD_WF8B_BPHLCD59_MASK                   0x80u
#define LCD_WF8B_BPHLCD59_SHIFT                  7
#define LCD_WF8B_BPHLCD58_MASK                   0x80u
#define LCD_WF8B_BPHLCD58_SHIFT                  7
#define LCD_WF8B_BPHLCD57_MASK                   0x80u
#define LCD_WF8B_BPHLCD57_SHIFT                  7
#define LCD_WF8B_BPHLCD0_MASK                    0x80u
#define LCD_WF8B_BPHLCD0_SHIFT                   7
#define LCD_WF8B_BPHLCD56_MASK                   0x80u
#define LCD_WF8B_BPHLCD56_SHIFT                  7
#define LCD_WF8B_BPHLCD55_MASK                   0x80u
#define LCD_WF8B_BPHLCD55_SHIFT                  7
#define LCD_WF8B_BPHLCD54_MASK                   0x80u
#define LCD_WF8B_BPHLCD54_SHIFT                  7
#define LCD_WF8B_BPHLCD53_MASK                   0x80u
#define LCD_WF8B_BPHLCD53_SHIFT                  7
#define LCD_WF8B_BPHLCD52_MASK                   0x80u
#define LCD_WF8B_BPHLCD52_SHIFT                  7
#define LCD_WF8B_BPHLCD51_MASK                   0x80u
#define LCD_WF8B_BPHLCD51_SHIFT                  7
#define LCD_WF8B_BPHLCD50_MASK                   0x80u
#define LCD_WF8B_BPHLCD50_SHIFT                  7
#define LCD_WF8B_BPHLCD1_MASK                    0x80u
#define LCD_WF8B_BPHLCD1_SHIFT                   7
#define LCD_WF8B_BPHLCD49_MASK                   0x80u
#define LCD_WF8B_BPHLCD49_SHIFT                  7
#define LCD_WF8B_BPHLCD48_MASK                   0x80u
#define LCD_WF8B_BPHLCD48_SHIFT                  7
#define LCD_WF8B_BPHLCD47_MASK                   0x80u
#define LCD_WF8B_BPHLCD47_SHIFT                  7
#define LCD_WF8B_BPHLCD46_MASK                   0x80u
#define LCD_WF8B_BPHLCD46_SHIFT                  7
#define LCD_WF8B_BPHLCD45_MASK                   0x80u
#define LCD_WF8B_BPHLCD45_SHIFT                  7
#define LCD_WF8B_BPHLCD44_MASK                   0x80u
#define LCD_WF8B_BPHLCD44_SHIFT                  7
#define LCD_WF8B_BPHLCD43_MASK                   0x80u
#define LCD_WF8B_BPHLCD43_SHIFT                  7
#define LCD_WF8B_BPHLCD2_MASK                    0x80u
#define LCD_WF8B_BPHLCD2_SHIFT                   7
#define LCD_WF8B_BPHLCD42_MASK                   0x80u
#define LCD_WF8B_BPHLCD42_SHIFT                  7
#define LCD_WF8B_BPHLCD41_MASK                   0x80u
#define LCD_WF8B_BPHLCD41_SHIFT                  7
#define LCD_WF8B_BPHLCD40_MASK                   0x80u
#define LCD_WF8B_BPHLCD40_SHIFT                  7
#define LCD_WF8B_BPHLCD39_MASK                   0x80u
#define LCD_WF8B_BPHLCD39_SHIFT                  7
#define LCD_WF8B_BPHLCD38_MASK                   0x80u
#define LCD_WF8B_BPHLCD38_SHIFT                  7
#define LCD_WF8B_BPHLCD37_MASK                   0x80u
#define LCD_WF8B_BPHLCD37_SHIFT                  7
#define LCD_WF8B_BPHLCD36_MASK                   0x80u
#define LCD_WF8B_BPHLCD36_SHIFT                  7
#define LCD_WF8B_BPHLCD3_MASK                    0x80u
#define LCD_WF8B_BPHLCD3_SHIFT                   7
#define LCD_WF8B_BPHLCD35_MASK                   0x80u
#define LCD_WF8B_BPHLCD35_SHIFT                  7
#define LCD_WF8B_BPHLCD34_MASK                   0x80u
#define LCD_WF8B_BPHLCD34_SHIFT                  7
#define LCD_WF8B_BPHLCD33_MASK                   0x80u
#define LCD_WF8B_BPHLCD33_SHIFT                  7
#define LCD_WF8B_BPHLCD32_MASK                   0x80u
#define LCD_WF8B_BPHLCD32_SHIFT                  7
#define LCD_WF8B_BPHLCD31_MASK                   0x80u
#define LCD_WF8B_BPHLCD31_SHIFT                  7
#define LCD_WF8B_BPHLCD30_MASK                   0x80u
#define LCD_WF8B_BPHLCD30_SHIFT                  7
#define LCD_WF8B_BPHLCD29_MASK                   0x80u
#define LCD_WF8B_BPHLCD29_SHIFT                  7
#define LCD_WF8B_BPHLCD4_MASK                    0x80u
#define LCD_WF8B_BPHLCD4_SHIFT                   7
#define LCD_WF8B_BPHLCD28_MASK                   0x80u
#define LCD_WF8B_BPHLCD28_SHIFT                  7
#define LCD_WF8B_BPHLCD27_MASK                   0x80u
#define LCD_WF8B_BPHLCD27_SHIFT                  7
#define LCD_WF8B_BPHLCD26_MASK                   0x80u
#define LCD_WF8B_BPHLCD26_SHIFT                  7
#define LCD_WF8B_BPHLCD25_MASK                   0x80u
#define LCD_WF8B_BPHLCD25_SHIFT                  7
#define LCD_WF8B_BPHLCD24_MASK                   0x80u
#define LCD_WF8B_BPHLCD24_SHIFT                  7
#define LCD_WF8B_BPHLCD23_MASK                   0x80u
#define LCD_WF8B_BPHLCD23_SHIFT                  7
#define LCD_WF8B_BPHLCD22_MASK                   0x80u
#define LCD_WF8B_BPHLCD22_SHIFT                  7
#define LCD_WF8B_BPHLCD5_MASK                    0x80u
#define LCD_WF8B_BPHLCD5_SHIFT                   7
#define LCD_WF8B_BPHLCD21_MASK                   0x80u
#define LCD_WF8B_BPHLCD21_SHIFT                  7
#define LCD_WF8B_BPHLCD20_MASK                   0x80u
#define LCD_WF8B_BPHLCD20_SHIFT                  7
#define LCD_WF8B_BPHLCD19_MASK                   0x80u
#define LCD_WF8B_BPHLCD19_SHIFT                  7
#define LCD_WF8B_BPHLCD18_MASK                   0x80u
#define LCD_WF8B_BPHLCD18_SHIFT                  7
#define LCD_WF8B_BPHLCD17_MASK                   0x80u
#define LCD_WF8B_BPHLCD17_SHIFT                  7
#define LCD_WF8B_BPHLCD16_MASK                   0x80u
#define LCD_WF8B_BPHLCD16_SHIFT                  7
#define LCD_WF8B_BPHLCD15_MASK                   0x80u
#define LCD_WF8B_BPHLCD15_SHIFT                  7
#define LCD_WF8B_BPHLCD6_MASK                    0x80u
#define LCD_WF8B_BPHLCD6_SHIFT                   7
#define LCD_WF8B_BPHLCD14_MASK                   0x80u
#define LCD_WF8B_BPHLCD14_SHIFT                  7
#define LCD_WF8B_BPHLCD13_MASK                   0x80u
#define LCD_WF8B_BPHLCD13_SHIFT                  7
#define LCD_WF8B_BPHLCD12_MASK                   0x80u
#define LCD_WF8B_BPHLCD12_SHIFT                  7
#define LCD_WF8B_BPHLCD11_MASK                   0x80u
#define LCD_WF8B_BPHLCD11_SHIFT                  7
#define LCD_WF8B_BPHLCD10_MASK                   0x80u
#define LCD_WF8B_BPHLCD10_SHIFT                  7
#define LCD_WF8B_BPHLCD9_MASK                    0x80u
#define LCD_WF8B_BPHLCD9_SHIFT                   7
#define LCD_WF8B_BPHLCD8_MASK                    0x80u
#define LCD_WF8B_BPHLCD8_SHIFT                   7
#define LCD_WF8B_BPHLCD7_MASK                    0x80u
#define LCD_WF8B_BPHLCD7_SHIFT                   7

/*!
 * @}
 */ /* end of group LCD_Register_Masks */


/* LCD - Peripheral instance base addresses */
/** Peripheral LCD base pointer */
#define LCD_BASE_PTR                             ((LCD_MemMapPtr)0x40043000u)
/** Array initializer of LCD peripheral base pointers */
#define LCD_BASE_PTRS                            { LCD_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- LCD - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LCD_Register_Accessor_Macros LCD - Register accessor macros
 * @{
 */


/* LCD - Register instance definitions */
/* LCD */
#define LCD_GCR                                  LCD_GCR_REG(LCD_BASE_PTR)
#define LCD_AR                                   LCD_AR_REG(LCD_BASE_PTR)
#define LCD_FDCR                                 LCD_FDCR_REG(LCD_BASE_PTR)
#define LCD_FDSR                                 LCD_FDSR_REG(LCD_BASE_PTR)
#define LCD_PENL                                 LCD_PEN_REG(LCD_BASE_PTR,0)
#define LCD_PENH                                 LCD_PEN_REG(LCD_BASE_PTR,1)
#define LCD_BPENL                                LCD_BPEN_REG(LCD_BASE_PTR,0)
#define LCD_BPENH                                LCD_BPEN_REG(LCD_BASE_PTR,1)
#define LCD_WF0                                  LCD_WF8B_REG(LCD_BASE_PTR,0)
#define LCD_WF3TO0                               LCD_WF_REG(LCD_BASE_PTR,0)
#define LCD_WF1                                  LCD_WF8B_REG(LCD_BASE_PTR,1)
#define LCD_WF2                                  LCD_WF8B_REG(LCD_BASE_PTR,2)
#define LCD_WF3                                  LCD_WF8B_REG(LCD_BASE_PTR,3)
#define LCD_WF4                                  LCD_WF8B_REG(LCD_BASE_PTR,4)
#define LCD_WF7TO4                               LCD_WF_REG(LCD_BASE_PTR,1)
#define LCD_WF5                                  LCD_WF8B_REG(LCD_BASE_PTR,5)
#define LCD_WF6                                  LCD_WF8B_REG(LCD_BASE_PTR,6)
#define LCD_WF7                                  LCD_WF8B_REG(LCD_BASE_PTR,7)
#define LCD_WF11TO8                              LCD_WF_REG(LCD_BASE_PTR,2)
#define LCD_WF8                                  LCD_WF8B_REG(LCD_BASE_PTR,8)
#define LCD_WF9                                  LCD_WF8B_REG(LCD_BASE_PTR,9)
#define LCD_WF10                                 LCD_WF8B_REG(LCD_BASE_PTR,10)
#define LCD_WF11                                 LCD_WF8B_REG(LCD_BASE_PTR,11)
#define LCD_WF12                                 LCD_WF8B_REG(LCD_BASE_PTR,12)
#define LCD_WF15TO12                             LCD_WF_REG(LCD_BASE_PTR,3)
#define LCD_WF13                                 LCD_WF8B_REG(LCD_BASE_PTR,13)
#define LCD_WF14                                 LCD_WF8B_REG(LCD_BASE_PTR,14)
#define LCD_WF15                                 LCD_WF8B_REG(LCD_BASE_PTR,15)
#define LCD_WF16                                 LCD_WF8B_REG(LCD_BASE_PTR,16)
#define LCD_WF19TO16                             LCD_WF_REG(LCD_BASE_PTR,4)
#define LCD_WF17                                 LCD_WF8B_REG(LCD_BASE_PTR,17)
#define LCD_WF18                                 LCD_WF8B_REG(LCD_BASE_PTR,18)
#define LCD_WF19                                 LCD_WF8B_REG(LCD_BASE_PTR,19)
#define LCD_WF20                                 LCD_WF8B_REG(LCD_BASE_PTR,20)
#define LCD_WF23TO20                             LCD_WF_REG(LCD_BASE_PTR,5)
#define LCD_WF21                                 LCD_WF8B_REG(LCD_BASE_PTR,21)
#define LCD_WF22                                 LCD_WF8B_REG(LCD_BASE_PTR,22)
#define LCD_WF23                                 LCD_WF8B_REG(LCD_BASE_PTR,23)
#define LCD_WF24                                 LCD_WF8B_REG(LCD_BASE_PTR,24)
#define LCD_WF27TO24                             LCD_WF_REG(LCD_BASE_PTR,6)
#define LCD_WF25                                 LCD_WF8B_REG(LCD_BASE_PTR,25)
#define LCD_WF26                                 LCD_WF8B_REG(LCD_BASE_PTR,26)
#define LCD_WF27                                 LCD_WF8B_REG(LCD_BASE_PTR,27)
#define LCD_WF28                                 LCD_WF8B_REG(LCD_BASE_PTR,28)
#define LCD_WF31TO28                             LCD_WF_REG(LCD_BASE_PTR,7)
#define LCD_WF29                                 LCD_WF8B_REG(LCD_BASE_PTR,29)
#define LCD_WF30                                 LCD_WF8B_REG(LCD_BASE_PTR,30)
#define LCD_WF31                                 LCD_WF8B_REG(LCD_BASE_PTR,31)
#define LCD_WF32                                 LCD_WF8B_REG(LCD_BASE_PTR,32)
#define LCD_WF35TO32                             LCD_WF_REG(LCD_BASE_PTR,8)
#define LCD_WF33                                 LCD_WF8B_REG(LCD_BASE_PTR,33)
#define LCD_WF34                                 LCD_WF8B_REG(LCD_BASE_PTR,34)
#define LCD_WF35                                 LCD_WF8B_REG(LCD_BASE_PTR,35)
#define LCD_WF36                                 LCD_WF8B_REG(LCD_BASE_PTR,36)
#define LCD_WF39TO36                             LCD_WF_REG(LCD_BASE_PTR,9)
#define LCD_WF37                                 LCD_WF8B_REG(LCD_BASE_PTR,37)
#define LCD_WF38                                 LCD_WF8B_REG(LCD_BASE_PTR,38)
#define LCD_WF39                                 LCD_WF8B_REG(LCD_BASE_PTR,39)
#define LCD_WF40                                 LCD_WF8B_REG(LCD_BASE_PTR,40)
#define LCD_WF43TO40                             LCD_WF_REG(LCD_BASE_PTR,10)
#define LCD_WF41                                 LCD_WF8B_REG(LCD_BASE_PTR,41)
#define LCD_WF42                                 LCD_WF8B_REG(LCD_BASE_PTR,42)
#define LCD_WF43                                 LCD_WF8B_REG(LCD_BASE_PTR,43)
#define LCD_WF44                                 LCD_WF8B_REG(LCD_BASE_PTR,44)
#define LCD_WF47TO44                             LCD_WF_REG(LCD_BASE_PTR,11)
#define LCD_WF45                                 LCD_WF8B_REG(LCD_BASE_PTR,45)
#define LCD_WF46                                 LCD_WF8B_REG(LCD_BASE_PTR,46)
#define LCD_WF47                                 LCD_WF8B_REG(LCD_BASE_PTR,47)
#define LCD_WF48                                 LCD_WF8B_REG(LCD_BASE_PTR,48)
#define LCD_WF51TO48                             LCD_WF_REG(LCD_BASE_PTR,12)
#define LCD_WF49                                 LCD_WF8B_REG(LCD_BASE_PTR,49)
#define LCD_WF50                                 LCD_WF8B_REG(LCD_BASE_PTR,50)
#define LCD_WF51                                 LCD_WF8B_REG(LCD_BASE_PTR,51)
#define LCD_WF52                                 LCD_WF8B_REG(LCD_BASE_PTR,52)
#define LCD_WF55TO52                             LCD_WF_REG(LCD_BASE_PTR,13)
#define LCD_WF53                                 LCD_WF8B_REG(LCD_BASE_PTR,53)
#define LCD_WF54                                 LCD_WF8B_REG(LCD_BASE_PTR,54)
#define LCD_WF55                                 LCD_WF8B_REG(LCD_BASE_PTR,55)
#define LCD_WF56                                 LCD_WF8B_REG(LCD_BASE_PTR,56)
#define LCD_WF59TO56                             LCD_WF_REG(LCD_BASE_PTR,14)
#define LCD_WF57                                 LCD_WF8B_REG(LCD_BASE_PTR,57)
#define LCD_WF58                                 LCD_WF8B_REG(LCD_BASE_PTR,58)
#define LCD_WF59                                 LCD_WF8B_REG(LCD_BASE_PTR,59)
#define LCD_WF60                                 LCD_WF8B_REG(LCD_BASE_PTR,60)
#define LCD_WF63TO60                             LCD_WF_REG(LCD_BASE_PTR,15)
#define LCD_WF61                                 LCD_WF8B_REG(LCD_BASE_PTR,61)
#define LCD_WF62                                 LCD_WF8B_REG(LCD_BASE_PTR,62)
#define LCD_WF63                                 LCD_WF8B_REG(LCD_BASE_PTR,63)

/* LCD - Register array accessors */
#define LCD_PEN(index)                           LCD_PEN_REG(LCD_BASE_PTR,index)
#define LCD_BPEN(index)                          LCD_BPEN_REG(LCD_BASE_PTR,index)
#define LCD_WF(index2)                           LCD_WF_REG(LCD_BASE_PTR,index2)
#define LCD_WF8B(index2)                         LCD_WF8B_REG(LCD_BASE_PTR,index2)

/*!
 * @}
 */ /* end of group LCD_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group LCD_Peripheral */


/* ----------------------------------------------------------------------------
   -- LLWU
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LLWU_Peripheral LLWU
 * @{
 */

/** LLWU - Peripheral register structure */
typedef struct LLWU_MemMap {
  uint8_t PE1;                                     /**< LLWU Pin Enable 1 register, offset: 0x0 */
  uint8_t PE2;                                     /**< LLWU Pin Enable 2 register, offset: 0x1 */
  uint8_t PE3;                                     /**< LLWU Pin Enable 3 register, offset: 0x2 */
  uint8_t PE4;                                     /**< LLWU Pin Enable 4 register, offset: 0x3 */
  uint8_t ME;                                      /**< LLWU Module Enable register, offset: 0x4 */
  uint8_t F1;                                      /**< LLWU Flag 1 register, offset: 0x5 */
  uint8_t F2;                                      /**< LLWU Flag 2 register, offset: 0x6 */
  uint8_t F3;                                      /**< LLWU Flag 3 register, offset: 0x7 */
  uint8_t FILT1;                                   /**< LLWU Pin Filter 1 register, offset: 0x8 */
  uint8_t FILT2;                                   /**< LLWU Pin Filter 2 register, offset: 0x9 */
} volatile *LLWU_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- LLWU - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LLWU_Register_Accessor_Macros LLWU - Register accessor macros
 * @{
 */


/* LLWU - Register accessors */
#define LLWU_PE1_REG(base)                       ((base)->PE1)
#define LLWU_PE2_REG(base)                       ((base)->PE2)
#define LLWU_PE3_REG(base)                       ((base)->PE3)
#define LLWU_PE4_REG(base)                       ((base)->PE4)
#define LLWU_ME_REG(base)                        ((base)->ME)
#define LLWU_F1_REG(base)                        ((base)->F1)
#define LLWU_F2_REG(base)                        ((base)->F2)
#define LLWU_F3_REG(base)                        ((base)->F3)
#define LLWU_FILT1_REG(base)                     ((base)->FILT1)
#define LLWU_FILT2_REG(base)                     ((base)->FILT2)

/*!
 * @}
 */ /* end of group LLWU_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- LLWU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LLWU_Register_Masks LLWU Register Masks
 * @{
 */

/* PE1 Bit Fields */
#define LLWU_PE1_WUPE0_MASK                      0x3u
#define LLWU_PE1_WUPE0_SHIFT                     0
#define LLWU_PE1_WUPE0(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE1_WUPE0_SHIFT))&LLWU_PE1_WUPE0_MASK)
#define LLWU_PE1_WUPE1_MASK                      0xCu
#define LLWU_PE1_WUPE1_SHIFT                     2
#define LLWU_PE1_WUPE1(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE1_WUPE1_SHIFT))&LLWU_PE1_WUPE1_MASK)
#define LLWU_PE1_WUPE2_MASK                      0x30u
#define LLWU_PE1_WUPE2_SHIFT                     4
#define LLWU_PE1_WUPE2(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE1_WUPE2_SHIFT))&LLWU_PE1_WUPE2_MASK)
#define LLWU_PE1_WUPE3_MASK                      0xC0u
#define LLWU_PE1_WUPE3_SHIFT                     6
#define LLWU_PE1_WUPE3(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE1_WUPE3_SHIFT))&LLWU_PE1_WUPE3_MASK)
/* PE2 Bit Fields */
#define LLWU_PE2_WUPE4_MASK                      0x3u
#define LLWU_PE2_WUPE4_SHIFT                     0
#define LLWU_PE2_WUPE4(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE2_WUPE4_SHIFT))&LLWU_PE2_WUPE4_MASK)
#define LLWU_PE2_WUPE5_MASK                      0xCu
#define LLWU_PE2_WUPE5_SHIFT                     2
#define LLWU_PE2_WUPE5(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE2_WUPE5_SHIFT))&LLWU_PE2_WUPE5_MASK)
#define LLWU_PE2_WUPE6_MASK                      0x30u
#define LLWU_PE2_WUPE6_SHIFT                     4
#define LLWU_PE2_WUPE6(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE2_WUPE6_SHIFT))&LLWU_PE2_WUPE6_MASK)
#define LLWU_PE2_WUPE7_MASK                      0xC0u
#define LLWU_PE2_WUPE7_SHIFT                     6
#define LLWU_PE2_WUPE7(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE2_WUPE7_SHIFT))&LLWU_PE2_WUPE7_MASK)
/* PE3 Bit Fields */
#define LLWU_PE3_WUPE8_MASK                      0x3u
#define LLWU_PE3_WUPE8_SHIFT                     0
#define LLWU_PE3_WUPE8(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE3_WUPE8_SHIFT))&LLWU_PE3_WUPE8_MASK)
#define LLWU_PE3_WUPE9_MASK                      0xCu
#define LLWU_PE3_WUPE9_SHIFT                     2
#define LLWU_PE3_WUPE9(x)                        (((uint8_t)(((uint8_t)(x))<<LLWU_PE3_WUPE9_SHIFT))&LLWU_PE3_WUPE9_MASK)
#define LLWU_PE3_WUPE10_MASK                     0x30u
#define LLWU_PE3_WUPE10_SHIFT                    4
#define LLWU_PE3_WUPE10(x)                       (((uint8_t)(((uint8_t)(x))<<LLWU_PE3_WUPE10_SHIFT))&LLWU_PE3_WUPE10_MASK)
#define LLWU_PE3_WUPE11_MASK                     0xC0u
#define LLWU_PE3_WUPE11_SHIFT                    6
#define LLWU_PE3_WUPE11(x)                       (((uint8_t)(((uint8_t)(x))<<LLWU_PE3_WUPE11_SHIFT))&LLWU_PE3_WUPE11_MASK)
/* PE4 Bit Fields */
#define LLWU_PE4_WUPE12_MASK                     0x3u
#define LLWU_PE4_WUPE12_SHIFT                    0
#define LLWU_PE4_WUPE12(x)                       (((uint8_t)(((uint8_t)(x))<<LLWU_PE4_WUPE12_SHIFT))&LLWU_PE4_WUPE12_MASK)
#define LLWU_PE4_WUPE13_MASK                     0xCu
#define LLWU_PE4_WUPE13_SHIFT                    2
#define LLWU_PE4_WUPE13(x)                       (((uint8_t)(((uint8_t)(x))<<LLWU_PE4_WUPE13_SHIFT))&LLWU_PE4_WUPE13_MASK)
#define LLWU_PE4_WUPE14_MASK                     0x30u
#define LLWU_PE4_WUPE14_SHIFT                    4
#define LLWU_PE4_WUPE14(x)                       (((uint8_t)(((uint8_t)(x))<<LLWU_PE4_WUPE14_SHIFT))&LLWU_PE4_WUPE14_MASK)
#define LLWU_PE4_WUPE15_MASK                     0xC0u
#define LLWU_PE4_WUPE15_SHIFT                    6
#define LLWU_PE4_WUPE15(x)                       (((uint8_t)(((uint8_t)(x))<<LLWU_PE4_WUPE15_SHIFT))&LLWU_PE4_WUPE15_MASK)
/* ME Bit Fields */
#define LLWU_ME_WUME0_MASK                       0x1u
#define LLWU_ME_WUME0_SHIFT                      0
#define LLWU_ME_WUME1_MASK                       0x2u
#define LLWU_ME_WUME1_SHIFT                      1
#define LLWU_ME_WUME2_MASK                       0x4u
#define LLWU_ME_WUME2_SHIFT                      2
#define LLWU_ME_WUME3_MASK                       0x8u
#define LLWU_ME_WUME3_SHIFT                      3
#define LLWU_ME_WUME4_MASK                       0x10u
#define LLWU_ME_WUME4_SHIFT                      4
#define LLWU_ME_WUME5_MASK                       0x20u
#define LLWU_ME_WUME5_SHIFT                      5
#define LLWU_ME_WUME6_MASK                       0x40u
#define LLWU_ME_WUME6_SHIFT                      6
#define LLWU_ME_WUME7_MASK                       0x80u
#define LLWU_ME_WUME7_SHIFT                      7
/* F1 Bit Fields */
#define LLWU_F1_WUF0_MASK                        0x1u
#define LLWU_F1_WUF0_SHIFT                       0
#define LLWU_F1_WUF1_MASK                        0x2u
#define LLWU_F1_WUF1_SHIFT                       1
#define LLWU_F1_WUF2_MASK                        0x4u
#define LLWU_F1_WUF2_SHIFT                       2
#define LLWU_F1_WUF3_MASK                        0x8u
#define LLWU_F1_WUF3_SHIFT                       3
#define LLWU_F1_WUF4_MASK                        0x10u
#define LLWU_F1_WUF4_SHIFT                       4
#define LLWU_F1_WUF5_MASK                        0x20u
#define LLWU_F1_WUF5_SHIFT                       5
#define LLWU_F1_WUF6_MASK                        0x40u
#define LLWU_F1_WUF6_SHIFT                       6
#define LLWU_F1_WUF7_MASK                        0x80u
#define LLWU_F1_WUF7_SHIFT                       7
/* F2 Bit Fields */
#define LLWU_F2_WUF8_MASK                        0x1u
#define LLWU_F2_WUF8_SHIFT                       0
#define LLWU_F2_WUF9_MASK                        0x2u
#define LLWU_F2_WUF9_SHIFT                       1
#define LLWU_F2_WUF10_MASK                       0x4u
#define LLWU_F2_WUF10_SHIFT                      2
#define LLWU_F2_WUF11_MASK                       0x8u
#define LLWU_F2_WUF11_SHIFT                      3
#define LLWU_F2_WUF12_MASK                       0x10u
#define LLWU_F2_WUF12_SHIFT                      4
#define LLWU_F2_WUF13_MASK                       0x20u
#define LLWU_F2_WUF13_SHIFT                      5
#define LLWU_F2_WUF14_MASK                       0x40u
#define LLWU_F2_WUF14_SHIFT                      6
#define LLWU_F2_WUF15_MASK                       0x80u
#define LLWU_F2_WUF15_SHIFT                      7
/* F3 Bit Fields */
#define LLWU_F3_MWUF0_MASK                       0x1u
#define LLWU_F3_MWUF0_SHIFT                      0
#define LLWU_F3_MWUF1_MASK                       0x2u
#define LLWU_F3_MWUF1_SHIFT                      1
#define LLWU_F3_MWUF2_MASK                       0x4u
#define LLWU_F3_MWUF2_SHIFT                      2
#define LLWU_F3_MWUF3_MASK                       0x8u
#define LLWU_F3_MWUF3_SHIFT                      3
#define LLWU_F3_MWUF4_MASK                       0x10u
#define LLWU_F3_MWUF4_SHIFT                      4
#define LLWU_F3_MWUF5_MASK                       0x20u
#define LLWU_F3_MWUF5_SHIFT                      5
#define LLWU_F3_MWUF6_MASK                       0x40u
#define LLWU_F3_MWUF6_SHIFT                      6
#define LLWU_F3_MWUF7_MASK                       0x80u
#define LLWU_F3_MWUF7_SHIFT                      7
/* FILT1 Bit Fields */
#define LLWU_FILT1_FILTSEL_MASK                  0xFu
#define LLWU_FILT1_FILTSEL_SHIFT                 0
#define LLWU_FILT1_FILTSEL(x)                    (((uint8_t)(((uint8_t)(x))<<LLWU_FILT1_FILTSEL_SHIFT))&LLWU_FILT1_FILTSEL_MASK)
#define LLWU_FILT1_FILTE_MASK                    0x60u
#define LLWU_FILT1_FILTE_SHIFT                   5
#define LLWU_FILT1_FILTE(x)                      (((uint8_t)(((uint8_t)(x))<<LLWU_FILT1_FILTE_SHIFT))&LLWU_FILT1_FILTE_MASK)
#define LLWU_FILT1_FILTF_MASK                    0x80u
#define LLWU_FILT1_FILTF_SHIFT                   7
/* FILT2 Bit Fields */
#define LLWU_FILT2_FILTSEL_MASK                  0xFu
#define LLWU_FILT2_FILTSEL_SHIFT                 0
#define LLWU_FILT2_FILTSEL(x)                    (((uint8_t)(((uint8_t)(x))<<LLWU_FILT2_FILTSEL_SHIFT))&LLWU_FILT2_FILTSEL_MASK)
#define LLWU_FILT2_FILTE_MASK                    0x60u
#define LLWU_FILT2_FILTE_SHIFT                   5
#define LLWU_FILT2_FILTE(x)                      (((uint8_t)(((uint8_t)(x))<<LLWU_FILT2_FILTE_SHIFT))&LLWU_FILT2_FILTE_MASK)
#define LLWU_FILT2_FILTF_MASK                    0x80u
#define LLWU_FILT2_FILTF_SHIFT                   7

/*!
 * @}
 */ /* end of group LLWU_Register_Masks */


/* LLWU - Peripheral instance base addresses */
/** Peripheral LLWU base pointer */
#define LLWU_BASE_PTR                            ((LLWU_MemMapPtr)0x4007C000u)
/** Array initializer of LLWU peripheral base pointers */
#define LLWU_BASE_PTRS                           { LLWU_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- LLWU - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LLWU_Register_Accessor_Macros LLWU - Register accessor macros
 * @{
 */


/* LLWU - Register instance definitions */
/* LLWU */
#define LLWU_PE1                                 LLWU_PE1_REG(LLWU_BASE_PTR)
#define LLWU_PE2                                 LLWU_PE2_REG(LLWU_BASE_PTR)
#define LLWU_PE3                                 LLWU_PE3_REG(LLWU_BASE_PTR)
#define LLWU_PE4                                 LLWU_PE4_REG(LLWU_BASE_PTR)
#define LLWU_ME                                  LLWU_ME_REG(LLWU_BASE_PTR)
#define LLWU_F1                                  LLWU_F1_REG(LLWU_BASE_PTR)
#define LLWU_F2                                  LLWU_F2_REG(LLWU_BASE_PTR)
#define LLWU_F3                                  LLWU_F3_REG(LLWU_BASE_PTR)
#define LLWU_FILT1                               LLWU_FILT1_REG(LLWU_BASE_PTR)
#define LLWU_FILT2                               LLWU_FILT2_REG(LLWU_BASE_PTR)

/*!
 * @}
 */ /* end of group LLWU_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group LLWU_Peripheral */


/* ----------------------------------------------------------------------------
   -- LPTMR
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Peripheral LPTMR
 * @{
 */

/** LPTMR - Peripheral register structure */
typedef struct LPTMR_MemMap {
  uint32_t CSR;                                    /**< Low Power Timer Control Status Register, offset: 0x0 */
  uint32_t PSR;                                    /**< Low Power Timer Prescale Register, offset: 0x4 */
  uint32_t CMR;                                    /**< Low Power Timer Compare Register, offset: 0x8 */
  uint32_t CNR;                                    /**< Low Power Timer Counter Register, offset: 0xC */
} volatile *LPTMR_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- LPTMR - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Register_Accessor_Macros LPTMR - Register accessor macros
 * @{
 */


/* LPTMR - Register accessors */
#define LPTMR_CSR_REG(base)                      ((base)->CSR)
#define LPTMR_PSR_REG(base)                      ((base)->PSR)
#define LPTMR_CMR_REG(base)                      ((base)->CMR)
#define LPTMR_CNR_REG(base)                      ((base)->CNR)

/*!
 * @}
 */ /* end of group LPTMR_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- LPTMR Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Register_Masks LPTMR Register Masks
 * @{
 */

/* CSR Bit Fields */
#define LPTMR_CSR_TEN_MASK                       0x1u
#define LPTMR_CSR_TEN_SHIFT                      0
#define LPTMR_CSR_TMS_MASK                       0x2u
#define LPTMR_CSR_TMS_SHIFT                      1
#define LPTMR_CSR_TFC_MASK                       0x4u
#define LPTMR_CSR_TFC_SHIFT                      2
#define LPTMR_CSR_TPP_MASK                       0x8u
#define LPTMR_CSR_TPP_SHIFT                      3
#define LPTMR_CSR_TPS_MASK                       0x30u
#define LPTMR_CSR_TPS_SHIFT                      4
#define LPTMR_CSR_TPS(x)                         (((uint32_t)(((uint32_t)(x))<<LPTMR_CSR_TPS_SHIFT))&LPTMR_CSR_TPS_MASK)
#define LPTMR_CSR_TIE_MASK                       0x40u
#define LPTMR_CSR_TIE_SHIFT                      6
#define LPTMR_CSR_TCF_MASK                       0x80u
#define LPTMR_CSR_TCF_SHIFT                      7
/* PSR Bit Fields */
#define LPTMR_PSR_PCS_MASK                       0x3u
#define LPTMR_PSR_PCS_SHIFT                      0
#define LPTMR_PSR_PCS(x)                         (((uint32_t)(((uint32_t)(x))<<LPTMR_PSR_PCS_SHIFT))&LPTMR_PSR_PCS_MASK)
#define LPTMR_PSR_PBYP_MASK                      0x4u
#define LPTMR_PSR_PBYP_SHIFT                     2
#define LPTMR_PSR_PRESCALE_MASK                  0x78u
#define LPTMR_PSR_PRESCALE_SHIFT                 3
#define LPTMR_PSR_PRESCALE(x)                    (((uint32_t)(((uint32_t)(x))<<LPTMR_PSR_PRESCALE_SHIFT))&LPTMR_PSR_PRESCALE_MASK)
/* CMR Bit Fields */
#define LPTMR_CMR_COMPARE_MASK                   0xFFFFu
#define LPTMR_CMR_COMPARE_SHIFT                  0
#define LPTMR_CMR_COMPARE(x)                     (((uint32_t)(((uint32_t)(x))<<LPTMR_CMR_COMPARE_SHIFT))&LPTMR_CMR_COMPARE_MASK)
/* CNR Bit Fields */
#define LPTMR_CNR_COUNTER_MASK                   0xFFFFu
#define LPTMR_CNR_COUNTER_SHIFT                  0
#define LPTMR_CNR_COUNTER(x)                     (((uint32_t)(((uint32_t)(x))<<LPTMR_CNR_COUNTER_SHIFT))&LPTMR_CNR_COUNTER_MASK)

/*!
 * @}
 */ /* end of group LPTMR_Register_Masks */


/* LPTMR - Peripheral instance base addresses */
/** Peripheral LPTMR base pointer */
#define LPTMR_BASE_PTR                           ((LPTMR_MemMapPtr)0x4003C000u)
/** Array initializer of LPTMR peripheral base pointers */
#define LPTMR_BASE_PTRS                          { LPTMR_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- LPTMR - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Register_Accessor_Macros LPTMR - Register accessor macros
 * @{
 */


/* LPTMR - Register instance definitions */
/* LPTMR */
#define LPTMR_CSR                                LPTMR_CSR_REG(LPTMR_BASE_PTR)
#define LPTMR_PSR                                LPTMR_PSR_REG(LPTMR_BASE_PTR)
#define LPTMR_CMR                                LPTMR_CMR_REG(LPTMR_BASE_PTR)
#define LPTMR_CNR                                LPTMR_CNR_REG(LPTMR_BASE_PTR)

/*!
 * @}
 */ /* end of group LPTMR_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group LPTMR_Peripheral */


/* ----------------------------------------------------------------------------
   -- MCG
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Peripheral MCG
 * @{
 */

/** MCG - Peripheral register structure */
typedef struct MCG_MemMap {
  uint8_t C1;                                      /**< MCG Control 1 Register, offset: 0x0 */
  uint8_t C2;                                      /**< MCG Control 2 Register, offset: 0x1 */
  uint8_t C3;                                      /**< MCG Control 3 Register, offset: 0x2 */
  uint8_t C4;                                      /**< MCG Control 4 Register, offset: 0x3 */
  uint8_t C5;                                      /**< MCG Control 5 Register, offset: 0x4 */
  uint8_t C6;                                      /**< MCG Control 6 Register, offset: 0x5 */
  uint8_t S;                                       /**< MCG Status Register, offset: 0x6 */
  uint8_t RESERVED_0[1];
  uint8_t SC;                                      /**< MCG Status and Control Register, offset: 0x8 */
  uint8_t RESERVED_1[1];
  uint8_t ATCVH;                                   /**< MCG Auto Trim Compare Value High Register, offset: 0xA */
  uint8_t ATCVL;                                   /**< MCG Auto Trim Compare Value Low Register, offset: 0xB */
  uint8_t C7;                                      /**< MCG Control 7 Register, offset: 0xC */
  uint8_t C8;                                      /**< MCG Control 8 Register, offset: 0xD */
  uint8_t C9;                                      /**< MCG Control 9 Register, offset: 0xE */
  uint8_t C10;                                     /**< MCG Control 10 Register, offset: 0xF */
} volatile *MCG_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MCG - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Register_Accessor_Macros MCG - Register accessor macros
 * @{
 */


/* MCG - Register accessors */
#define MCG_C1_REG(base)                         ((base)->C1)
#define MCG_C2_REG(base)                         ((base)->C2)
#define MCG_C3_REG(base)                         ((base)->C3)
#define MCG_C4_REG(base)                         ((base)->C4)
#define MCG_C5_REG(base)                         ((base)->C5)
#define MCG_C6_REG(base)                         ((base)->C6)
#define MCG_S_REG(base)                          ((base)->S)
#define MCG_SC_REG(base)                         ((base)->SC)
#define MCG_ATCVH_REG(base)                      ((base)->ATCVH)
#define MCG_ATCVL_REG(base)                      ((base)->ATCVL)
#define MCG_C7_REG(base)                         ((base)->C7)
#define MCG_C8_REG(base)                         ((base)->C8)
#define MCG_C9_REG(base)                         ((base)->C9)
#define MCG_C10_REG(base)                        ((base)->C10)

/*!
 * @}
 */ /* end of group MCG_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- MCG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Register_Masks MCG Register Masks
 * @{
 */

/* C1 Bit Fields */
#define MCG_C1_IREFSTEN_MASK                     0x1u
#define MCG_C1_IREFSTEN_SHIFT                    0
#define MCG_C1_IRCLKEN_MASK                      0x2u
#define MCG_C1_IRCLKEN_SHIFT                     1
#define MCG_C1_IREFS_MASK                        0x4u
#define MCG_C1_IREFS_SHIFT                       2
#define MCG_C1_FRDIV_MASK                        0x38u
#define MCG_C1_FRDIV_SHIFT                       3
#define MCG_C1_FRDIV(x)                          (((uint8_t)(((uint8_t)(x))<<MCG_C1_FRDIV_SHIFT))&MCG_C1_FRDIV_MASK)
#define MCG_C1_CLKS_MASK                         0xC0u
#define MCG_C1_CLKS_SHIFT                        6
#define MCG_C1_CLKS(x)                           (((uint8_t)(((uint8_t)(x))<<MCG_C1_CLKS_SHIFT))&MCG_C1_CLKS_MASK)
/* C2 Bit Fields */
#define MCG_C2_IRCS_MASK                         0x1u
#define MCG_C2_IRCS_SHIFT                        0
#define MCG_C2_LP_MASK                           0x2u
#define MCG_C2_LP_SHIFT                          1
#define MCG_C2_EREFS0_MASK                       0x4u
#define MCG_C2_EREFS0_SHIFT                      2
#define MCG_C2_HGO0_MASK                         0x8u
#define MCG_C2_HGO0_SHIFT                        3
#define MCG_C2_RANGE0_MASK                       0x30u
#define MCG_C2_RANGE0_SHIFT                      4
#define MCG_C2_RANGE0(x)                         (((uint8_t)(((uint8_t)(x))<<MCG_C2_RANGE0_SHIFT))&MCG_C2_RANGE0_MASK)
#define MCG_C2_LOCRE0_MASK                       0x80u
#define MCG_C2_LOCRE0_SHIFT                      7
/* C3 Bit Fields */
#define MCG_C3_SCTRIM_MASK                       0xFFu
#define MCG_C3_SCTRIM_SHIFT                      0
#define MCG_C3_SCTRIM(x)                         (((uint8_t)(((uint8_t)(x))<<MCG_C3_SCTRIM_SHIFT))&MCG_C3_SCTRIM_MASK)
/* C4 Bit Fields */
#define MCG_C4_SCFTRIM_MASK                      0x1u
#define MCG_C4_SCFTRIM_SHIFT                     0
#define MCG_C4_FCTRIM_MASK                       0x1Eu
#define MCG_C4_FCTRIM_SHIFT                      1
#define MCG_C4_FCTRIM(x)                         (((uint8_t)(((uint8_t)(x))<<MCG_C4_FCTRIM_SHIFT))&MCG_C4_FCTRIM_MASK)
#define MCG_C4_DRST_DRS_MASK                     0x60u
#define MCG_C4_DRST_DRS_SHIFT                    5
#define MCG_C4_DRST_DRS(x)                       (((uint8_t)(((uint8_t)(x))<<MCG_C4_DRST_DRS_SHIFT))&MCG_C4_DRST_DRS_MASK)
#define MCG_C4_DMX32_MASK                        0x80u
#define MCG_C4_DMX32_SHIFT                       7
/* C5 Bit Fields */
#define MCG_C5_PLLSTEN0_MASK                     0x20u
#define MCG_C5_PLLSTEN0_SHIFT                    5
#define MCG_C5_PLLCLKEN0_MASK                    0x40u
#define MCG_C5_PLLCLKEN0_SHIFT                   6
/* C6 Bit Fields */
#define MCG_C6_CHGPMP_BIAS_MASK                  0x1Fu
#define MCG_C6_CHGPMP_BIAS_SHIFT                 0
#define MCG_C6_CHGPMP_BIAS(x)                    (((uint8_t)(((uint8_t)(x))<<MCG_C6_CHGPMP_BIAS_SHIFT))&MCG_C6_CHGPMP_BIAS_MASK)
#define MCG_C6_CME0_MASK                         0x20u
#define MCG_C6_CME0_SHIFT                        5
#define MCG_C6_PLLS_MASK                         0x40u
#define MCG_C6_PLLS_SHIFT                        6
#define MCG_C6_LOLIE0_MASK                       0x80u
#define MCG_C6_LOLIE0_SHIFT                      7
/* S Bit Fields */
#define MCG_S_IRCST_MASK                         0x1u
#define MCG_S_IRCST_SHIFT                        0
#define MCG_S_OSCINIT0_MASK                      0x2u
#define MCG_S_OSCINIT0_SHIFT                     1
#define MCG_S_CLKST_MASK                         0xCu
#define MCG_S_CLKST_SHIFT                        2
#define MCG_S_CLKST(x)                           (((uint8_t)(((uint8_t)(x))<<MCG_S_CLKST_SHIFT))&MCG_S_CLKST_MASK)
#define MCG_S_IREFST_MASK                        0x10u
#define MCG_S_IREFST_SHIFT                       4
#define MCG_S_PLLST_MASK                         0x20u
#define MCG_S_PLLST_SHIFT                        5
#define MCG_S_LOCK0_MASK                         0x40u
#define MCG_S_LOCK0_SHIFT                        6
#define MCG_S_LOLS_MASK                          0x80u
#define MCG_S_LOLS_SHIFT                         7
/* SC Bit Fields */
#define MCG_SC_LOCS0_MASK                        0x1u
#define MCG_SC_LOCS0_SHIFT                       0
#define MCG_SC_FCRDIV_MASK                       0xEu
#define MCG_SC_FCRDIV_SHIFT                      1
#define MCG_SC_FCRDIV(x)                         (((uint8_t)(((uint8_t)(x))<<MCG_SC_FCRDIV_SHIFT))&MCG_SC_FCRDIV_MASK)
#define MCG_SC_FLTPRSRV_MASK                     0x10u
#define MCG_SC_FLTPRSRV_SHIFT                    4
#define MCG_SC_ATMF_MASK                         0x20u
#define MCG_SC_ATMF_SHIFT                        5
#define MCG_SC_ATMS_MASK                         0x40u
#define MCG_SC_ATMS_SHIFT                        6
#define MCG_SC_ATME_MASK                         0x80u
#define MCG_SC_ATME_SHIFT                        7
/* ATCVH Bit Fields */
#define MCG_ATCVH_ATCVH_MASK                     0xFFu
#define MCG_ATCVH_ATCVH_SHIFT                    0
#define MCG_ATCVH_ATCVH(x)                       (((uint8_t)(((uint8_t)(x))<<MCG_ATCVH_ATCVH_SHIFT))&MCG_ATCVH_ATCVH_MASK)
/* ATCVL Bit Fields */
#define MCG_ATCVL_ATCVL_MASK                     0xFFu
#define MCG_ATCVL_ATCVL_SHIFT                    0
#define MCG_ATCVL_ATCVL(x)                       (((uint8_t)(((uint8_t)(x))<<MCG_ATCVL_ATCVL_SHIFT))&MCG_ATCVL_ATCVL_MASK)
/* C7 Bit Fields */
#define MCG_C7_OSCSEL_MASK                       0x1u
#define MCG_C7_OSCSEL_SHIFT                      0
#define MCG_C7_PLL32KREFSEL_MASK                 0xC0u
#define MCG_C7_PLL32KREFSEL_SHIFT                6
#define MCG_C7_PLL32KREFSEL(x)                   (((uint8_t)(((uint8_t)(x))<<MCG_C7_PLL32KREFSEL_SHIFT))&MCG_C7_PLL32KREFSEL_MASK)
/* C8 Bit Fields */
#define MCG_C8_LOCS1_MASK                        0x1u
#define MCG_C8_LOCS1_SHIFT                       0
#define MCG_C8_COARSE_LOLIE_MASK                 0x10u
#define MCG_C8_COARSE_LOLIE_SHIFT                4
#define MCG_C8_CME1_MASK                         0x20u
#define MCG_C8_CME1_SHIFT                        5
#define MCG_C8_LOLRE_MASK                        0x40u
#define MCG_C8_LOLRE_SHIFT                       6
#define MCG_C8_LOCRE1_MASK                       0x80u
#define MCG_C8_LOCRE1_SHIFT                      7
/* C9 Bit Fields */
#define MCG_C9_COARSE_LOCK_MASK                  0x40u
#define MCG_C9_COARSE_LOCK_SHIFT                 6
#define MCG_C9_COARSE_LOLS_MASK                  0x80u
#define MCG_C9_COARSE_LOLS_SHIFT                 7

/*!
 * @}
 */ /* end of group MCG_Register_Masks */


/* MCG - Peripheral instance base addresses */
/** Peripheral MCG base pointer */
#define MCG_BASE_PTR                             ((MCG_MemMapPtr)0x40064000u)
/** Array initializer of MCG peripheral base pointers */
#define MCG_BASE_PTRS                            { MCG_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- MCG - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Register_Accessor_Macros MCG - Register accessor macros
 * @{
 */


/* MCG - Register instance definitions */
/* MCG */
#define MCG_C1                                   MCG_C1_REG(MCG_BASE_PTR)
#define MCG_C2                                   MCG_C2_REG(MCG_BASE_PTR)
#define MCG_C3                                   MCG_C3_REG(MCG_BASE_PTR)
#define MCG_C4                                   MCG_C4_REG(MCG_BASE_PTR)
#define MCG_C5                                   MCG_C5_REG(MCG_BASE_PTR)
#define MCG_C6                                   MCG_C6_REG(MCG_BASE_PTR)
#define MCG_S                                    MCG_S_REG(MCG_BASE_PTR)
#define MCG_SC                                   MCG_SC_REG(MCG_BASE_PTR)
#define MCG_ATCVH                                MCG_ATCVH_REG(MCG_BASE_PTR)
#define MCG_ATCVL                                MCG_ATCVL_REG(MCG_BASE_PTR)
#define MCG_C7                                   MCG_C7_REG(MCG_BASE_PTR)
#define MCG_C8                                   MCG_C8_REG(MCG_BASE_PTR)
#define MCG_C9                                   MCG_C9_REG(MCG_BASE_PTR)
#define MCG_C10                                  MCG_C10_REG(MCG_BASE_PTR)

/*!
 * @}
 */ /* end of group MCG_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group MCG_Peripheral */


/* ----------------------------------------------------------------------------
   -- MCM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Peripheral MCM
 * @{
 */

/** MCM - Peripheral register structure */
typedef struct MCM_MemMap {
  uint8_t RESERVED_0[8];
  uint16_t PLASC;                                  /**< Crossbar Switch (AXBS) Slave Configuration, offset: 0x8 */
  uint16_t PLAMC;                                  /**< Crossbar Switch (AXBS) Master Configuration, offset: 0xA */
  uint32_t PLACR;                                  /**< Platform Control Register, offset: 0xC */
  uint8_t RESERVED_1[32];
  uint32_t PID;                                    /**< Process ID register, offset: 0x30 */
  uint8_t RESERVED_2[12];
  uint32_t CPO;                                    /**< Compute Operation Control Register, offset: 0x40 */
  uint8_t RESERVED_3[60];
  uint32_t MATCR[1];                               /**< Master Attribute Configuration Register, array offset: 0x80, array step: 0x4 */
} volatile *MCM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MCM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Register_Accessor_Macros MCM - Register accessor macros
 * @{
 */


/* MCM - Register accessors */
#define MCM_PLASC_REG(base)                      ((base)->PLASC)
#define MCM_PLAMC_REG(base)                      ((base)->PLAMC)
#define MCM_PLACR_REG(base)                      ((base)->PLACR)
#define MCM_PID_REG(base)                        ((base)->PID)
#define MCM_CPO_REG(base)                        ((base)->CPO)
#define MCM_MATCR_REG(base,index)                ((base)->MATCR[index])

/*!
 * @}
 */ /* end of group MCM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- MCM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Register_Masks MCM Register Masks
 * @{
 */

/* PLASC Bit Fields */
#define MCM_PLASC_ASC_MASK                       0xFFu
#define MCM_PLASC_ASC_SHIFT                      0
#define MCM_PLASC_ASC(x)                         (((uint16_t)(((uint16_t)(x))<<MCM_PLASC_ASC_SHIFT))&MCM_PLASC_ASC_MASK)
/* PLAMC Bit Fields */
#define MCM_PLAMC_AMC_MASK                       0xFFu
#define MCM_PLAMC_AMC_SHIFT                      0
#define MCM_PLAMC_AMC(x)                         (((uint16_t)(((uint16_t)(x))<<MCM_PLAMC_AMC_SHIFT))&MCM_PLAMC_AMC_MASK)
/* PLACR Bit Fields */
#define MCM_PLACR_ARB_MASK                       0x200u
#define MCM_PLACR_ARB_SHIFT                      9
#define MCM_PLACR_CFCC_MASK                      0x400u
#define MCM_PLACR_CFCC_SHIFT                     10
#define MCM_PLACR_DFCDA_MASK                     0x800u
#define MCM_PLACR_DFCDA_SHIFT                    11
#define MCM_PLACR_DFCIC_MASK                     0x1000u
#define MCM_PLACR_DFCIC_SHIFT                    12
#define MCM_PLACR_DFCC_MASK                      0x2000u
#define MCM_PLACR_DFCC_SHIFT                     13
#define MCM_PLACR_EFDS_MASK                      0x4000u
#define MCM_PLACR_EFDS_SHIFT                     14
#define MCM_PLACR_DFCS_MASK                      0x8000u
#define MCM_PLACR_DFCS_SHIFT                     15
#define MCM_PLACR_ESFC_MASK                      0x10000u
#define MCM_PLACR_ESFC_SHIFT                     16
/* PID Bit Fields */
#define MCM_PID_PID_MASK                         0xFFu
#define MCM_PID_PID_SHIFT                        0
#define MCM_PID_PID(x)                           (((uint32_t)(((uint32_t)(x))<<MCM_PID_PID_SHIFT))&MCM_PID_PID_MASK)
/* CPO Bit Fields */
#define MCM_CPO_CPOREQ_MASK                      0x1u
#define MCM_CPO_CPOREQ_SHIFT                     0
#define MCM_CPO_CPOACK_MASK                      0x2u
#define MCM_CPO_CPOACK_SHIFT                     1
#define MCM_CPO_CPOWOI_MASK                      0x4u
#define MCM_CPO_CPOWOI_SHIFT                     2
/* MATCR Bit Fields */
#define MCM_MATCR_ATC0_MASK                      0x7u
#define MCM_MATCR_ATC0_SHIFT                     0
#define MCM_MATCR_ATC0(x)                        (((uint32_t)(((uint32_t)(x))<<MCM_MATCR_ATC0_SHIFT))&MCM_MATCR_ATC0_MASK)
#define MCM_MATCR_RO0_MASK                       0x80u
#define MCM_MATCR_RO0_SHIFT                      7
#define MCM_MATCR_ATC2_MASK                      0x70000u
#define MCM_MATCR_ATC2_SHIFT                     16
#define MCM_MATCR_ATC2(x)                        (((uint32_t)(((uint32_t)(x))<<MCM_MATCR_ATC2_SHIFT))&MCM_MATCR_ATC2_MASK)
#define MCM_MATCR_RO2_MASK                       0x800000u
#define MCM_MATCR_RO2_SHIFT                      23

/*!
 * @}
 */ /* end of group MCM_Register_Masks */


/* MCM - Peripheral instance base addresses */
/** Peripheral MCM base pointer */
#define MCM_BASE_PTR                             ((MCM_MemMapPtr)0xF0003000u)
/** Array initializer of MCM peripheral base pointers */
#define MCM_BASE_PTRS                            { MCM_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- MCM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Register_Accessor_Macros MCM - Register accessor macros
 * @{
 */


/* MCM - Register instance definitions */
/* MCM */
#define MCM_PLASC                                MCM_PLASC_REG(MCM_BASE_PTR)
#define MCM_PLAMC                                MCM_PLAMC_REG(MCM_BASE_PTR)
#define MCM_PLACR                                MCM_PLACR_REG(MCM_BASE_PTR)
#define MCM_PID                                  MCM_PID_REG(MCM_BASE_PTR)
#define MCM_CPO                                  MCM_CPO_REG(MCM_BASE_PTR)
#define MCM_MATCR0                               MCM_MATCR_REG(MCM_BASE_PTR,0)

/* MCM - Register array accessors */
#define MCM_MATCR(index)                         MCM_MATCR_REG(MCM_BASE_PTR,index)

/*!
 * @}
 */ /* end of group MCM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group MCM_Peripheral */


/* ----------------------------------------------------------------------------
   -- MPU
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MPU_Peripheral MPU
 * @{
 */

/** MPU - Peripheral register structure */
typedef struct MPU_MemMap {
  uint32_t CESR;                                   /**< Control/Error Status Register, offset: 0x0 */
  uint8_t RESERVED_0[12];
  struct {                                         /* offset: 0x10, array step: 0x8 */
    uint32_t EAR;                                    /**< Error Address Register, slave port n, array offset: 0x10, array step: 0x8 */
    uint32_t EDR;                                    /**< Error Detail Register, slave port n, array offset: 0x14, array step: 0x8 */
  } SP[2];
  uint8_t RESERVED_1[992];
  uint32_t WORD[8][4];                             /**< Region Descriptor n, Word 0..Region Descriptor n, Word 3, array offset: 0x400, array step: index*0x10, index2*0x4 */
  uint8_t RESERVED_2[896];
  uint32_t RGDAAC[8];                              /**< Region Descriptor Alternate Access Control n, array offset: 0x800, array step: 0x4 */
} volatile *MPU_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MPU - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MPU_Register_Accessor_Macros MPU - Register accessor macros
 * @{
 */


/* MPU - Register accessors */
#define MPU_CESR_REG(base)                       ((base)->CESR)
#define MPU_EAR_REG(base,index)                  ((base)->SP[index].EAR)
#define MPU_EDR_REG(base,index)                  ((base)->SP[index].EDR)
#define MPU_WORD_REG(base,index,index2)          ((base)->WORD[index][index2])
#define MPU_RGDAAC_REG(base,index)               ((base)->RGDAAC[index])

/*!
 * @}
 */ /* end of group MPU_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- MPU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MPU_Register_Masks MPU Register Masks
 * @{
 */

/* CESR Bit Fields */
#define MPU_CESR_VLD_MASK                        0x1u
#define MPU_CESR_VLD_SHIFT                       0
#define MPU_CESR_NRGD_MASK                       0xF00u
#define MPU_CESR_NRGD_SHIFT                      8
#define MPU_CESR_NRGD(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_CESR_NRGD_SHIFT))&MPU_CESR_NRGD_MASK)
#define MPU_CESR_NSP_MASK                        0xF000u
#define MPU_CESR_NSP_SHIFT                       12
#define MPU_CESR_NSP(x)                          (((uint32_t)(((uint32_t)(x))<<MPU_CESR_NSP_SHIFT))&MPU_CESR_NSP_MASK)
#define MPU_CESR_HRL_MASK                        0xF0000u
#define MPU_CESR_HRL_SHIFT                       16
#define MPU_CESR_HRL(x)                          (((uint32_t)(((uint32_t)(x))<<MPU_CESR_HRL_SHIFT))&MPU_CESR_HRL_MASK)
#define MPU_CESR_SPERR_MASK                      0xC0000000u
#define MPU_CESR_SPERR_SHIFT                     30
#define MPU_CESR_SPERR(x)                        (((uint32_t)(((uint32_t)(x))<<MPU_CESR_SPERR_SHIFT))&MPU_CESR_SPERR_MASK)
/* EAR Bit Fields */
#define MPU_EAR_EADDR_MASK                       0xFFFFFFFFu
#define MPU_EAR_EADDR_SHIFT                      0
#define MPU_EAR_EADDR(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_EAR_EADDR_SHIFT))&MPU_EAR_EADDR_MASK)
/* EDR Bit Fields */
#define MPU_EDR_ERW_MASK                         0x1u
#define MPU_EDR_ERW_SHIFT                        0
#define MPU_EDR_EATTR_MASK                       0xEu
#define MPU_EDR_EATTR_SHIFT                      1
#define MPU_EDR_EATTR(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_EDR_EATTR_SHIFT))&MPU_EDR_EATTR_MASK)
#define MPU_EDR_EMN_MASK                         0xF0u
#define MPU_EDR_EMN_SHIFT                        4
#define MPU_EDR_EMN(x)                           (((uint32_t)(((uint32_t)(x))<<MPU_EDR_EMN_SHIFT))&MPU_EDR_EMN_MASK)
#define MPU_EDR_EPID_MASK                        0xFF00u
#define MPU_EDR_EPID_SHIFT                       8
#define MPU_EDR_EPID(x)                          (((uint32_t)(((uint32_t)(x))<<MPU_EDR_EPID_SHIFT))&MPU_EDR_EPID_MASK)
#define MPU_EDR_EACD_MASK                        0xFFFF0000u
#define MPU_EDR_EACD_SHIFT                       16
#define MPU_EDR_EACD(x)                          (((uint32_t)(((uint32_t)(x))<<MPU_EDR_EACD_SHIFT))&MPU_EDR_EACD_MASK)
/* WORD Bit Fields */
#define MPU_WORD_VLD_MASK                        0x1u
#define MPU_WORD_VLD_SHIFT                       0
#define MPU_WORD_M0UM_MASK                       0x7u
#define MPU_WORD_M0UM_SHIFT                      0
#define MPU_WORD_M0UM(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_WORD_M0UM_SHIFT))&MPU_WORD_M0UM_MASK)
#define MPU_WORD_M0SM_MASK                       0x18u
#define MPU_WORD_M0SM_SHIFT                      3
#define MPU_WORD_M0SM(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_WORD_M0SM_SHIFT))&MPU_WORD_M0SM_MASK)
#define MPU_WORD_M0PE_MASK                       0x20u
#define MPU_WORD_M0PE_SHIFT                      5
#define MPU_WORD_ENDADDR_MASK                    0xFFFFFFE0u
#define MPU_WORD_ENDADDR_SHIFT                   5
#define MPU_WORD_ENDADDR(x)                      (((uint32_t)(((uint32_t)(x))<<MPU_WORD_ENDADDR_SHIFT))&MPU_WORD_ENDADDR_MASK)
#define MPU_WORD_SRTADDR_MASK                    0xFFFFFFE0u
#define MPU_WORD_SRTADDR_SHIFT                   5
#define MPU_WORD_SRTADDR(x)                      (((uint32_t)(((uint32_t)(x))<<MPU_WORD_SRTADDR_SHIFT))&MPU_WORD_SRTADDR_MASK)
#define MPU_WORD_M1UM_MASK                       0x1C0u
#define MPU_WORD_M1UM_SHIFT                      6
#define MPU_WORD_M1UM(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_WORD_M1UM_SHIFT))&MPU_WORD_M1UM_MASK)
#define MPU_WORD_M1SM_MASK                       0x600u
#define MPU_WORD_M1SM_SHIFT                      9
#define MPU_WORD_M1SM(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_WORD_M1SM_SHIFT))&MPU_WORD_M1SM_MASK)
#define MPU_WORD_M1PE_MASK                       0x800u
#define MPU_WORD_M1PE_SHIFT                      11
#define MPU_WORD_M2UM_MASK                       0x7000u
#define MPU_WORD_M2UM_SHIFT                      12
#define MPU_WORD_M2UM(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_WORD_M2UM_SHIFT))&MPU_WORD_M2UM_MASK)
#define MPU_WORD_M2SM_MASK                       0x18000u
#define MPU_WORD_M2SM_SHIFT                      15
#define MPU_WORD_M2SM(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_WORD_M2SM_SHIFT))&MPU_WORD_M2SM_MASK)
#define MPU_WORD_PIDMASK_MASK                    0xFF0000u
#define MPU_WORD_PIDMASK_SHIFT                   16
#define MPU_WORD_PIDMASK(x)                      (((uint32_t)(((uint32_t)(x))<<MPU_WORD_PIDMASK_SHIFT))&MPU_WORD_PIDMASK_MASK)
#define MPU_WORD_M2PE_MASK                       0x20000u
#define MPU_WORD_M2PE_SHIFT                      17
#define MPU_WORD_M3UM_MASK                       0x1C0000u
#define MPU_WORD_M3UM_SHIFT                      18
#define MPU_WORD_M3UM(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_WORD_M3UM_SHIFT))&MPU_WORD_M3UM_MASK)
#define MPU_WORD_M3SM_MASK                       0x600000u
#define MPU_WORD_M3SM_SHIFT                      21
#define MPU_WORD_M3SM(x)                         (((uint32_t)(((uint32_t)(x))<<MPU_WORD_M3SM_SHIFT))&MPU_WORD_M3SM_MASK)
#define MPU_WORD_M3PE_MASK                       0x800000u
#define MPU_WORD_M3PE_SHIFT                      23
#define MPU_WORD_PID_MASK                        0xFF000000u
#define MPU_WORD_PID_SHIFT                       24
#define MPU_WORD_PID(x)                          (((uint32_t)(((uint32_t)(x))<<MPU_WORD_PID_SHIFT))&MPU_WORD_PID_MASK)
#define MPU_WORD_M4WE_MASK                       0x1000000u
#define MPU_WORD_M4WE_SHIFT                      24
#define MPU_WORD_M4RE_MASK                       0x2000000u
#define MPU_WORD_M4RE_SHIFT                      25
#define MPU_WORD_M5WE_MASK                       0x4000000u
#define MPU_WORD_M5WE_SHIFT                      26
#define MPU_WORD_M5RE_MASK                       0x8000000u
#define MPU_WORD_M5RE_SHIFT                      27
#define MPU_WORD_M6WE_MASK                       0x10000000u
#define MPU_WORD_M6WE_SHIFT                      28
#define MPU_WORD_M6RE_MASK                       0x20000000u
#define MPU_WORD_M6RE_SHIFT                      29
#define MPU_WORD_M7WE_MASK                       0x40000000u
#define MPU_WORD_M7WE_SHIFT                      30
#define MPU_WORD_M7RE_MASK                       0x80000000u
#define MPU_WORD_M7RE_SHIFT                      31
/* RGDAAC Bit Fields */
#define MPU_RGDAAC_M0UM_MASK                     0x7u
#define MPU_RGDAAC_M0UM_SHIFT                    0
#define MPU_RGDAAC_M0UM(x)                       (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC_M0UM_SHIFT))&MPU_RGDAAC_M0UM_MASK)
#define MPU_RGDAAC_M0SM_MASK                     0x18u
#define MPU_RGDAAC_M0SM_SHIFT                    3
#define MPU_RGDAAC_M0SM(x)                       (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC_M0SM_SHIFT))&MPU_RGDAAC_M0SM_MASK)
#define MPU_RGDAAC_M0PE_MASK                     0x20u
#define MPU_RGDAAC_M0PE_SHIFT                    5
#define MPU_RGDAAC_M1UM_MASK                     0x1C0u
#define MPU_RGDAAC_M1UM_SHIFT                    6
#define MPU_RGDAAC_M1UM(x)                       (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC_M1UM_SHIFT))&MPU_RGDAAC_M1UM_MASK)
#define MPU_RGDAAC_M1SM_MASK                     0x600u
#define MPU_RGDAAC_M1SM_SHIFT                    9
#define MPU_RGDAAC_M1SM(x)                       (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC_M1SM_SHIFT))&MPU_RGDAAC_M1SM_MASK)
#define MPU_RGDAAC_M1PE_MASK                     0x800u
#define MPU_RGDAAC_M1PE_SHIFT                    11
#define MPU_RGDAAC_M2UM_MASK                     0x7000u
#define MPU_RGDAAC_M2UM_SHIFT                    12
#define MPU_RGDAAC_M2UM(x)                       (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC_M2UM_SHIFT))&MPU_RGDAAC_M2UM_MASK)
#define MPU_RGDAAC_M2SM_MASK                     0x18000u
#define MPU_RGDAAC_M2SM_SHIFT                    15
#define MPU_RGDAAC_M2SM(x)                       (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC_M2SM_SHIFT))&MPU_RGDAAC_M2SM_MASK)
#define MPU_RGDAAC_M2PE_MASK                     0x20000u
#define MPU_RGDAAC_M2PE_SHIFT                    17
#define MPU_RGDAAC_M3UM_MASK                     0x1C0000u
#define MPU_RGDAAC_M3UM_SHIFT                    18
#define MPU_RGDAAC_M3UM(x)                       (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC_M3UM_SHIFT))&MPU_RGDAAC_M3UM_MASK)
#define MPU_RGDAAC_M3SM_MASK                     0x600000u
#define MPU_RGDAAC_M3SM_SHIFT                    21
#define MPU_RGDAAC_M3SM(x)                       (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC_M3SM_SHIFT))&MPU_RGDAAC_M3SM_MASK)
#define MPU_RGDAAC_M3PE_MASK                     0x800000u
#define MPU_RGDAAC_M3PE_SHIFT                    23
#define MPU_RGDAAC_M4WE_MASK                     0x1000000u
#define MPU_RGDAAC_M4WE_SHIFT                    24
#define MPU_RGDAAC_M4RE_MASK                     0x2000000u
#define MPU_RGDAAC_M4RE_SHIFT                    25
#define MPU_RGDAAC_M5WE_MASK                     0x4000000u
#define MPU_RGDAAC_M5WE_SHIFT                    26
#define MPU_RGDAAC_M5RE_MASK                     0x8000000u
#define MPU_RGDAAC_M5RE_SHIFT                    27
#define MPU_RGDAAC_M6WE_MASK                     0x10000000u
#define MPU_RGDAAC_M6WE_SHIFT                    28
#define MPU_RGDAAC_M6RE_MASK                     0x20000000u
#define MPU_RGDAAC_M6RE_SHIFT                    29
#define MPU_RGDAAC_M7WE_MASK                     0x40000000u
#define MPU_RGDAAC_M7WE_SHIFT                    30
#define MPU_RGDAAC_M7RE_MASK                     0x80000000u
#define MPU_RGDAAC_M7RE_SHIFT                    31

/*!
 * @}
 */ /* end of group MPU_Register_Masks */


/* MPU - Peripheral instance base addresses */
/** Peripheral MPU base pointer */
#define MPU_BASE_PTR                             ((MPU_MemMapPtr)0x4000A000u)
/** Array initializer of MPU peripheral base pointers */
#define MPU_BASE_PTRS                            { MPU_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- MPU - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MPU_Register_Accessor_Macros MPU - Register accessor macros
 * @{
 */


/* MPU - Register instance definitions */
/* MPU */
#define MPU_CESR                                 MPU_CESR_REG(MPU_BASE_PTR)
#define MPU_EAR0                                 MPU_EAR_REG(MPU_BASE_PTR,0)
#define MPU_EDR0                                 MPU_EDR_REG(MPU_BASE_PTR,0)
#define MPU_EAR1                                 MPU_EAR_REG(MPU_BASE_PTR,1)
#define MPU_EDR1                                 MPU_EDR_REG(MPU_BASE_PTR,1)
#define MPU_RGD0_WORD0                           MPU_WORD_REG(MPU_BASE_PTR,0,0)
#define MPU_RGD0_WORD1                           MPU_WORD_REG(MPU_BASE_PTR,0,1)
#define MPU_RGD0_WORD2                           MPU_WORD_REG(MPU_BASE_PTR,0,2)
#define MPU_RGD0_WORD3                           MPU_WORD_REG(MPU_BASE_PTR,0,3)
#define MPU_RGD1_WORD0                           MPU_WORD_REG(MPU_BASE_PTR,1,0)
#define MPU_RGD1_WORD1                           MPU_WORD_REG(MPU_BASE_PTR,1,1)
#define MPU_RGD1_WORD2                           MPU_WORD_REG(MPU_BASE_PTR,1,2)
#define MPU_RGD1_WORD3                           MPU_WORD_REG(MPU_BASE_PTR,1,3)
#define MPU_RGD2_WORD0                           MPU_WORD_REG(MPU_BASE_PTR,2,0)
#define MPU_RGD2_WORD1                           MPU_WORD_REG(MPU_BASE_PTR,2,1)
#define MPU_RGD2_WORD2                           MPU_WORD_REG(MPU_BASE_PTR,2,2)
#define MPU_RGD2_WORD3                           MPU_WORD_REG(MPU_BASE_PTR,2,3)
#define MPU_RGD3_WORD0                           MPU_WORD_REG(MPU_BASE_PTR,3,0)
#define MPU_RGD3_WORD1                           MPU_WORD_REG(MPU_BASE_PTR,3,1)
#define MPU_RGD3_WORD2                           MPU_WORD_REG(MPU_BASE_PTR,3,2)
#define MPU_RGD3_WORD3                           MPU_WORD_REG(MPU_BASE_PTR,3,3)
#define MPU_RGD4_WORD0                           MPU_WORD_REG(MPU_BASE_PTR,4,0)
#define MPU_RGD4_WORD1                           MPU_WORD_REG(MPU_BASE_PTR,4,1)
#define MPU_RGD4_WORD2                           MPU_WORD_REG(MPU_BASE_PTR,4,2)
#define MPU_RGD4_WORD3                           MPU_WORD_REG(MPU_BASE_PTR,4,3)
#define MPU_RGD5_WORD0                           MPU_WORD_REG(MPU_BASE_PTR,5,0)
#define MPU_RGD5_WORD1                           MPU_WORD_REG(MPU_BASE_PTR,5,1)
#define MPU_RGD5_WORD2                           MPU_WORD_REG(MPU_BASE_PTR,5,2)
#define MPU_RGD5_WORD3                           MPU_WORD_REG(MPU_BASE_PTR,5,3)
#define MPU_RGD6_WORD0                           MPU_WORD_REG(MPU_BASE_PTR,6,0)
#define MPU_RGD6_WORD1                           MPU_WORD_REG(MPU_BASE_PTR,6,1)
#define MPU_RGD6_WORD2                           MPU_WORD_REG(MPU_BASE_PTR,6,2)
#define MPU_RGD6_WORD3                           MPU_WORD_REG(MPU_BASE_PTR,6,3)
#define MPU_RGD7_WORD0                           MPU_WORD_REG(MPU_BASE_PTR,7,0)
#define MPU_RGD7_WORD1                           MPU_WORD_REG(MPU_BASE_PTR,7,1)
#define MPU_RGD7_WORD2                           MPU_WORD_REG(MPU_BASE_PTR,7,2)
#define MPU_RGD7_WORD3                           MPU_WORD_REG(MPU_BASE_PTR,7,3)
#define MPU_RGDAAC0                              MPU_RGDAAC_REG(MPU_BASE_PTR,0)
#define MPU_RGDAAC1                              MPU_RGDAAC_REG(MPU_BASE_PTR,1)
#define MPU_RGDAAC2                              MPU_RGDAAC_REG(MPU_BASE_PTR,2)
#define MPU_RGDAAC3                              MPU_RGDAAC_REG(MPU_BASE_PTR,3)
#define MPU_RGDAAC4                              MPU_RGDAAC_REG(MPU_BASE_PTR,4)
#define MPU_RGDAAC5                              MPU_RGDAAC_REG(MPU_BASE_PTR,5)
#define MPU_RGDAAC6                              MPU_RGDAAC_REG(MPU_BASE_PTR,6)
#define MPU_RGDAAC7                              MPU_RGDAAC_REG(MPU_BASE_PTR,7)

/* MPU - Register array accessors */
#define MPU_EAR(index)                           MPU_EAR_REG(MPU_BASE_PTR,index)
#define MPU_EDR(index)                           MPU_EDR_REG(MPU_BASE_PTR,index)
#define MPU_WORD(index,index2)                   MPU_WORD_REG(MPU_BASE_PTR,index,index2)
#define MPU_RGDAAC(index)                        MPU_RGDAAC_REG(MPU_BASE_PTR,index)

/*!
 * @}
 */ /* end of group MPU_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group MPU_Peripheral */


/* ----------------------------------------------------------------------------
   -- MTB
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Peripheral MTB
 * @{
 */

/** MTB - Peripheral register structure */
typedef struct MTB_MemMap {
  uint32_t POSITION;                               /**< MTB Position Register, offset: 0x0 */
  uint32_t MASTER;                                 /**< MTB Master Register, offset: 0x4 */
  uint32_t FLOW;                                   /**< MTB Flow Register, offset: 0x8 */
  uint32_t BASE;                                   /**< MTB Base Register, offset: 0xC */
  uint8_t RESERVED_0[3824];
  uint32_t MODECTRL;                               /**< Integration Mode Control Register, offset: 0xF00 */
  uint8_t RESERVED_1[156];
  uint32_t TAGSET;                                 /**< Claim TAG Set Register, offset: 0xFA0 */
  uint32_t TAGCLEAR;                               /**< Claim TAG Clear Register, offset: 0xFA4 */
  uint8_t RESERVED_2[8];
  uint32_t LOCKACCESS;                             /**< Lock Access Register, offset: 0xFB0 */
  uint32_t LOCKSTAT;                               /**< Lock Status Register, offset: 0xFB4 */
  uint32_t AUTHSTAT;                               /**< Authentication Status Register, offset: 0xFB8 */
  uint32_t DEVICEARCH;                             /**< Device Architecture Register, offset: 0xFBC */
  uint8_t RESERVED_3[8];
  uint32_t DEVICECFG;                              /**< Device Configuration Register, offset: 0xFC8 */
  uint32_t DEVICETYPID;                            /**< Device Type Identifier Register, offset: 0xFCC */
  uint32_t PERIPHID[8];                            /**< Peripheral ID Register, array offset: 0xFD0, array step: 0x4 */
  uint32_t COMPID[4];                              /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} volatile *MTB_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MTB - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Register_Accessor_Macros MTB - Register accessor macros
 * @{
 */


/* MTB - Register accessors */
#define MTB_POSITION_REG(base)                   ((base)->POSITION)
#define MTB_MASTER_REG(base)                     ((base)->MASTER)
#define MTB_FLOW_REG(base)                       ((base)->FLOW)
#define MTB_BASE_REG(base)                       ((base)->BASE)
#define MTB_MODECTRL_REG(base)                   ((base)->MODECTRL)
#define MTB_TAGSET_REG(base)                     ((base)->TAGSET)
#define MTB_TAGCLEAR_REG(base)                   ((base)->TAGCLEAR)
#define MTB_LOCKACCESS_REG(base)                 ((base)->LOCKACCESS)
#define MTB_LOCKSTAT_REG(base)                   ((base)->LOCKSTAT)
#define MTB_AUTHSTAT_REG(base)                   ((base)->AUTHSTAT)
#define MTB_DEVICEARCH_REG(base)                 ((base)->DEVICEARCH)
#define MTB_DEVICECFG_REG(base)                  ((base)->DEVICECFG)
#define MTB_DEVICETYPID_REG(base)                ((base)->DEVICETYPID)
#define MTB_PERIPHID_REG(base,index)             ((base)->PERIPHID[index])
#define MTB_COMPID_REG(base,index)               ((base)->COMPID[index])

/*!
 * @}
 */ /* end of group MTB_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- MTB Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Register_Masks MTB Register Masks
 * @{
 */

/* POSITION Bit Fields */
#define MTB_POSITION_WRAP_MASK                   0x4u
#define MTB_POSITION_WRAP_SHIFT                  2
#define MTB_POSITION_POINTER_MASK                0xFFFFFFF8u
#define MTB_POSITION_POINTER_SHIFT               3
#define MTB_POSITION_POINTER(x)                  (((uint32_t)(((uint32_t)(x))<<MTB_POSITION_POINTER_SHIFT))&MTB_POSITION_POINTER_MASK)
/* MASTER Bit Fields */
#define MTB_MASTER_MASK_MASK                     0x1Fu
#define MTB_MASTER_MASK_SHIFT                    0
#define MTB_MASTER_MASK(x)                       (((uint32_t)(((uint32_t)(x))<<MTB_MASTER_MASK_SHIFT))&MTB_MASTER_MASK_MASK)
#define MTB_MASTER_TSTARTEN_MASK                 0x20u
#define MTB_MASTER_TSTARTEN_SHIFT                5
#define MTB_MASTER_TSTOPEN_MASK                  0x40u
#define MTB_MASTER_TSTOPEN_SHIFT                 6
#define MTB_MASTER_SFRWPRIV_MASK                 0x80u
#define MTB_MASTER_SFRWPRIV_SHIFT                7
#define MTB_MASTER_RAMPRIV_MASK                  0x100u
#define MTB_MASTER_RAMPRIV_SHIFT                 8
#define MTB_MASTER_HALTREQ_MASK                  0x200u
#define MTB_MASTER_HALTREQ_SHIFT                 9
#define MTB_MASTER_EN_MASK                       0x80000000u
#define MTB_MASTER_EN_SHIFT                      31
/* FLOW Bit Fields */
#define MTB_FLOW_AUTOSTOP_MASK                   0x1u
#define MTB_FLOW_AUTOSTOP_SHIFT                  0
#define MTB_FLOW_AUTOHALT_MASK                   0x2u
#define MTB_FLOW_AUTOHALT_SHIFT                  1
#define MTB_FLOW_WATERMARK_MASK                  0xFFFFFFF8u
#define MTB_FLOW_WATERMARK_SHIFT                 3
#define MTB_FLOW_WATERMARK(x)                    (((uint32_t)(((uint32_t)(x))<<MTB_FLOW_WATERMARK_SHIFT))&MTB_FLOW_WATERMARK_MASK)
/* BASE Bit Fields */
#define MTB_BASE_BASEADDR_MASK                   0xFFFFFFFFu
#define MTB_BASE_BASEADDR_SHIFT                  0
#define MTB_BASE_BASEADDR(x)                     (((uint32_t)(((uint32_t)(x))<<MTB_BASE_BASEADDR_SHIFT))&MTB_BASE_BASEADDR_MASK)
/* MODECTRL Bit Fields */
#define MTB_MODECTRL_MODECTRL_MASK               0xFFFFFFFFu
#define MTB_MODECTRL_MODECTRL_SHIFT              0
#define MTB_MODECTRL_MODECTRL(x)                 (((uint32_t)(((uint32_t)(x))<<MTB_MODECTRL_MODECTRL_SHIFT))&MTB_MODECTRL_MODECTRL_MASK)
/* TAGSET Bit Fields */
#define MTB_TAGSET_TAGSET_MASK                   0xFFFFFFFFu
#define MTB_TAGSET_TAGSET_SHIFT                  0
#define MTB_TAGSET_TAGSET(x)                     (((uint32_t)(((uint32_t)(x))<<MTB_TAGSET_TAGSET_SHIFT))&MTB_TAGSET_TAGSET_MASK)
/* TAGCLEAR Bit Fields */
#define MTB_TAGCLEAR_TAGCLEAR_MASK               0xFFFFFFFFu
#define MTB_TAGCLEAR_TAGCLEAR_SHIFT              0
#define MTB_TAGCLEAR_TAGCLEAR(x)                 (((uint32_t)(((uint32_t)(x))<<MTB_TAGCLEAR_TAGCLEAR_SHIFT))&MTB_TAGCLEAR_TAGCLEAR_MASK)
/* LOCKACCESS Bit Fields */
#define MTB_LOCKACCESS_LOCKACCESS_MASK           0xFFFFFFFFu
#define MTB_LOCKACCESS_LOCKACCESS_SHIFT          0
#define MTB_LOCKACCESS_LOCKACCESS(x)             (((uint32_t)(((uint32_t)(x))<<MTB_LOCKACCESS_LOCKACCESS_SHIFT))&MTB_LOCKACCESS_LOCKACCESS_MASK)
/* LOCKSTAT Bit Fields */
#define MTB_LOCKSTAT_LOCKSTAT_MASK               0xFFFFFFFFu
#define MTB_LOCKSTAT_LOCKSTAT_SHIFT              0
#define MTB_LOCKSTAT_LOCKSTAT(x)                 (((uint32_t)(((uint32_t)(x))<<MTB_LOCKSTAT_LOCKSTAT_SHIFT))&MTB_LOCKSTAT_LOCKSTAT_MASK)
/* AUTHSTAT Bit Fields */
#define MTB_AUTHSTAT_BIT0_MASK                   0x1u
#define MTB_AUTHSTAT_BIT0_SHIFT                  0
#define MTB_AUTHSTAT_BIT1_MASK                   0x2u
#define MTB_AUTHSTAT_BIT1_SHIFT                  1
#define MTB_AUTHSTAT_BIT2_MASK                   0x4u
#define MTB_AUTHSTAT_BIT2_SHIFT                  2
#define MTB_AUTHSTAT_BIT3_MASK                   0x8u
#define MTB_AUTHSTAT_BIT3_SHIFT                  3
/* DEVICEARCH Bit Fields */
#define MTB_DEVICEARCH_DEVICEARCH_MASK           0xFFFFFFFFu
#define MTB_DEVICEARCH_DEVICEARCH_SHIFT          0
#define MTB_DEVICEARCH_DEVICEARCH(x)             (((uint32_t)(((uint32_t)(x))<<MTB_DEVICEARCH_DEVICEARCH_SHIFT))&MTB_DEVICEARCH_DEVICEARCH_MASK)
/* DEVICECFG Bit Fields */
#define MTB_DEVICECFG_DEVICECFG_MASK             0xFFFFFFFFu
#define MTB_DEVICECFG_DEVICECFG_SHIFT            0
#define MTB_DEVICECFG_DEVICECFG(x)               (((uint32_t)(((uint32_t)(x))<<MTB_DEVICECFG_DEVICECFG_SHIFT))&MTB_DEVICECFG_DEVICECFG_MASK)
/* DEVICETYPID Bit Fields */
#define MTB_DEVICETYPID_DEVICETYPID_MASK         0xFFFFFFFFu
#define MTB_DEVICETYPID_DEVICETYPID_SHIFT        0
#define MTB_DEVICETYPID_DEVICETYPID(x)           (((uint32_t)(((uint32_t)(x))<<MTB_DEVICETYPID_DEVICETYPID_SHIFT))&MTB_DEVICETYPID_DEVICETYPID_MASK)
/* PERIPHID Bit Fields */
#define MTB_PERIPHID_PERIPHID_MASK               0xFFFFFFFFu
#define MTB_PERIPHID_PERIPHID_SHIFT              0
#define MTB_PERIPHID_PERIPHID(x)                 (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID_PERIPHID_SHIFT))&MTB_PERIPHID_PERIPHID_MASK)
/* COMPID Bit Fields */
#define MTB_COMPID_COMPID_MASK                   0xFFFFFFFFu
#define MTB_COMPID_COMPID_SHIFT                  0
#define MTB_COMPID_COMPID(x)                     (((uint32_t)(((uint32_t)(x))<<MTB_COMPID_COMPID_SHIFT))&MTB_COMPID_COMPID_MASK)

/*!
 * @}
 */ /* end of group MTB_Register_Masks */


/* MTB - Peripheral instance base addresses */
/** Peripheral MTB base pointer */
#define MTB_BASE_PTR                             ((MTB_MemMapPtr)0xF0000000u)
/** Array initializer of MTB peripheral base pointers */
#define MTB_BASE_PTRS                            { MTB_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- MTB - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Register_Accessor_Macros MTB - Register accessor macros
 * @{
 */


/* MTB - Register instance definitions */
/* MTB */
#define MTB_POSITION                             MTB_POSITION_REG(MTB_BASE_PTR)
#define MTB_MASTER                               MTB_MASTER_REG(MTB_BASE_PTR)
#define MTB_FLOW                                 MTB_FLOW_REG(MTB_BASE_PTR)
#define MTB_BASE                                 MTB_BASE_REG(MTB_BASE_PTR)
#define MTB_MODECTRL                             MTB_MODECTRL_REG(MTB_BASE_PTR)
#define MTB_TAGSET                               MTB_TAGSET_REG(MTB_BASE_PTR)
#define MTB_TAGCLEAR                             MTB_TAGCLEAR_REG(MTB_BASE_PTR)
#define MTB_LOCKACCESS                           MTB_LOCKACCESS_REG(MTB_BASE_PTR)
#define MTB_LOCKSTAT                             MTB_LOCKSTAT_REG(MTB_BASE_PTR)
#define MTB_AUTHSTAT                             MTB_AUTHSTAT_REG(MTB_BASE_PTR)
#define MTB_DEVICEARCH                           MTB_DEVICEARCH_REG(MTB_BASE_PTR)
#define MTB_DEVICECFG                            MTB_DEVICECFG_REG(MTB_BASE_PTR)
#define MTB_DEVICETYPID                          MTB_DEVICETYPID_REG(MTB_BASE_PTR)
#define MTB_PERIPHID4                            MTB_PERIPHID_REG(MTB_BASE_PTR,0)
#define MTB_PERIPHID5                            MTB_PERIPHID_REG(MTB_BASE_PTR,1)
#define MTB_PERIPHID6                            MTB_PERIPHID_REG(MTB_BASE_PTR,2)
#define MTB_PERIPHID7                            MTB_PERIPHID_REG(MTB_BASE_PTR,3)
#define MTB_PERIPHID0                            MTB_PERIPHID_REG(MTB_BASE_PTR,4)
#define MTB_PERIPHID1                            MTB_PERIPHID_REG(MTB_BASE_PTR,5)
#define MTB_PERIPHID2                            MTB_PERIPHID_REG(MTB_BASE_PTR,6)
#define MTB_PERIPHID3                            MTB_PERIPHID_REG(MTB_BASE_PTR,7)
#define MTB_COMPID0                              MTB_COMPID_REG(MTB_BASE_PTR,0)
#define MTB_COMPID1                              MTB_COMPID_REG(MTB_BASE_PTR,1)
#define MTB_COMPID2                              MTB_COMPID_REG(MTB_BASE_PTR,2)
#define MTB_COMPID3                              MTB_COMPID_REG(MTB_BASE_PTR,3)

/* MTB - Register array accessors */
#define MTB_PERIPHID(index)                      MTB_PERIPHID_REG(MTB_BASE_PTR,index)
#define MTB_COMPID(index)                        MTB_COMPID_REG(MTB_BASE_PTR,index)

/*!
 * @}
 */ /* end of group MTB_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group MTB_Peripheral */


/* ----------------------------------------------------------------------------
   -- MTBDWT
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Peripheral MTBDWT
 * @{
 */

/** MTBDWT - Peripheral register structure */
typedef struct MTBDWT_MemMap {
  uint32_t CTRL;                                   /**< MTB DWT Control Register, offset: 0x0 */
  uint8_t RESERVED_0[28];
  struct {                                         /* offset: 0x20, array step: 0x10 */
    uint32_t COMP;                                   /**< MTB_DWT Comparator Register, array offset: 0x20, array step: 0x10 */
    uint32_t MASK;                                   /**< MTB_DWT Comparator Mask Register, array offset: 0x24, array step: 0x10 */
    uint32_t FCT;                                    /**< MTB_DWT Comparator Function Register 0..MTB_DWT Comparator Function Register 1, array offset: 0x28, array step: 0x10 */
    uint8_t RESERVED_0[4];
  } COMPARATOR[2];
  uint8_t RESERVED_1[448];
  uint32_t TBCTRL;                                 /**< MTB_DWT Trace Buffer Control Register, offset: 0x200 */
  uint8_t RESERVED_2[3524];
  uint32_t DEVICECFG;                              /**< Device Configuration Register, offset: 0xFC8 */
  uint32_t DEVICETYPID;                            /**< Device Type Identifier Register, offset: 0xFCC */
  uint32_t PERIPHID[8];                            /**< Peripheral ID Register, array offset: 0xFD0, array step: 0x4 */
  uint32_t COMPID[4];                              /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} volatile *MTBDWT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MTBDWT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Register_Accessor_Macros MTBDWT - Register accessor macros
 * @{
 */


/* MTBDWT - Register accessors */
#define MTBDWT_CTRL_REG(base)                    ((base)->CTRL)
#define MTBDWT_COMP_REG(base,index)              ((base)->COMPARATOR[index].COMP)
#define MTBDWT_MASK_REG(base,index)              ((base)->COMPARATOR[index].MASK)
#define MTBDWT_FCT_REG(base,index)               ((base)->COMPARATOR[index].FCT)
#define MTBDWT_TBCTRL_REG(base)                  ((base)->TBCTRL)
#define MTBDWT_DEVICECFG_REG(base)               ((base)->DEVICECFG)
#define MTBDWT_DEVICETYPID_REG(base)             ((base)->DEVICETYPID)
#define MTBDWT_PERIPHID_REG(base,index)          ((base)->PERIPHID[index])
#define MTBDWT_COMPID_REG(base,index)            ((base)->COMPID[index])

/*!
 * @}
 */ /* end of group MTBDWT_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- MTBDWT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Register_Masks MTBDWT Register Masks
 * @{
 */

/* CTRL Bit Fields */
#define MTBDWT_CTRL_DWTCFGCTRL_MASK              0xFFFFFFFu
#define MTBDWT_CTRL_DWTCFGCTRL_SHIFT             0
#define MTBDWT_CTRL_DWTCFGCTRL(x)                (((uint32_t)(((uint32_t)(x))<<MTBDWT_CTRL_DWTCFGCTRL_SHIFT))&MTBDWT_CTRL_DWTCFGCTRL_MASK)
#define MTBDWT_CTRL_NUMCMP_MASK                  0xF0000000u
#define MTBDWT_CTRL_NUMCMP_SHIFT                 28
#define MTBDWT_CTRL_NUMCMP(x)                    (((uint32_t)(((uint32_t)(x))<<MTBDWT_CTRL_NUMCMP_SHIFT))&MTBDWT_CTRL_NUMCMP_MASK)
/* COMP Bit Fields */
#define MTBDWT_COMP_COMP_MASK                    0xFFFFFFFFu
#define MTBDWT_COMP_COMP_SHIFT                   0
#define MTBDWT_COMP_COMP(x)                      (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMP_COMP_SHIFT))&MTBDWT_COMP_COMP_MASK)
/* MASK Bit Fields */
#define MTBDWT_MASK_MASK_MASK                    0x1Fu
#define MTBDWT_MASK_MASK_SHIFT                   0
#define MTBDWT_MASK_MASK(x)                      (((uint32_t)(((uint32_t)(x))<<MTBDWT_MASK_MASK_SHIFT))&MTBDWT_MASK_MASK_MASK)
/* FCT Bit Fields */
#define MTBDWT_FCT_FUNCTION_MASK                 0xFu
#define MTBDWT_FCT_FUNCTION_SHIFT                0
#define MTBDWT_FCT_FUNCTION(x)                   (((uint32_t)(((uint32_t)(x))<<MTBDWT_FCT_FUNCTION_SHIFT))&MTBDWT_FCT_FUNCTION_MASK)
#define MTBDWT_FCT_DATAVMATCH_MASK               0x100u
#define MTBDWT_FCT_DATAVMATCH_SHIFT              8
#define MTBDWT_FCT_DATAVSIZE_MASK                0xC00u
#define MTBDWT_FCT_DATAVSIZE_SHIFT               10
#define MTBDWT_FCT_DATAVSIZE(x)                  (((uint32_t)(((uint32_t)(x))<<MTBDWT_FCT_DATAVSIZE_SHIFT))&MTBDWT_FCT_DATAVSIZE_MASK)
#define MTBDWT_FCT_DATAVADDR0_MASK               0xF000u
#define MTBDWT_FCT_DATAVADDR0_SHIFT              12
#define MTBDWT_FCT_DATAVADDR0(x)                 (((uint32_t)(((uint32_t)(x))<<MTBDWT_FCT_DATAVADDR0_SHIFT))&MTBDWT_FCT_DATAVADDR0_MASK)
#define MTBDWT_FCT_MATCHED_MASK                  0x1000000u
#define MTBDWT_FCT_MATCHED_SHIFT                 24
/* TBCTRL Bit Fields */
#define MTBDWT_TBCTRL_ACOMP0_MASK                0x1u
#define MTBDWT_TBCTRL_ACOMP0_SHIFT               0
#define MTBDWT_TBCTRL_ACOMP1_MASK                0x2u
#define MTBDWT_TBCTRL_ACOMP1_SHIFT               1
#define MTBDWT_TBCTRL_NUMCOMP_MASK               0xF0000000u
#define MTBDWT_TBCTRL_NUMCOMP_SHIFT              28
#define MTBDWT_TBCTRL_NUMCOMP(x)                 (((uint32_t)(((uint32_t)(x))<<MTBDWT_TBCTRL_NUMCOMP_SHIFT))&MTBDWT_TBCTRL_NUMCOMP_MASK)
/* DEVICECFG Bit Fields */
#define MTBDWT_DEVICECFG_DEVICECFG_MASK          0xFFFFFFFFu
#define MTBDWT_DEVICECFG_DEVICECFG_SHIFT         0
#define MTBDWT_DEVICECFG_DEVICECFG(x)            (((uint32_t)(((uint32_t)(x))<<MTBDWT_DEVICECFG_DEVICECFG_SHIFT))&MTBDWT_DEVICECFG_DEVICECFG_MASK)
/* DEVICETYPID Bit Fields */
#define MTBDWT_DEVICETYPID_DEVICETYPID_MASK      0xFFFFFFFFu
#define MTBDWT_DEVICETYPID_DEVICETYPID_SHIFT     0
#define MTBDWT_DEVICETYPID_DEVICETYPID(x)        (((uint32_t)(((uint32_t)(x))<<MTBDWT_DEVICETYPID_DEVICETYPID_SHIFT))&MTBDWT_DEVICETYPID_DEVICETYPID_MASK)
/* PERIPHID Bit Fields */
#define MTBDWT_PERIPHID_PERIPHID_MASK            0xFFFFFFFFu
#define MTBDWT_PERIPHID_PERIPHID_SHIFT           0
#define MTBDWT_PERIPHID_PERIPHID(x)              (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID_PERIPHID_SHIFT))&MTBDWT_PERIPHID_PERIPHID_MASK)
/* COMPID Bit Fields */
#define MTBDWT_COMPID_COMPID_MASK                0xFFFFFFFFu
#define MTBDWT_COMPID_COMPID_SHIFT               0
#define MTBDWT_COMPID_COMPID(x)                  (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMPID_COMPID_SHIFT))&MTBDWT_COMPID_COMPID_MASK)

/*!
 * @}
 */ /* end of group MTBDWT_Register_Masks */


/* MTBDWT - Peripheral instance base addresses */
/** Peripheral MTBDWT base pointer */
#define MTBDWT_BASE_PTR                          ((MTBDWT_MemMapPtr)0xF0001000u)
/** Array initializer of MTBDWT peripheral base pointers */
#define MTBDWT_BASE_PTRS                         { MTBDWT_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- MTBDWT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Register_Accessor_Macros MTBDWT - Register accessor macros
 * @{
 */


/* MTBDWT - Register instance definitions */
/* MTBDWT */
#define MTBDWT_CTRL                              MTBDWT_CTRL_REG(MTBDWT_BASE_PTR)
#define MTBDWT_COMP0                             MTBDWT_COMP_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_MASK0                             MTBDWT_MASK_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_FCT0                              MTBDWT_FCT_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_COMP1                             MTBDWT_COMP_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_MASK1                             MTBDWT_MASK_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_FCT1                              MTBDWT_FCT_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_TBCTRL                            MTBDWT_TBCTRL_REG(MTBDWT_BASE_PTR)
#define MTBDWT_DEVICECFG                         MTBDWT_DEVICECFG_REG(MTBDWT_BASE_PTR)
#define MTBDWT_DEVICETYPID                       MTBDWT_DEVICETYPID_REG(MTBDWT_BASE_PTR)
#define MTBDWT_PERIPHID4                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_PERIPHID5                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_PERIPHID6                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,2)
#define MTBDWT_PERIPHID7                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,3)
#define MTBDWT_PERIPHID0                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,4)
#define MTBDWT_PERIPHID1                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,5)
#define MTBDWT_PERIPHID2                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,6)
#define MTBDWT_PERIPHID3                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,7)
#define MTBDWT_COMPID0                           MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_COMPID1                           MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_COMPID2                           MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,2)
#define MTBDWT_COMPID3                           MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,3)

/* MTBDWT - Register array accessors */
#define MTBDWT_COMP(index)                       MTBDWT_COMP_REG(MTBDWT_BASE_PTR,index)
#define MTBDWT_MASK(index)                       MTBDWT_MASK_REG(MTBDWT_BASE_PTR,index)
#define MTBDWT_FCT(index)                        MTBDWT_FCT_REG(MTBDWT_BASE_PTR,index)
#define MTBDWT_PERIPHID(index)                   MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,index)
#define MTBDWT_COMPID(index)                     MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,index)

/*!
 * @}
 */ /* end of group MTBDWT_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group MTBDWT_Peripheral */


/* ----------------------------------------------------------------------------
   -- NV
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Peripheral NV
 * @{
 */

/** NV - Peripheral register structure */
typedef struct NV_MemMap {
  uint8_t BACKKEY3;                                /**< Backdoor Comparison Key 3., offset: 0x0 */
  uint8_t BACKKEY2;                                /**< Backdoor Comparison Key 2., offset: 0x1 */
  uint8_t BACKKEY1;                                /**< Backdoor Comparison Key 1., offset: 0x2 */
  uint8_t BACKKEY0;                                /**< Backdoor Comparison Key 0., offset: 0x3 */
  uint8_t BACKKEY7;                                /**< Backdoor Comparison Key 7., offset: 0x4 */
  uint8_t BACKKEY6;                                /**< Backdoor Comparison Key 6., offset: 0x5 */
  uint8_t BACKKEY5;                                /**< Backdoor Comparison Key 5., offset: 0x6 */
  uint8_t BACKKEY4;                                /**< Backdoor Comparison Key 4., offset: 0x7 */
  uint8_t FPROT3;                                  /**< Non-volatile P-Flash Protection 1 - Low Register, offset: 0x8 */
  uint8_t FPROT2;                                  /**< Non-volatile P-Flash Protection 1 - High Register, offset: 0x9 */
  uint8_t FPROT1;                                  /**< Non-volatile P-Flash Protection 0 - Low Register, offset: 0xA */
  uint8_t FPROT0;                                  /**< Non-volatile P-Flash Protection 0 - High Register, offset: 0xB */
  uint8_t FSEC;                                    /**< Non-volatile Flash Security Register, offset: 0xC */
  uint8_t FOPT;                                    /**< Non-volatile Flash Option Register, offset: 0xD */
} volatile *NV_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- NV - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Register_Accessor_Macros NV - Register accessor macros
 * @{
 */


/* NV - Register accessors */
#define NV_BACKKEY3_REG(base)                    ((base)->BACKKEY3)
#define NV_BACKKEY2_REG(base)                    ((base)->BACKKEY2)
#define NV_BACKKEY1_REG(base)                    ((base)->BACKKEY1)
#define NV_BACKKEY0_REG(base)                    ((base)->BACKKEY0)
#define NV_BACKKEY7_REG(base)                    ((base)->BACKKEY7)
#define NV_BACKKEY6_REG(base)                    ((base)->BACKKEY6)
#define NV_BACKKEY5_REG(base)                    ((base)->BACKKEY5)
#define NV_BACKKEY4_REG(base)                    ((base)->BACKKEY4)
#define NV_FPROT3_REG(base)                      ((base)->FPROT3)
#define NV_FPROT2_REG(base)                      ((base)->FPROT2)
#define NV_FPROT1_REG(base)                      ((base)->FPROT1)
#define NV_FPROT0_REG(base)                      ((base)->FPROT0)
#define NV_FSEC_REG(base)                        ((base)->FSEC)
#define NV_FOPT_REG(base)                        ((base)->FOPT)

/*!
 * @}
 */ /* end of group NV_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- NV Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Register_Masks NV Register Masks
 * @{
 */

/* BACKKEY3 Bit Fields */
#define NV_BACKKEY3_KEY_MASK                     0xFFu
#define NV_BACKKEY3_KEY_SHIFT                    0
#define NV_BACKKEY3_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY3_KEY_SHIFT))&NV_BACKKEY3_KEY_MASK)
/* BACKKEY2 Bit Fields */
#define NV_BACKKEY2_KEY_MASK                     0xFFu
#define NV_BACKKEY2_KEY_SHIFT                    0
#define NV_BACKKEY2_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY2_KEY_SHIFT))&NV_BACKKEY2_KEY_MASK)
/* BACKKEY1 Bit Fields */
#define NV_BACKKEY1_KEY_MASK                     0xFFu
#define NV_BACKKEY1_KEY_SHIFT                    0
#define NV_BACKKEY1_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY1_KEY_SHIFT))&NV_BACKKEY1_KEY_MASK)
/* BACKKEY0 Bit Fields */
#define NV_BACKKEY0_KEY_MASK                     0xFFu
#define NV_BACKKEY0_KEY_SHIFT                    0
#define NV_BACKKEY0_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY0_KEY_SHIFT))&NV_BACKKEY0_KEY_MASK)
/* BACKKEY7 Bit Fields */
#define NV_BACKKEY7_KEY_MASK                     0xFFu
#define NV_BACKKEY7_KEY_SHIFT                    0
#define NV_BACKKEY7_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY7_KEY_SHIFT))&NV_BACKKEY7_KEY_MASK)
/* BACKKEY6 Bit Fields */
#define NV_BACKKEY6_KEY_MASK                     0xFFu
#define NV_BACKKEY6_KEY_SHIFT                    0
#define NV_BACKKEY6_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY6_KEY_SHIFT))&NV_BACKKEY6_KEY_MASK)
/* BACKKEY5 Bit Fields */
#define NV_BACKKEY5_KEY_MASK                     0xFFu
#define NV_BACKKEY5_KEY_SHIFT                    0
#define NV_BACKKEY5_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY5_KEY_SHIFT))&NV_BACKKEY5_KEY_MASK)
/* BACKKEY4 Bit Fields */
#define NV_BACKKEY4_KEY_MASK                     0xFFu
#define NV_BACKKEY4_KEY_SHIFT                    0
#define NV_BACKKEY4_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY4_KEY_SHIFT))&NV_BACKKEY4_KEY_MASK)
/* FPROT3 Bit Fields */
#define NV_FPROT3_PROT_MASK                      0xFFu
#define NV_FPROT3_PROT_SHIFT                     0
#define NV_FPROT3_PROT(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FPROT3_PROT_SHIFT))&NV_FPROT3_PROT_MASK)
/* FPROT2 Bit Fields */
#define NV_FPROT2_PROT_MASK                      0xFFu
#define NV_FPROT2_PROT_SHIFT                     0
#define NV_FPROT2_PROT(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FPROT2_PROT_SHIFT))&NV_FPROT2_PROT_MASK)
/* FPROT1 Bit Fields */
#define NV_FPROT1_PROT_MASK                      0xFFu
#define NV_FPROT1_PROT_SHIFT                     0
#define NV_FPROT1_PROT(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FPROT1_PROT_SHIFT))&NV_FPROT1_PROT_MASK)
/* FPROT0 Bit Fields */
#define NV_FPROT0_PROT_MASK                      0xFFu
#define NV_FPROT0_PROT_SHIFT                     0
#define NV_FPROT0_PROT(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FPROT0_PROT_SHIFT))&NV_FPROT0_PROT_MASK)
/* FSEC Bit Fields */
#define NV_FSEC_SEC_MASK                         0x3u
#define NV_FSEC_SEC_SHIFT                        0
#define NV_FSEC_SEC(x)                           (((uint8_t)(((uint8_t)(x))<<NV_FSEC_SEC_SHIFT))&NV_FSEC_SEC_MASK)
#define NV_FSEC_FSLACC_MASK                      0xCu
#define NV_FSEC_FSLACC_SHIFT                     2
#define NV_FSEC_FSLACC(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FSEC_FSLACC_SHIFT))&NV_FSEC_FSLACC_MASK)
#define NV_FSEC_MEEN_MASK                        0x30u
#define NV_FSEC_MEEN_SHIFT                       4
#define NV_FSEC_MEEN(x)                          (((uint8_t)(((uint8_t)(x))<<NV_FSEC_MEEN_SHIFT))&NV_FSEC_MEEN_MASK)
#define NV_FSEC_KEYEN_MASK                       0xC0u
#define NV_FSEC_KEYEN_SHIFT                      6
#define NV_FSEC_KEYEN(x)                         (((uint8_t)(((uint8_t)(x))<<NV_FSEC_KEYEN_SHIFT))&NV_FSEC_KEYEN_MASK)
/* FOPT Bit Fields */
#define NV_FOPT_LPBOOT_MASK                      0x1u
#define NV_FOPT_LPBOOT_SHIFT                     0
#define NV_FOPT_NMI_EN_MASK                      0x4u
#define NV_FOPT_NMI_EN_SHIFT                     2
#define NV_FOPT_EXE_MODE_MASK                    0x8u
#define NV_FOPT_EXE_MODE_SHIFT                   3
#define NV_FOPT_CLK_SRC_MASK                     0x20u
#define NV_FOPT_CLK_SRC_SHIFT                    5

/*!
 * @}
 */ /* end of group NV_Register_Masks */


/* NV - Peripheral instance base addresses */
/** Peripheral FTFA_FlashConfig base pointer */
#define FTFA_FlashConfig_BASE_PTR                ((NV_MemMapPtr)0x400u)
/** Array initializer of NV peripheral base pointers */
#define NV_BASE_PTRS                             { FTFA_FlashConfig_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- NV - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Register_Accessor_Macros NV - Register accessor macros
 * @{
 */


/* NV - Register instance definitions */
/* FTFA_FlashConfig */
#define NV_BACKKEY3                              NV_BACKKEY3_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY2                              NV_BACKKEY2_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY1                              NV_BACKKEY1_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY0                              NV_BACKKEY0_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY7                              NV_BACKKEY7_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY6                              NV_BACKKEY6_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY5                              NV_BACKKEY5_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY4                              NV_BACKKEY4_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FPROT3                                NV_FPROT3_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FPROT2                                NV_FPROT2_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FPROT1                                NV_FPROT1_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FPROT0                                NV_FPROT0_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FSEC                                  NV_FSEC_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FOPT                                  NV_FOPT_REG(FTFA_FlashConfig_BASE_PTR)

/*!
 * @}
 */ /* end of group NV_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group NV_Peripheral */


/* ----------------------------------------------------------------------------
   -- NVIC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NVIC_Peripheral NVIC
 * @{
 */

/** NVIC - Peripheral register structure */
typedef struct NVIC_MemMap {
  uint32_t ISER;                                   /**< Interrupt Set Enable Register, offset: 0x0 */
  uint8_t RESERVED_0[124];
  uint32_t ICER;                                   /**< Interrupt Clear Enable Register, offset: 0x80 */
  uint8_t RESERVED_1[124];
  uint32_t ISPR;                                   /**< Interrupt Set Pending Register, offset: 0x100 */
  uint8_t RESERVED_2[124];
  uint32_t ICPR;                                   /**< Interrupt Clear Pending Register, offset: 0x180 */
  uint8_t RESERVED_3[380];
  uint32_t IP[8];                                  /**< Interrupt Priority Register n, array offset: 0x300, array step: 0x4 */
} volatile *NVIC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- NVIC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NVIC_Register_Accessor_Macros NVIC - Register accessor macros
 * @{
 */


/* NVIC - Register accessors */
#define NVIC_ISER_REG(base)                      ((base)->ISER)
#define NVIC_ICER_REG(base)                      ((base)->ICER)
#define NVIC_ISPR_REG(base)                      ((base)->ISPR)
#define NVIC_ICPR_REG(base)                      ((base)->ICPR)
#define NVIC_IP_REG(base,index)                  ((base)->IP[index])

/*!
 * @}
 */ /* end of group NVIC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- NVIC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NVIC_Register_Masks NVIC Register Masks
 * @{
 */

/* ISER Bit Fields */
#define NVIC_ISER_SETENA_MASK                    0xFFFFFFFFu
#define NVIC_ISER_SETENA_SHIFT                   0
#define NVIC_ISER_SETENA(x)                      (((uint32_t)(((uint32_t)(x))<<NVIC_ISER_SETENA_SHIFT))&NVIC_ISER_SETENA_MASK)
/* ICER Bit Fields */
#define NVIC_ICER_CLRENA_MASK                    0xFFFFFFFFu
#define NVIC_ICER_CLRENA_SHIFT                   0
#define NVIC_ICER_CLRENA(x)                      (((uint32_t)(((uint32_t)(x))<<NVIC_ICER_CLRENA_SHIFT))&NVIC_ICER_CLRENA_MASK)
/* ISPR Bit Fields */
#define NVIC_ISPR_SETPEND_MASK                   0xFFFFFFFFu
#define NVIC_ISPR_SETPEND_SHIFT                  0
#define NVIC_ISPR_SETPEND(x)                     (((uint32_t)(((uint32_t)(x))<<NVIC_ISPR_SETPEND_SHIFT))&NVIC_ISPR_SETPEND_MASK)
/* ICPR Bit Fields */
#define NVIC_ICPR_CLRPEND_MASK                   0xFFFFFFFFu
#define NVIC_ICPR_CLRPEND_SHIFT                  0
#define NVIC_ICPR_CLRPEND(x)                     (((uint32_t)(((uint32_t)(x))<<NVIC_ICPR_CLRPEND_SHIFT))&NVIC_ICPR_CLRPEND_MASK)
/* IP Bit Fields */
#define NVIC_IP_PRI_0_MASK                       0xFFu
#define NVIC_IP_PRI_0_SHIFT                      0
#define NVIC_IP_PRI_0(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_0_SHIFT))&NVIC_IP_PRI_0_MASK)
#define NVIC_IP_PRI_28_MASK                      0xFFu
#define NVIC_IP_PRI_28_SHIFT                     0
#define NVIC_IP_PRI_28(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_28_SHIFT))&NVIC_IP_PRI_28_MASK)
#define NVIC_IP_PRI_24_MASK                      0xFFu
#define NVIC_IP_PRI_24_SHIFT                     0
#define NVIC_IP_PRI_24(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_24_SHIFT))&NVIC_IP_PRI_24_MASK)
#define NVIC_IP_PRI_20_MASK                      0xFFu
#define NVIC_IP_PRI_20_SHIFT                     0
#define NVIC_IP_PRI_20(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_20_SHIFT))&NVIC_IP_PRI_20_MASK)
#define NVIC_IP_PRI_4_MASK                       0xFFu
#define NVIC_IP_PRI_4_SHIFT                      0
#define NVIC_IP_PRI_4(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_4_SHIFT))&NVIC_IP_PRI_4_MASK)
#define NVIC_IP_PRI_16_MASK                      0xFFu
#define NVIC_IP_PRI_16_SHIFT                     0
#define NVIC_IP_PRI_16(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_16_SHIFT))&NVIC_IP_PRI_16_MASK)
#define NVIC_IP_PRI_12_MASK                      0xFFu
#define NVIC_IP_PRI_12_SHIFT                     0
#define NVIC_IP_PRI_12(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_12_SHIFT))&NVIC_IP_PRI_12_MASK)
#define NVIC_IP_PRI_8_MASK                       0xFFu
#define NVIC_IP_PRI_8_SHIFT                      0
#define NVIC_IP_PRI_8(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_8_SHIFT))&NVIC_IP_PRI_8_MASK)
#define NVIC_IP_PRI_13_MASK                      0xFF00u
#define NVIC_IP_PRI_13_SHIFT                     8
#define NVIC_IP_PRI_13(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_13_SHIFT))&NVIC_IP_PRI_13_MASK)
#define NVIC_IP_PRI_21_MASK                      0xFF00u
#define NVIC_IP_PRI_21_SHIFT                     8
#define NVIC_IP_PRI_21(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_21_SHIFT))&NVIC_IP_PRI_21_MASK)
#define NVIC_IP_PRI_29_MASK                      0xFF00u
#define NVIC_IP_PRI_29_SHIFT                     8
#define NVIC_IP_PRI_29(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_29_SHIFT))&NVIC_IP_PRI_29_MASK)
#define NVIC_IP_PRI_1_MASK                       0xFF00u
#define NVIC_IP_PRI_1_SHIFT                      8
#define NVIC_IP_PRI_1(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_1_SHIFT))&NVIC_IP_PRI_1_MASK)
#define NVIC_IP_PRI_9_MASK                       0xFF00u
#define NVIC_IP_PRI_9_SHIFT                      8
#define NVIC_IP_PRI_9(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_9_SHIFT))&NVIC_IP_PRI_9_MASK)
#define NVIC_IP_PRI_17_MASK                      0xFF00u
#define NVIC_IP_PRI_17_SHIFT                     8
#define NVIC_IP_PRI_17(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_17_SHIFT))&NVIC_IP_PRI_17_MASK)
#define NVIC_IP_PRI_25_MASK                      0xFF00u
#define NVIC_IP_PRI_25_SHIFT                     8
#define NVIC_IP_PRI_25(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_25_SHIFT))&NVIC_IP_PRI_25_MASK)
#define NVIC_IP_PRI_5_MASK                       0xFF00u
#define NVIC_IP_PRI_5_SHIFT                      8
#define NVIC_IP_PRI_5(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_5_SHIFT))&NVIC_IP_PRI_5_MASK)
#define NVIC_IP_PRI_2_MASK                       0xFF0000u
#define NVIC_IP_PRI_2_SHIFT                      16
#define NVIC_IP_PRI_2(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_2_SHIFT))&NVIC_IP_PRI_2_MASK)
#define NVIC_IP_PRI_26_MASK                      0xFF0000u
#define NVIC_IP_PRI_26_SHIFT                     16
#define NVIC_IP_PRI_26(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_26_SHIFT))&NVIC_IP_PRI_26_MASK)
#define NVIC_IP_PRI_18_MASK                      0xFF0000u
#define NVIC_IP_PRI_18_SHIFT                     16
#define NVIC_IP_PRI_18(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_18_SHIFT))&NVIC_IP_PRI_18_MASK)
#define NVIC_IP_PRI_14_MASK                      0xFF0000u
#define NVIC_IP_PRI_14_SHIFT                     16
#define NVIC_IP_PRI_14(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_14_SHIFT))&NVIC_IP_PRI_14_MASK)
#define NVIC_IP_PRI_6_MASK                       0xFF0000u
#define NVIC_IP_PRI_6_SHIFT                      16
#define NVIC_IP_PRI_6(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_6_SHIFT))&NVIC_IP_PRI_6_MASK)
#define NVIC_IP_PRI_30_MASK                      0xFF0000u
#define NVIC_IP_PRI_30_SHIFT                     16
#define NVIC_IP_PRI_30(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_30_SHIFT))&NVIC_IP_PRI_30_MASK)
#define NVIC_IP_PRI_22_MASK                      0xFF0000u
#define NVIC_IP_PRI_22_SHIFT                     16
#define NVIC_IP_PRI_22(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_22_SHIFT))&NVIC_IP_PRI_22_MASK)
#define NVIC_IP_PRI_10_MASK                      0xFF0000u
#define NVIC_IP_PRI_10_SHIFT                     16
#define NVIC_IP_PRI_10(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_10_SHIFT))&NVIC_IP_PRI_10_MASK)
#define NVIC_IP_PRI_31_MASK                      0xFF000000u
#define NVIC_IP_PRI_31_SHIFT                     24
#define NVIC_IP_PRI_31(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_31_SHIFT))&NVIC_IP_PRI_31_MASK)
#define NVIC_IP_PRI_27_MASK                      0xFF000000u
#define NVIC_IP_PRI_27_SHIFT                     24
#define NVIC_IP_PRI_27(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_27_SHIFT))&NVIC_IP_PRI_27_MASK)
#define NVIC_IP_PRI_23_MASK                      0xFF000000u
#define NVIC_IP_PRI_23_SHIFT                     24
#define NVIC_IP_PRI_23(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_23_SHIFT))&NVIC_IP_PRI_23_MASK)
#define NVIC_IP_PRI_3_MASK                       0xFF000000u
#define NVIC_IP_PRI_3_SHIFT                      24
#define NVIC_IP_PRI_3(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_3_SHIFT))&NVIC_IP_PRI_3_MASK)
#define NVIC_IP_PRI_19_MASK                      0xFF000000u
#define NVIC_IP_PRI_19_SHIFT                     24
#define NVIC_IP_PRI_19(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_19_SHIFT))&NVIC_IP_PRI_19_MASK)
#define NVIC_IP_PRI_15_MASK                      0xFF000000u
#define NVIC_IP_PRI_15_SHIFT                     24
#define NVIC_IP_PRI_15(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_15_SHIFT))&NVIC_IP_PRI_15_MASK)
#define NVIC_IP_PRI_11_MASK                      0xFF000000u
#define NVIC_IP_PRI_11_SHIFT                     24
#define NVIC_IP_PRI_11(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_11_SHIFT))&NVIC_IP_PRI_11_MASK)
#define NVIC_IP_PRI_7_MASK                       0xFF000000u
#define NVIC_IP_PRI_7_SHIFT                      24
#define NVIC_IP_PRI_7(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_7_SHIFT))&NVIC_IP_PRI_7_MASK)

/*!
 * @}
 */ /* end of group NVIC_Register_Masks */


/* NVIC - Peripheral instance base addresses */
/** Peripheral NVIC base pointer */
#define NVIC_BASE_PTR                            ((NVIC_MemMapPtr)0xE000E100u)
/** Array initializer of NVIC peripheral base pointers */
#define NVIC_BASE_PTRS                           { NVIC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- NVIC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NVIC_Register_Accessor_Macros NVIC - Register accessor macros
 * @{
 */


/* NVIC - Register instance definitions */
/* NVIC */
#define NVIC_ISER                                NVIC_ISER_REG(NVIC_BASE_PTR)
#define NVIC_ICER                                NVIC_ICER_REG(NVIC_BASE_PTR)
#define NVIC_ISPR                                NVIC_ISPR_REG(NVIC_BASE_PTR)
#define NVIC_ICPR                                NVIC_ICPR_REG(NVIC_BASE_PTR)
#define NVIC_IPR0                                NVIC_IP_REG(NVIC_BASE_PTR,0)
#define NVIC_IPR1                                NVIC_IP_REG(NVIC_BASE_PTR,1)
#define NVIC_IPR2                                NVIC_IP_REG(NVIC_BASE_PTR,2)
#define NVIC_IPR3                                NVIC_IP_REG(NVIC_BASE_PTR,3)
#define NVIC_IPR4                                NVIC_IP_REG(NVIC_BASE_PTR,4)
#define NVIC_IPR5                                NVIC_IP_REG(NVIC_BASE_PTR,5)
#define NVIC_IPR6                                NVIC_IP_REG(NVIC_BASE_PTR,6)
#define NVIC_IPR7                                NVIC_IP_REG(NVIC_BASE_PTR,7)

/* NVIC - Register array accessors */
#define NVIC_IP(index)                           NVIC_IP_REG(NVIC_BASE_PTR,index)

/*!
 * @}
 */ /* end of group NVIC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group NVIC_Peripheral */


/* ----------------------------------------------------------------------------
   -- OSC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Peripheral OSC
 * @{
 */

/** OSC - Peripheral register structure */
typedef struct OSC_MemMap {
  uint8_t CR;                                      /**< OSC Control Register, offset: 0x0 */
} volatile *OSC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- OSC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Register_Accessor_Macros OSC - Register accessor macros
 * @{
 */


/* OSC - Register accessors */
#define OSC_CR_REG(base)                         ((base)->CR)

/*!
 * @}
 */ /* end of group OSC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- OSC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Register_Masks OSC Register Masks
 * @{
 */

/* CR Bit Fields */
#define OSC_CR_SC16P_MASK                        0x1u
#define OSC_CR_SC16P_SHIFT                       0
#define OSC_CR_SC8P_MASK                         0x2u
#define OSC_CR_SC8P_SHIFT                        1
#define OSC_CR_SC4P_MASK                         0x4u
#define OSC_CR_SC4P_SHIFT                        2
#define OSC_CR_SC2P_MASK                         0x8u
#define OSC_CR_SC2P_SHIFT                        3
#define OSC_CR_EREFSTEN_MASK                     0x20u
#define OSC_CR_EREFSTEN_SHIFT                    5
#define OSC_CR_ERCLKEN_MASK                      0x80u
#define OSC_CR_ERCLKEN_SHIFT                     7

/*!
 * @}
 */ /* end of group OSC_Register_Masks */


/* OSC - Peripheral instance base addresses */
/** Peripheral OSC base pointer */
#define OSC_BASE_PTR                             ((OSC_MemMapPtr)0x40066000u)
/** Array initializer of OSC peripheral base pointers */
#define OSC_BASE_PTRS                            { OSC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- OSC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Register_Accessor_Macros OSC - Register accessor macros
 * @{
 */


/* OSC - Register instance definitions */
/* OSC */
#define OSC_CR                                   OSC_CR_REG(OSC_BASE_PTR)

/*!
 * @}
 */ /* end of group OSC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group OSC_Peripheral */


/* ----------------------------------------------------------------------------
   -- PIT
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PIT_Peripheral PIT
 * @{
 */

/** PIT - Peripheral register structure */
typedef struct PIT_MemMap {
  uint32_t MCR;                                    /**< PIT Module Control Register, offset: 0x0 */
  uint8_t RESERVED_0[252];
  struct {                                         /* offset: 0x100, array step: 0x10 */
    uint32_t LDVAL;                                  /**< Timer Load Value Register, array offset: 0x100, array step: 0x10 */
    uint32_t CVAL;                                   /**< Current Timer Value Register, array offset: 0x104, array step: 0x10 */
    uint32_t TCTRL;                                  /**< Timer Control Register, array offset: 0x108, array step: 0x10 */
    uint32_t TFLG;                                   /**< Timer Flag Register, array offset: 0x10C, array step: 0x10 */
  } CHANNEL[2];
} volatile *PIT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- PIT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PIT_Register_Accessor_Macros PIT - Register accessor macros
 * @{
 */


/* PIT - Register accessors */
#define PIT_MCR_REG(base)                        ((base)->MCR)
#define PIT_LDVAL_REG(base,index)                ((base)->CHANNEL[index].LDVAL)
#define PIT_CVAL_REG(base,index)                 ((base)->CHANNEL[index].CVAL)
#define PIT_TCTRL_REG(base,index)                ((base)->CHANNEL[index].TCTRL)
#define PIT_TFLG_REG(base,index)                 ((base)->CHANNEL[index].TFLG)

/*!
 * @}
 */ /* end of group PIT_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- PIT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PIT_Register_Masks PIT Register Masks
 * @{
 */

/* MCR Bit Fields */
#define PIT_MCR_FRZ_MASK                         0x1u
#define PIT_MCR_FRZ_SHIFT                        0
#define PIT_MCR_MDIS_MASK                        0x2u
#define PIT_MCR_MDIS_SHIFT                       1
/* LDVAL Bit Fields */
#define PIT_LDVAL_TSV_MASK                       0xFFFFFFFFu
#define PIT_LDVAL_TSV_SHIFT                      0
#define PIT_LDVAL_TSV(x)                         (((uint32_t)(((uint32_t)(x))<<PIT_LDVAL_TSV_SHIFT))&PIT_LDVAL_TSV_MASK)
/* CVAL Bit Fields */
#define PIT_CVAL_TVL_MASK                        0xFFFFFFFFu
#define PIT_CVAL_TVL_SHIFT                       0
#define PIT_CVAL_TVL(x)                          (((uint32_t)(((uint32_t)(x))<<PIT_CVAL_TVL_SHIFT))&PIT_CVAL_TVL_MASK)
/* TCTRL Bit Fields */
#define PIT_TCTRL_TEN_MASK                       0x1u
#define PIT_TCTRL_TEN_SHIFT                      0
#define PIT_TCTRL_TIE_MASK                       0x2u
#define PIT_TCTRL_TIE_SHIFT                      1
#define PIT_TCTRL_CHN_MASK                       0x4u
#define PIT_TCTRL_CHN_SHIFT                      2
/* TFLG Bit Fields */
#define PIT_TFLG_TIF_MASK                        0x1u
#define PIT_TFLG_TIF_SHIFT                       0

/*!
 * @}
 */ /* end of group PIT_Register_Masks */


/* PIT - Peripheral instance base addresses */
/** Peripheral PIT0 base pointer */
#define PIT0_BASE_PTR                            ((PIT_MemMapPtr)0x4002D000u)
/** Peripheral PIT1 base pointer */
#define PIT1_BASE_PTR                            ((PIT_MemMapPtr)0x4002E000u)
/** Array initializer of PIT peripheral base pointers */
#define PIT_BASE_PTRS                            { PIT0_BASE_PTR, PIT1_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- PIT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PIT_Register_Accessor_Macros PIT - Register accessor macros
 * @{
 */


/* PIT - Register instance definitions */
/* PIT0 */
#define PIT0_MCR                                 PIT_MCR_REG(PIT0_BASE_PTR)
#define PIT0_LDVAL0                              PIT_LDVAL_REG(PIT0_BASE_PTR,0)
#define PIT0_CVAL0                               PIT_CVAL_REG(PIT0_BASE_PTR,0)
#define PIT0_TCTRL0                              PIT_TCTRL_REG(PIT0_BASE_PTR,0)
#define PIT0_TFLG0                               PIT_TFLG_REG(PIT0_BASE_PTR,0)
#define PIT0_LDVAL1                              PIT_LDVAL_REG(PIT0_BASE_PTR,1)
#define PIT0_CVAL1                               PIT_CVAL_REG(PIT0_BASE_PTR,1)
#define PIT0_TCTRL1                              PIT_TCTRL_REG(PIT0_BASE_PTR,1)
#define PIT0_TFLG1                               PIT_TFLG_REG(PIT0_BASE_PTR,1)
/* PIT1 */
#define PIT1_MCR                                 PIT_MCR_REG(PIT1_BASE_PTR)
#define PIT1_LDVAL0                              PIT_LDVAL_REG(PIT1_BASE_PTR,0)
#define PIT1_CVAL0                               PIT_CVAL_REG(PIT1_BASE_PTR,0)
#define PIT1_TCTRL0                              PIT_TCTRL_REG(PIT1_BASE_PTR,0)
#define PIT1_TFLG0                               PIT_TFLG_REG(PIT1_BASE_PTR,0)
#define PIT1_LDVAL1                              PIT_LDVAL_REG(PIT1_BASE_PTR,1)
#define PIT1_CVAL1                               PIT_CVAL_REG(PIT1_BASE_PTR,1)
#define PIT1_TCTRL1                              PIT_TCTRL_REG(PIT1_BASE_PTR,1)
#define PIT1_TFLG1                               PIT_TFLG_REG(PIT1_BASE_PTR,1)

/* PIT - Register array accessors */
#define PIT0_LDVAL(index)                        PIT_LDVAL_REG(PIT0_BASE_PTR,index)
#define PIT1_LDVAL(index)                        PIT_LDVAL_REG(PIT1_BASE_PTR,index)
#define PIT0_CVAL(index)                         PIT_CVAL_REG(PIT0_BASE_PTR,index)
#define PIT1_CVAL(index)                         PIT_CVAL_REG(PIT1_BASE_PTR,index)
#define PIT0_TCTRL(index)                        PIT_TCTRL_REG(PIT0_BASE_PTR,index)
#define PIT1_TCTRL(index)                        PIT_TCTRL_REG(PIT1_BASE_PTR,index)
#define PIT0_TFLG(index)                         PIT_TFLG_REG(PIT0_BASE_PTR,index)
#define PIT1_TFLG(index)                         PIT_TFLG_REG(PIT1_BASE_PTR,index)

/*!
 * @}
 */ /* end of group PIT_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group PIT_Peripheral */


/* ----------------------------------------------------------------------------
   -- PMC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Peripheral PMC
 * @{
 */

/** PMC - Peripheral register structure */
typedef struct PMC_MemMap {
  uint8_t LVDSC1;                                  /**< Low Voltage Detect Status And Control 1 register, offset: 0x0 */
  uint8_t LVDSC2;                                  /**< Low Voltage Detect Status And Control 2 register, offset: 0x1 */
  uint8_t REGSC;                                   /**< Regulator Status And Control register, offset: 0x2 */
} volatile *PMC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- PMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Register_Accessor_Macros PMC - Register accessor macros
 * @{
 */


/* PMC - Register accessors */
#define PMC_LVDSC1_REG(base)                     ((base)->LVDSC1)
#define PMC_LVDSC2_REG(base)                     ((base)->LVDSC2)
#define PMC_REGSC_REG(base)                      ((base)->REGSC)

/*!
 * @}
 */ /* end of group PMC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- PMC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Register_Masks PMC Register Masks
 * @{
 */

/* LVDSC1 Bit Fields */
#define PMC_LVDSC1_LVDV_MASK                     0x3u
#define PMC_LVDSC1_LVDV_SHIFT                    0
#define PMC_LVDSC1_LVDV(x)                       (((uint8_t)(((uint8_t)(x))<<PMC_LVDSC1_LVDV_SHIFT))&PMC_LVDSC1_LVDV_MASK)
#define PMC_LVDSC1_LVDRE_MASK                    0x10u
#define PMC_LVDSC1_LVDRE_SHIFT                   4
#define PMC_LVDSC1_LVDIE_MASK                    0x20u
#define PMC_LVDSC1_LVDIE_SHIFT                   5
#define PMC_LVDSC1_LVDACK_MASK                   0x40u
#define PMC_LVDSC1_LVDACK_SHIFT                  6
#define PMC_LVDSC1_LVDF_MASK                     0x80u
#define PMC_LVDSC1_LVDF_SHIFT                    7
/* LVDSC2 Bit Fields */
#define PMC_LVDSC2_LVWV_MASK                     0x3u
#define PMC_LVDSC2_LVWV_SHIFT                    0
#define PMC_LVDSC2_LVWV(x)                       (((uint8_t)(((uint8_t)(x))<<PMC_LVDSC2_LVWV_SHIFT))&PMC_LVDSC2_LVWV_MASK)
#define PMC_LVDSC2_LVWIE_MASK                    0x20u
#define PMC_LVDSC2_LVWIE_SHIFT                   5
#define PMC_LVDSC2_LVWACK_MASK                   0x40u
#define PMC_LVDSC2_LVWACK_SHIFT                  6
#define PMC_LVDSC2_LVWF_MASK                     0x80u
#define PMC_LVDSC2_LVWF_SHIFT                    7
/* REGSC Bit Fields */
#define PMC_REGSC_BGBE_MASK                      0x1u
#define PMC_REGSC_BGBE_SHIFT                     0
#define PMC_REGSC_BGBDS_MASK                     0x2u
#define PMC_REGSC_BGBDS_SHIFT                    1
#define PMC_REGSC_REGONS_MASK                    0x4u
#define PMC_REGSC_REGONS_SHIFT                   2
#define PMC_REGSC_ACKISO_MASK                    0x8u
#define PMC_REGSC_ACKISO_SHIFT                   3
#define PMC_REGSC_BGEN_MASK                      0x10u
#define PMC_REGSC_BGEN_SHIFT                     4

/*!
 * @}
 */ /* end of group PMC_Register_Masks */


/* PMC - Peripheral instance base addresses */
/** Peripheral PMC base pointer */
#define PMC_BASE_PTR                             ((PMC_MemMapPtr)0x4007D000u)
/** Array initializer of PMC peripheral base pointers */
#define PMC_BASE_PTRS                            { PMC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- PMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Register_Accessor_Macros PMC - Register accessor macros
 * @{
 */


/* PMC - Register instance definitions */
/* PMC */
#define PMC_LVDSC1                               PMC_LVDSC1_REG(PMC_BASE_PTR)
#define PMC_LVDSC2                               PMC_LVDSC2_REG(PMC_BASE_PTR)
#define PMC_REGSC                                PMC_REGSC_REG(PMC_BASE_PTR)

/*!
 * @}
 */ /* end of group PMC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group PMC_Peripheral */


/* ----------------------------------------------------------------------------
   -- PORT
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Peripheral PORT
 * @{
 */

/** PORT - Peripheral register structure */
typedef struct PORT_MemMap {
  uint32_t PCR[8];                                 /**< Pin Control Register n, array offset: 0x0, array step: 0x4 */
  uint8_t RESERVED_0[96];
  uint32_t GPCLR;                                  /**< Global Pin Control Low Register, offset: 0x80 */
  uint32_t GPCHR;                                  /**< Global Pin Control High Register, offset: 0x84 */
  uint8_t RESERVED_1[24];
  uint32_t ISFR;                                   /**< Interrupt Status Flag Register, offset: 0xA0 */
  uint8_t RESERVED_2[28];
  uint32_t DFER;                                   /**< Digital Filter Enable Register, offset: 0xC0 */
  uint32_t DFCR;                                   /**< Digital Filter Clock Register, offset: 0xC4 */
  uint32_t DFWR;                                   /**< Digital Filter Width Register, offset: 0xC8 */
} volatile *PORT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- PORT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Register_Accessor_Macros PORT - Register accessor macros
 * @{
 */


/* PORT - Register accessors */
#define PORT_PCR_REG(base,index)                 ((base)->PCR[index])
#define PORT_GPCLR_REG(base)                     ((base)->GPCLR)
#define PORT_GPCHR_REG(base)                     ((base)->GPCHR)
#define PORT_ISFR_REG(base)                      ((base)->ISFR)
#define PORT_DFER_REG(base)                      ((base)->DFER)
#define PORT_DFCR_REG(base)                      ((base)->DFCR)
#define PORT_DFWR_REG(base)                      ((base)->DFWR)

/*!
 * @}
 */ /* end of group PORT_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- PORT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Register_Masks PORT Register Masks
 * @{
 */

/* PCR Bit Fields */
#define PORT_PCR_PS_MASK                         0x1u
#define PORT_PCR_PS_SHIFT                        0
#define PORT_PCR_PE_MASK                         0x2u
#define PORT_PCR_PE_SHIFT                        1
#define PORT_PCR_SRE_MASK                        0x4u
#define PORT_PCR_SRE_SHIFT                       2
#define PORT_PCR_MUX_MASK                        0x700u
#define PORT_PCR_MUX_SHIFT                       8
#define PORT_PCR_MUX(x)                          (((uint32_t)(((uint32_t)(x))<<PORT_PCR_MUX_SHIFT))&PORT_PCR_MUX_MASK)
#define PORT_PCR_LK_MASK                         0x8000u
#define PORT_PCR_LK_SHIFT                        15
#define PORT_PCR_IRQC_MASK                       0xF0000u
#define PORT_PCR_IRQC_SHIFT                      16
#define PORT_PCR_IRQC(x)                         (((uint32_t)(((uint32_t)(x))<<PORT_PCR_IRQC_SHIFT))&PORT_PCR_IRQC_MASK)
#define PORT_PCR_ISF_MASK                        0x1000000u
#define PORT_PCR_ISF_SHIFT                       24
/* GPCLR Bit Fields */
#define PORT_GPCLR_GPWD_MASK                     0xFFFFu
#define PORT_GPCLR_GPWD_SHIFT                    0
#define PORT_GPCLR_GPWD(x)                       (((uint32_t)(((uint32_t)(x))<<PORT_GPCLR_GPWD_SHIFT))&PORT_GPCLR_GPWD_MASK)
#define PORT_GPCLR_GPWE_MASK                     0xFFFF0000u
#define PORT_GPCLR_GPWE_SHIFT                    16
#define PORT_GPCLR_GPWE(x)                       (((uint32_t)(((uint32_t)(x))<<PORT_GPCLR_GPWE_SHIFT))&PORT_GPCLR_GPWE_MASK)
/* GPCHR Bit Fields */
#define PORT_GPCHR_GPWD_MASK                     0xFFFFu
#define PORT_GPCHR_GPWD_SHIFT                    0
#define PORT_GPCHR_GPWD(x)                       (((uint32_t)(((uint32_t)(x))<<PORT_GPCHR_GPWD_SHIFT))&PORT_GPCHR_GPWD_MASK)
#define PORT_GPCHR_GPWE_MASK                     0xFFFF0000u
#define PORT_GPCHR_GPWE_SHIFT                    16
#define PORT_GPCHR_GPWE(x)                       (((uint32_t)(((uint32_t)(x))<<PORT_GPCHR_GPWE_SHIFT))&PORT_GPCHR_GPWE_MASK)
/* ISFR Bit Fields */
#define PORT_ISFR_ISF_MASK                       0xFFFFFFFFu
#define PORT_ISFR_ISF_SHIFT                      0
#define PORT_ISFR_ISF(x)                         (((uint32_t)(((uint32_t)(x))<<PORT_ISFR_ISF_SHIFT))&PORT_ISFR_ISF_MASK)
/* DFER Bit Fields */
#define PORT_DFER_DFE_MASK                       0xFFFFFFFFu
#define PORT_DFER_DFE_SHIFT                      0
#define PORT_DFER_DFE(x)                         (((uint32_t)(((uint32_t)(x))<<PORT_DFER_DFE_SHIFT))&PORT_DFER_DFE_MASK)
/* DFCR Bit Fields */
#define PORT_DFCR_CS_MASK                        0x1u
#define PORT_DFCR_CS_SHIFT                       0
/* DFWR Bit Fields */
#define PORT_DFWR_FILT_MASK                      0x1Fu
#define PORT_DFWR_FILT_SHIFT                     0
#define PORT_DFWR_FILT(x)                        (((uint32_t)(((uint32_t)(x))<<PORT_DFWR_FILT_SHIFT))&PORT_DFWR_FILT_MASK)

/*!
 * @}
 */ /* end of group PORT_Register_Masks */


/* PORT - Peripheral instance base addresses */
/** Peripheral PORTA base pointer */
#define PORTA_BASE_PTR                           ((PORT_MemMapPtr)0x40046000u)
/** Peripheral PORTB base pointer */
#define PORTB_BASE_PTR                           ((PORT_MemMapPtr)0x40047000u)
/** Peripheral PORTC base pointer */
#define PORTC_BASE_PTR                           ((PORT_MemMapPtr)0x40048000u)
/** Peripheral PORTD base pointer */
#define PORTD_BASE_PTR                           ((PORT_MemMapPtr)0x40049000u)
/** Peripheral PORTE base pointer */
#define PORTE_BASE_PTR                           ((PORT_MemMapPtr)0x4004A000u)
/** Peripheral PORTF base pointer */
#define PORTF_BASE_PTR                           ((PORT_MemMapPtr)0x4004B000u)
/** Peripheral PORTG base pointer */
#define PORTG_BASE_PTR                           ((PORT_MemMapPtr)0x4004C000u)
/** Peripheral PORTH base pointer */
#define PORTH_BASE_PTR                           ((PORT_MemMapPtr)0x4004D000u)
/** Peripheral PORTI base pointer */
#define PORTI_BASE_PTR                           ((PORT_MemMapPtr)0x4004E000u)
/** Array initializer of PORT peripheral base pointers */
#define PORT_BASE_PTRS                           { PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR, PORTF_BASE_PTR, PORTG_BASE_PTR, PORTH_BASE_PTR, PORTI_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- PORT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Register_Accessor_Macros PORT - Register accessor macros
 * @{
 */


/* PORT - Register instance definitions */
/* PORTA */
#define PORTA_PCR0                               PORT_PCR_REG(PORTA_BASE_PTR,0)
#define PORTA_PCR1                               PORT_PCR_REG(PORTA_BASE_PTR,1)
#define PORTA_PCR2                               PORT_PCR_REG(PORTA_BASE_PTR,2)
#define PORTA_PCR3                               PORT_PCR_REG(PORTA_BASE_PTR,3)
#define PORTA_PCR4                               PORT_PCR_REG(PORTA_BASE_PTR,4)
#define PORTA_PCR5                               PORT_PCR_REG(PORTA_BASE_PTR,5)
#define PORTA_PCR6                               PORT_PCR_REG(PORTA_BASE_PTR,6)
#define PORTA_PCR7                               PORT_PCR_REG(PORTA_BASE_PTR,7)
#define PORTA_GPCLR                              PORT_GPCLR_REG(PORTA_BASE_PTR)
#define PORTA_GPCHR                              PORT_GPCHR_REG(PORTA_BASE_PTR)
#define PORTA_ISFR                               PORT_ISFR_REG(PORTA_BASE_PTR)
#define PORTA_DFER                               PORT_DFER_REG(PORTA_BASE_PTR)
#define PORTA_DFCR                               PORT_DFCR_REG(PORTA_BASE_PTR)
#define PORTA_DFWR                               PORT_DFWR_REG(PORTA_BASE_PTR)
/* PORTB */
#define PORTB_PCR0                               PORT_PCR_REG(PORTB_BASE_PTR,0)
#define PORTB_PCR1                               PORT_PCR_REG(PORTB_BASE_PTR,1)
#define PORTB_PCR2                               PORT_PCR_REG(PORTB_BASE_PTR,2)
#define PORTB_PCR3                               PORT_PCR_REG(PORTB_BASE_PTR,3)
#define PORTB_PCR4                               PORT_PCR_REG(PORTB_BASE_PTR,4)
#define PORTB_PCR5                               PORT_PCR_REG(PORTB_BASE_PTR,5)
#define PORTB_PCR6                               PORT_PCR_REG(PORTB_BASE_PTR,6)
#define PORTB_PCR7                               PORT_PCR_REG(PORTB_BASE_PTR,7)
#define PORTB_GPCLR                              PORT_GPCLR_REG(PORTB_BASE_PTR)
#define PORTB_GPCHR                              PORT_GPCHR_REG(PORTB_BASE_PTR)
#define PORTB_ISFR                               PORT_ISFR_REG(PORTB_BASE_PTR)
#define PORTB_DFER                               PORT_DFER_REG(PORTB_BASE_PTR)
#define PORTB_DFCR                               PORT_DFCR_REG(PORTB_BASE_PTR)
#define PORTB_DFWR                               PORT_DFWR_REG(PORTB_BASE_PTR)
/* PORTC */
#define PORTC_PCR0                               PORT_PCR_REG(PORTC_BASE_PTR,0)
#define PORTC_PCR1                               PORT_PCR_REG(PORTC_BASE_PTR,1)
#define PORTC_PCR2                               PORT_PCR_REG(PORTC_BASE_PTR,2)
#define PORTC_PCR3                               PORT_PCR_REG(PORTC_BASE_PTR,3)
#define PORTC_PCR4                               PORT_PCR_REG(PORTC_BASE_PTR,4)
#define PORTC_PCR5                               PORT_PCR_REG(PORTC_BASE_PTR,5)
#define PORTC_PCR6                               PORT_PCR_REG(PORTC_BASE_PTR,6)
#define PORTC_PCR7                               PORT_PCR_REG(PORTC_BASE_PTR,7)
#define PORTC_GPCLR                              PORT_GPCLR_REG(PORTC_BASE_PTR)
#define PORTC_GPCHR                              PORT_GPCHR_REG(PORTC_BASE_PTR)
#define PORTC_ISFR                               PORT_ISFR_REG(PORTC_BASE_PTR)
#define PORTC_DFER                               PORT_DFER_REG(PORTC_BASE_PTR)
#define PORTC_DFCR                               PORT_DFCR_REG(PORTC_BASE_PTR)
#define PORTC_DFWR                               PORT_DFWR_REG(PORTC_BASE_PTR)
/* PORTD */
#define PORTD_PCR0                               PORT_PCR_REG(PORTD_BASE_PTR,0)
#define PORTD_PCR1                               PORT_PCR_REG(PORTD_BASE_PTR,1)
#define PORTD_PCR2                               PORT_PCR_REG(PORTD_BASE_PTR,2)
#define PORTD_PCR3                               PORT_PCR_REG(PORTD_BASE_PTR,3)
#define PORTD_PCR4                               PORT_PCR_REG(PORTD_BASE_PTR,4)
#define PORTD_PCR5                               PORT_PCR_REG(PORTD_BASE_PTR,5)
#define PORTD_PCR6                               PORT_PCR_REG(PORTD_BASE_PTR,6)
#define PORTD_PCR7                               PORT_PCR_REG(PORTD_BASE_PTR,7)
#define PORTD_GPCLR                              PORT_GPCLR_REG(PORTD_BASE_PTR)
#define PORTD_GPCHR                              PORT_GPCHR_REG(PORTD_BASE_PTR)
#define PORTD_ISFR                               PORT_ISFR_REG(PORTD_BASE_PTR)
#define PORTD_DFER                               PORT_DFER_REG(PORTD_BASE_PTR)
#define PORTD_DFCR                               PORT_DFCR_REG(PORTD_BASE_PTR)
#define PORTD_DFWR                               PORT_DFWR_REG(PORTD_BASE_PTR)
/* PORTE */
#define PORTE_PCR0                               PORT_PCR_REG(PORTE_BASE_PTR,0)
#define PORTE_PCR1                               PORT_PCR_REG(PORTE_BASE_PTR,1)
#define PORTE_PCR2                               PORT_PCR_REG(PORTE_BASE_PTR,2)
#define PORTE_PCR3                               PORT_PCR_REG(PORTE_BASE_PTR,3)
#define PORTE_PCR4                               PORT_PCR_REG(PORTE_BASE_PTR,4)
#define PORTE_PCR5                               PORT_PCR_REG(PORTE_BASE_PTR,5)
#define PORTE_PCR6                               PORT_PCR_REG(PORTE_BASE_PTR,6)
#define PORTE_PCR7                               PORT_PCR_REG(PORTE_BASE_PTR,7)
#define PORTE_GPCLR                              PORT_GPCLR_REG(PORTE_BASE_PTR)
#define PORTE_GPCHR                              PORT_GPCHR_REG(PORTE_BASE_PTR)
#define PORTE_ISFR                               PORT_ISFR_REG(PORTE_BASE_PTR)
#define PORTE_DFER                               PORT_DFER_REG(PORTE_BASE_PTR)
#define PORTE_DFCR                               PORT_DFCR_REG(PORTE_BASE_PTR)
#define PORTE_DFWR                               PORT_DFWR_REG(PORTE_BASE_PTR)
/* PORTF */
#define PORTF_PCR0                               PORT_PCR_REG(PORTF_BASE_PTR,0)
#define PORTF_PCR1                               PORT_PCR_REG(PORTF_BASE_PTR,1)
#define PORTF_PCR2                               PORT_PCR_REG(PORTF_BASE_PTR,2)
#define PORTF_PCR3                               PORT_PCR_REG(PORTF_BASE_PTR,3)
#define PORTF_PCR4                               PORT_PCR_REG(PORTF_BASE_PTR,4)
#define PORTF_PCR5                               PORT_PCR_REG(PORTF_BASE_PTR,5)
#define PORTF_PCR6                               PORT_PCR_REG(PORTF_BASE_PTR,6)
#define PORTF_PCR7                               PORT_PCR_REG(PORTF_BASE_PTR,7)
#define PORTF_GPCLR                              PORT_GPCLR_REG(PORTF_BASE_PTR)
#define PORTF_GPCHR                              PORT_GPCHR_REG(PORTF_BASE_PTR)
#define PORTF_ISFR                               PORT_ISFR_REG(PORTF_BASE_PTR)
#define PORTF_DFER                               PORT_DFER_REG(PORTF_BASE_PTR)
#define PORTF_DFCR                               PORT_DFCR_REG(PORTF_BASE_PTR)
#define PORTF_DFWR                               PORT_DFWR_REG(PORTF_BASE_PTR)
/* PORTG */
#define PORTG_PCR0                               PORT_PCR_REG(PORTG_BASE_PTR,0)
#define PORTG_PCR1                               PORT_PCR_REG(PORTG_BASE_PTR,1)
#define PORTG_PCR2                               PORT_PCR_REG(PORTG_BASE_PTR,2)
#define PORTG_PCR3                               PORT_PCR_REG(PORTG_BASE_PTR,3)
#define PORTG_PCR4                               PORT_PCR_REG(PORTG_BASE_PTR,4)
#define PORTG_PCR5                               PORT_PCR_REG(PORTG_BASE_PTR,5)
#define PORTG_PCR6                               PORT_PCR_REG(PORTG_BASE_PTR,6)
#define PORTG_PCR7                               PORT_PCR_REG(PORTG_BASE_PTR,7)
#define PORTG_GPCLR                              PORT_GPCLR_REG(PORTG_BASE_PTR)
#define PORTG_GPCHR                              PORT_GPCHR_REG(PORTG_BASE_PTR)
#define PORTG_ISFR                               PORT_ISFR_REG(PORTG_BASE_PTR)
#define PORTG_DFER                               PORT_DFER_REG(PORTG_BASE_PTR)
#define PORTG_DFCR                               PORT_DFCR_REG(PORTG_BASE_PTR)
#define PORTG_DFWR                               PORT_DFWR_REG(PORTG_BASE_PTR)
/* PORTH */
#define PORTH_PCR0                               PORT_PCR_REG(PORTH_BASE_PTR,0)
#define PORTH_PCR1                               PORT_PCR_REG(PORTH_BASE_PTR,1)
#define PORTH_PCR2                               PORT_PCR_REG(PORTH_BASE_PTR,2)
#define PORTH_PCR3                               PORT_PCR_REG(PORTH_BASE_PTR,3)
#define PORTH_PCR4                               PORT_PCR_REG(PORTH_BASE_PTR,4)
#define PORTH_PCR5                               PORT_PCR_REG(PORTH_BASE_PTR,5)
#define PORTH_PCR6                               PORT_PCR_REG(PORTH_BASE_PTR,6)
#define PORTH_PCR7                               PORT_PCR_REG(PORTH_BASE_PTR,7)
#define PORTH_GPCLR                              PORT_GPCLR_REG(PORTH_BASE_PTR)
#define PORTH_GPCHR                              PORT_GPCHR_REG(PORTH_BASE_PTR)
#define PORTH_ISFR                               PORT_ISFR_REG(PORTH_BASE_PTR)
#define PORTH_DFER                               PORT_DFER_REG(PORTH_BASE_PTR)
#define PORTH_DFCR                               PORT_DFCR_REG(PORTH_BASE_PTR)
#define PORTH_DFWR                               PORT_DFWR_REG(PORTH_BASE_PTR)
/* PORTI */
#define PORTI_PCR0                               PORT_PCR_REG(PORTI_BASE_PTR,0)
#define PORTI_PCR1                               PORT_PCR_REG(PORTI_BASE_PTR,1)
#define PORTI_PCR2                               PORT_PCR_REG(PORTI_BASE_PTR,2)
#define PORTI_PCR3                               PORT_PCR_REG(PORTI_BASE_PTR,3)
#define PORTI_PCR4                               PORT_PCR_REG(PORTI_BASE_PTR,4)
#define PORTI_PCR5                               PORT_PCR_REG(PORTI_BASE_PTR,5)
#define PORTI_PCR6                               PORT_PCR_REG(PORTI_BASE_PTR,6)
#define PORTI_PCR7                               PORT_PCR_REG(PORTI_BASE_PTR,7)
#define PORTI_GPCLR                              PORT_GPCLR_REG(PORTI_BASE_PTR)
#define PORTI_GPCHR                              PORT_GPCHR_REG(PORTI_BASE_PTR)
#define PORTI_ISFR                               PORT_ISFR_REG(PORTI_BASE_PTR)
#define PORTI_DFER                               PORT_DFER_REG(PORTI_BASE_PTR)
#define PORTI_DFCR                               PORT_DFCR_REG(PORTI_BASE_PTR)
#define PORTI_DFWR                               PORT_DFWR_REG(PORTI_BASE_PTR)

/* PORT - Register array accessors */
#define PORTA_PCR(index)                         PORT_PCR_REG(PORTA_BASE_PTR,index)
#define PORTB_PCR(index)                         PORT_PCR_REG(PORTB_BASE_PTR,index)
#define PORTC_PCR(index)                         PORT_PCR_REG(PORTC_BASE_PTR,index)
#define PORTD_PCR(index)                         PORT_PCR_REG(PORTD_BASE_PTR,index)
#define PORTE_PCR(index)                         PORT_PCR_REG(PORTE_BASE_PTR,index)
#define PORTF_PCR(index)                         PORT_PCR_REG(PORTF_BASE_PTR,index)
#define PORTG_PCR(index)                         PORT_PCR_REG(PORTG_BASE_PTR,index)
#define PORTH_PCR(index)                         PORT_PCR_REG(PORTH_BASE_PTR,index)
#define PORTI_PCR(index)                         PORT_PCR_REG(PORTI_BASE_PTR,index)

/*!
 * @}
 */ /* end of group PORT_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group PORT_Peripheral */


/* ----------------------------------------------------------------------------
   -- RCM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Peripheral RCM
 * @{
 */

/** RCM - Peripheral register structure */
typedef struct RCM_MemMap {
  uint8_t SRS0;                                    /**< System Reset Status Register 0, offset: 0x0 */
  uint8_t SRS1;                                    /**< System Reset Status Register 1, offset: 0x1 */
  uint8_t RESERVED_0[2];
  uint8_t RPFC;                                    /**< Reset Pin Filter Control register, offset: 0x4 */
  uint8_t RPFW;                                    /**< Reset Pin Filter Width register, offset: 0x5 */
} volatile *RCM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- RCM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Register_Accessor_Macros RCM - Register accessor macros
 * @{
 */


/* RCM - Register accessors */
#define RCM_SRS0_REG(base)                       ((base)->SRS0)
#define RCM_SRS1_REG(base)                       ((base)->SRS1)
#define RCM_RPFC_REG(base)                       ((base)->RPFC)
#define RCM_RPFW_REG(base)                       ((base)->RPFW)

/*!
 * @}
 */ /* end of group RCM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- RCM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Register_Masks RCM Register Masks
 * @{
 */

/* SRS0 Bit Fields */
#define RCM_SRS0_WAKEUP_MASK                     0x1u
#define RCM_SRS0_WAKEUP_SHIFT                    0
#define RCM_SRS0_LVD_MASK                        0x2u
#define RCM_SRS0_LVD_SHIFT                       1
#define RCM_SRS0_LOC_MASK                        0x4u
#define RCM_SRS0_LOC_SHIFT                       2
#define RCM_SRS0_LOL_MASK                        0x8u
#define RCM_SRS0_LOL_SHIFT                       3
#define RCM_SRS0_WDOG_MASK                       0x20u
#define RCM_SRS0_WDOG_SHIFT                      5
#define RCM_SRS0_PIN_MASK                        0x40u
#define RCM_SRS0_PIN_SHIFT                       6
#define RCM_SRS0_POR_MASK                        0x80u
#define RCM_SRS0_POR_SHIFT                       7
/* SRS1 Bit Fields */
#define RCM_SRS1_LOCKUP_MASK                     0x2u
#define RCM_SRS1_LOCKUP_SHIFT                    1
#define RCM_SRS1_SW_MASK                         0x4u
#define RCM_SRS1_SW_SHIFT                        2
#define RCM_SRS1_MDM_AP_MASK                     0x8u
#define RCM_SRS1_MDM_AP_SHIFT                    3
#define RCM_SRS1_SACKERR_MASK                    0x20u
#define RCM_SRS1_SACKERR_SHIFT                   5
/* RPFC Bit Fields */
#define RCM_RPFC_RSTFLTSRW_MASK                  0x3u
#define RCM_RPFC_RSTFLTSRW_SHIFT                 0
#define RCM_RPFC_RSTFLTSRW(x)                    (((uint8_t)(((uint8_t)(x))<<RCM_RPFC_RSTFLTSRW_SHIFT))&RCM_RPFC_RSTFLTSRW_MASK)
#define RCM_RPFC_RSTFLTSS_MASK                   0x4u
#define RCM_RPFC_RSTFLTSS_SHIFT                  2
/* RPFW Bit Fields */
#define RCM_RPFW_RSTFLTSEL_MASK                  0x1Fu
#define RCM_RPFW_RSTFLTSEL_SHIFT                 0
#define RCM_RPFW_RSTFLTSEL(x)                    (((uint8_t)(((uint8_t)(x))<<RCM_RPFW_RSTFLTSEL_SHIFT))&RCM_RPFW_RSTFLTSEL_MASK)

/*!
 * @}
 */ /* end of group RCM_Register_Masks */


/* RCM - Peripheral instance base addresses */
/** Peripheral RCM base pointer */
#define RCM_BASE_PTR                             ((RCM_MemMapPtr)0x4007B000u)
/** Array initializer of RCM peripheral base pointers */
#define RCM_BASE_PTRS                            { RCM_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- RCM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Register_Accessor_Macros RCM - Register accessor macros
 * @{
 */


/* RCM - Register instance definitions */
/* RCM */
#define RCM_SRS0                                 RCM_SRS0_REG(RCM_BASE_PTR)
#define RCM_SRS1                                 RCM_SRS1_REG(RCM_BASE_PTR)
#define RCM_RPFC                                 RCM_RPFC_REG(RCM_BASE_PTR)
#define RCM_RPFW                                 RCM_RPFW_REG(RCM_BASE_PTR)

/*!
 * @}
 */ /* end of group RCM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group RCM_Peripheral */


/* ----------------------------------------------------------------------------
   -- RNG
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RNG_Peripheral RNG
 * @{
 */

/** RNG - Peripheral register structure */
typedef struct RNG_MemMap {
  uint32_t CR;                                     /**< RNGA Control Register, offset: 0x0 */
  uint32_t SR;                                     /**< RNGA Status Register, offset: 0x4 */
  uint32_t ER;                                     /**< RNGA Entropy Register, offset: 0x8 */
  uint32_t OR;                                     /**< RNGA Output Register, offset: 0xC */
} volatile *RNG_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- RNG - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RNG_Register_Accessor_Macros RNG - Register accessor macros
 * @{
 */


/* RNG - Register accessors */
#define RNG_CR_REG(base)                         ((base)->CR)
#define RNG_SR_REG(base)                         ((base)->SR)
#define RNG_ER_REG(base)                         ((base)->ER)
#define RNG_OR_REG(base)                         ((base)->OR)

/*!
 * @}
 */ /* end of group RNG_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- RNG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RNG_Register_Masks RNG Register Masks
 * @{
 */

/* CR Bit Fields */
#define RNG_CR_GO_MASK                           0x1u
#define RNG_CR_GO_SHIFT                          0
#define RNG_CR_HA_MASK                           0x2u
#define RNG_CR_HA_SHIFT                          1
#define RNG_CR_INTM_MASK                         0x4u
#define RNG_CR_INTM_SHIFT                        2
#define RNG_CR_CLRI_MASK                         0x8u
#define RNG_CR_CLRI_SHIFT                        3
#define RNG_CR_SLP_MASK                          0x10u
#define RNG_CR_SLP_SHIFT                         4
/* SR Bit Fields */
#define RNG_SR_SECV_MASK                         0x1u
#define RNG_SR_SECV_SHIFT                        0
#define RNG_SR_LRS_MASK                          0x2u
#define RNG_SR_LRS_SHIFT                         1
#define RNG_SR_ORU_MASK                          0x4u
#define RNG_SR_ORU_SHIFT                         2
#define RNG_SR_ERRI_MASK                         0x8u
#define RNG_SR_ERRI_SHIFT                        3
#define RNG_SR_SLP_MASK                          0x10u
#define RNG_SR_SLP_SHIFT                         4
#define RNG_SR_OREG_LVL_MASK                     0xFF00u
#define RNG_SR_OREG_LVL_SHIFT                    8
#define RNG_SR_OREG_LVL(x)                       (((uint32_t)(((uint32_t)(x))<<RNG_SR_OREG_LVL_SHIFT))&RNG_SR_OREG_LVL_MASK)
#define RNG_SR_OREG_SIZE_MASK                    0xFF0000u
#define RNG_SR_OREG_SIZE_SHIFT                   16
#define RNG_SR_OREG_SIZE(x)                      (((uint32_t)(((uint32_t)(x))<<RNG_SR_OREG_SIZE_SHIFT))&RNG_SR_OREG_SIZE_MASK)
/* ER Bit Fields */
#define RNG_ER_EXT_ENT_MASK                      0xFFFFFFFFu
#define RNG_ER_EXT_ENT_SHIFT                     0
#define RNG_ER_EXT_ENT(x)                        (((uint32_t)(((uint32_t)(x))<<RNG_ER_EXT_ENT_SHIFT))&RNG_ER_EXT_ENT_MASK)
/* OR Bit Fields */
#define RNG_OR_RANDOUT_MASK                      0xFFFFFFFFu
#define RNG_OR_RANDOUT_SHIFT                     0
#define RNG_OR_RANDOUT(x)                        (((uint32_t)(((uint32_t)(x))<<RNG_OR_RANDOUT_SHIFT))&RNG_OR_RANDOUT_MASK)

/*!
 * @}
 */ /* end of group RNG_Register_Masks */


/* RNG - Peripheral instance base addresses */
/** Peripheral RNG base pointer */
#define RNG_BASE_PTR                             ((RNG_MemMapPtr)0x40029000u)
/** Array initializer of RNG peripheral base pointers */
#define RNG_BASE_PTRS                            { RNG_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- RNG - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RNG_Register_Accessor_Macros RNG - Register accessor macros
 * @{
 */


/* RNG - Register instance definitions */
/* RNG */
#define RNG_CR                                   RNG_CR_REG(RNG_BASE_PTR)
#define RNG_SR                                   RNG_SR_REG(RNG_BASE_PTR)
#define RNG_ER                                   RNG_ER_REG(RNG_BASE_PTR)
#define RNG_OR                                   RNG_OR_REG(RNG_BASE_PTR)

/*!
 * @}
 */ /* end of group RNG_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group RNG_Peripheral */


/* ----------------------------------------------------------------------------
   -- ROM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Peripheral ROM
 * @{
 */

/** ROM - Peripheral register structure */
typedef struct ROM_MemMap {
  uint32_t ENTRY[3];                               /**< Entry, array offset: 0x0, array step: 0x4 */
  uint32_t TABLEMARK;                              /**< End of Table Marker Register, offset: 0xC */
  uint8_t RESERVED_0[4028];
  uint32_t SYSACCESS;                              /**< System Access Register, offset: 0xFCC */
  uint32_t PERIPHID4;                              /**< Peripheral ID Register, offset: 0xFD0 */
  uint32_t PERIPHID5;                              /**< Peripheral ID Register, offset: 0xFD4 */
  uint32_t PERIPHID6;                              /**< Peripheral ID Register, offset: 0xFD8 */
  uint32_t PERIPHID7;                              /**< Peripheral ID Register, offset: 0xFDC */
  uint32_t PERIPHID0;                              /**< Peripheral ID Register, offset: 0xFE0 */
  uint32_t PERIPHID1;                              /**< Peripheral ID Register, offset: 0xFE4 */
  uint32_t PERIPHID2;                              /**< Peripheral ID Register, offset: 0xFE8 */
  uint32_t PERIPHID3;                              /**< Peripheral ID Register, offset: 0xFEC */
  uint32_t COMPID[4];                              /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} volatile *ROM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- ROM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Register_Accessor_Macros ROM - Register accessor macros
 * @{
 */


/* ROM - Register accessors */
#define ROM_ENTRY_REG(base,index)                ((base)->ENTRY[index])
#define ROM_TABLEMARK_REG(base)                  ((base)->TABLEMARK)
#define ROM_SYSACCESS_REG(base)                  ((base)->SYSACCESS)
#define ROM_PERIPHID4_REG(base)                  ((base)->PERIPHID4)
#define ROM_PERIPHID5_REG(base)                  ((base)->PERIPHID5)
#define ROM_PERIPHID6_REG(base)                  ((base)->PERIPHID6)
#define ROM_PERIPHID7_REG(base)                  ((base)->PERIPHID7)
#define ROM_PERIPHID0_REG(base)                  ((base)->PERIPHID0)
#define ROM_PERIPHID1_REG(base)                  ((base)->PERIPHID1)
#define ROM_PERIPHID2_REG(base)                  ((base)->PERIPHID2)
#define ROM_PERIPHID3_REG(base)                  ((base)->PERIPHID3)
#define ROM_COMPID_REG(base,index)               ((base)->COMPID[index])

/*!
 * @}
 */ /* end of group ROM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- ROM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Register_Masks ROM Register Masks
 * @{
 */

/* ENTRY Bit Fields */
#define ROM_ENTRY_ENTRY_MASK                     0xFFFFFFFFu
#define ROM_ENTRY_ENTRY_SHIFT                    0
#define ROM_ENTRY_ENTRY(x)                       (((uint32_t)(((uint32_t)(x))<<ROM_ENTRY_ENTRY_SHIFT))&ROM_ENTRY_ENTRY_MASK)
/* TABLEMARK Bit Fields */
#define ROM_TABLEMARK_MARK_MASK                  0xFFFFFFFFu
#define ROM_TABLEMARK_MARK_SHIFT                 0
#define ROM_TABLEMARK_MARK(x)                    (((uint32_t)(((uint32_t)(x))<<ROM_TABLEMARK_MARK_SHIFT))&ROM_TABLEMARK_MARK_MASK)
/* SYSACCESS Bit Fields */
#define ROM_SYSACCESS_SYSACCESS_MASK             0xFFFFFFFFu
#define ROM_SYSACCESS_SYSACCESS_SHIFT            0
#define ROM_SYSACCESS_SYSACCESS(x)               (((uint32_t)(((uint32_t)(x))<<ROM_SYSACCESS_SYSACCESS_SHIFT))&ROM_SYSACCESS_SYSACCESS_MASK)
/* PERIPHID4 Bit Fields */
#define ROM_PERIPHID4_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID4_PERIPHID_SHIFT             0
#define ROM_PERIPHID4_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID4_PERIPHID_SHIFT))&ROM_PERIPHID4_PERIPHID_MASK)
/* PERIPHID5 Bit Fields */
#define ROM_PERIPHID5_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID5_PERIPHID_SHIFT             0
#define ROM_PERIPHID5_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID5_PERIPHID_SHIFT))&ROM_PERIPHID5_PERIPHID_MASK)
/* PERIPHID6 Bit Fields */
#define ROM_PERIPHID6_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID6_PERIPHID_SHIFT             0
#define ROM_PERIPHID6_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID6_PERIPHID_SHIFT))&ROM_PERIPHID6_PERIPHID_MASK)
/* PERIPHID7 Bit Fields */
#define ROM_PERIPHID7_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID7_PERIPHID_SHIFT             0
#define ROM_PERIPHID7_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID7_PERIPHID_SHIFT))&ROM_PERIPHID7_PERIPHID_MASK)
/* PERIPHID0 Bit Fields */
#define ROM_PERIPHID0_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID0_PERIPHID_SHIFT             0
#define ROM_PERIPHID0_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID0_PERIPHID_SHIFT))&ROM_PERIPHID0_PERIPHID_MASK)
/* PERIPHID1 Bit Fields */
#define ROM_PERIPHID1_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID1_PERIPHID_SHIFT             0
#define ROM_PERIPHID1_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID1_PERIPHID_SHIFT))&ROM_PERIPHID1_PERIPHID_MASK)
/* PERIPHID2 Bit Fields */
#define ROM_PERIPHID2_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID2_PERIPHID_SHIFT             0
#define ROM_PERIPHID2_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID2_PERIPHID_SHIFT))&ROM_PERIPHID2_PERIPHID_MASK)
/* PERIPHID3 Bit Fields */
#define ROM_PERIPHID3_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID3_PERIPHID_SHIFT             0
#define ROM_PERIPHID3_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID3_PERIPHID_SHIFT))&ROM_PERIPHID3_PERIPHID_MASK)
/* COMPID Bit Fields */
#define ROM_COMPID_COMPID_MASK                   0xFFFFFFFFu
#define ROM_COMPID_COMPID_SHIFT                  0
#define ROM_COMPID_COMPID(x)                     (((uint32_t)(((uint32_t)(x))<<ROM_COMPID_COMPID_SHIFT))&ROM_COMPID_COMPID_MASK)

/*!
 * @}
 */ /* end of group ROM_Register_Masks */


/* ROM - Peripheral instance base addresses */
/** Peripheral ROM base pointer */
#define ROM_BASE_PTR                             ((ROM_MemMapPtr)0xF0002000u)
/** Array initializer of ROM peripheral base pointers */
#define ROM_BASE_PTRS                            { ROM_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- ROM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Register_Accessor_Macros ROM - Register accessor macros
 * @{
 */


/* ROM - Register instance definitions */
/* ROM */
#define ROM_ENTRY0                               ROM_ENTRY_REG(ROM_BASE_PTR,0)
#define ROM_ENTRY1                               ROM_ENTRY_REG(ROM_BASE_PTR,1)
#define ROM_ENTRY2                               ROM_ENTRY_REG(ROM_BASE_PTR,2)
#define ROM_TABLEMARK                            ROM_TABLEMARK_REG(ROM_BASE_PTR)
#define ROM_SYSACCESS                            ROM_SYSACCESS_REG(ROM_BASE_PTR)
#define ROM_PERIPHID4                            ROM_PERIPHID4_REG(ROM_BASE_PTR)
#define ROM_PERIPHID5                            ROM_PERIPHID5_REG(ROM_BASE_PTR)
#define ROM_PERIPHID6                            ROM_PERIPHID6_REG(ROM_BASE_PTR)
#define ROM_PERIPHID7                            ROM_PERIPHID7_REG(ROM_BASE_PTR)
#define ROM_PERIPHID0                            ROM_PERIPHID0_REG(ROM_BASE_PTR)
#define ROM_PERIPHID1                            ROM_PERIPHID1_REG(ROM_BASE_PTR)
#define ROM_PERIPHID2                            ROM_PERIPHID2_REG(ROM_BASE_PTR)
#define ROM_PERIPHID3                            ROM_PERIPHID3_REG(ROM_BASE_PTR)
#define ROM_COMPID0                              ROM_COMPID_REG(ROM_BASE_PTR,0)
#define ROM_COMPID1                              ROM_COMPID_REG(ROM_BASE_PTR,1)
#define ROM_COMPID2                              ROM_COMPID_REG(ROM_BASE_PTR,2)
#define ROM_COMPID3                              ROM_COMPID_REG(ROM_BASE_PTR,3)

/* ROM - Register array accessors */
#define ROM_ENTRY(index)                         ROM_ENTRY_REG(ROM_BASE_PTR,index)
#define ROM_COMPID(index)                        ROM_COMPID_REG(ROM_BASE_PTR,index)

/*!
 * @}
 */ /* end of group ROM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group ROM_Peripheral */


/* ----------------------------------------------------------------------------
   -- RTC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTC_Peripheral RTC
 * @{
 */

/** RTC - Peripheral register structure */
typedef struct RTC_MemMap {
  uint16_t YEARMON;                                /**< RTC Year and Month Counters Register, offset: 0x0 */
  uint16_t DAYS;                                   /**< RTC Days and Day-of-Week Counters Register, offset: 0x2 */
  uint16_t HOURMIN;                                /**< RTC Hours and Minutes Counters Register, offset: 0x4 */
  uint16_t SECONDS;                                /**< RTC Seconds Counters Register, offset: 0x6 */
  uint16_t ALM_YEARMON;                            /**< RTC Year and Months Alarm Register, offset: 0x8 */
  uint16_t ALM_DAYS;                               /**< RTC Days Alarm Register, offset: 0xA */
  uint16_t ALM_HOURMIN;                            /**< RTC Hours and Minutes Alarm Register, offset: 0xC */
  uint16_t ALM_SECONDS;                            /**< RTC Seconds Alarm Register, offset: 0xE */
  uint16_t CTRL;                                   /**< RTC Control Register, offset: 0x10 */
  uint16_t STATUS;                                 /**< RTC Status Register, offset: 0x12 */
  uint16_t ISR;                                    /**< RTC Interrupt Status Register, offset: 0x14 */
  uint16_t IER;                                    /**< RTC Interrupt Enable Register, offset: 0x16 */
  uint8_t RESERVED_0[8];
  uint16_t GP_DATA_REG;                            /**< RTC General Purpose Data Register, offset: 0x20 */
  uint16_t DST_HOUR;                               /**< RTC Daylight Saving Hour Register, offset: 0x22 */
  uint16_t DST_MONTH;                              /**< RTC Daylight Saving Month Register, offset: 0x24 */
  uint16_t DST_DAY;                                /**< RTC Daylight Saving Day Register, offset: 0x26 */
  uint16_t COMPEN;                                 /**< RTC Compensation Register, offset: 0x28 */
  uint8_t RESERVED_1[2];
  uint16_t TAMPER_DIRECTION;                       /**< Tamper Direction Register, offset: 0x2C */
  uint16_t TAMPER_QSCR;                            /**< Tamper Queue Status and Control Register, offset: 0x2E */
  uint8_t RESERVED_2[2];
  uint16_t TAMPER_SCR;                             /**< RTC Tamper Status and Control Register, offset: 0x32 */
  uint16_t FILTER01_CFG;                           /**< RTC Tamper 0 1 Filter Configuration Register, offset: 0x34 */
  uint16_t FILTER2_CFG;                            /**< RTC Tamper 2 Filter Configuration Register, offset: 0x36 */
  uint8_t RESERVED_3[8];
  uint16_t TAMPER_QUEUE;                           /**< Tamper Queue Register, offset: 0x40 */
  uint16_t CTRL2;                                  /**< RTC Control 2 Register, offset: 0x42 */
} volatile *RTC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- RTC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTC_Register_Accessor_Macros RTC - Register accessor macros
 * @{
 */


/* RTC - Register accessors */
#define RTC_YEARMON_REG(base)                    ((base)->YEARMON)
#define RTC_DAYS_REG(base)                       ((base)->DAYS)
#define RTC_HOURMIN_REG(base)                    ((base)->HOURMIN)
#define RTC_SECONDS_REG(base)                    ((base)->SECONDS)
#define RTC_ALM_YEARMON_REG(base)                ((base)->ALM_YEARMON)
#define RTC_ALM_DAYS_REG(base)                   ((base)->ALM_DAYS)
#define RTC_ALM_HOURMIN_REG(base)                ((base)->ALM_HOURMIN)
#define RTC_ALM_SECONDS_REG(base)                ((base)->ALM_SECONDS)
#define RTC_CTRL_REG(base)                       ((base)->CTRL)
#define RTC_STATUS_REG(base)                     ((base)->STATUS)
#define RTC_ISR_REG(base)                        ((base)->ISR)
#define RTC_IER_REG(base)                        ((base)->IER)
#define RTC_GP_DATA_REG_REG(base)                ((base)->GP_DATA_REG)
#define RTC_DST_HOUR_REG(base)                   ((base)->DST_HOUR)
#define RTC_DST_MONTH_REG(base)                  ((base)->DST_MONTH)
#define RTC_DST_DAY_REG(base)                    ((base)->DST_DAY)
#define RTC_COMPEN_REG(base)                     ((base)->COMPEN)
#define RTC_TAMPER_DIRECTION_REG(base)           ((base)->TAMPER_DIRECTION)
#define RTC_TAMPER_QSCR_REG(base)                ((base)->TAMPER_QSCR)
#define RTC_TAMPER_SCR_REG(base)                 ((base)->TAMPER_SCR)
#define RTC_FILTER01_CFG_REG(base)               ((base)->FILTER01_CFG)
#define RTC_FILTER2_CFG_REG(base)                ((base)->FILTER2_CFG)
#define RTC_TAMPER_QUEUE_REG(base)               ((base)->TAMPER_QUEUE)
#define RTC_CTRL2_REG(base)                      ((base)->CTRL2)

/*!
 * @}
 */ /* end of group RTC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- RTC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTC_Register_Masks RTC Register Masks
 * @{
 */

/* YEARMON Bit Fields */
#define RTC_YEARMON_MON_CNT_MASK                 0xFu
#define RTC_YEARMON_MON_CNT_SHIFT                0
#define RTC_YEARMON_MON_CNT(x)                   (((uint16_t)(((uint16_t)(x))<<RTC_YEARMON_MON_CNT_SHIFT))&RTC_YEARMON_MON_CNT_MASK)
#define RTC_YEARMON_YROFST_MASK                  0xFF00u
#define RTC_YEARMON_YROFST_SHIFT                 8
#define RTC_YEARMON_YROFST(x)                    (((uint16_t)(((uint16_t)(x))<<RTC_YEARMON_YROFST_SHIFT))&RTC_YEARMON_YROFST_MASK)
/* DAYS Bit Fields */
#define RTC_DAYS_DAY_CNT_MASK                    0x1Fu
#define RTC_DAYS_DAY_CNT_SHIFT                   0
#define RTC_DAYS_DAY_CNT(x)                      (((uint16_t)(((uint16_t)(x))<<RTC_DAYS_DAY_CNT_SHIFT))&RTC_DAYS_DAY_CNT_MASK)
#define RTC_DAYS_DOW_MASK                        0x700u
#define RTC_DAYS_DOW_SHIFT                       8
#define RTC_DAYS_DOW(x)                          (((uint16_t)(((uint16_t)(x))<<RTC_DAYS_DOW_SHIFT))&RTC_DAYS_DOW_MASK)
/* HOURMIN Bit Fields */
#define RTC_HOURMIN_MIN_CNT_MASK                 0x3Fu
#define RTC_HOURMIN_MIN_CNT_SHIFT                0
#define RTC_HOURMIN_MIN_CNT(x)                   (((uint16_t)(((uint16_t)(x))<<RTC_HOURMIN_MIN_CNT_SHIFT))&RTC_HOURMIN_MIN_CNT_MASK)
#define RTC_HOURMIN_HOUR_CNT_MASK                0x1F00u
#define RTC_HOURMIN_HOUR_CNT_SHIFT               8
#define RTC_HOURMIN_HOUR_CNT(x)                  (((uint16_t)(((uint16_t)(x))<<RTC_HOURMIN_HOUR_CNT_SHIFT))&RTC_HOURMIN_HOUR_CNT_MASK)
/* SECONDS Bit Fields */
#define RTC_SECONDS_SEC_CNT_MASK                 0x3Fu
#define RTC_SECONDS_SEC_CNT_SHIFT                0
#define RTC_SECONDS_SEC_CNT(x)                   (((uint16_t)(((uint16_t)(x))<<RTC_SECONDS_SEC_CNT_SHIFT))&RTC_SECONDS_SEC_CNT_MASK)
/* ALM_YEARMON Bit Fields */
#define RTC_ALM_YEARMON_ALM_MON_MASK             0xFu
#define RTC_ALM_YEARMON_ALM_MON_SHIFT            0
#define RTC_ALM_YEARMON_ALM_MON(x)               (((uint16_t)(((uint16_t)(x))<<RTC_ALM_YEARMON_ALM_MON_SHIFT))&RTC_ALM_YEARMON_ALM_MON_MASK)
#define RTC_ALM_YEARMON_ALM_YEAR_MASK            0xFF00u
#define RTC_ALM_YEARMON_ALM_YEAR_SHIFT           8
#define RTC_ALM_YEARMON_ALM_YEAR(x)              (((uint16_t)(((uint16_t)(x))<<RTC_ALM_YEARMON_ALM_YEAR_SHIFT))&RTC_ALM_YEARMON_ALM_YEAR_MASK)
/* ALM_DAYS Bit Fields */
#define RTC_ALM_DAYS_ALM_DAY_MASK                0x1Fu
#define RTC_ALM_DAYS_ALM_DAY_SHIFT               0
#define RTC_ALM_DAYS_ALM_DAY(x)                  (((uint16_t)(((uint16_t)(x))<<RTC_ALM_DAYS_ALM_DAY_SHIFT))&RTC_ALM_DAYS_ALM_DAY_MASK)
/* ALM_HOURMIN Bit Fields */
#define RTC_ALM_HOURMIN_ALM_MIN_MASK             0x3Fu
#define RTC_ALM_HOURMIN_ALM_MIN_SHIFT            0
#define RTC_ALM_HOURMIN_ALM_MIN(x)               (((uint16_t)(((uint16_t)(x))<<RTC_ALM_HOURMIN_ALM_MIN_SHIFT))&RTC_ALM_HOURMIN_ALM_MIN_MASK)
#define RTC_ALM_HOURMIN_ALM_HOUR_MASK            0x1F00u
#define RTC_ALM_HOURMIN_ALM_HOUR_SHIFT           8
#define RTC_ALM_HOURMIN_ALM_HOUR(x)              (((uint16_t)(((uint16_t)(x))<<RTC_ALM_HOURMIN_ALM_HOUR_SHIFT))&RTC_ALM_HOURMIN_ALM_HOUR_MASK)
/* ALM_SECONDS Bit Fields */
#define RTC_ALM_SECONDS_ALM_SEC_MASK             0x3Fu
#define RTC_ALM_SECONDS_ALM_SEC_SHIFT            0
#define RTC_ALM_SECONDS_ALM_SEC(x)               (((uint16_t)(((uint16_t)(x))<<RTC_ALM_SECONDS_ALM_SEC_SHIFT))&RTC_ALM_SECONDS_ALM_SEC_MASK)
#define RTC_ALM_SECONDS_DEC_SEC_MASK             0x100u
#define RTC_ALM_SECONDS_DEC_SEC_SHIFT            8
#define RTC_ALM_SECONDS_INC_SEC_MASK             0x200u
#define RTC_ALM_SECONDS_INC_SEC_SHIFT            9
/* CTRL Bit Fields */
#define RTC_CTRL_FINEEN_MASK                     0x1u
#define RTC_CTRL_FINEEN_SHIFT                    0
#define RTC_CTRL_COMP_EN_MASK                    0x2u
#define RTC_CTRL_COMP_EN_SHIFT                   1
#define RTC_CTRL_ALM_MATCH_MASK                  0xCu
#define RTC_CTRL_ALM_MATCH_SHIFT                 2
#define RTC_CTRL_ALM_MATCH(x)                    (((uint16_t)(((uint16_t)(x))<<RTC_CTRL_ALM_MATCH_SHIFT))&RTC_CTRL_ALM_MATCH_MASK)
#define RTC_CTRL_TIMER_STB_MASK_MASK             0x10u
#define RTC_CTRL_TIMER_STB_MASK_SHIFT            4
#define RTC_CTRL_DST_EN_MASK                     0x40u
#define RTC_CTRL_DST_EN_SHIFT                    6
#define RTC_CTRL_SWR_MASK                        0x100u
#define RTC_CTRL_SWR_SHIFT                       8
#define RTC_CTRL_CLKOUT_MASK                     0x6000u
#define RTC_CTRL_CLKOUT_SHIFT                    13
#define RTC_CTRL_CLKOUT(x)                       (((uint16_t)(((uint16_t)(x))<<RTC_CTRL_CLKOUT_SHIFT))&RTC_CTRL_CLKOUT_MASK)
/* STATUS Bit Fields */
#define RTC_STATUS_INVAL_BIT_MASK                0x1u
#define RTC_STATUS_INVAL_BIT_SHIFT               0
#define RTC_STATUS_WRITE_PROT_EN_MASK            0x2u
#define RTC_STATUS_WRITE_PROT_EN_SHIFT           1
#define RTC_STATUS_CPU_LOW_VOLT_MASK             0x4u
#define RTC_STATUS_CPU_LOW_VOLT_SHIFT            2
#define RTC_STATUS_RST_SRC_MASK                  0x8u
#define RTC_STATUS_RST_SRC_SHIFT                 3
#define RTC_STATUS_CMP_INT_MASK                  0x20u
#define RTC_STATUS_CMP_INT_SHIFT                 5
#define RTC_STATUS_WE_MASK                       0xC0u
#define RTC_STATUS_WE_SHIFT                      6
#define RTC_STATUS_WE(x)                         (((uint16_t)(((uint16_t)(x))<<RTC_STATUS_WE_SHIFT))&RTC_STATUS_WE_MASK)
#define RTC_STATUS_BUS_ERR_MASK                  0x100u
#define RTC_STATUS_BUS_ERR_SHIFT                 8
#define RTC_STATUS_CMP_DONE_MASK                 0x800u
#define RTC_STATUS_CMP_DONE_SHIFT                11
/* ISR Bit Fields */
#define RTC_ISR_TAMPER_IS_MASK                   0x1u
#define RTC_ISR_TAMPER_IS_SHIFT                  0
#define RTC_ISR_ALM_IS_MASK                      0x4u
#define RTC_ISR_ALM_IS_SHIFT                     2
#define RTC_ISR_DAY_IS_MASK                      0x8u
#define RTC_ISR_DAY_IS_SHIFT                     3
#define RTC_ISR_HOUR_IS_MASK                     0x10u
#define RTC_ISR_HOUR_IS_SHIFT                    4
#define RTC_ISR_MIN_IS_MASK                      0x20u
#define RTC_ISR_MIN_IS_SHIFT                     5
#define RTC_ISR_IS_1HZ_MASK                      0x40u
#define RTC_ISR_IS_1HZ_SHIFT                     6
#define RTC_ISR_IS_2HZ_MASK                      0x80u
#define RTC_ISR_IS_2HZ_SHIFT                     7
#define RTC_ISR_IS_4HZ_MASK                      0x100u
#define RTC_ISR_IS_4HZ_SHIFT                     8
#define RTC_ISR_IS_8HZ_MASK                      0x200u
#define RTC_ISR_IS_8HZ_SHIFT                     9
#define RTC_ISR_IS_16HZ_MASK                     0x400u
#define RTC_ISR_IS_16HZ_SHIFT                    10
#define RTC_ISR_IS_32HZ_MASK                     0x800u
#define RTC_ISR_IS_32HZ_SHIFT                    11
#define RTC_ISR_IS_64HZ_MASK                     0x1000u
#define RTC_ISR_IS_64HZ_SHIFT                    12
#define RTC_ISR_IS_128HZ_MASK                    0x2000u
#define RTC_ISR_IS_128HZ_SHIFT                   13
#define RTC_ISR_IS_256HZ_MASK                    0x4000u
#define RTC_ISR_IS_256HZ_SHIFT                   14
#define RTC_ISR_IS_512HZ_MASK                    0x8000u
#define RTC_ISR_IS_512HZ_SHIFT                   15
/* IER Bit Fields */
#define RTC_IER_TAMPER_IE_MASK                   0x1u
#define RTC_IER_TAMPER_IE_SHIFT                  0
#define RTC_IER_ALM_IE_MASK                      0x4u
#define RTC_IER_ALM_IE_SHIFT                     2
#define RTC_IER_DAY_IE_MASK                      0x8u
#define RTC_IER_DAY_IE_SHIFT                     3
#define RTC_IER_HOUR_IE_MASK                     0x10u
#define RTC_IER_HOUR_IE_SHIFT                    4
#define RTC_IER_MIN_IE_MASK                      0x20u
#define RTC_IER_MIN_IE_SHIFT                     5
#define RTC_IER_IE_1HZ_MASK                      0x40u
#define RTC_IER_IE_1HZ_SHIFT                     6
#define RTC_IER_IE_2HZ_MASK                      0x80u
#define RTC_IER_IE_2HZ_SHIFT                     7
#define RTC_IER_IE_4HZ_MASK                      0x100u
#define RTC_IER_IE_4HZ_SHIFT                     8
#define RTC_IER_IE_8HZ_MASK                      0x200u
#define RTC_IER_IE_8HZ_SHIFT                     9
#define RTC_IER_IE_16HZ_MASK                     0x400u
#define RTC_IER_IE_16HZ_SHIFT                    10
#define RTC_IER_IE_32HZ_MASK                     0x800u
#define RTC_IER_IE_32HZ_SHIFT                    11
#define RTC_IER_IE_64HZ_MASK                     0x1000u
#define RTC_IER_IE_64HZ_SHIFT                    12
#define RTC_IER_IE_128HZ_MASK                    0x2000u
#define RTC_IER_IE_128HZ_SHIFT                   13
#define RTC_IER_IE_256HZ_MASK                    0x4000u
#define RTC_IER_IE_256HZ_SHIFT                   14
#define RTC_IER_IE_512HZ_MASK                    0x8000u
#define RTC_IER_IE_512HZ_SHIFT                   15
/* GP_DATA_REG Bit Fields */
#define RTC_GP_DATA_REG_GP_DATA_REG_MASK         0xFFFFu
#define RTC_GP_DATA_REG_GP_DATA_REG_SHIFT        0
#define RTC_GP_DATA_REG_GP_DATA_REG(x)           (((uint16_t)(((uint16_t)(x))<<RTC_GP_DATA_REG_GP_DATA_REG_SHIFT))&RTC_GP_DATA_REG_GP_DATA_REG_MASK)
/* DST_HOUR Bit Fields */
#define RTC_DST_HOUR_DST_END_HOUR_MASK           0x1Fu
#define RTC_DST_HOUR_DST_END_HOUR_SHIFT          0
#define RTC_DST_HOUR_DST_END_HOUR(x)             (((uint16_t)(((uint16_t)(x))<<RTC_DST_HOUR_DST_END_HOUR_SHIFT))&RTC_DST_HOUR_DST_END_HOUR_MASK)
#define RTC_DST_HOUR_DST_START_HOUR_MASK         0x1F00u
#define RTC_DST_HOUR_DST_START_HOUR_SHIFT        8
#define RTC_DST_HOUR_DST_START_HOUR(x)           (((uint16_t)(((uint16_t)(x))<<RTC_DST_HOUR_DST_START_HOUR_SHIFT))&RTC_DST_HOUR_DST_START_HOUR_MASK)
/* DST_MONTH Bit Fields */
#define RTC_DST_MONTH_DST_END_MONTH_MASK         0xFu
#define RTC_DST_MONTH_DST_END_MONTH_SHIFT        0
#define RTC_DST_MONTH_DST_END_MONTH(x)           (((uint16_t)(((uint16_t)(x))<<RTC_DST_MONTH_DST_END_MONTH_SHIFT))&RTC_DST_MONTH_DST_END_MONTH_MASK)
#define RTC_DST_MONTH_DST_START_MONTH_MASK       0xF00u
#define RTC_DST_MONTH_DST_START_MONTH_SHIFT      8
#define RTC_DST_MONTH_DST_START_MONTH(x)         (((uint16_t)(((uint16_t)(x))<<RTC_DST_MONTH_DST_START_MONTH_SHIFT))&RTC_DST_MONTH_DST_START_MONTH_MASK)
/* DST_DAY Bit Fields */
#define RTC_DST_DAY_DST_END_DAY_MASK             0x1Fu
#define RTC_DST_DAY_DST_END_DAY_SHIFT            0
#define RTC_DST_DAY_DST_END_DAY(x)               (((uint16_t)(((uint16_t)(x))<<RTC_DST_DAY_DST_END_DAY_SHIFT))&RTC_DST_DAY_DST_END_DAY_MASK)
#define RTC_DST_DAY_DST_START_DAY_MASK           0x1F00u
#define RTC_DST_DAY_DST_START_DAY_SHIFT          8
#define RTC_DST_DAY_DST_START_DAY(x)             (((uint16_t)(((uint16_t)(x))<<RTC_DST_DAY_DST_START_DAY_SHIFT))&RTC_DST_DAY_DST_START_DAY_MASK)
/* COMPEN Bit Fields */
#define RTC_COMPEN_COMPEN_VAL_MASK               0xFFFFu
#define RTC_COMPEN_COMPEN_VAL_SHIFT              0
#define RTC_COMPEN_COMPEN_VAL(x)                 (((uint16_t)(((uint16_t)(x))<<RTC_COMPEN_COMPEN_VAL_SHIFT))&RTC_COMPEN_COMPEN_VAL_MASK)
/* TAMPER_DIRECTION Bit Fields */
#define RTC_TAMPER_DIRECTION_A_P_TAMP_MASK       0xFu
#define RTC_TAMPER_DIRECTION_A_P_TAMP_SHIFT      0
#define RTC_TAMPER_DIRECTION_A_P_TAMP(x)         (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_DIRECTION_A_P_TAMP_SHIFT))&RTC_TAMPER_DIRECTION_A_P_TAMP_MASK)
#define RTC_TAMPER_DIRECTION_I_O_TAMP_MASK       0xF00u
#define RTC_TAMPER_DIRECTION_I_O_TAMP_SHIFT      8
#define RTC_TAMPER_DIRECTION_I_O_TAMP(x)         (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_DIRECTION_I_O_TAMP_SHIFT))&RTC_TAMPER_DIRECTION_I_O_TAMP_MASK)
/* TAMPER_QSCR Bit Fields */
#define RTC_TAMPER_QSCR_Q_FULL_MASK              0x1u
#define RTC_TAMPER_QSCR_Q_FULL_SHIFT             0
#define RTC_TAMPER_QSCR_Q_FULL_INT_EN_MASK       0x2u
#define RTC_TAMPER_QSCR_Q_FULL_INT_EN_SHIFT      1
#define RTC_TAMPER_QSCR_Q_CLEAR_MASK             0x4u
#define RTC_TAMPER_QSCR_Q_CLEAR_SHIFT            2
#define RTC_TAMPER_QSCR_LFSR_CLK_SEL_MASK        0x700u
#define RTC_TAMPER_QSCR_LFSR_CLK_SEL_SHIFT       8
#define RTC_TAMPER_QSCR_LFSR_CLK_SEL(x)          (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_QSCR_LFSR_CLK_SEL_SHIFT))&RTC_TAMPER_QSCR_LFSR_CLK_SEL_MASK)
#define RTC_TAMPER_QSCR_LFSR_DURATION_MASK       0xF000u
#define RTC_TAMPER_QSCR_LFSR_DURATION_SHIFT      12
#define RTC_TAMPER_QSCR_LFSR_DURATION(x)         (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_QSCR_LFSR_DURATION_SHIFT))&RTC_TAMPER_QSCR_LFSR_DURATION_MASK)
/* TAMPER_SCR Bit Fields */
#define RTC_TAMPER_SCR_TMPR_EN_MASK              0xFu
#define RTC_TAMPER_SCR_TMPR_EN_SHIFT             0
#define RTC_TAMPER_SCR_TMPR_EN(x)                (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_SCR_TMPR_EN_SHIFT))&RTC_TAMPER_SCR_TMPR_EN_MASK)
#define RTC_TAMPER_SCR_TMPR_STS_MASK             0xF00u
#define RTC_TAMPER_SCR_TMPR_STS_SHIFT            8
#define RTC_TAMPER_SCR_TMPR_STS(x)               (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_SCR_TMPR_STS_SHIFT))&RTC_TAMPER_SCR_TMPR_STS_MASK)
/* FILTER01_CFG Bit Fields */
#define RTC_FILTER01_CFG_FIL_DUR1_MASK           0xFu
#define RTC_FILTER01_CFG_FIL_DUR1_SHIFT          0
#define RTC_FILTER01_CFG_FIL_DUR1(x)             (((uint16_t)(((uint16_t)(x))<<RTC_FILTER01_CFG_FIL_DUR1_SHIFT))&RTC_FILTER01_CFG_FIL_DUR1_MASK)
#define RTC_FILTER01_CFG_CLK_SEL1_MASK           0x70u
#define RTC_FILTER01_CFG_CLK_SEL1_SHIFT          4
#define RTC_FILTER01_CFG_CLK_SEL1(x)             (((uint16_t)(((uint16_t)(x))<<RTC_FILTER01_CFG_CLK_SEL1_SHIFT))&RTC_FILTER01_CFG_CLK_SEL1_MASK)
#define RTC_FILTER01_CFG_POL1_MASK               0x80u
#define RTC_FILTER01_CFG_POL1_SHIFT              7
#define RTC_FILTER01_CFG_FIL_DUR0_MASK           0xF00u
#define RTC_FILTER01_CFG_FIL_DUR0_SHIFT          8
#define RTC_FILTER01_CFG_FIL_DUR0(x)             (((uint16_t)(((uint16_t)(x))<<RTC_FILTER01_CFG_FIL_DUR0_SHIFT))&RTC_FILTER01_CFG_FIL_DUR0_MASK)
#define RTC_FILTER01_CFG_CLK_SEL0_MASK           0x7000u
#define RTC_FILTER01_CFG_CLK_SEL0_SHIFT          12
#define RTC_FILTER01_CFG_CLK_SEL0(x)             (((uint16_t)(((uint16_t)(x))<<RTC_FILTER01_CFG_CLK_SEL0_SHIFT))&RTC_FILTER01_CFG_CLK_SEL0_MASK)
#define RTC_FILTER01_CFG_POL0_MASK               0x8000u
#define RTC_FILTER01_CFG_POL0_SHIFT              15
/* FILTER2_CFG Bit Fields */
#define RTC_FILTER2_CFG_FIL_DUR2_MASK            0xF00u
#define RTC_FILTER2_CFG_FIL_DUR2_SHIFT           8
#define RTC_FILTER2_CFG_FIL_DUR2(x)              (((uint16_t)(((uint16_t)(x))<<RTC_FILTER2_CFG_FIL_DUR2_SHIFT))&RTC_FILTER2_CFG_FIL_DUR2_MASK)
#define RTC_FILTER2_CFG_CLK_SEL2_MASK            0x7000u
#define RTC_FILTER2_CFG_CLK_SEL2_SHIFT           12
#define RTC_FILTER2_CFG_CLK_SEL2(x)              (((uint16_t)(((uint16_t)(x))<<RTC_FILTER2_CFG_CLK_SEL2_SHIFT))&RTC_FILTER2_CFG_CLK_SEL2_MASK)
#define RTC_FILTER2_CFG_POL2_MASK                0x8000u
#define RTC_FILTER2_CFG_POL2_SHIFT               15
/* TAMPER_QUEUE Bit Fields */
#define RTC_TAMPER_QUEUE_TAMPER_DATA_MASK        0xFFFFu
#define RTC_TAMPER_QUEUE_TAMPER_DATA_SHIFT       0
#define RTC_TAMPER_QUEUE_TAMPER_DATA(x)          (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_QUEUE_TAMPER_DATA_SHIFT))&RTC_TAMPER_QUEUE_TAMPER_DATA_MASK)
/* CTRL2 Bit Fields */
#define RTC_CTRL2_TAMP_CFG_OVER_MASK             0x1u
#define RTC_CTRL2_TAMP_CFG_OVER_SHIFT            0
#define RTC_CTRL2_WAKEUP_STATUS_MASK             0x60u
#define RTC_CTRL2_WAKEUP_STATUS_SHIFT            5
#define RTC_CTRL2_WAKEUP_STATUS(x)               (((uint16_t)(((uint16_t)(x))<<RTC_CTRL2_WAKEUP_STATUS_SHIFT))&RTC_CTRL2_WAKEUP_STATUS_MASK)
#define RTC_CTRL2_WAKEUP_MODE_MASK               0x80u
#define RTC_CTRL2_WAKEUP_MODE_SHIFT              7

/*!
 * @}
 */ /* end of group RTC_Register_Masks */


/* RTC - Peripheral instance base addresses */
/** Peripheral RTC base pointer */
#define RTC_BASE_PTR                             ((RTC_MemMapPtr)0x40050000u)
/** Array initializer of RTC peripheral base pointers */
#define RTC_BASE_PTRS                            { RTC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- RTC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTC_Register_Accessor_Macros RTC - Register accessor macros
 * @{
 */


/* RTC - Register instance definitions */
/* RTC */
#define RTC_YEARMON                              RTC_YEARMON_REG(RTC_BASE_PTR)
#define RTC_DAYS                                 RTC_DAYS_REG(RTC_BASE_PTR)
#define RTC_HOURMIN                              RTC_HOURMIN_REG(RTC_BASE_PTR)
#define RTC_SECONDS                              RTC_SECONDS_REG(RTC_BASE_PTR)
#define RTC_ALM_YEARMON                          RTC_ALM_YEARMON_REG(RTC_BASE_PTR)
#define RTC_ALM_DAYS                             RTC_ALM_DAYS_REG(RTC_BASE_PTR)
#define RTC_ALM_HOURMIN                          RTC_ALM_HOURMIN_REG(RTC_BASE_PTR)
#define RTC_ALM_SECONDS                          RTC_ALM_SECONDS_REG(RTC_BASE_PTR)
#define RTC_CTRL                                 RTC_CTRL_REG(RTC_BASE_PTR)
#define RTC_STATUS                               RTC_STATUS_REG(RTC_BASE_PTR)
#define RTC_ISR                                  RTC_ISR_REG(RTC_BASE_PTR)
#define RTC_IER                                  RTC_IER_REG(RTC_BASE_PTR)
#define RTC_GP_DATA_REG                          RTC_GP_DATA_REG_REG(RTC_BASE_PTR)
#define RTC_DST_HOUR                             RTC_DST_HOUR_REG(RTC_BASE_PTR)
#define RTC_DST_MONTH                            RTC_DST_MONTH_REG(RTC_BASE_PTR)
#define RTC_DST_DAY                              RTC_DST_DAY_REG(RTC_BASE_PTR)
#define RTC_COMPEN                               RTC_COMPEN_REG(RTC_BASE_PTR)
#define RTC_TAMPER_DIRECTION                     RTC_TAMPER_DIRECTION_REG(RTC_BASE_PTR)
#define RTC_TAMPER_QSCR                          RTC_TAMPER_QSCR_REG(RTC_BASE_PTR)
#define RTC_TAMPER_SCR                           RTC_TAMPER_SCR_REG(RTC_BASE_PTR)
#define RTC_FILTER01_CFG                         RTC_FILTER01_CFG_REG(RTC_BASE_PTR)
#define RTC_FILTER2_CFG                          RTC_FILTER2_CFG_REG(RTC_BASE_PTR)
#define RTC_TAMPER_QUEUE                         RTC_TAMPER_QUEUE_REG(RTC_BASE_PTR)
#define RTC_CTRL2                                RTC_CTRL2_REG(RTC_BASE_PTR)

/*!
 * @}
 */ /* end of group RTC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group RTC_Peripheral */


/* ----------------------------------------------------------------------------
   -- SCB
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCB_Peripheral SCB
 * @{
 */

/** SCB - Peripheral register structure */
typedef struct SCB_MemMap {
  uint8_t RESERVED_0[8];
  uint32_t ACTLR;                                  /**< Auxiliary Control Register,, offset: 0x8 */
  uint8_t RESERVED_1[3316];
  uint32_t CPUID;                                  /**< CPUID Base Register, offset: 0xD00 */
  uint32_t ICSR;                                   /**< Interrupt Control and State Register, offset: 0xD04 */
  uint32_t VTOR;                                   /**< Vector Table Offset Register, offset: 0xD08 */
  uint32_t AIRCR;                                  /**< Application Interrupt and Reset Control Register, offset: 0xD0C */
  uint32_t SCR;                                    /**< System Control Register, offset: 0xD10 */
  uint32_t CCR;                                    /**< Configuration and Control Register, offset: 0xD14 */
  uint8_t RESERVED_2[4];
  uint32_t SHPR2;                                  /**< System Handler Priority Register 2, offset: 0xD1C */
  uint32_t SHPR3;                                  /**< System Handler Priority Register 3, offset: 0xD20 */
  uint32_t SHCSR;                                  /**< System Handler Control and State Register, offset: 0xD24 */
  uint8_t RESERVED_3[8];
  uint32_t DFSR;                                   /**< Debug Fault Status Register, offset: 0xD30 */
} volatile *SCB_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCB_Register_Accessor_Macros SCB - Register accessor macros
 * @{
 */


/* SCB - Register accessors */
#define SCB_ACTLR_REG(base)                      ((base)->ACTLR)
#define SCB_CPUID_REG(base)                      ((base)->CPUID)
#define SCB_ICSR_REG(base)                       ((base)->ICSR)
#define SCB_VTOR_REG(base)                       ((base)->VTOR)
#define SCB_AIRCR_REG(base)                      ((base)->AIRCR)
#define SCB_SCR_REG(base)                        ((base)->SCR)
#define SCB_CCR_REG(base)                        ((base)->CCR)
#define SCB_SHPR2_REG(base)                      ((base)->SHPR2)
#define SCB_SHPR3_REG(base)                      ((base)->SHPR3)
#define SCB_SHCSR_REG(base)                      ((base)->SHCSR)
#define SCB_DFSR_REG(base)                       ((base)->DFSR)

/*!
 * @}
 */ /* end of group SCB_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SCB Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCB_Register_Masks SCB Register Masks
 * @{
 */

/* CPUID Bit Fields */
#define SCB_CPUID_REVISION_MASK                  0xFu
#define SCB_CPUID_REVISION_SHIFT                 0
#define SCB_CPUID_REVISION(x)                    (((uint32_t)(((uint32_t)(x))<<SCB_CPUID_REVISION_SHIFT))&SCB_CPUID_REVISION_MASK)
#define SCB_CPUID_PARTNO_MASK                    0xFFF0u
#define SCB_CPUID_PARTNO_SHIFT                   4
#define SCB_CPUID_PARTNO(x)                      (((uint32_t)(((uint32_t)(x))<<SCB_CPUID_PARTNO_SHIFT))&SCB_CPUID_PARTNO_MASK)
#define SCB_CPUID_VARIANT_MASK                   0xF00000u
#define SCB_CPUID_VARIANT_SHIFT                  20
#define SCB_CPUID_VARIANT(x)                     (((uint32_t)(((uint32_t)(x))<<SCB_CPUID_VARIANT_SHIFT))&SCB_CPUID_VARIANT_MASK)
#define SCB_CPUID_IMPLEMENTER_MASK               0xFF000000u
#define SCB_CPUID_IMPLEMENTER_SHIFT              24
#define SCB_CPUID_IMPLEMENTER(x)                 (((uint32_t)(((uint32_t)(x))<<SCB_CPUID_IMPLEMENTER_SHIFT))&SCB_CPUID_IMPLEMENTER_MASK)
/* ICSR Bit Fields */
#define SCB_ICSR_VECTACTIVE_MASK                 0x3Fu
#define SCB_ICSR_VECTACTIVE_SHIFT                0
#define SCB_ICSR_VECTACTIVE(x)                   (((uint32_t)(((uint32_t)(x))<<SCB_ICSR_VECTACTIVE_SHIFT))&SCB_ICSR_VECTACTIVE_MASK)
#define SCB_ICSR_VECTPENDING_MASK                0x3F000u
#define SCB_ICSR_VECTPENDING_SHIFT               12
#define SCB_ICSR_VECTPENDING(x)                  (((uint32_t)(((uint32_t)(x))<<SCB_ICSR_VECTPENDING_SHIFT))&SCB_ICSR_VECTPENDING_MASK)
#define SCB_ICSR_ISRPENDING_MASK                 0x400000u
#define SCB_ICSR_ISRPENDING_SHIFT                22
#define SCB_ICSR_PENDSTCLR_MASK                  0x2000000u
#define SCB_ICSR_PENDSTCLR_SHIFT                 25
#define SCB_ICSR_PENDSTSET_MASK                  0x4000000u
#define SCB_ICSR_PENDSTSET_SHIFT                 26
#define SCB_ICSR_PENDSVCLR_MASK                  0x8000000u
#define SCB_ICSR_PENDSVCLR_SHIFT                 27
#define SCB_ICSR_PENDSVSET_MASK                  0x10000000u
#define SCB_ICSR_PENDSVSET_SHIFT                 28
#define SCB_ICSR_NMIPENDSET_MASK                 0x80000000u
#define SCB_ICSR_NMIPENDSET_SHIFT                31
/* VTOR Bit Fields */
#define SCB_VTOR_TBLOFF_MASK                     0xFFFFFF80u
#define SCB_VTOR_TBLOFF_SHIFT                    7
#define SCB_VTOR_TBLOFF(x)                       (((uint32_t)(((uint32_t)(x))<<SCB_VTOR_TBLOFF_SHIFT))&SCB_VTOR_TBLOFF_MASK)
/* AIRCR Bit Fields */
#define SCB_AIRCR_VECTCLRACTIVE_MASK             0x2u
#define SCB_AIRCR_VECTCLRACTIVE_SHIFT            1
#define SCB_AIRCR_SYSRESETREQ_MASK               0x4u
#define SCB_AIRCR_SYSRESETREQ_SHIFT              2
#define SCB_AIRCR_ENDIANNESS_MASK                0x8000u
#define SCB_AIRCR_ENDIANNESS_SHIFT               15
#define SCB_AIRCR_VECTKEY_MASK                   0xFFFF0000u
#define SCB_AIRCR_VECTKEY_SHIFT                  16
#define SCB_AIRCR_VECTKEY(x)                     (((uint32_t)(((uint32_t)(x))<<SCB_AIRCR_VECTKEY_SHIFT))&SCB_AIRCR_VECTKEY_MASK)
/* SCR Bit Fields */
#define SCB_SCR_SLEEPONEXIT_MASK                 0x2u
#define SCB_SCR_SLEEPONEXIT_SHIFT                1
#define SCB_SCR_SLEEPDEEP_MASK                   0x4u
#define SCB_SCR_SLEEPDEEP_SHIFT                  2
#define SCB_SCR_SEVONPEND_MASK                   0x10u
#define SCB_SCR_SEVONPEND_SHIFT                  4
/* CCR Bit Fields */
#define SCB_CCR_UNALIGN_TRP_MASK                 0x8u
#define SCB_CCR_UNALIGN_TRP_SHIFT                3
#define SCB_CCR_STKALIGN_MASK                    0x200u
#define SCB_CCR_STKALIGN_SHIFT                   9
/* SHPR2 Bit Fields */
#define SCB_SHPR2_PRI_11_MASK                    0xFF000000u
#define SCB_SHPR2_PRI_11_SHIFT                   24
#define SCB_SHPR2_PRI_11(x)                      (((uint32_t)(((uint32_t)(x))<<SCB_SHPR2_PRI_11_SHIFT))&SCB_SHPR2_PRI_11_MASK)
/* SHPR3 Bit Fields */
#define SCB_SHPR3_PRI_14_MASK                    0xFF0000u
#define SCB_SHPR3_PRI_14_SHIFT                   16
#define SCB_SHPR3_PRI_14(x)                      (((uint32_t)(((uint32_t)(x))<<SCB_SHPR3_PRI_14_SHIFT))&SCB_SHPR3_PRI_14_MASK)
#define SCB_SHPR3_PRI_15_MASK                    0xFF000000u
#define SCB_SHPR3_PRI_15_SHIFT                   24
#define SCB_SHPR3_PRI_15(x)                      (((uint32_t)(((uint32_t)(x))<<SCB_SHPR3_PRI_15_SHIFT))&SCB_SHPR3_PRI_15_MASK)
/* SHCSR Bit Fields */
#define SCB_SHCSR_SVCALLPENDED_MASK              0x8000u
#define SCB_SHCSR_SVCALLPENDED_SHIFT             15
/* DFSR Bit Fields */
#define SCB_DFSR_HALTED_MASK                     0x1u
#define SCB_DFSR_HALTED_SHIFT                    0
#define SCB_DFSR_BKPT_MASK                       0x2u
#define SCB_DFSR_BKPT_SHIFT                      1
#define SCB_DFSR_DWTTRAP_MASK                    0x4u
#define SCB_DFSR_DWTTRAP_SHIFT                   2
#define SCB_DFSR_VCATCH_MASK                     0x8u
#define SCB_DFSR_VCATCH_SHIFT                    3
#define SCB_DFSR_EXTERNAL_MASK                   0x10u
#define SCB_DFSR_EXTERNAL_SHIFT                  4

/*!
 * @}
 */ /* end of group SCB_Register_Masks */


/* SCB - Peripheral instance base addresses */
/** Peripheral SystemControl base pointer */
#define SystemControl_BASE_PTR                   ((SCB_MemMapPtr)0xE000E000u)
/** Array initializer of SCB peripheral base pointers */
#define SCB_BASE_PTRS                            { SystemControl_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCB_Register_Accessor_Macros SCB - Register accessor macros
 * @{
 */


/* SCB - Register instance definitions */
/* SystemControl */
#define SCB_ACTLR                                SCB_ACTLR_REG(SystemControl_BASE_PTR)
#define SCB_CPUID                                SCB_CPUID_REG(SystemControl_BASE_PTR)
#define SCB_ICSR                                 SCB_ICSR_REG(SystemControl_BASE_PTR)
#define SCB_VTOR                                 SCB_VTOR_REG(SystemControl_BASE_PTR)
#define SCB_AIRCR                                SCB_AIRCR_REG(SystemControl_BASE_PTR)
#define SCB_SCR                                  SCB_SCR_REG(SystemControl_BASE_PTR)
#define SCB_CCR                                  SCB_CCR_REG(SystemControl_BASE_PTR)
#define SCB_SHPR2                                SCB_SHPR2_REG(SystemControl_BASE_PTR)
#define SCB_SHPR3                                SCB_SHPR3_REG(SystemControl_BASE_PTR)
#define SCB_SHCSR                                SCB_SHCSR_REG(SystemControl_BASE_PTR)
#define SCB_DFSR                                 SCB_DFSR_REG(SystemControl_BASE_PTR)

/*!
 * @}
 */ /* end of group SCB_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SCB_Peripheral */


/* ----------------------------------------------------------------------------
   -- SIM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Peripheral SIM
 * @{
 */

/** SIM - Peripheral register structure */
typedef struct SIM_MemMap {
  uint32_t SOPT1;                                  /**< System Options Register 1, offset: 0x0 */
  uint32_t SOPT1_CFG;                              /**< SOPT1 Configuration Register, offset: 0x4 */
  uint8_t RESERVED_0[4092];
  uint32_t CTRL_REG;                               /**< System Control Register, offset: 0x1004 */
  uint8_t RESERVED_1[28];
  uint32_t SDID;                                   /**< System Device Identification Register, offset: 0x1024 */
  uint8_t RESERVED_2[12];
  uint32_t SCGC4;                                  /**< System Clock Gating Control Register 4, offset: 0x1034 */
  uint32_t SCGC5;                                  /**< System Clock Gating Control Register 5, offset: 0x1038 */
  uint32_t SCGC6;                                  /**< System Clock Gating Control Register 6, offset: 0x103C */
  uint32_t SCGC7;                                  /**< System Clock Gating Control Register 7, offset: 0x1040 */
  uint32_t CLKDIV1;                                /**< System Clock Divider Register 1, offset: 0x1044 */
  uint8_t RESERVED_3[4];
  uint32_t FCFG1;                                  /**< Flash Configuration Register 1, offset: 0x104C */
  uint32_t FCFG2;                                  /**< Flash Configuration Register 2, offset: 0x1050 */
  uint32_t UID0;                                   /**< Unique Identification Register 0, offset: 0x1054 */
  uint32_t UID1;                                   /**< Unique Identification Register 1, offset: 0x1058 */
  uint32_t UID2;                                   /**< Unique Identification Register 2, offset: 0x105C */
  uint32_t UID3;                                   /**< Unique Identification Register 3, offset: 0x1060 */
  uint8_t RESERVED_4[8];
  uint32_t MISC_CTL;                               /**< Miscellaneous Control Register, offset: 0x106C */
} volatile *SIM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SIM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Register_Accessor_Macros SIM - Register accessor macros
 * @{
 */


/* SIM - Register accessors */
#define SIM_SOPT1_REG(base)                      ((base)->SOPT1)
#define SIM_SOPT1_CFG_REG(base)                  ((base)->SOPT1_CFG)
#define SIM_CTRL_REG_REG(base)                   ((base)->CTRL_REG)
#define SIM_SDID_REG(base)                       ((base)->SDID)
#define SIM_SCGC4_REG(base)                      ((base)->SCGC4)
#define SIM_SCGC5_REG(base)                      ((base)->SCGC5)
#define SIM_SCGC6_REG(base)                      ((base)->SCGC6)
#define SIM_SCGC7_REG(base)                      ((base)->SCGC7)
#define SIM_CLKDIV1_REG(base)                    ((base)->CLKDIV1)
#define SIM_FCFG1_REG(base)                      ((base)->FCFG1)
#define SIM_FCFG2_REG(base)                      ((base)->FCFG2)
#define SIM_UID0_REG(base)                       ((base)->UID0)
#define SIM_UID1_REG(base)                       ((base)->UID1)
#define SIM_UID2_REG(base)                       ((base)->UID2)
#define SIM_UID3_REG(base)                       ((base)->UID3)
#define SIM_MISC_CTL_REG(base)                   ((base)->MISC_CTL)

/*!
 * @}
 */ /* end of group SIM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SIM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Register_Masks SIM Register Masks
 * @{
 */

/* SOPT1 Bit Fields */
#define SIM_SOPT1_SRAMSIZE_MASK                  0xF000u
#define SIM_SOPT1_SRAMSIZE_SHIFT                 12
#define SIM_SOPT1_SRAMSIZE(x)                    (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_SRAMSIZE_SHIFT))&SIM_SOPT1_SRAMSIZE_MASK)
#define SIM_SOPT1_OSC32KSEL_MASK                 0xC0000u
#define SIM_SOPT1_OSC32KSEL_SHIFT                18
#define SIM_SOPT1_OSC32KSEL(x)                   (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_OSC32KSEL_SHIFT))&SIM_SOPT1_OSC32KSEL_MASK)
/* SOPT1_CFG Bit Fields */
#define SIM_SOPT1_CFG_LPTMR1SEL_MASK             0x3u
#define SIM_SOPT1_CFG_LPTMR1SEL_SHIFT            0
#define SIM_SOPT1_CFG_LPTMR1SEL(x)               (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_CFG_LPTMR1SEL_SHIFT))&SIM_SOPT1_CFG_LPTMR1SEL_MASK)
#define SIM_SOPT1_CFG_LPTMR2SEL_MASK             0xCu
#define SIM_SOPT1_CFG_LPTMR2SEL_SHIFT            2
#define SIM_SOPT1_CFG_LPTMR2SEL(x)               (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_CFG_LPTMR2SEL_SHIFT))&SIM_SOPT1_CFG_LPTMR2SEL_MASK)
#define SIM_SOPT1_CFG_LPTMR3SEL_MASK             0x30u
#define SIM_SOPT1_CFG_LPTMR3SEL_SHIFT            4
#define SIM_SOPT1_CFG_LPTMR3SEL(x)               (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_CFG_LPTMR3SEL_SHIFT))&SIM_SOPT1_CFG_LPTMR3SEL_MASK)
#define SIM_SOPT1_CFG_CMPOLPTMR0SEL_MASK         0x40u
#define SIM_SOPT1_CFG_CMPOLPTMR0SEL_SHIFT        6
#define SIM_SOPT1_CFG_RAMSBDIS_MASK              0x100u
#define SIM_SOPT1_CFG_RAMSBDIS_SHIFT             8
#define SIM_SOPT1_CFG_RAMBPEN_MASK               0x200u
#define SIM_SOPT1_CFG_RAMBPEN_SHIFT              9
/* CTRL_REG Bit Fields */
#define SIM_CTRL_REG_NMIDIS_MASK                 0x1u
#define SIM_CTRL_REG_NMIDIS_SHIFT                0
#define SIM_CTRL_REG_PLL_VLP_EN_MASK             0x2u
#define SIM_CTRL_REG_PLL_VLP_EN_SHIFT            1
#define SIM_CTRL_REG_PTC2_HD_EN_MASK             0x4u
#define SIM_CTRL_REG_PTC2_HD_EN_SHIFT            2
#define SIM_CTRL_REG_SAR_TRG_CLK_SEL_MASK        0x18u
#define SIM_CTRL_REG_SAR_TRG_CLK_SEL_SHIFT       3
#define SIM_CTRL_REG_SAR_TRG_CLK_SEL(x)          (((uint32_t)(((uint32_t)(x))<<SIM_CTRL_REG_SAR_TRG_CLK_SEL_SHIFT))&SIM_CTRL_REG_SAR_TRG_CLK_SEL_MASK)
#define SIM_CTRL_REG_CLKOUTSEL_MASK              0xE0u
#define SIM_CTRL_REG_CLKOUTSEL_SHIFT             5
#define SIM_CTRL_REG_CLKOUTSEL(x)                (((uint32_t)(((uint32_t)(x))<<SIM_CTRL_REG_CLKOUTSEL_SHIFT))&SIM_CTRL_REG_CLKOUTSEL_MASK)
/* SDID Bit Fields */
#define SIM_SDID_PINID_MASK                      0xFu
#define SIM_SDID_PINID_SHIFT                     0
#define SIM_SDID_PINID(x)                        (((uint32_t)(((uint32_t)(x))<<SIM_SDID_PINID_SHIFT))&SIM_SDID_PINID_MASK)
#define SIM_SDID_DIEID_MASK                      0xF0u
#define SIM_SDID_DIEID_SHIFT                     4
#define SIM_SDID_DIEID(x)                        (((uint32_t)(((uint32_t)(x))<<SIM_SDID_DIEID_SHIFT))&SIM_SDID_DIEID_MASK)
#define SIM_SDID_REVID_MASK                      0xF00u
#define SIM_SDID_REVID_SHIFT                     8
#define SIM_SDID_REVID(x)                        (((uint32_t)(((uint32_t)(x))<<SIM_SDID_REVID_SHIFT))&SIM_SDID_REVID_MASK)
#define SIM_SDID_SRAMSIZE_MASK                   0xF000u
#define SIM_SDID_SRAMSIZE_SHIFT                  12
#define SIM_SDID_SRAMSIZE(x)                     (((uint32_t)(((uint32_t)(x))<<SIM_SDID_SRAMSIZE_SHIFT))&SIM_SDID_SRAMSIZE_MASK)
#define SIM_SDID_ATTR_MASK                       0xF0000u
#define SIM_SDID_ATTR_SHIFT                      16
#define SIM_SDID_ATTR(x)                         (((uint32_t)(((uint32_t)(x))<<SIM_SDID_ATTR_SHIFT))&SIM_SDID_ATTR_MASK)
#define SIM_SDID_SERIESID_MASK                   0xF00000u
#define SIM_SDID_SERIESID_SHIFT                  20
#define SIM_SDID_SERIESID(x)                     (((uint32_t)(((uint32_t)(x))<<SIM_SDID_SERIESID_SHIFT))&SIM_SDID_SERIESID_MASK)
#define SIM_SDID_SUBFAMID_MASK                   0xF000000u
#define SIM_SDID_SUBFAMID_SHIFT                  24
#define SIM_SDID_SUBFAMID(x)                     (((uint32_t)(((uint32_t)(x))<<SIM_SDID_SUBFAMID_SHIFT))&SIM_SDID_SUBFAMID_MASK)
#define SIM_SDID_FAMID_MASK                      0xF0000000u
#define SIM_SDID_FAMID_SHIFT                     28
#define SIM_SDID_FAMID(x)                        (((uint32_t)(((uint32_t)(x))<<SIM_SDID_FAMID_SHIFT))&SIM_SDID_FAMID_MASK)
/* SCGC4 Bit Fields */
#define SIM_SCGC4_EWM_MASK                       0x2u
#define SIM_SCGC4_EWM_SHIFT                      1
#define SIM_SCGC4_MCG_MASK                       0x10u
#define SIM_SCGC4_MCG_SHIFT                      4
#define SIM_SCGC4_OSC_MASK                       0x40u
#define SIM_SCGC4_OSC_SHIFT                      6
#define SIM_SCGC4_I2C0_MASK                      0x80u
#define SIM_SCGC4_I2C0_SHIFT                     7
#define SIM_SCGC4_I2C1_MASK                      0x100u
#define SIM_SCGC4_I2C1_SHIFT                     8
#define SIM_SCGC4_UART0_MASK                     0x400u
#define SIM_SCGC4_UART0_SHIFT                    10
#define SIM_SCGC4_UART1_MASK                     0x800u
#define SIM_SCGC4_UART1_SHIFT                    11
#define SIM_SCGC4_UART2_MASK                     0x1000u
#define SIM_SCGC4_UART2_SHIFT                    12
#define SIM_SCGC4_UART3_MASK                     0x2000u
#define SIM_SCGC4_UART3_SHIFT                    13
#define SIM_SCGC4_VREF_MASK                      0x8000u
#define SIM_SCGC4_VREF_SHIFT                     15
#define SIM_SCGC4_CMP0_MASK                      0x40000u
#define SIM_SCGC4_CMP0_SHIFT                     18
#define SIM_SCGC4_CMP1_MASK                      0x80000u
#define SIM_SCGC4_CMP1_SHIFT                     19
#define SIM_SCGC4_SPI0_MASK                      0x200000u
#define SIM_SCGC4_SPI0_SHIFT                     21
#define SIM_SCGC4_SPI1_MASK                      0x400000u
#define SIM_SCGC4_SPI1_SHIFT                     22
/* SCGC5 Bit Fields */
#define SIM_SCGC5_SLCD_MASK                      0x8u
#define SIM_SCGC5_SLCD_SHIFT                     3
#define SIM_SCGC5_PORTA_MASK                     0x40u
#define SIM_SCGC5_PORTA_SHIFT                    6
#define SIM_SCGC5_PORTB_MASK                     0x80u
#define SIM_SCGC5_PORTB_SHIFT                    7
#define SIM_SCGC5_PORTC_MASK                     0x100u
#define SIM_SCGC5_PORTC_SHIFT                    8
#define SIM_SCGC5_PORTD_MASK                     0x200u
#define SIM_SCGC5_PORTD_SHIFT                    9
#define SIM_SCGC5_PORTE_MASK                     0x400u
#define SIM_SCGC5_PORTE_SHIFT                    10
#define SIM_SCGC5_PORTF_MASK                     0x800u
#define SIM_SCGC5_PORTF_SHIFT                    11
#define SIM_SCGC5_PORTG_MASK                     0x1000u
#define SIM_SCGC5_PORTG_SHIFT                    12
#define SIM_SCGC5_PORTH_MASK                     0x2000u
#define SIM_SCGC5_PORTH_SHIFT                    13
#define SIM_SCGC5_PORTI_MASK                     0x4000u
#define SIM_SCGC5_PORTI_SHIFT                    14
#define SIM_SCGC5_IRTC_MASK                      0x10000u
#define SIM_SCGC5_IRTC_SHIFT                     16
#define SIM_SCGC5_IRTCREGFILE_MASK               0x20000u
#define SIM_SCGC5_IRTCREGFILE_SHIFT              17
#define SIM_SCGC5_WDOG_MASK                      0x80000u
#define SIM_SCGC5_WDOG_SHIFT                     19
#define SIM_SCGC5_XBAR_MASK                      0x200000u
#define SIM_SCGC5_XBAR_SHIFT                     21
#define SIM_SCGC5_TMR0_MASK                      0x800000u
#define SIM_SCGC5_TMR0_SHIFT                     23
#define SIM_SCGC5_TMR1_MASK                      0x1000000u
#define SIM_SCGC5_TMR1_SHIFT                     24
#define SIM_SCGC5_TMR2_MASK                      0x2000000u
#define SIM_SCGC5_TMR2_SHIFT                     25
#define SIM_SCGC5_TMR3_MASK                      0x4000000u
#define SIM_SCGC5_TMR3_SHIFT                     26
/* SCGC6 Bit Fields */
#define SIM_SCGC6_FTFA_MASK                      0x1u
#define SIM_SCGC6_FTFA_SHIFT                     0
#define SIM_SCGC6_DMAMUX0_MASK                   0x2u
#define SIM_SCGC6_DMAMUX0_SHIFT                  1
#define SIM_SCGC6_DMAMUX1_MASK                   0x4u
#define SIM_SCGC6_DMAMUX1_SHIFT                  2
#define SIM_SCGC6_DMAMUX2_MASK                   0x8u
#define SIM_SCGC6_DMAMUX2_SHIFT                  3
#define SIM_SCGC6_DMAMUX3_MASK                   0x10u
#define SIM_SCGC6_DMAMUX3_SHIFT                  4
#define SIM_SCGC6_RNGA_MASK                      0x200u
#define SIM_SCGC6_RNGA_SHIFT                     9
#define SIM_SCGC6_ADC_MASK                       0x800u
#define SIM_SCGC6_ADC_SHIFT                      11
#define SIM_SCGC6_PIT0_MASK                      0x2000u
#define SIM_SCGC6_PIT0_SHIFT                     13
#define SIM_SCGC6_PIT1_MASK                      0x4000u
#define SIM_SCGC6_PIT1_SHIFT                     14
#define SIM_SCGC6_AFE_MASK                       0x10000u
#define SIM_SCGC6_AFE_SHIFT                      16
#define SIM_SCGC6_CRC_MASK                       0x100000u
#define SIM_SCGC6_CRC_SHIFT                      20
#define SIM_SCGC6_LPTMR_MASK                     0x10000000u
#define SIM_SCGC6_LPTMR_SHIFT                    28
#define SIM_SCGC6_SIM_LP_MASK                    0x40000000u
#define SIM_SCGC6_SIM_LP_SHIFT                   30
#define SIM_SCGC6_SIM_HP_MASK                    0x80000000u
#define SIM_SCGC6_SIM_HP_SHIFT                   31
/* SCGC7 Bit Fields */
#define SIM_SCGC7_MPU_MASK                       0x1u
#define SIM_SCGC7_MPU_SHIFT                      0
#define SIM_SCGC7_DMA_MASK                       0x2u
#define SIM_SCGC7_DMA_SHIFT                      1
/* CLKDIV1 Bit Fields */
#define SIM_CLKDIV1_SYSCLKMODE_MASK              0x8000000u
#define SIM_CLKDIV1_SYSCLKMODE_SHIFT             27
#define SIM_CLKDIV1_SYSDIV_MASK                  0xF0000000u
#define SIM_CLKDIV1_SYSDIV_SHIFT                 28
#define SIM_CLKDIV1_SYSDIV(x)                    (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV1_SYSDIV_SHIFT))&SIM_CLKDIV1_SYSDIV_MASK)
/* FCFG1 Bit Fields */
#define SIM_FCFG1_FLASHDIS_MASK                  0x1u
#define SIM_FCFG1_FLASHDIS_SHIFT                 0
#define SIM_FCFG1_FLASHDOZE_MASK                 0x2u
#define SIM_FCFG1_FLASHDOZE_SHIFT                1
#define SIM_FCFG1_PFSIZE_MASK                    0xF000000u
#define SIM_FCFG1_PFSIZE_SHIFT                   24
#define SIM_FCFG1_PFSIZE(x)                      (((uint32_t)(((uint32_t)(x))<<SIM_FCFG1_PFSIZE_SHIFT))&SIM_FCFG1_PFSIZE_MASK)
/* FCFG2 Bit Fields */
#define SIM_FCFG2_MAXADDR_MASK                   0x7F000000u
#define SIM_FCFG2_MAXADDR_SHIFT                  24
#define SIM_FCFG2_MAXADDR(x)                     (((uint32_t)(((uint32_t)(x))<<SIM_FCFG2_MAXADDR_SHIFT))&SIM_FCFG2_MAXADDR_MASK)
/* UID0 Bit Fields */
#define SIM_UID0_UID_MASK                        0xFFFFFFFFu
#define SIM_UID0_UID_SHIFT                       0
#define SIM_UID0_UID(x)                          (((uint32_t)(((uint32_t)(x))<<SIM_UID0_UID_SHIFT))&SIM_UID0_UID_MASK)
/* UID1 Bit Fields */
#define SIM_UID1_UID_MASK                        0xFFFFFFFFu
#define SIM_UID1_UID_SHIFT                       0
#define SIM_UID1_UID(x)                          (((uint32_t)(((uint32_t)(x))<<SIM_UID1_UID_SHIFT))&SIM_UID1_UID_MASK)
/* UID2 Bit Fields */
#define SIM_UID2_UID_MASK                        0xFFFFFFFFu
#define SIM_UID2_UID_SHIFT                       0
#define SIM_UID2_UID(x)                          (((uint32_t)(((uint32_t)(x))<<SIM_UID2_UID_SHIFT))&SIM_UID2_UID_MASK)
/* UID3 Bit Fields */
#define SIM_UID3_UID_MASK                        0xFFFFFFFFu
#define SIM_UID3_UID_SHIFT                       0
#define SIM_UID3_UID(x)                          (((uint32_t)(((uint32_t)(x))<<SIM_UID3_UID_SHIFT))&SIM_UID3_UID_MASK)
/* MISC_CTL Bit Fields */
#define SIM_MISC_CTL_XBARAFEMODOUTSEL_MASK       0x3u
#define SIM_MISC_CTL_XBARAFEMODOUTSEL_SHIFT      0
#define SIM_MISC_CTL_XBARAFEMODOUTSEL(x)         (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_XBARAFEMODOUTSEL_SHIFT))&SIM_MISC_CTL_XBARAFEMODOUTSEL_MASK)
#define SIM_MISC_CTL_DMADONESEL_MASK             0xCu
#define SIM_MISC_CTL_DMADONESEL_SHIFT            2
#define SIM_MISC_CTL_DMADONESEL(x)               (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_DMADONESEL_SHIFT))&SIM_MISC_CTL_DMADONESEL_MASK)
#define SIM_MISC_CTL_AFECLKSEL_MASK              0x30u
#define SIM_MISC_CTL_AFECLKSEL_SHIFT             4
#define SIM_MISC_CTL_AFECLKSEL(x)                (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_AFECLKSEL_SHIFT))&SIM_MISC_CTL_AFECLKSEL_MASK)
#define SIM_MISC_CTL_AFECLKPADDIR_MASK           0x40u
#define SIM_MISC_CTL_AFECLKPADDIR_SHIFT          6
#define SIM_MISC_CTL_UARTMODTYPE_MASK            0x80u
#define SIM_MISC_CTL_UARTMODTYPE_SHIFT           7
#define SIM_MISC_CTL_UART0IRSEL_MASK             0x100u
#define SIM_MISC_CTL_UART0IRSEL_SHIFT            8
#define SIM_MISC_CTL_UART1IRSEL_MASK             0x200u
#define SIM_MISC_CTL_UART1IRSEL_SHIFT            9
#define SIM_MISC_CTL_UART2IRSEL_MASK             0x400u
#define SIM_MISC_CTL_UART2IRSEL_SHIFT            10
#define SIM_MISC_CTL_UART3IRSEL_MASK             0x800u
#define SIM_MISC_CTL_UART3IRSEL_SHIFT            11
#define SIM_MISC_CTL_XBARPITOUTSEL_MASK          0x3000u
#define SIM_MISC_CTL_XBARPITOUTSEL_SHIFT         12
#define SIM_MISC_CTL_XBARPITOUTSEL(x)            (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_XBARPITOUTSEL_SHIFT))&SIM_MISC_CTL_XBARPITOUTSEL_MASK)
#define SIM_MISC_CTL_EWMINSEL_MASK               0x4000u
#define SIM_MISC_CTL_EWMINSEL_SHIFT              14
#define SIM_MISC_CTL_TMR0PLLCLKSEL_MASK          0x8000u
#define SIM_MISC_CTL_TMR0PLLCLKSEL_SHIFT         15
#define SIM_MISC_CTL_TMR0SCSSEL_MASK             0x10000u
#define SIM_MISC_CTL_TMR0SCSSEL_SHIFT            16
#define SIM_MISC_CTL_TMR1SCSSEL_MASK             0x20000u
#define SIM_MISC_CTL_TMR1SCSSEL_SHIFT            17
#define SIM_MISC_CTL_TMR2SCSSEL_MASK             0x40000u
#define SIM_MISC_CTL_TMR2SCSSEL_SHIFT            18
#define SIM_MISC_CTL_TMR3SCSSEL_MASK             0x80000u
#define SIM_MISC_CTL_TMR3SCSSEL_SHIFT            19
#define SIM_MISC_CTL_TMR0PCSSEL_MASK             0x300000u
#define SIM_MISC_CTL_TMR0PCSSEL_SHIFT            20
#define SIM_MISC_CTL_TMR0PCSSEL(x)               (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_TMR0PCSSEL_SHIFT))&SIM_MISC_CTL_TMR0PCSSEL_MASK)
#define SIM_MISC_CTL_TMR1PCSSEL_MASK             0xC00000u
#define SIM_MISC_CTL_TMR1PCSSEL_SHIFT            22
#define SIM_MISC_CTL_TMR1PCSSEL(x)               (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_TMR1PCSSEL_SHIFT))&SIM_MISC_CTL_TMR1PCSSEL_MASK)
#define SIM_MISC_CTL_TMR2PCSSEL_MASK             0x3000000u
#define SIM_MISC_CTL_TMR2PCSSEL_SHIFT            24
#define SIM_MISC_CTL_TMR2PCSSEL(x)               (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_TMR2PCSSEL_SHIFT))&SIM_MISC_CTL_TMR2PCSSEL_MASK)
#define SIM_MISC_CTL_TMR3PCSSEL_MASK             0xC000000u
#define SIM_MISC_CTL_TMR3PCSSEL_SHIFT            26
#define SIM_MISC_CTL_TMR3PCSSEL(x)               (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_TMR3PCSSEL_SHIFT))&SIM_MISC_CTL_TMR3PCSSEL_MASK)
#define SIM_MISC_CTL_RTCCLKSEL_MASK              0x10000000u
#define SIM_MISC_CTL_RTCCLKSEL_SHIFT             28
#define SIM_MISC_CTL_VREFBUFOUTEN_MASK           0x20000000u
#define SIM_MISC_CTL_VREFBUFOUTEN_SHIFT          29
#define SIM_MISC_CTL_VREFBUFINSEL_MASK           0x40000000u
#define SIM_MISC_CTL_VREFBUFINSEL_SHIFT          30
#define SIM_MISC_CTL_VREFBUFPD_MASK              0x80000000u
#define SIM_MISC_CTL_VREFBUFPD_SHIFT             31

/*!
 * @}
 */ /* end of group SIM_Register_Masks */


/* SIM - Peripheral instance base addresses */
/** Peripheral SIM base pointer */
#define SIM_BASE_PTR                             ((SIM_MemMapPtr)0x4003E000u)
/** Array initializer of SIM peripheral base pointers */
#define SIM_BASE_PTRS                            { SIM_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SIM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Register_Accessor_Macros SIM - Register accessor macros
 * @{
 */


/* SIM - Register instance definitions */
/* SIM */
#define SIM_SOPT1                                SIM_SOPT1_REG(SIM_BASE_PTR)
#define SIM_SOPT1_CFG                            SIM_SOPT1_CFG_REG(SIM_BASE_PTR)
#define SIM_CTRL_REG                             SIM_CTRL_REG_REG(SIM_BASE_PTR)
#define SIM_SDID                                 SIM_SDID_REG(SIM_BASE_PTR)
#define SIM_SCGC4                                SIM_SCGC4_REG(SIM_BASE_PTR)
#define SIM_SCGC5                                SIM_SCGC5_REG(SIM_BASE_PTR)
#define SIM_SCGC6                                SIM_SCGC6_REG(SIM_BASE_PTR)
#define SIM_SCGC7                                SIM_SCGC7_REG(SIM_BASE_PTR)
#define SIM_CLKDIV1                              SIM_CLKDIV1_REG(SIM_BASE_PTR)
#define SIM_FCFG1                                SIM_FCFG1_REG(SIM_BASE_PTR)
#define SIM_FCFG2                                SIM_FCFG2_REG(SIM_BASE_PTR)
#define SIM_UID0                                 SIM_UID0_REG(SIM_BASE_PTR)
#define SIM_UID1                                 SIM_UID1_REG(SIM_BASE_PTR)
#define SIM_UID2                                 SIM_UID2_REG(SIM_BASE_PTR)
#define SIM_UID3                                 SIM_UID3_REG(SIM_BASE_PTR)
#define SIM_MISC_CTL                             SIM_MISC_CTL_REG(SIM_BASE_PTR)

/*!
 * @}
 */ /* end of group SIM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SIM_Peripheral */


/* ----------------------------------------------------------------------------
   -- SMC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Peripheral SMC
 * @{
 */

/** SMC - Peripheral register structure */
typedef struct SMC_MemMap {
  uint8_t PMPROT;                                  /**< Power Mode Protection register, offset: 0x0 */
  uint8_t PMCTRL;                                  /**< Power Mode Control register, offset: 0x1 */
  uint8_t STOPCTRL;                                /**< Stop Control Register, offset: 0x2 */
  uint8_t PMSTAT;                                  /**< Power Mode Status register, offset: 0x3 */
} volatile *SMC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Register_Accessor_Macros SMC - Register accessor macros
 * @{
 */


/* SMC - Register accessors */
#define SMC_PMPROT_REG(base)                     ((base)->PMPROT)
#define SMC_PMCTRL_REG(base)                     ((base)->PMCTRL)
#define SMC_STOPCTRL_REG(base)                   ((base)->STOPCTRL)
#define SMC_PMSTAT_REG(base)                     ((base)->PMSTAT)

/*!
 * @}
 */ /* end of group SMC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SMC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Register_Masks SMC Register Masks
 * @{
 */

/* PMPROT Bit Fields */
#define SMC_PMPROT_AVLLS_MASK                    0x2u
#define SMC_PMPROT_AVLLS_SHIFT                   1
#define SMC_PMPROT_AVLP_MASK                     0x20u
#define SMC_PMPROT_AVLP_SHIFT                    5
/* PMCTRL Bit Fields */
#define SMC_PMCTRL_STOPM_MASK                    0x7u
#define SMC_PMCTRL_STOPM_SHIFT                   0
#define SMC_PMCTRL_STOPM(x)                      (((uint8_t)(((uint8_t)(x))<<SMC_PMCTRL_STOPM_SHIFT))&SMC_PMCTRL_STOPM_MASK)
#define SMC_PMCTRL_STOPA_MASK                    0x8u
#define SMC_PMCTRL_STOPA_SHIFT                   3
#define SMC_PMCTRL_RUNM_MASK                     0x60u
#define SMC_PMCTRL_RUNM_SHIFT                    5
#define SMC_PMCTRL_RUNM(x)                       (((uint8_t)(((uint8_t)(x))<<SMC_PMCTRL_RUNM_SHIFT))&SMC_PMCTRL_RUNM_MASK)
/* STOPCTRL Bit Fields */
#define SMC_STOPCTRL_VLLSM_MASK                  0x7u
#define SMC_STOPCTRL_VLLSM_SHIFT                 0
#define SMC_STOPCTRL_VLLSM(x)                    (((uint8_t)(((uint8_t)(x))<<SMC_STOPCTRL_VLLSM_SHIFT))&SMC_STOPCTRL_VLLSM_MASK)
#define SMC_STOPCTRL_PORPO_MASK                  0x20u
#define SMC_STOPCTRL_PORPO_SHIFT                 5
#define SMC_STOPCTRL_PSTOPO_MASK                 0xC0u
#define SMC_STOPCTRL_PSTOPO_SHIFT                6
#define SMC_STOPCTRL_PSTOPO(x)                   (((uint8_t)(((uint8_t)(x))<<SMC_STOPCTRL_PSTOPO_SHIFT))&SMC_STOPCTRL_PSTOPO_MASK)
/* PMSTAT Bit Fields */
#define SMC_PMSTAT_PMSTAT_MASK                   0x7Fu
#define SMC_PMSTAT_PMSTAT_SHIFT                  0
#define SMC_PMSTAT_PMSTAT(x)                     (((uint8_t)(((uint8_t)(x))<<SMC_PMSTAT_PMSTAT_SHIFT))&SMC_PMSTAT_PMSTAT_MASK)

/*!
 * @}
 */ /* end of group SMC_Register_Masks */


/* SMC - Peripheral instance base addresses */
/** Peripheral SMC base pointer */
#define SMC_BASE_PTR                             ((SMC_MemMapPtr)0x4007E000u)
/** Array initializer of SMC peripheral base pointers */
#define SMC_BASE_PTRS                            { SMC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Register_Accessor_Macros SMC - Register accessor macros
 * @{
 */


/* SMC - Register instance definitions */
/* SMC */
#define SMC_PMPROT                               SMC_PMPROT_REG(SMC_BASE_PTR)
#define SMC_PMCTRL                               SMC_PMCTRL_REG(SMC_BASE_PTR)
#define SMC_STOPCTRL                             SMC_STOPCTRL_REG(SMC_BASE_PTR)
#define SMC_PMSTAT                               SMC_PMSTAT_REG(SMC_BASE_PTR)

/*!
 * @}
 */ /* end of group SMC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SMC_Peripheral */


/* ----------------------------------------------------------------------------
   -- SPI
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Peripheral SPI
 * @{
 */

/** SPI - Peripheral register structure */
typedef struct SPI_MemMap {
  uint8_t S;                                       /**< SPI status register, offset: 0x0 */
  uint8_t BR;                                      /**< SPI baud rate register, offset: 0x1 */
  uint8_t C2;                                      /**< SPI control register 2, offset: 0x2 */
  uint8_t C1;                                      /**< SPI control register 1, offset: 0x3 */
  uint8_t ML;                                      /**< SPI match register low, offset: 0x4 */
  uint8_t MH;                                      /**< SPI match register high, offset: 0x5 */
  uint8_t DL;                                      /**< SPI data register low, offset: 0x6 */
  uint8_t DH;                                      /**< SPI data register high, offset: 0x7 */
  uint8_t RESERVED_0[2];
  uint8_t CI;                                      /**< SPI clear interrupt register, offset: 0xA */
  uint8_t C3;                                      /**< SPI control register 3, offset: 0xB */
} volatile *SPI_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SPI - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Register_Accessor_Macros SPI - Register accessor macros
 * @{
 */


/* SPI - Register accessors */
#define SPI_S_REG(base)                          ((base)->S)
#define SPI_BR_REG(base)                         ((base)->BR)
#define SPI_C2_REG(base)                         ((base)->C2)
#define SPI_C1_REG(base)                         ((base)->C1)
#define SPI_ML_REG(base)                         ((base)->ML)
#define SPI_MH_REG(base)                         ((base)->MH)
#define SPI_DL_REG(base)                         ((base)->DL)
#define SPI_DH_REG(base)                         ((base)->DH)
#define SPI_CI_REG(base)                         ((base)->CI)
#define SPI_C3_REG(base)                         ((base)->C3)

/*!
 * @}
 */ /* end of group SPI_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SPI Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Register_Masks SPI Register Masks
 * @{
 */

/* S Bit Fields */
#define SPI_S_RFIFOEF_MASK                       0x1u
#define SPI_S_RFIFOEF_SHIFT                      0
#define SPI_S_TXFULLF_MASK                       0x2u
#define SPI_S_TXFULLF_SHIFT                      1
#define SPI_S_TNEAREF_MASK                       0x4u
#define SPI_S_TNEAREF_SHIFT                      2
#define SPI_S_RNFULLF_MASK                       0x8u
#define SPI_S_RNFULLF_SHIFT                      3
#define SPI_S_MODF_MASK                          0x10u
#define SPI_S_MODF_SHIFT                         4
#define SPI_S_SPTEF_MASK                         0x20u
#define SPI_S_SPTEF_SHIFT                        5
#define SPI_S_SPMF_MASK                          0x40u
#define SPI_S_SPMF_SHIFT                         6
#define SPI_S_SPRF_MASK                          0x80u
#define SPI_S_SPRF_SHIFT                         7
/* BR Bit Fields */
#define SPI_BR_SPR_MASK                          0xFu
#define SPI_BR_SPR_SHIFT                         0
#define SPI_BR_SPR(x)                            (((uint8_t)(((uint8_t)(x))<<SPI_BR_SPR_SHIFT))&SPI_BR_SPR_MASK)
#define SPI_BR_SPPR_MASK                         0x70u
#define SPI_BR_SPPR_SHIFT                        4
#define SPI_BR_SPPR(x)                           (((uint8_t)(((uint8_t)(x))<<SPI_BR_SPPR_SHIFT))&SPI_BR_SPPR_MASK)
/* C2 Bit Fields */
#define SPI_C2_SPC0_MASK                         0x1u
#define SPI_C2_SPC0_SHIFT                        0
#define SPI_C2_SPISWAI_MASK                      0x2u
#define SPI_C2_SPISWAI_SHIFT                     1
#define SPI_C2_RXDMAE_MASK                       0x4u
#define SPI_C2_RXDMAE_SHIFT                      2
#define SPI_C2_BIDIROE_MASK                      0x8u
#define SPI_C2_BIDIROE_SHIFT                     3
#define SPI_C2_MODFEN_MASK                       0x10u
#define SPI_C2_MODFEN_SHIFT                      4
#define SPI_C2_TXDMAE_MASK                       0x20u
#define SPI_C2_TXDMAE_SHIFT                      5
#define SPI_C2_SPIMODE_MASK                      0x40u
#define SPI_C2_SPIMODE_SHIFT                     6
#define SPI_C2_SPMIE_MASK                        0x80u
#define SPI_C2_SPMIE_SHIFT                       7
/* C1 Bit Fields */
#define SPI_C1_LSBFE_MASK                        0x1u
#define SPI_C1_LSBFE_SHIFT                       0
#define SPI_C1_SSOE_MASK                         0x2u
#define SPI_C1_SSOE_SHIFT                        1
#define SPI_C1_CPHA_MASK                         0x4u
#define SPI_C1_CPHA_SHIFT                        2
#define SPI_C1_CPOL_MASK                         0x8u
#define SPI_C1_CPOL_SHIFT                        3
#define SPI_C1_MSTR_MASK                         0x10u
#define SPI_C1_MSTR_SHIFT                        4
#define SPI_C1_SPTIE_MASK                        0x20u
#define SPI_C1_SPTIE_SHIFT                       5
#define SPI_C1_SPE_MASK                          0x40u
#define SPI_C1_SPE_SHIFT                         6
#define SPI_C1_SPIE_MASK                         0x80u
#define SPI_C1_SPIE_SHIFT                        7
/* ML Bit Fields */
#define SPI_ML_Bits_MASK                         0xFFu
#define SPI_ML_Bits_SHIFT                        0
#define SPI_ML_Bits(x)                           (((uint8_t)(((uint8_t)(x))<<SPI_ML_Bits_SHIFT))&SPI_ML_Bits_MASK)
/* MH Bit Fields */
#define SPI_MH_Bits_MASK                         0xFFu
#define SPI_MH_Bits_SHIFT                        0
#define SPI_MH_Bits(x)                           (((uint8_t)(((uint8_t)(x))<<SPI_MH_Bits_SHIFT))&SPI_MH_Bits_MASK)
/* DL Bit Fields */
#define SPI_DL_Bits_MASK                         0xFFu
#define SPI_DL_Bits_SHIFT                        0
#define SPI_DL_Bits(x)                           (((uint8_t)(((uint8_t)(x))<<SPI_DL_Bits_SHIFT))&SPI_DL_Bits_MASK)
/* DH Bit Fields */
#define SPI_DH_Bits_MASK                         0xFFu
#define SPI_DH_Bits_SHIFT                        0
#define SPI_DH_Bits(x)                           (((uint8_t)(((uint8_t)(x))<<SPI_DH_Bits_SHIFT))&SPI_DH_Bits_MASK)
/* CI Bit Fields */
#define SPI_CI_SPRFCI_MASK                       0x1u
#define SPI_CI_SPRFCI_SHIFT                      0
#define SPI_CI_SPTEFCI_MASK                      0x2u
#define SPI_CI_SPTEFCI_SHIFT                     1
#define SPI_CI_RNFULLFCI_MASK                    0x4u
#define SPI_CI_RNFULLFCI_SHIFT                   2
#define SPI_CI_TNEAREFCI_MASK                    0x8u
#define SPI_CI_TNEAREFCI_SHIFT                   3
#define SPI_CI_RXFOF_MASK                        0x10u
#define SPI_CI_RXFOF_SHIFT                       4
#define SPI_CI_TXFOF_MASK                        0x20u
#define SPI_CI_TXFOF_SHIFT                       5
#define SPI_CI_RXFERR_MASK                       0x40u
#define SPI_CI_RXFERR_SHIFT                      6
#define SPI_CI_TXFERR_MASK                       0x80u
#define SPI_CI_TXFERR_SHIFT                      7
/* C3 Bit Fields */
#define SPI_C3_FIFOMODE_MASK                     0x1u
#define SPI_C3_FIFOMODE_SHIFT                    0
#define SPI_C3_RNFULLIEN_MASK                    0x2u
#define SPI_C3_RNFULLIEN_SHIFT                   1
#define SPI_C3_TNEARIEN_MASK                     0x4u
#define SPI_C3_TNEARIEN_SHIFT                    2
#define SPI_C3_INTCLR_MASK                       0x8u
#define SPI_C3_INTCLR_SHIFT                      3
#define SPI_C3_RNFULLF_MARK_MASK                 0x10u
#define SPI_C3_RNFULLF_MARK_SHIFT                4
#define SPI_C3_TNEAREF_MARK_MASK                 0x20u
#define SPI_C3_TNEAREF_MARK_SHIFT                5

/*!
 * @}
 */ /* end of group SPI_Register_Masks */


/* SPI - Peripheral instance base addresses */
/** Peripheral SPI0 base pointer */
#define SPI0_BASE_PTR                            ((SPI_MemMapPtr)0x40075000u)
/** Peripheral SPI1 base pointer */
#define SPI1_BASE_PTR                            ((SPI_MemMapPtr)0x40076000u)
/** Array initializer of SPI peripheral base pointers */
#define SPI_BASE_PTRS                            { SPI0_BASE_PTR, SPI1_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SPI - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Register_Accessor_Macros SPI - Register accessor macros
 * @{
 */


/* SPI - Register instance definitions */
/* SPI0 */
#define SPI0_S                                   SPI_S_REG(SPI0_BASE_PTR)
#define SPI0_BR                                  SPI_BR_REG(SPI0_BASE_PTR)
#define SPI0_C2                                  SPI_C2_REG(SPI0_BASE_PTR)
#define SPI0_C1                                  SPI_C1_REG(SPI0_BASE_PTR)
#define SPI0_ML                                  SPI_ML_REG(SPI0_BASE_PTR)
#define SPI0_MH                                  SPI_MH_REG(SPI0_BASE_PTR)
#define SPI0_DL                                  SPI_DL_REG(SPI0_BASE_PTR)
#define SPI0_DH                                  SPI_DH_REG(SPI0_BASE_PTR)
#define SPI0_CI                                  SPI_CI_REG(SPI0_BASE_PTR)
#define SPI0_C3                                  SPI_C3_REG(SPI0_BASE_PTR)
/* SPI1 */
#define SPI1_S                                   SPI_S_REG(SPI1_BASE_PTR)
#define SPI1_BR                                  SPI_BR_REG(SPI1_BASE_PTR)
#define SPI1_C2                                  SPI_C2_REG(SPI1_BASE_PTR)
#define SPI1_C1                                  SPI_C1_REG(SPI1_BASE_PTR)
#define SPI1_ML                                  SPI_ML_REG(SPI1_BASE_PTR)
#define SPI1_MH                                  SPI_MH_REG(SPI1_BASE_PTR)
#define SPI1_DL                                  SPI_DL_REG(SPI1_BASE_PTR)
#define SPI1_DH                                  SPI_DH_REG(SPI1_BASE_PTR)
#define SPI1_CI                                  SPI_CI_REG(SPI1_BASE_PTR)
#define SPI1_C3                                  SPI_C3_REG(SPI1_BASE_PTR)

/*!
 * @}
 */ /* end of group SPI_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SPI_Peripheral */


/* ----------------------------------------------------------------------------
   -- SysTick
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SysTick_Peripheral SysTick
 * @{
 */

/** SysTick - Peripheral register structure */
typedef struct SysTick_MemMap {
  uint32_t CSR;                                    /**< SysTick Control and Status Register, offset: 0x0 */
  uint32_t RVR;                                    /**< SysTick Reload Value Register, offset: 0x4 */
  uint32_t CVR;                                    /**< SysTick Current Value Register, offset: 0x8 */
  uint32_t CALIB;                                  /**< SysTick Calibration Value Register, offset: 0xC */
} volatile *SysTick_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SysTick - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SysTick_Register_Accessor_Macros SysTick - Register accessor macros
 * @{
 */


/* SysTick - Register accessors */
#define SysTick_CSR_REG(base)                    ((base)->CSR)
#define SysTick_RVR_REG(base)                    ((base)->RVR)
#define SysTick_CVR_REG(base)                    ((base)->CVR)
#define SysTick_CALIB_REG(base)                  ((base)->CALIB)

/*!
 * @}
 */ /* end of group SysTick_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SysTick Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SysTick_Register_Masks SysTick Register Masks
 * @{
 */

/* CSR Bit Fields */
#define SysTick_CSR_ENABLE_MASK                  0x1u
#define SysTick_CSR_ENABLE_SHIFT                 0
#define SysTick_CSR_TICKINT_MASK                 0x2u
#define SysTick_CSR_TICKINT_SHIFT                1
#define SysTick_CSR_CLKSOURCE_MASK               0x4u
#define SysTick_CSR_CLKSOURCE_SHIFT              2
#define SysTick_CSR_COUNTFLAG_MASK               0x10000u
#define SysTick_CSR_COUNTFLAG_SHIFT              16
/* RVR Bit Fields */
#define SysTick_RVR_RELOAD_MASK                  0xFFFFFFu
#define SysTick_RVR_RELOAD_SHIFT                 0
#define SysTick_RVR_RELOAD(x)                    (((uint32_t)(((uint32_t)(x))<<SysTick_RVR_RELOAD_SHIFT))&SysTick_RVR_RELOAD_MASK)
/* CVR Bit Fields */
#define SysTick_CVR_CURRENT_MASK                 0xFFFFFFu
#define SysTick_CVR_CURRENT_SHIFT                0
#define SysTick_CVR_CURRENT(x)                   (((uint32_t)(((uint32_t)(x))<<SysTick_CVR_CURRENT_SHIFT))&SysTick_CVR_CURRENT_MASK)
/* CALIB Bit Fields */
#define SysTick_CALIB_TENMS_MASK                 0xFFFFFFu
#define SysTick_CALIB_TENMS_SHIFT                0
#define SysTick_CALIB_TENMS(x)                   (((uint32_t)(((uint32_t)(x))<<SysTick_CALIB_TENMS_SHIFT))&SysTick_CALIB_TENMS_MASK)
#define SysTick_CALIB_SKEW_MASK                  0x40000000u
#define SysTick_CALIB_SKEW_SHIFT                 30
#define SysTick_CALIB_NOREF_MASK                 0x80000000u
#define SysTick_CALIB_NOREF_SHIFT                31

/*!
 * @}
 */ /* end of group SysTick_Register_Masks */


/* SysTick - Peripheral instance base addresses */
/** Peripheral SysTick base pointer */
#define SysTick_BASE_PTR                         ((SysTick_MemMapPtr)0xE000E010u)
/** Array initializer of SysTick peripheral base pointers */
#define SysTick_BASE_PTRS                        { SysTick_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SysTick - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SysTick_Register_Accessor_Macros SysTick - Register accessor macros
 * @{
 */


/* SysTick - Register instance definitions */
/* SysTick */
#define SYST_CSR                                 SysTick_CSR_REG(SysTick_BASE_PTR)
#define SYST_RVR                                 SysTick_RVR_REG(SysTick_BASE_PTR)
#define SYST_CVR                                 SysTick_CVR_REG(SysTick_BASE_PTR)
#define SYST_CALIB                               SysTick_CALIB_REG(SysTick_BASE_PTR)

/*!
 * @}
 */ /* end of group SysTick_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SysTick_Peripheral */


/* ----------------------------------------------------------------------------
   -- TMR
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TMR_Peripheral TMR
 * @{
 */

/** TMR - Peripheral register structure */
typedef struct TMR_MemMap {
  uint16_t COMP1;                                  /**< Timer Channel Compare Register 1, offset: 0x0 */
  uint16_t COMP2;                                  /**< Timer Channel Compare Register 2, offset: 0x2 */
  uint16_t CAPT;                                   /**< Timer Channel Capture Register, offset: 0x4 */
  uint16_t LOAD;                                   /**< Timer Channel Load Register, offset: 0x6 */
  uint16_t HOLD;                                   /**< Timer Channel Hold Register, offset: 0x8 */
  uint16_t CNTR;                                   /**< Timer Channel Counter Register, offset: 0xA */
  uint16_t CTRL;                                   /**< Timer Channel Control Register, offset: 0xC */
  uint16_t SCTRL;                                  /**< Timer Channel Status and Control Register, offset: 0xE */
  uint16_t CMPLD1;                                 /**< Timer Channel Comparator Load Register 1, offset: 0x10 */
  uint16_t CMPLD2;                                 /**< Timer Channel Comparator Load Register 2, offset: 0x12 */
  uint16_t CSCTRL;                                 /**< Timer Channel Comparator Status and Control Register, offset: 0x14 */
  uint16_t FILT;                                   /**< Timer Channel Input Filter Register, offset: 0x16 */
  uint8_t RESERVED_0[6];
  uint16_t ENBL;                                   /**< Timer Channel Enable Register, offset: 0x1E */
} volatile *TMR_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- TMR - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TMR_Register_Accessor_Macros TMR - Register accessor macros
 * @{
 */


/* TMR - Register accessors */
#define TMR_COMP1_REG(base)                      ((base)->COMP1)
#define TMR_COMP2_REG(base)                      ((base)->COMP2)
#define TMR_CAPT_REG(base)                       ((base)->CAPT)
#define TMR_LOAD_REG(base)                       ((base)->LOAD)
#define TMR_HOLD_REG(base)                       ((base)->HOLD)
#define TMR_CNTR_REG(base)                       ((base)->CNTR)
#define TMR_CTRL_REG(base)                       ((base)->CTRL)
#define TMR_SCTRL_REG(base)                      ((base)->SCTRL)
#define TMR_CMPLD1_REG(base)                     ((base)->CMPLD1)
#define TMR_CMPLD2_REG(base)                     ((base)->CMPLD2)
#define TMR_CSCTRL_REG(base)                     ((base)->CSCTRL)
#define TMR_FILT_REG(base)                       ((base)->FILT)
#define TMR_ENBL_REG(base)                       ((base)->ENBL)

/*!
 * @}
 */ /* end of group TMR_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- TMR Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TMR_Register_Masks TMR Register Masks
 * @{
 */

/* COMP1 Bit Fields */
#define TMR_COMP1_COMPARISON_1_MASK              0xFFFFu
#define TMR_COMP1_COMPARISON_1_SHIFT             0
#define TMR_COMP1_COMPARISON_1(x)                (((uint16_t)(((uint16_t)(x))<<TMR_COMP1_COMPARISON_1_SHIFT))&TMR_COMP1_COMPARISON_1_MASK)
/* COMP2 Bit Fields */
#define TMR_COMP2_COMPARISON_2_MASK              0xFFFFu
#define TMR_COMP2_COMPARISON_2_SHIFT             0
#define TMR_COMP2_COMPARISON_2(x)                (((uint16_t)(((uint16_t)(x))<<TMR_COMP2_COMPARISON_2_SHIFT))&TMR_COMP2_COMPARISON_2_MASK)
/* CAPT Bit Fields */
#define TMR_CAPT_CAPTURE_MASK                    0xFFFFu
#define TMR_CAPT_CAPTURE_SHIFT                   0
#define TMR_CAPT_CAPTURE(x)                      (((uint16_t)(((uint16_t)(x))<<TMR_CAPT_CAPTURE_SHIFT))&TMR_CAPT_CAPTURE_MASK)
/* LOAD Bit Fields */
#define TMR_LOAD_LOAD_MASK                       0xFFFFu
#define TMR_LOAD_LOAD_SHIFT                      0
#define TMR_LOAD_LOAD(x)                         (((uint16_t)(((uint16_t)(x))<<TMR_LOAD_LOAD_SHIFT))&TMR_LOAD_LOAD_MASK)
/* HOLD Bit Fields */
#define TMR_HOLD_HOLD_MASK                       0xFFFFu
#define TMR_HOLD_HOLD_SHIFT                      0
#define TMR_HOLD_HOLD(x)                         (((uint16_t)(((uint16_t)(x))<<TMR_HOLD_HOLD_SHIFT))&TMR_HOLD_HOLD_MASK)
/* CNTR Bit Fields */
#define TMR_CNTR_COUNTER_MASK                    0xFFFFu
#define TMR_CNTR_COUNTER_SHIFT                   0
#define TMR_CNTR_COUNTER(x)                      (((uint16_t)(((uint16_t)(x))<<TMR_CNTR_COUNTER_SHIFT))&TMR_CNTR_COUNTER_MASK)
/* CTRL Bit Fields */
#define TMR_CTRL_OUTMODE_MASK                    0x7u
#define TMR_CTRL_OUTMODE_SHIFT                   0
#define TMR_CTRL_OUTMODE(x)                      (((uint16_t)(((uint16_t)(x))<<TMR_CTRL_OUTMODE_SHIFT))&TMR_CTRL_OUTMODE_MASK)
#define TMR_CTRL_COINIT_MASK                     0x8u
#define TMR_CTRL_COINIT_SHIFT                    3
#define TMR_CTRL_DIR_MASK                        0x10u
#define TMR_CTRL_DIR_SHIFT                       4
#define TMR_CTRL_LENGTH_MASK                     0x20u
#define TMR_CTRL_LENGTH_SHIFT                    5
#define TMR_CTRL_ONCE_MASK                       0x40u
#define TMR_CTRL_ONCE_SHIFT                      6
#define TMR_CTRL_SCS_MASK                        0x180u
#define TMR_CTRL_SCS_SHIFT                       7
#define TMR_CTRL_SCS(x)                          (((uint16_t)(((uint16_t)(x))<<TMR_CTRL_SCS_SHIFT))&TMR_CTRL_SCS_MASK)
#define TMR_CTRL_PCS_MASK                        0x1E00u
#define TMR_CTRL_PCS_SHIFT                       9
#define TMR_CTRL_PCS(x)                          (((uint16_t)(((uint16_t)(x))<<TMR_CTRL_PCS_SHIFT))&TMR_CTRL_PCS_MASK)
#define TMR_CTRL_CM_MASK                         0xE000u
#define TMR_CTRL_CM_SHIFT                        13
#define TMR_CTRL_CM(x)                           (((uint16_t)(((uint16_t)(x))<<TMR_CTRL_CM_SHIFT))&TMR_CTRL_CM_MASK)
/* SCTRL Bit Fields */
#define TMR_SCTRL_OEN_MASK                       0x1u
#define TMR_SCTRL_OEN_SHIFT                      0
#define TMR_SCTRL_OPS_MASK                       0x2u
#define TMR_SCTRL_OPS_SHIFT                      1
#define TMR_SCTRL_FORCE_MASK                     0x4u
#define TMR_SCTRL_FORCE_SHIFT                    2
#define TMR_SCTRL_VAL_MASK                       0x8u
#define TMR_SCTRL_VAL_SHIFT                      3
#define TMR_SCTRL_EEOF_MASK                      0x10u
#define TMR_SCTRL_EEOF_SHIFT                     4
#define TMR_SCTRL_MSTR_MASK                      0x20u
#define TMR_SCTRL_MSTR_SHIFT                     5
#define TMR_SCTRL_CAPTURE_MODE_MASK              0xC0u
#define TMR_SCTRL_CAPTURE_MODE_SHIFT             6
#define TMR_SCTRL_CAPTURE_MODE(x)                (((uint16_t)(((uint16_t)(x))<<TMR_SCTRL_CAPTURE_MODE_SHIFT))&TMR_SCTRL_CAPTURE_MODE_MASK)
#define TMR_SCTRL_INPUT_MASK                     0x100u
#define TMR_SCTRL_INPUT_SHIFT                    8
#define TMR_SCTRL_IPS_MASK                       0x200u
#define TMR_SCTRL_IPS_SHIFT                      9
#define TMR_SCTRL_IEFIE_MASK                     0x400u
#define TMR_SCTRL_IEFIE_SHIFT                    10
#define TMR_SCTRL_IEF_MASK                       0x800u
#define TMR_SCTRL_IEF_SHIFT                      11
#define TMR_SCTRL_TOFIE_MASK                     0x1000u
#define TMR_SCTRL_TOFIE_SHIFT                    12
#define TMR_SCTRL_TOF_MASK                       0x2000u
#define TMR_SCTRL_TOF_SHIFT                      13
#define TMR_SCTRL_TCFIE_MASK                     0x4000u
#define TMR_SCTRL_TCFIE_SHIFT                    14
#define TMR_SCTRL_TCF_MASK                       0x8000u
#define TMR_SCTRL_TCF_SHIFT                      15
/* CMPLD1 Bit Fields */
#define TMR_CMPLD1_COMPARATOR_LOAD_1_MASK        0xFFFFu
#define TMR_CMPLD1_COMPARATOR_LOAD_1_SHIFT       0
#define TMR_CMPLD1_COMPARATOR_LOAD_1(x)          (((uint16_t)(((uint16_t)(x))<<TMR_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR_CMPLD1_COMPARATOR_LOAD_1_MASK)
/* CMPLD2 Bit Fields */
#define TMR_CMPLD2_COMPARATOR_LOAD_2_MASK        0xFFFFu
#define TMR_CMPLD2_COMPARATOR_LOAD_2_SHIFT       0
#define TMR_CMPLD2_COMPARATOR_LOAD_2(x)          (((uint16_t)(((uint16_t)(x))<<TMR_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR_CMPLD2_COMPARATOR_LOAD_2_MASK)
/* CSCTRL Bit Fields */
#define TMR_CSCTRL_CL1_MASK                      0x3u
#define TMR_CSCTRL_CL1_SHIFT                     0
#define TMR_CSCTRL_CL1(x)                        (((uint16_t)(((uint16_t)(x))<<TMR_CSCTRL_CL1_SHIFT))&TMR_CSCTRL_CL1_MASK)
#define TMR_CSCTRL_CL2_MASK                      0xCu
#define TMR_CSCTRL_CL2_SHIFT                     2
#define TMR_CSCTRL_CL2(x)                        (((uint16_t)(((uint16_t)(x))<<TMR_CSCTRL_CL2_SHIFT))&TMR_CSCTRL_CL2_MASK)
#define TMR_CSCTRL_TCF1_MASK                     0x10u
#define TMR_CSCTRL_TCF1_SHIFT                    4
#define TMR_CSCTRL_TCF2_MASK                     0x20u
#define TMR_CSCTRL_TCF2_SHIFT                    5
#define TMR_CSCTRL_TCF1EN_MASK                   0x40u
#define TMR_CSCTRL_TCF1EN_SHIFT                  6
#define TMR_CSCTRL_TCF2EN_MASK                   0x80u
#define TMR_CSCTRL_TCF2EN_SHIFT                  7
#define TMR_CSCTRL_UP_MASK                       0x200u
#define TMR_CSCTRL_UP_SHIFT                      9
#define TMR_CSCTRL_TCI_MASK                      0x400u
#define TMR_CSCTRL_TCI_SHIFT                     10
#define TMR_CSCTRL_ROC_MASK                      0x800u
#define TMR_CSCTRL_ROC_SHIFT                     11
#define TMR_CSCTRL_ALT_LOAD_MASK                 0x1000u
#define TMR_CSCTRL_ALT_LOAD_SHIFT                12
#define TMR_CSCTRL_FAULT_MASK                    0x2000u
#define TMR_CSCTRL_FAULT_SHIFT                   13
#define TMR_CSCTRL_DBG_EN_MASK                   0xC000u
#define TMR_CSCTRL_DBG_EN_SHIFT                  14
#define TMR_CSCTRL_DBG_EN(x)                     (((uint16_t)(((uint16_t)(x))<<TMR_CSCTRL_DBG_EN_SHIFT))&TMR_CSCTRL_DBG_EN_MASK)
/* FILT Bit Fields */
#define TMR_FILT_FILT_PER_MASK                   0xFFu
#define TMR_FILT_FILT_PER_SHIFT                  0
#define TMR_FILT_FILT_PER(x)                     (((uint16_t)(((uint16_t)(x))<<TMR_FILT_FILT_PER_SHIFT))&TMR_FILT_FILT_PER_MASK)
#define TMR_FILT_FILT_CNT_MASK                   0x700u
#define TMR_FILT_FILT_CNT_SHIFT                  8
#define TMR_FILT_FILT_CNT(x)                     (((uint16_t)(((uint16_t)(x))<<TMR_FILT_FILT_CNT_SHIFT))&TMR_FILT_FILT_CNT_MASK)
/* ENBL Bit Fields */
#define TMR_ENBL_ENBL_MASK                       0xFu
#define TMR_ENBL_ENBL_SHIFT                      0
#define TMR_ENBL_ENBL(x)                         (((uint16_t)(((uint16_t)(x))<<TMR_ENBL_ENBL_SHIFT))&TMR_ENBL_ENBL_MASK)

/*!
 * @}
 */ /* end of group TMR_Register_Masks */


/* TMR - Peripheral instance base addresses */
/** Peripheral TMR0 base pointer */
#define TMR0_BASE_PTR                            ((TMR_MemMapPtr)0x40057000u)
/** Peripheral TMR1 base pointer */
#define TMR1_BASE_PTR                            ((TMR_MemMapPtr)0x40058000u)
/** Peripheral TMR2 base pointer */
#define TMR2_BASE_PTR                            ((TMR_MemMapPtr)0x40059000u)
/** Peripheral TMR3 base pointer */
#define TMR3_BASE_PTR                            ((TMR_MemMapPtr)0x4005A000u)
/** Array initializer of TMR peripheral base pointers */
#define TMR_BASE_PTRS                            { TMR0_BASE_PTR, TMR1_BASE_PTR, TMR2_BASE_PTR, TMR3_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- TMR - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TMR_Register_Accessor_Macros TMR - Register accessor macros
 * @{
 */


/* TMR - Register instance definitions */
/* TMR0 */
#define TMR0_COMP1                               TMR_COMP1_REG(TMR0_BASE_PTR)
#define TMR0_COMP2                               TMR_COMP2_REG(TMR0_BASE_PTR)
#define TMR0_CAPT                                TMR_CAPT_REG(TMR0_BASE_PTR)
#define TMR0_LOAD                                TMR_LOAD_REG(TMR0_BASE_PTR)
#define TMR0_HOLD                                TMR_HOLD_REG(TMR0_BASE_PTR)
#define TMR0_CNTR                                TMR_CNTR_REG(TMR0_BASE_PTR)
#define TMR0_CTRL                                TMR_CTRL_REG(TMR0_BASE_PTR)
#define TMR0_SCTRL                               TMR_SCTRL_REG(TMR0_BASE_PTR)
#define TMR0_CMPLD1                              TMR_CMPLD1_REG(TMR0_BASE_PTR)
#define TMR0_CMPLD2                              TMR_CMPLD2_REG(TMR0_BASE_PTR)
#define TMR0_CSCTRL                              TMR_CSCTRL_REG(TMR0_BASE_PTR)
#define TMR0_FILT                                TMR_FILT_REG(TMR0_BASE_PTR)
#define TMR0_ENBL                                TMR_ENBL_REG(TMR0_BASE_PTR)
/* TMR1 */
#define TMR1_COMP1                               TMR_COMP1_REG(TMR1_BASE_PTR)
#define TMR1_COMP2                               TMR_COMP2_REG(TMR1_BASE_PTR)
#define TMR1_CAPT                                TMR_CAPT_REG(TMR1_BASE_PTR)
#define TMR1_LOAD                                TMR_LOAD_REG(TMR1_BASE_PTR)
#define TMR1_HOLD                                TMR_HOLD_REG(TMR1_BASE_PTR)
#define TMR1_CNTR                                TMR_CNTR_REG(TMR1_BASE_PTR)
#define TMR1_CTRL                                TMR_CTRL_REG(TMR1_BASE_PTR)
#define TMR1_SCTRL                               TMR_SCTRL_REG(TMR1_BASE_PTR)
#define TMR1_CMPLD1                              TMR_CMPLD1_REG(TMR1_BASE_PTR)
#define TMR1_CMPLD2                              TMR_CMPLD2_REG(TMR1_BASE_PTR)
#define TMR1_CSCTRL                              TMR_CSCTRL_REG(TMR1_BASE_PTR)
#define TMR1_FILT                                TMR_FILT_REG(TMR1_BASE_PTR)
/* TMR2 */
#define TMR2_COMP1                               TMR_COMP1_REG(TMR2_BASE_PTR)
#define TMR2_COMP2                               TMR_COMP2_REG(TMR2_BASE_PTR)
#define TMR2_CAPT                                TMR_CAPT_REG(TMR2_BASE_PTR)
#define TMR2_LOAD                                TMR_LOAD_REG(TMR2_BASE_PTR)
#define TMR2_HOLD                                TMR_HOLD_REG(TMR2_BASE_PTR)
#define TMR2_CNTR                                TMR_CNTR_REG(TMR2_BASE_PTR)
#define TMR2_CTRL                                TMR_CTRL_REG(TMR2_BASE_PTR)
#define TMR2_SCTRL                               TMR_SCTRL_REG(TMR2_BASE_PTR)
#define TMR2_CMPLD1                              TMR_CMPLD1_REG(TMR2_BASE_PTR)
#define TMR2_CMPLD2                              TMR_CMPLD2_REG(TMR2_BASE_PTR)
#define TMR2_CSCTRL                              TMR_CSCTRL_REG(TMR2_BASE_PTR)
#define TMR2_FILT                                TMR_FILT_REG(TMR2_BASE_PTR)
/* TMR3 */
#define TMR3_COMP1                               TMR_COMP1_REG(TMR3_BASE_PTR)
#define TMR3_COMP2                               TMR_COMP2_REG(TMR3_BASE_PTR)
#define TMR3_CAPT                                TMR_CAPT_REG(TMR3_BASE_PTR)
#define TMR3_LOAD                                TMR_LOAD_REG(TMR3_BASE_PTR)
#define TMR3_HOLD                                TMR_HOLD_REG(TMR3_BASE_PTR)
#define TMR3_CNTR                                TMR_CNTR_REG(TMR3_BASE_PTR)
#define TMR3_CTRL                                TMR_CTRL_REG(TMR3_BASE_PTR)
#define TMR3_SCTRL                               TMR_SCTRL_REG(TMR3_BASE_PTR)
#define TMR3_CMPLD1                              TMR_CMPLD1_REG(TMR3_BASE_PTR)
#define TMR3_CMPLD2                              TMR_CMPLD2_REG(TMR3_BASE_PTR)
#define TMR3_CSCTRL                              TMR_CSCTRL_REG(TMR3_BASE_PTR)
#define TMR3_FILT                                TMR_FILT_REG(TMR3_BASE_PTR)

/*!
 * @}
 */ /* end of group TMR_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group TMR_Peripheral */


/* ----------------------------------------------------------------------------
   -- UART
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART_Peripheral UART
 * @{
 */

/** UART - Peripheral register structure */
typedef struct UART_MemMap {
  uint8_t BDH;                                     /**< UART Baud Rate Registers: High, offset: 0x0 */
  uint8_t BDL;                                     /**< UART Baud Rate Registers: Low, offset: 0x1 */
  uint8_t C1;                                      /**< UART Control Register 1, offset: 0x2 */
  uint8_t C2;                                      /**< UART Control Register 2, offset: 0x3 */
  uint8_t S1;                                      /**< UART Status Register 1, offset: 0x4 */
  uint8_t S2;                                      /**< UART Status Register 2, offset: 0x5 */
  uint8_t C3;                                      /**< UART Control Register 3, offset: 0x6 */
  uint8_t D;                                       /**< UART Data Register, offset: 0x7 */
  uint8_t MA1;                                     /**< UART Match Address Registers 1, offset: 0x8 */
  uint8_t MA2;                                     /**< UART Match Address Registers 2, offset: 0x9 */
  uint8_t C4;                                      /**< UART Control Register 4, offset: 0xA */
  uint8_t C5;                                      /**< UART Control Register 5, offset: 0xB */
  uint8_t ED;                                      /**< UART Extended Data Register, offset: 0xC */
  uint8_t MODEM;                                   /**< UART Modem Register, offset: 0xD */
  uint8_t RESERVED_0[2];
  uint8_t PFIFO;                                   /**< UART FIFO Parameters, offset: 0x10 */
  uint8_t CFIFO;                                   /**< UART FIFO Control Register, offset: 0x11 */
  uint8_t SFIFO;                                   /**< UART FIFO Status Register, offset: 0x12 */
  uint8_t TWFIFO;                                  /**< UART FIFO Transmit Watermark, offset: 0x13 */
  uint8_t TCFIFO;                                  /**< UART FIFO Transmit Count, offset: 0x14 */
  uint8_t RWFIFO;                                  /**< UART FIFO Receive Watermark, offset: 0x15 */
  uint8_t RCFIFO;                                  /**< UART FIFO Receive Count, offset: 0x16 */
  uint8_t RESERVED_1[1];
  uint8_t C7816;                                   /**< UART 7816 Control Register, offset: 0x18 */
  uint8_t IE7816;                                  /**< UART 7816 Interrupt Enable Register, offset: 0x19 */
  uint8_t IS7816;                                  /**< UART 7816 Interrupt Status Register, offset: 0x1A */
  union {                                          /* offset: 0x1B */
    uint8_t WP7816_T_TYPE0;                          /**< UART 7816 Wait Parameter Register, offset: 0x1B */
    uint8_t WP7816_T_TYPE1;                          /**< UART 7816 Wait Parameter Register, offset: 0x1B */
  };
  uint8_t WN7816;                                  /**< UART 7816 Wait N Register, offset: 0x1C */
  uint8_t WF7816;                                  /**< UART 7816 Wait FD Register, offset: 0x1D */
  uint8_t ET7816;                                  /**< UART 7816 Error Threshold Register, offset: 0x1E */
  uint8_t TL7816;                                  /**< UART 7816 Transmit Length Register, offset: 0x1F */
} volatile *UART_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- UART - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART_Register_Accessor_Macros UART - Register accessor macros
 * @{
 */


/* UART - Register accessors */
#define UART_BDH_REG(base)                       ((base)->BDH)
#define UART_BDL_REG(base)                       ((base)->BDL)
#define UART_C1_REG(base)                        ((base)->C1)
#define UART_C2_REG(base)                        ((base)->C2)
#define UART_S1_REG(base)                        ((base)->S1)
#define UART_S2_REG(base)                        ((base)->S2)
#define UART_C3_REG(base)                        ((base)->C3)
#define UART_D_REG(base)                         ((base)->D)
#define UART_MA1_REG(base)                       ((base)->MA1)
#define UART_MA2_REG(base)                       ((base)->MA2)
#define UART_C4_REG(base)                        ((base)->C4)
#define UART_C5_REG(base)                        ((base)->C5)
#define UART_ED_REG(base)                        ((base)->ED)
#define UART_MODEM_REG(base)                     ((base)->MODEM)
#define UART_PFIFO_REG(base)                     ((base)->PFIFO)
#define UART_CFIFO_REG(base)                     ((base)->CFIFO)
#define UART_SFIFO_REG(base)                     ((base)->SFIFO)
#define UART_TWFIFO_REG(base)                    ((base)->TWFIFO)
#define UART_TCFIFO_REG(base)                    ((base)->TCFIFO)
#define UART_RWFIFO_REG(base)                    ((base)->RWFIFO)
#define UART_RCFIFO_REG(base)                    ((base)->RCFIFO)
#define UART_C7816_REG(base)                     ((base)->C7816)
#define UART_IE7816_REG(base)                    ((base)->IE7816)
#define UART_IS7816_REG(base)                    ((base)->IS7816)
#define UART_WP7816_T_TYPE0_REG(base)            ((base)->WP7816_T_TYPE0)
#define UART_WP7816_T_TYPE1_REG(base)            ((base)->WP7816_T_TYPE1)
#define UART_WN7816_REG(base)                    ((base)->WN7816)
#define UART_WF7816_REG(base)                    ((base)->WF7816)
#define UART_ET7816_REG(base)                    ((base)->ET7816)
#define UART_TL7816_REG(base)                    ((base)->TL7816)

/*!
 * @}
 */ /* end of group UART_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- UART Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART_Register_Masks UART Register Masks
 * @{
 */

/* BDH Bit Fields */
#define UART_BDH_SBR_MASK                        0x1Fu
#define UART_BDH_SBR_SHIFT                       0
#define UART_BDH_SBR(x)                          (((uint8_t)(((uint8_t)(x))<<UART_BDH_SBR_SHIFT))&UART_BDH_SBR_MASK)
#define UART_BDH_RXEDGIE_MASK                    0x40u
#define UART_BDH_RXEDGIE_SHIFT                   6
/* BDL Bit Fields */
#define UART_BDL_SBR_MASK                        0xFFu
#define UART_BDL_SBR_SHIFT                       0
#define UART_BDL_SBR(x)                          (((uint8_t)(((uint8_t)(x))<<UART_BDL_SBR_SHIFT))&UART_BDL_SBR_MASK)
/* C1 Bit Fields */
#define UART_C1_PT_MASK                          0x1u
#define UART_C1_PT_SHIFT                         0
#define UART_C1_PE_MASK                          0x2u
#define UART_C1_PE_SHIFT                         1
#define UART_C1_ILT_MASK                         0x4u
#define UART_C1_ILT_SHIFT                        2
#define UART_C1_WAKE_MASK                        0x8u
#define UART_C1_WAKE_SHIFT                       3
#define UART_C1_M_MASK                           0x10u
#define UART_C1_M_SHIFT                          4
#define UART_C1_RSRC_MASK                        0x20u
#define UART_C1_RSRC_SHIFT                       5
#define UART_C1_LOOPS_MASK                       0x80u
#define UART_C1_LOOPS_SHIFT                      7
/* C2 Bit Fields */
#define UART_C2_SBK_MASK                         0x1u
#define UART_C2_SBK_SHIFT                        0
#define UART_C2_RWU_MASK                         0x2u
#define UART_C2_RWU_SHIFT                        1
#define UART_C2_RE_MASK                          0x4u
#define UART_C2_RE_SHIFT                         2
#define UART_C2_TE_MASK                          0x8u
#define UART_C2_TE_SHIFT                         3
#define UART_C2_ILIE_MASK                        0x10u
#define UART_C2_ILIE_SHIFT                       4
#define UART_C2_RIE_MASK                         0x20u
#define UART_C2_RIE_SHIFT                        5
#define UART_C2_TCIE_MASK                        0x40u
#define UART_C2_TCIE_SHIFT                       6
#define UART_C2_TIE_MASK                         0x80u
#define UART_C2_TIE_SHIFT                        7
/* S1 Bit Fields */
#define UART_S1_PF_MASK                          0x1u
#define UART_S1_PF_SHIFT                         0
#define UART_S1_FE_MASK                          0x2u
#define UART_S1_FE_SHIFT                         1
#define UART_S1_NF_MASK                          0x4u
#define UART_S1_NF_SHIFT                         2
#define UART_S1_OR_MASK                          0x8u
#define UART_S1_OR_SHIFT                         3
#define UART_S1_IDLE_MASK                        0x10u
#define UART_S1_IDLE_SHIFT                       4
#define UART_S1_RDRF_MASK                        0x20u
#define UART_S1_RDRF_SHIFT                       5
#define UART_S1_TC_MASK                          0x40u
#define UART_S1_TC_SHIFT                         6
#define UART_S1_TDRE_MASK                        0x80u
#define UART_S1_TDRE_SHIFT                       7
/* S2 Bit Fields */
#define UART_S2_RAF_MASK                         0x1u
#define UART_S2_RAF_SHIFT                        0
#define UART_S2_BRK13_MASK                       0x4u
#define UART_S2_BRK13_SHIFT                      2
#define UART_S2_RWUID_MASK                       0x8u
#define UART_S2_RWUID_SHIFT                      3
#define UART_S2_RXINV_MASK                       0x10u
#define UART_S2_RXINV_SHIFT                      4
#define UART_S2_MSBF_MASK                        0x20u
#define UART_S2_MSBF_SHIFT                       5
#define UART_S2_RXEDGIF_MASK                     0x40u
#define UART_S2_RXEDGIF_SHIFT                    6
/* C3 Bit Fields */
#define UART_C3_PEIE_MASK                        0x1u
#define UART_C3_PEIE_SHIFT                       0
#define UART_C3_FEIE_MASK                        0x2u
#define UART_C3_FEIE_SHIFT                       1
#define UART_C3_NEIE_MASK                        0x4u
#define UART_C3_NEIE_SHIFT                       2
#define UART_C3_ORIE_MASK                        0x8u
#define UART_C3_ORIE_SHIFT                       3
#define UART_C3_TXINV_MASK                       0x10u
#define UART_C3_TXINV_SHIFT                      4
#define UART_C3_TXDIR_MASK                       0x20u
#define UART_C3_TXDIR_SHIFT                      5
#define UART_C3_T8_MASK                          0x40u
#define UART_C3_T8_SHIFT                         6
#define UART_C3_R8_MASK                          0x80u
#define UART_C3_R8_SHIFT                         7
/* D Bit Fields */
#define UART_D_RT_MASK                           0xFFu
#define UART_D_RT_SHIFT                          0
#define UART_D_RT(x)                             (((uint8_t)(((uint8_t)(x))<<UART_D_RT_SHIFT))&UART_D_RT_MASK)
/* MA1 Bit Fields */
#define UART_MA1_MA_MASK                         0xFFu
#define UART_MA1_MA_SHIFT                        0
#define UART_MA1_MA(x)                           (((uint8_t)(((uint8_t)(x))<<UART_MA1_MA_SHIFT))&UART_MA1_MA_MASK)
/* MA2 Bit Fields */
#define UART_MA2_MA_MASK                         0xFFu
#define UART_MA2_MA_SHIFT                        0
#define UART_MA2_MA(x)                           (((uint8_t)(((uint8_t)(x))<<UART_MA2_MA_SHIFT))&UART_MA2_MA_MASK)
/* C4 Bit Fields */
#define UART_C4_BRFA_MASK                        0x1Fu
#define UART_C4_BRFA_SHIFT                       0
#define UART_C4_BRFA(x)                          (((uint8_t)(((uint8_t)(x))<<UART_C4_BRFA_SHIFT))&UART_C4_BRFA_MASK)
#define UART_C4_M10_MASK                         0x20u
#define UART_C4_M10_SHIFT                        5
#define UART_C4_MAEN2_MASK                       0x40u
#define UART_C4_MAEN2_SHIFT                      6
#define UART_C4_MAEN1_MASK                       0x80u
#define UART_C4_MAEN1_SHIFT                      7
/* C5 Bit Fields */
#define UART_C5_RDMAS_MASK                       0x20u
#define UART_C5_RDMAS_SHIFT                      5
#define UART_C5_TDMAS_MASK                       0x80u
#define UART_C5_TDMAS_SHIFT                      7
/* ED Bit Fields */
#define UART_ED_PARITYE_MASK                     0x40u
#define UART_ED_PARITYE_SHIFT                    6
#define UART_ED_NOISY_MASK                       0x80u
#define UART_ED_NOISY_SHIFT                      7
/* MODEM Bit Fields */
#define UART_MODEM_TXCTSE_MASK                   0x1u
#define UART_MODEM_TXCTSE_SHIFT                  0
#define UART_MODEM_TXRTSE_MASK                   0x2u
#define UART_MODEM_TXRTSE_SHIFT                  1
#define UART_MODEM_TXRTSPOL_MASK                 0x4u
#define UART_MODEM_TXRTSPOL_SHIFT                2
#define UART_MODEM_RXRTSE_MASK                   0x8u
#define UART_MODEM_RXRTSE_SHIFT                  3
/* PFIFO Bit Fields */
#define UART_PFIFO_RXFIFOSIZE_MASK               0x7u
#define UART_PFIFO_RXFIFOSIZE_SHIFT              0
#define UART_PFIFO_RXFIFOSIZE(x)                 (((uint8_t)(((uint8_t)(x))<<UART_PFIFO_RXFIFOSIZE_SHIFT))&UART_PFIFO_RXFIFOSIZE_MASK)
#define UART_PFIFO_RXFE_MASK                     0x8u
#define UART_PFIFO_RXFE_SHIFT                    3
#define UART_PFIFO_TXFIFOSIZE_MASK               0x70u
#define UART_PFIFO_TXFIFOSIZE_SHIFT              4
#define UART_PFIFO_TXFIFOSIZE(x)                 (((uint8_t)(((uint8_t)(x))<<UART_PFIFO_TXFIFOSIZE_SHIFT))&UART_PFIFO_TXFIFOSIZE_MASK)
#define UART_PFIFO_TXFE_MASK                     0x80u
#define UART_PFIFO_TXFE_SHIFT                    7
/* CFIFO Bit Fields */
#define UART_CFIFO_RXUFE_MASK                    0x1u
#define UART_CFIFO_RXUFE_SHIFT                   0
#define UART_CFIFO_TXOFE_MASK                    0x2u
#define UART_CFIFO_TXOFE_SHIFT                   1
#define UART_CFIFO_RXOFE_MASK                    0x4u
#define UART_CFIFO_RXOFE_SHIFT                   2
#define UART_CFIFO_RXFLUSH_MASK                  0x40u
#define UART_CFIFO_RXFLUSH_SHIFT                 6
#define UART_CFIFO_TXFLUSH_MASK                  0x80u
#define UART_CFIFO_TXFLUSH_SHIFT                 7
/* SFIFO Bit Fields */
#define UART_SFIFO_RXUF_MASK                     0x1u
#define UART_SFIFO_RXUF_SHIFT                    0
#define UART_SFIFO_TXOF_MASK                     0x2u
#define UART_SFIFO_TXOF_SHIFT                    1
#define UART_SFIFO_RXOF_MASK                     0x4u
#define UART_SFIFO_RXOF_SHIFT                    2
#define UART_SFIFO_RXEMPT_MASK                   0x40u
#define UART_SFIFO_RXEMPT_SHIFT                  6
#define UART_SFIFO_TXEMPT_MASK                   0x80u
#define UART_SFIFO_TXEMPT_SHIFT                  7
/* TWFIFO Bit Fields */
#define UART_TWFIFO_TXWATER_MASK                 0xFFu
#define UART_TWFIFO_TXWATER_SHIFT                0
#define UART_TWFIFO_TXWATER(x)                   (((uint8_t)(((uint8_t)(x))<<UART_TWFIFO_TXWATER_SHIFT))&UART_TWFIFO_TXWATER_MASK)
/* TCFIFO Bit Fields */
#define UART_TCFIFO_TXCOUNT_MASK                 0xFFu
#define UART_TCFIFO_TXCOUNT_SHIFT                0
#define UART_TCFIFO_TXCOUNT(x)                   (((uint8_t)(((uint8_t)(x))<<UART_TCFIFO_TXCOUNT_SHIFT))&UART_TCFIFO_TXCOUNT_MASK)
/* RWFIFO Bit Fields */
#define UART_RWFIFO_RXWATER_MASK                 0xFFu
#define UART_RWFIFO_RXWATER_SHIFT                0
#define UART_RWFIFO_RXWATER(x)                   (((uint8_t)(((uint8_t)(x))<<UART_RWFIFO_RXWATER_SHIFT))&UART_RWFIFO_RXWATER_MASK)
/* RCFIFO Bit Fields */
#define UART_RCFIFO_RXCOUNT_MASK                 0xFFu
#define UART_RCFIFO_RXCOUNT_SHIFT                0
#define UART_RCFIFO_RXCOUNT(x)                   (((uint8_t)(((uint8_t)(x))<<UART_RCFIFO_RXCOUNT_SHIFT))&UART_RCFIFO_RXCOUNT_MASK)
/* C7816 Bit Fields */
#define UART_C7816_ISO_7816E_MASK                0x1u
#define UART_C7816_ISO_7816E_SHIFT               0
#define UART_C7816_TTYPE_MASK                    0x2u
#define UART_C7816_TTYPE_SHIFT                   1
#define UART_C7816_INIT_MASK                     0x4u
#define UART_C7816_INIT_SHIFT                    2
#define UART_C7816_ANACK_MASK                    0x8u
#define UART_C7816_ANACK_SHIFT                   3
#define UART_C7816_ONACK_MASK                    0x10u
#define UART_C7816_ONACK_SHIFT                   4
/* IE7816 Bit Fields */
#define UART_IE7816_RXTE_MASK                    0x1u
#define UART_IE7816_RXTE_SHIFT                   0
#define UART_IE7816_TXTE_MASK                    0x2u
#define UART_IE7816_TXTE_SHIFT                   1
#define UART_IE7816_GTVE_MASK                    0x4u
#define UART_IE7816_GTVE_SHIFT                   2
#define UART_IE7816_INITDE_MASK                  0x10u
#define UART_IE7816_INITDE_SHIFT                 4
#define UART_IE7816_BWTE_MASK                    0x20u
#define UART_IE7816_BWTE_SHIFT                   5
#define UART_IE7816_CWTE_MASK                    0x40u
#define UART_IE7816_CWTE_SHIFT                   6
#define UART_IE7816_WTE_MASK                     0x80u
#define UART_IE7816_WTE_SHIFT                    7
/* IS7816 Bit Fields */
#define UART_IS7816_RXT_MASK                     0x1u
#define UART_IS7816_RXT_SHIFT                    0
#define UART_IS7816_TXT_MASK                     0x2u
#define UART_IS7816_TXT_SHIFT                    1
#define UART_IS7816_GTV_MASK                     0x4u
#define UART_IS7816_GTV_SHIFT                    2
#define UART_IS7816_INITD_MASK                   0x10u
#define UART_IS7816_INITD_SHIFT                  4
#define UART_IS7816_BWT_MASK                     0x20u
#define UART_IS7816_BWT_SHIFT                    5
#define UART_IS7816_CWT_MASK                     0x40u
#define UART_IS7816_CWT_SHIFT                    6
#define UART_IS7816_WT_MASK                      0x80u
#define UART_IS7816_WT_SHIFT                     7
/* WP7816_T_TYPE0 Bit Fields */
#define UART_WP7816_T_TYPE0_WI_MASK              0xFFu
#define UART_WP7816_T_TYPE0_WI_SHIFT             0
#define UART_WP7816_T_TYPE0_WI(x)                (((uint8_t)(((uint8_t)(x))<<UART_WP7816_T_TYPE0_WI_SHIFT))&UART_WP7816_T_TYPE0_WI_MASK)
/* WP7816_T_TYPE1 Bit Fields */
#define UART_WP7816_T_TYPE1_BWI_MASK             0xFu
#define UART_WP7816_T_TYPE1_BWI_SHIFT            0
#define UART_WP7816_T_TYPE1_BWI(x)               (((uint8_t)(((uint8_t)(x))<<UART_WP7816_T_TYPE1_BWI_SHIFT))&UART_WP7816_T_TYPE1_BWI_MASK)
#define UART_WP7816_T_TYPE1_CWI_MASK             0xF0u
#define UART_WP7816_T_TYPE1_CWI_SHIFT            4
#define UART_WP7816_T_TYPE1_CWI(x)               (((uint8_t)(((uint8_t)(x))<<UART_WP7816_T_TYPE1_CWI_SHIFT))&UART_WP7816_T_TYPE1_CWI_MASK)
/* WN7816 Bit Fields */
#define UART_WN7816_GTN_MASK                     0xFFu
#define UART_WN7816_GTN_SHIFT                    0
#define UART_WN7816_GTN(x)                       (((uint8_t)(((uint8_t)(x))<<UART_WN7816_GTN_SHIFT))&UART_WN7816_GTN_MASK)
/* WF7816 Bit Fields */
#define UART_WF7816_GTFD_MASK                    0xFFu
#define UART_WF7816_GTFD_SHIFT                   0
#define UART_WF7816_GTFD(x)                      (((uint8_t)(((uint8_t)(x))<<UART_WF7816_GTFD_SHIFT))&UART_WF7816_GTFD_MASK)
/* ET7816 Bit Fields */
#define UART_ET7816_RXTHRESHOLD_MASK             0xFu
#define UART_ET7816_RXTHRESHOLD_SHIFT            0
#define UART_ET7816_RXTHRESHOLD(x)               (((uint8_t)(((uint8_t)(x))<<UART_ET7816_RXTHRESHOLD_SHIFT))&UART_ET7816_RXTHRESHOLD_MASK)
#define UART_ET7816_TXTHRESHOLD_MASK             0xF0u
#define UART_ET7816_TXTHRESHOLD_SHIFT            4
#define UART_ET7816_TXTHRESHOLD(x)               (((uint8_t)(((uint8_t)(x))<<UART_ET7816_TXTHRESHOLD_SHIFT))&UART_ET7816_TXTHRESHOLD_MASK)
/* TL7816 Bit Fields */
#define UART_TL7816_TLEN_MASK                    0xFFu
#define UART_TL7816_TLEN_SHIFT                   0
#define UART_TL7816_TLEN(x)                      (((uint8_t)(((uint8_t)(x))<<UART_TL7816_TLEN_SHIFT))&UART_TL7816_TLEN_MASK)

/*!
 * @}
 */ /* end of group UART_Register_Masks */


/* UART - Peripheral instance base addresses */
/** Peripheral UART0 base pointer */
#define UART0_BASE_PTR                           ((UART_MemMapPtr)0x4006A000u)
/** Peripheral UART1 base pointer */
#define UART1_BASE_PTR                           ((UART_MemMapPtr)0x4006B000u)
/** Peripheral UART2 base pointer */
#define UART2_BASE_PTR                           ((UART_MemMapPtr)0x4006C000u)
/** Peripheral UART3 base pointer */
#define UART3_BASE_PTR                           ((UART_MemMapPtr)0x4006D000u)
/** Array initializer of UART peripheral base pointers */
#define UART_BASE_PTRS                           { UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- UART - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART_Register_Accessor_Macros UART - Register accessor macros
 * @{
 */


/* UART - Register instance definitions */
/* UART0 */
#define UART0_BDH                                UART_BDH_REG(UART0_BASE_PTR)
#define UART0_BDL                                UART_BDL_REG(UART0_BASE_PTR)
#define UART0_C1                                 UART_C1_REG(UART0_BASE_PTR)
#define UART0_C2                                 UART_C2_REG(UART0_BASE_PTR)
#define UART0_S1                                 UART_S1_REG(UART0_BASE_PTR)
#define UART0_S2                                 UART_S2_REG(UART0_BASE_PTR)
#define UART0_C3                                 UART_C3_REG(UART0_BASE_PTR)
#define UART0_D                                  UART_D_REG(UART0_BASE_PTR)
#define UART0_MA1                                UART_MA1_REG(UART0_BASE_PTR)
#define UART0_MA2                                UART_MA2_REG(UART0_BASE_PTR)
#define UART0_C4                                 UART_C4_REG(UART0_BASE_PTR)
#define UART0_C5                                 UART_C5_REG(UART0_BASE_PTR)
#define UART0_ED                                 UART_ED_REG(UART0_BASE_PTR)
#define UART0_MODEM                              UART_MODEM_REG(UART0_BASE_PTR)
#define UART0_PFIFO                              UART_PFIFO_REG(UART0_BASE_PTR)
#define UART0_CFIFO                              UART_CFIFO_REG(UART0_BASE_PTR)
#define UART0_SFIFO                              UART_SFIFO_REG(UART0_BASE_PTR)
#define UART0_TWFIFO                             UART_TWFIFO_REG(UART0_BASE_PTR)
#define UART0_TCFIFO                             UART_TCFIFO_REG(UART0_BASE_PTR)
#define UART0_RWFIFO                             UART_RWFIFO_REG(UART0_BASE_PTR)
#define UART0_RCFIFO                             UART_RCFIFO_REG(UART0_BASE_PTR)
/* UART1 */
#define UART1_BDH                                UART_BDH_REG(UART1_BASE_PTR)
#define UART1_BDL                                UART_BDL_REG(UART1_BASE_PTR)
#define UART1_C1                                 UART_C1_REG(UART1_BASE_PTR)
#define UART1_C2                                 UART_C2_REG(UART1_BASE_PTR)
#define UART1_S1                                 UART_S1_REG(UART1_BASE_PTR)
#define UART1_S2                                 UART_S2_REG(UART1_BASE_PTR)
#define UART1_C3                                 UART_C3_REG(UART1_BASE_PTR)
#define UART1_D                                  UART_D_REG(UART1_BASE_PTR)
#define UART1_MA1                                UART_MA1_REG(UART1_BASE_PTR)
#define UART1_MA2                                UART_MA2_REG(UART1_BASE_PTR)
#define UART1_C4                                 UART_C4_REG(UART1_BASE_PTR)
#define UART1_C5                                 UART_C5_REG(UART1_BASE_PTR)
#define UART1_ED                                 UART_ED_REG(UART1_BASE_PTR)
#define UART1_MODEM                              UART_MODEM_REG(UART1_BASE_PTR)
#define UART1_PFIFO                              UART_PFIFO_REG(UART1_BASE_PTR)
#define UART1_CFIFO                              UART_CFIFO_REG(UART1_BASE_PTR)
#define UART1_SFIFO                              UART_SFIFO_REG(UART1_BASE_PTR)
#define UART1_TWFIFO                             UART_TWFIFO_REG(UART1_BASE_PTR)
#define UART1_TCFIFO                             UART_TCFIFO_REG(UART1_BASE_PTR)
#define UART1_RWFIFO                             UART_RWFIFO_REG(UART1_BASE_PTR)
#define UART1_RCFIFO                             UART_RCFIFO_REG(UART1_BASE_PTR)
#define UART1_C7816                              UART_C7816_REG(UART1_BASE_PTR)
#define UART1_IE7816                             UART_IE7816_REG(UART1_BASE_PTR)
#define UART1_IS7816                             UART_IS7816_REG(UART1_BASE_PTR)
#define UART1_WP7816T0                           UART_WP7816_T_TYPE0_REG(UART1_BASE_PTR)
#define UART1_WP7816T1                           UART_WP7816_T_TYPE1_REG(UART1_BASE_PTR)
#define UART1_WN7816                             UART_WN7816_REG(UART1_BASE_PTR)
#define UART1_WF7816                             UART_WF7816_REG(UART1_BASE_PTR)
#define UART1_ET7816                             UART_ET7816_REG(UART1_BASE_PTR)
#define UART1_TL7816                             UART_TL7816_REG(UART1_BASE_PTR)
/* UART2 */
#define UART2_BDH                                UART_BDH_REG(UART2_BASE_PTR)
#define UART2_BDL                                UART_BDL_REG(UART2_BASE_PTR)
#define UART2_C1                                 UART_C1_REG(UART2_BASE_PTR)
#define UART2_C2                                 UART_C2_REG(UART2_BASE_PTR)
#define UART2_S1                                 UART_S1_REG(UART2_BASE_PTR)
#define UART2_S2                                 UART_S2_REG(UART2_BASE_PTR)
#define UART2_C3                                 UART_C3_REG(UART2_BASE_PTR)
#define UART2_D                                  UART_D_REG(UART2_BASE_PTR)
#define UART2_MA1                                UART_MA1_REG(UART2_BASE_PTR)
#define UART2_MA2                                UART_MA2_REG(UART2_BASE_PTR)
#define UART2_C4                                 UART_C4_REG(UART2_BASE_PTR)
#define UART2_C5                                 UART_C5_REG(UART2_BASE_PTR)
#define UART2_ED                                 UART_ED_REG(UART2_BASE_PTR)
#define UART2_MODEM                              UART_MODEM_REG(UART2_BASE_PTR)
#define UART2_PFIFO                              UART_PFIFO_REG(UART2_BASE_PTR)
#define UART2_CFIFO                              UART_CFIFO_REG(UART2_BASE_PTR)
#define UART2_SFIFO                              UART_SFIFO_REG(UART2_BASE_PTR)
#define UART2_TWFIFO                             UART_TWFIFO_REG(UART2_BASE_PTR)
#define UART2_TCFIFO                             UART_TCFIFO_REG(UART2_BASE_PTR)
#define UART2_RWFIFO                             UART_RWFIFO_REG(UART2_BASE_PTR)
#define UART2_RCFIFO                             UART_RCFIFO_REG(UART2_BASE_PTR)
/* UART3 */
#define UART3_BDH                                UART_BDH_REG(UART3_BASE_PTR)
#define UART3_BDL                                UART_BDL_REG(UART3_BASE_PTR)
#define UART3_C1                                 UART_C1_REG(UART3_BASE_PTR)
#define UART3_C2                                 UART_C2_REG(UART3_BASE_PTR)
#define UART3_S1                                 UART_S1_REG(UART3_BASE_PTR)
#define UART3_S2                                 UART_S2_REG(UART3_BASE_PTR)
#define UART3_C3                                 UART_C3_REG(UART3_BASE_PTR)
#define UART3_D                                  UART_D_REG(UART3_BASE_PTR)
#define UART3_MA1                                UART_MA1_REG(UART3_BASE_PTR)
#define UART3_MA2                                UART_MA2_REG(UART3_BASE_PTR)
#define UART3_C4                                 UART_C4_REG(UART3_BASE_PTR)
#define UART3_C5                                 UART_C5_REG(UART3_BASE_PTR)
#define UART3_ED                                 UART_ED_REG(UART3_BASE_PTR)
#define UART3_MODEM                              UART_MODEM_REG(UART3_BASE_PTR)
#define UART3_PFIFO                              UART_PFIFO_REG(UART3_BASE_PTR)
#define UART3_CFIFO                              UART_CFIFO_REG(UART3_BASE_PTR)
#define UART3_SFIFO                              UART_SFIFO_REG(UART3_BASE_PTR)
#define UART3_TWFIFO                             UART_TWFIFO_REG(UART3_BASE_PTR)
#define UART3_TCFIFO                             UART_TCFIFO_REG(UART3_BASE_PTR)
#define UART3_RWFIFO                             UART_RWFIFO_REG(UART3_BASE_PTR)
#define UART3_RCFIFO                             UART_RCFIFO_REG(UART3_BASE_PTR)
#define UART3_C7816                              UART_C7816_REG(UART3_BASE_PTR)
#define UART3_IE7816                             UART_IE7816_REG(UART3_BASE_PTR)
#define UART3_IS7816                             UART_IS7816_REG(UART3_BASE_PTR)
#define UART3_WP7816T0                           UART_WP7816_T_TYPE0_REG(UART3_BASE_PTR)
#define UART3_WP7816T1                           UART_WP7816_T_TYPE1_REG(UART3_BASE_PTR)
#define UART3_WN7816                             UART_WN7816_REG(UART3_BASE_PTR)
#define UART3_WF7816                             UART_WF7816_REG(UART3_BASE_PTR)
#define UART3_ET7816                             UART_ET7816_REG(UART3_BASE_PTR)
#define UART3_TL7816                             UART_TL7816_REG(UART3_BASE_PTR)

/*!
 * @}
 */ /* end of group UART_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group UART_Peripheral */


/* ----------------------------------------------------------------------------
   -- VREF
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup VREF_Peripheral VREF
 * @{
 */

/** VREF - Peripheral register structure */
typedef struct VREF_MemMap {
  uint8_t VREFH_TRM;                               /**< VREF Trim Register, offset: 0x0 */
  uint8_t VREFH_SC;                                /**< VREF Status and Control Register, offset: 0x1 */
  uint8_t RESERVED_0[3];
  uint8_t VREFL_TRM;                               /**< VREFL TRIM Register, offset: 0x5 */
} volatile *VREF_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- VREF - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup VREF_Register_Accessor_Macros VREF - Register accessor macros
 * @{
 */


/* VREF - Register accessors */
#define VREF_VREFH_TRM_REG(base)                 ((base)->VREFH_TRM)
#define VREF_VREFH_SC_REG(base)                  ((base)->VREFH_SC)
#define VREF_VREFL_TRM_REG(base)                 ((base)->VREFL_TRM)

/*!
 * @}
 */ /* end of group VREF_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- VREF Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup VREF_Register_Masks VREF Register Masks
 * @{
 */

/* VREFH_TRM Bit Fields */
#define VREF_VREFH_TRM_TRIM_MASK                 0x3Fu
#define VREF_VREFH_TRM_TRIM_SHIFT                0
#define VREF_VREFH_TRM_TRIM(x)                   (((uint8_t)(((uint8_t)(x))<<VREF_VREFH_TRM_TRIM_SHIFT))&VREF_VREFH_TRM_TRIM_MASK)
#define VREF_VREFH_TRM_CHOPEN_MASK               0x40u
#define VREF_VREFH_TRM_CHOPEN_SHIFT              6
/* VREFH_SC Bit Fields */
#define VREF_VREFH_SC_MODE_LV_MASK               0x3u
#define VREF_VREFH_SC_MODE_LV_SHIFT              0
#define VREF_VREFH_SC_MODE_LV(x)                 (((uint8_t)(((uint8_t)(x))<<VREF_VREFH_SC_MODE_LV_SHIFT))&VREF_VREFH_SC_MODE_LV_MASK)
#define VREF_VREFH_SC_VREFST_MASK                0x4u
#define VREF_VREFH_SC_VREFST_SHIFT               2
#define VREF_VREFH_SC_REGEN_MASK                 0x40u
#define VREF_VREFH_SC_REGEN_SHIFT                6
#define VREF_VREFH_SC_VREFEN_MASK                0x80u
#define VREF_VREFH_SC_VREFEN_SHIFT               7
/* VREFL_TRM Bit Fields */
#define VREF_VREFL_TRM_VREFL_TRIM_MASK           0x7u
#define VREF_VREFL_TRM_VREFL_TRIM_SHIFT          0
#define VREF_VREFL_TRM_VREFL_TRIM(x)             (((uint8_t)(((uint8_t)(x))<<VREF_VREFL_TRM_VREFL_TRIM_SHIFT))&VREF_VREFL_TRM_VREFL_TRIM_MASK)
#define VREF_VREFL_TRM_VREFL_EN_MASK             0x8u
#define VREF_VREFL_TRM_VREFL_EN_SHIFT            3
#define VREF_VREFL_TRM_VREFL_SEL_MASK            0x10u
#define VREF_VREFL_TRM_VREFL_SEL_SHIFT           4

/*!
 * @}
 */ /* end of group VREF_Register_Masks */


/* VREF - Peripheral instance base addresses */
/** Peripheral VREF base pointer */
#define VREF_BASE_PTR                            ((VREF_MemMapPtr)0x4006F000u)
/** Array initializer of VREF peripheral base pointers */
#define VREF_BASE_PTRS                           { VREF_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- VREF - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup VREF_Register_Accessor_Macros VREF - Register accessor macros
 * @{
 */


/* VREF - Register instance definitions */
/* VREF */
#define VREF_VREFH_TRM                           VREF_VREFH_TRM_REG(VREF_BASE_PTR)
#define VREF_VREFH_SC                            VREF_VREFH_SC_REG(VREF_BASE_PTR)
#define VREF_VREFL_TRM                           VREF_VREFL_TRM_REG(VREF_BASE_PTR)

/*!
 * @}
 */ /* end of group VREF_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group VREF_Peripheral */


/* ----------------------------------------------------------------------------
   -- WDOG
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup WDOG_Peripheral WDOG
 * @{
 */

/** WDOG - Peripheral register structure */
typedef struct WDOG_MemMap {
  uint16_t STCTRLH;                                /**< Watchdog Status and Control Register High, offset: 0x0 */
  uint16_t STCTRLL;                                /**< Watchdog Status and Control Register Low, offset: 0x2 */
  uint16_t TOVALH;                                 /**< Watchdog Time-out Value Register High, offset: 0x4 */
  uint16_t TOVALL;                                 /**< Watchdog Time-out Value Register Low, offset: 0x6 */
  uint16_t WINH;                                   /**< Watchdog Window Register High, offset: 0x8 */
  uint16_t WINL;                                   /**< Watchdog Window Register Low, offset: 0xA */
  uint16_t REFRESH;                                /**< Watchdog Refresh register, offset: 0xC */
  uint16_t UNLOCK;                                 /**< Watchdog Unlock register, offset: 0xE */
  uint16_t TMROUTH;                                /**< Watchdog Timer Output Register High, offset: 0x10 */
  uint16_t TMROUTL;                                /**< Watchdog Timer Output Register Low, offset: 0x12 */
  uint16_t RSTCNT;                                 /**< Watchdog Reset Count register, offset: 0x14 */
  uint16_t PRESC;                                  /**< Watchdog Prescaler register, offset: 0x16 */
} volatile *WDOG_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- WDOG - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup WDOG_Register_Accessor_Macros WDOG - Register accessor macros
 * @{
 */


/* WDOG - Register accessors */
#define WDOG_STCTRLH_REG(base)                   ((base)->STCTRLH)
#define WDOG_STCTRLL_REG(base)                   ((base)->STCTRLL)
#define WDOG_TOVALH_REG(base)                    ((base)->TOVALH)
#define WDOG_TOVALL_REG(base)                    ((base)->TOVALL)
#define WDOG_WINH_REG(base)                      ((base)->WINH)
#define WDOG_WINL_REG(base)                      ((base)->WINL)
#define WDOG_REFRESH_REG(base)                   ((base)->REFRESH)
#define WDOG_UNLOCK_REG(base)                    ((base)->UNLOCK)
#define WDOG_TMROUTH_REG(base)                   ((base)->TMROUTH)
#define WDOG_TMROUTL_REG(base)                   ((base)->TMROUTL)
#define WDOG_RSTCNT_REG(base)                    ((base)->RSTCNT)
#define WDOG_PRESC_REG(base)                     ((base)->PRESC)

/*!
 * @}
 */ /* end of group WDOG_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- WDOG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup WDOG_Register_Masks WDOG Register Masks
 * @{
 */

/* STCTRLH Bit Fields */
#define WDOG_STCTRLH_WDOGEN_MASK                 0x1u
#define WDOG_STCTRLH_WDOGEN_SHIFT                0
#define WDOG_STCTRLH_CLKSRC_MASK                 0x2u
#define WDOG_STCTRLH_CLKSRC_SHIFT                1
#define WDOG_STCTRLH_IRQRSTEN_MASK               0x4u
#define WDOG_STCTRLH_IRQRSTEN_SHIFT              2
#define WDOG_STCTRLH_WINEN_MASK                  0x8u
#define WDOG_STCTRLH_WINEN_SHIFT                 3
#define WDOG_STCTRLH_ALLOWUPDATE_MASK            0x10u
#define WDOG_STCTRLH_ALLOWUPDATE_SHIFT           4
#define WDOG_STCTRLH_DBGEN_MASK                  0x20u
#define WDOG_STCTRLH_DBGEN_SHIFT                 5
#define WDOG_STCTRLH_STOPEN_MASK                 0x40u
#define WDOG_STCTRLH_STOPEN_SHIFT                6
#define WDOG_STCTRLH_TESTWDOG_MASK               0x400u
#define WDOG_STCTRLH_TESTWDOG_SHIFT              10
#define WDOG_STCTRLH_TESTSEL_MASK                0x800u
#define WDOG_STCTRLH_TESTSEL_SHIFT               11
#define WDOG_STCTRLH_BYTESEL_MASK                0x3000u
#define WDOG_STCTRLH_BYTESEL_SHIFT               12
#define WDOG_STCTRLH_BYTESEL(x)                  (((uint16_t)(((uint16_t)(x))<<WDOG_STCTRLH_BYTESEL_SHIFT))&WDOG_STCTRLH_BYTESEL_MASK)
#define WDOG_STCTRLH_DISTESTWDOG_MASK            0x4000u
#define WDOG_STCTRLH_DISTESTWDOG_SHIFT           14
/* STCTRLL Bit Fields */
#define WDOG_STCTRLL_INTFLG_MASK                 0x8000u
#define WDOG_STCTRLL_INTFLG_SHIFT                15
/* TOVALH Bit Fields */
#define WDOG_TOVALH_TOVALHIGH_MASK               0xFFFFu
#define WDOG_TOVALH_TOVALHIGH_SHIFT              0
#define WDOG_TOVALH_TOVALHIGH(x)                 (((uint16_t)(((uint16_t)(x))<<WDOG_TOVALH_TOVALHIGH_SHIFT))&WDOG_TOVALH_TOVALHIGH_MASK)
/* TOVALL Bit Fields */
#define WDOG_TOVALL_TOVALLOW_MASK                0xFFFFu
#define WDOG_TOVALL_TOVALLOW_SHIFT               0
#define WDOG_TOVALL_TOVALLOW(x)                  (((uint16_t)(((uint16_t)(x))<<WDOG_TOVALL_TOVALLOW_SHIFT))&WDOG_TOVALL_TOVALLOW_MASK)
/* WINH Bit Fields */
#define WDOG_WINH_WINHIGH_MASK                   0xFFFFu
#define WDOG_WINH_WINHIGH_SHIFT                  0
#define WDOG_WINH_WINHIGH(x)                     (((uint16_t)(((uint16_t)(x))<<WDOG_WINH_WINHIGH_SHIFT))&WDOG_WINH_WINHIGH_MASK)
/* WINL Bit Fields */
#define WDOG_WINL_WINLOW_MASK                    0xFFFFu
#define WDOG_WINL_WINLOW_SHIFT                   0
#define WDOG_WINL_WINLOW(x)                      (((uint16_t)(((uint16_t)(x))<<WDOG_WINL_WINLOW_SHIFT))&WDOG_WINL_WINLOW_MASK)
/* REFRESH Bit Fields */
#define WDOG_REFRESH_WDOGREFRESH_MASK            0xFFFFu
#define WDOG_REFRESH_WDOGREFRESH_SHIFT           0
#define WDOG_REFRESH_WDOGREFRESH(x)              (((uint16_t)(((uint16_t)(x))<<WDOG_REFRESH_WDOGREFRESH_SHIFT))&WDOG_REFRESH_WDOGREFRESH_MASK)
/* UNLOCK Bit Fields */
#define WDOG_UNLOCK_WDOGUNLOCK_MASK              0xFFFFu
#define WDOG_UNLOCK_WDOGUNLOCK_SHIFT             0
#define WDOG_UNLOCK_WDOGUNLOCK(x)                (((uint16_t)(((uint16_t)(x))<<WDOG_UNLOCK_WDOGUNLOCK_SHIFT))&WDOG_UNLOCK_WDOGUNLOCK_MASK)
/* TMROUTH Bit Fields */
#define WDOG_TMROUTH_TIMEROUTHIGH_MASK           0xFFFFu
#define WDOG_TMROUTH_TIMEROUTHIGH_SHIFT          0
#define WDOG_TMROUTH_TIMEROUTHIGH(x)             (((uint16_t)(((uint16_t)(x))<<WDOG_TMROUTH_TIMEROUTHIGH_SHIFT))&WDOG_TMROUTH_TIMEROUTHIGH_MASK)
/* TMROUTL Bit Fields */
#define WDOG_TMROUTL_TIMEROUTLOW_MASK            0xFFFFu
#define WDOG_TMROUTL_TIMEROUTLOW_SHIFT           0
#define WDOG_TMROUTL_TIMEROUTLOW(x)              (((uint16_t)(((uint16_t)(x))<<WDOG_TMROUTL_TIMEROUTLOW_SHIFT))&WDOG_TMROUTL_TIMEROUTLOW_MASK)
/* RSTCNT Bit Fields */
#define WDOG_RSTCNT_RSTCNT_MASK                  0xFFFFu
#define WDOG_RSTCNT_RSTCNT_SHIFT                 0
#define WDOG_RSTCNT_RSTCNT(x)                    (((uint16_t)(((uint16_t)(x))<<WDOG_RSTCNT_RSTCNT_SHIFT))&WDOG_RSTCNT_RSTCNT_MASK)
/* PRESC Bit Fields */
#define WDOG_PRESC_PRESCVAL_MASK                 0x700u
#define WDOG_PRESC_PRESCVAL_SHIFT                8
#define WDOG_PRESC_PRESCVAL(x)                   (((uint16_t)(((uint16_t)(x))<<WDOG_PRESC_PRESCVAL_SHIFT))&WDOG_PRESC_PRESCVAL_MASK)

/*!
 * @}
 */ /* end of group WDOG_Register_Masks */


/* WDOG - Peripheral instance base addresses */
/** Peripheral WDOG base pointer */
#define WDOG_BASE_PTR                            ((WDOG_MemMapPtr)0x40053000u)
/** Array initializer of WDOG peripheral base pointers */
#define WDOG_BASE_PTRS                           { WDOG_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- WDOG - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup WDOG_Register_Accessor_Macros WDOG - Register accessor macros
 * @{
 */


/* WDOG - Register instance definitions */
/* WDOG */
#define WDOG_STCTRLH                             WDOG_STCTRLH_REG(WDOG_BASE_PTR)
#define WDOG_STCTRLL                             WDOG_STCTRLL_REG(WDOG_BASE_PTR)
#define WDOG_TOVALH                              WDOG_TOVALH_REG(WDOG_BASE_PTR)
#define WDOG_TOVALL                              WDOG_TOVALL_REG(WDOG_BASE_PTR)
#define WDOG_WINH                                WDOG_WINH_REG(WDOG_BASE_PTR)
#define WDOG_WINL                                WDOG_WINL_REG(WDOG_BASE_PTR)
#define WDOG_REFRESH                             WDOG_REFRESH_REG(WDOG_BASE_PTR)
#define WDOG_UNLOCK                              WDOG_UNLOCK_REG(WDOG_BASE_PTR)
#define WDOG_TMROUTH                             WDOG_TMROUTH_REG(WDOG_BASE_PTR)
#define WDOG_TMROUTL                             WDOG_TMROUTL_REG(WDOG_BASE_PTR)
#define WDOG_RSTCNT                              WDOG_RSTCNT_REG(WDOG_BASE_PTR)
#define WDOG_PRESC                               WDOG_PRESC_REG(WDOG_BASE_PTR)

/*!
 * @}
 */ /* end of group WDOG_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group WDOG_Peripheral */


/* ----------------------------------------------------------------------------
   -- XBAR
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup XBAR_Peripheral XBAR
 * @{
 */

/** XBAR - Peripheral register structure */
typedef struct XBAR_MemMap {
  uint16_t SEL0;                                   /**< Crossbar Select Register 0, offset: 0x0 */
  uint16_t SEL1;                                   /**< Crossbar Select Register 1, offset: 0x2 */
  uint16_t SEL2;                                   /**< Crossbar Select Register 2, offset: 0x4 */
  uint16_t SEL3;                                   /**< Crossbar Select Register 3, offset: 0x6 */
  uint16_t SEL4;                                   /**< Crossbar Select Register 4, offset: 0x8 */
  uint16_t SEL5;                                   /**< Crossbar Select Register 5, offset: 0xA */
  uint16_t SEL6;                                   /**< Crossbar Select Register 6, offset: 0xC */
  uint16_t SEL7;                                   /**< Crossbar Select Register 7, offset: 0xE */
  uint16_t SEL8;                                   /**< Crossbar Select Register 8, offset: 0x10 */
  uint16_t SEL9;                                   /**< Crossbar Select Register 9, offset: 0x12 */
  uint16_t SEL10;                                  /**< Crossbar Select Register 10, offset: 0x14 */
  uint16_t SEL11;                                  /**< Crossbar Select Register 11, offset: 0x16 */
  uint16_t SEL12;                                  /**< Crossbar Select Register 12, offset: 0x18 */
  uint16_t SEL13;                                  /**< Crossbar Select Register 13, offset: 0x1A */
  uint16_t SEL14;                                  /**< Crossbar Select Register 14, offset: 0x1C */
  uint16_t SEL15;                                  /**< Crossbar Select Register 15, offset: 0x1E */
  uint16_t SEL16;                                  /**< Crossbar Select Register 16, offset: 0x20 */
  uint16_t CTRL0;                                  /**< Crossbar Control Register 0, offset: 0x22 */
} volatile *XBAR_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- XBAR - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup XBAR_Register_Accessor_Macros XBAR - Register accessor macros
 * @{
 */


/* XBAR - Register accessors */
#define XBAR_SEL0_REG(base)                      ((base)->SEL0)
#define XBAR_SEL1_REG(base)                      ((base)->SEL1)
#define XBAR_SEL2_REG(base)                      ((base)->SEL2)
#define XBAR_SEL3_REG(base)                      ((base)->SEL3)
#define XBAR_SEL4_REG(base)                      ((base)->SEL4)
#define XBAR_SEL5_REG(base)                      ((base)->SEL5)
#define XBAR_SEL6_REG(base)                      ((base)->SEL6)
#define XBAR_SEL7_REG(base)                      ((base)->SEL7)
#define XBAR_SEL8_REG(base)                      ((base)->SEL8)
#define XBAR_SEL9_REG(base)                      ((base)->SEL9)
#define XBAR_SEL10_REG(base)                     ((base)->SEL10)
#define XBAR_SEL11_REG(base)                     ((base)->SEL11)
#define XBAR_SEL12_REG(base)                     ((base)->SEL12)
#define XBAR_SEL13_REG(base)                     ((base)->SEL13)
#define XBAR_SEL14_REG(base)                     ((base)->SEL14)
#define XBAR_SEL15_REG(base)                     ((base)->SEL15)
#define XBAR_SEL16_REG(base)                     ((base)->SEL16)
#define XBAR_CTRL0_REG(base)                     ((base)->CTRL0)

/*!
 * @}
 */ /* end of group XBAR_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- XBAR Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup XBAR_Register_Masks XBAR Register Masks
 * @{
 */

/* SEL0 Bit Fields */
#define XBAR_SEL0_SEL0_MASK                      0x3Fu
#define XBAR_SEL0_SEL0_SHIFT                     0
#define XBAR_SEL0_SEL0(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL0_SEL0_SHIFT))&XBAR_SEL0_SEL0_MASK)
#define XBAR_SEL0_SEL1_MASK                      0x3F00u
#define XBAR_SEL0_SEL1_SHIFT                     8
#define XBAR_SEL0_SEL1(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL0_SEL1_SHIFT))&XBAR_SEL0_SEL1_MASK)
/* SEL1 Bit Fields */
#define XBAR_SEL1_SEL2_MASK                      0x3Fu
#define XBAR_SEL1_SEL2_SHIFT                     0
#define XBAR_SEL1_SEL2(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL1_SEL2_SHIFT))&XBAR_SEL1_SEL2_MASK)
#define XBAR_SEL1_SEL3_MASK                      0x3F00u
#define XBAR_SEL1_SEL3_SHIFT                     8
#define XBAR_SEL1_SEL3(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL1_SEL3_SHIFT))&XBAR_SEL1_SEL3_MASK)
/* SEL2 Bit Fields */
#define XBAR_SEL2_SEL4_MASK                      0x3Fu
#define XBAR_SEL2_SEL4_SHIFT                     0
#define XBAR_SEL2_SEL4(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL2_SEL4_SHIFT))&XBAR_SEL2_SEL4_MASK)
#define XBAR_SEL2_SEL5_MASK                      0x3F00u
#define XBAR_SEL2_SEL5_SHIFT                     8
#define XBAR_SEL2_SEL5(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL2_SEL5_SHIFT))&XBAR_SEL2_SEL5_MASK)
/* SEL3 Bit Fields */
#define XBAR_SEL3_SEL6_MASK                      0x3Fu
#define XBAR_SEL3_SEL6_SHIFT                     0
#define XBAR_SEL3_SEL6(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL3_SEL6_SHIFT))&XBAR_SEL3_SEL6_MASK)
#define XBAR_SEL3_SEL7_MASK                      0x3F00u
#define XBAR_SEL3_SEL7_SHIFT                     8
#define XBAR_SEL3_SEL7(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL3_SEL7_SHIFT))&XBAR_SEL3_SEL7_MASK)
/* SEL4 Bit Fields */
#define XBAR_SEL4_SEL8_MASK                      0x3Fu
#define XBAR_SEL4_SEL8_SHIFT                     0
#define XBAR_SEL4_SEL8(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL4_SEL8_SHIFT))&XBAR_SEL4_SEL8_MASK)
#define XBAR_SEL4_SEL9_MASK                      0x3F00u
#define XBAR_SEL4_SEL9_SHIFT                     8
#define XBAR_SEL4_SEL9(x)                        (((uint16_t)(((uint16_t)(x))<<XBAR_SEL4_SEL9_SHIFT))&XBAR_SEL4_SEL9_MASK)
/* SEL5 Bit Fields */
#define XBAR_SEL5_SEL10_MASK                     0x3Fu
#define XBAR_SEL5_SEL10_SHIFT                    0
#define XBAR_SEL5_SEL10(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL5_SEL10_SHIFT))&XBAR_SEL5_SEL10_MASK)
#define XBAR_SEL5_SEL11_MASK                     0x3F00u
#define XBAR_SEL5_SEL11_SHIFT                    8
#define XBAR_SEL5_SEL11(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL5_SEL11_SHIFT))&XBAR_SEL5_SEL11_MASK)
/* SEL6 Bit Fields */
#define XBAR_SEL6_SEL12_MASK                     0x3Fu
#define XBAR_SEL6_SEL12_SHIFT                    0
#define XBAR_SEL6_SEL12(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL6_SEL12_SHIFT))&XBAR_SEL6_SEL12_MASK)
#define XBAR_SEL6_SEL13_MASK                     0x3F00u
#define XBAR_SEL6_SEL13_SHIFT                    8
#define XBAR_SEL6_SEL13(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL6_SEL13_SHIFT))&XBAR_SEL6_SEL13_MASK)
/* SEL7 Bit Fields */
#define XBAR_SEL7_SEL14_MASK                     0x3Fu
#define XBAR_SEL7_SEL14_SHIFT                    0
#define XBAR_SEL7_SEL14(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL7_SEL14_SHIFT))&XBAR_SEL7_SEL14_MASK)
#define XBAR_SEL7_SEL15_MASK                     0x3F00u
#define XBAR_SEL7_SEL15_SHIFT                    8
#define XBAR_SEL7_SEL15(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL7_SEL15_SHIFT))&XBAR_SEL7_SEL15_MASK)
/* SEL8 Bit Fields */
#define XBAR_SEL8_SEL16_MASK                     0x3Fu
#define XBAR_SEL8_SEL16_SHIFT                    0
#define XBAR_SEL8_SEL16(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL8_SEL16_SHIFT))&XBAR_SEL8_SEL16_MASK)
#define XBAR_SEL8_SEL17_MASK                     0x3F00u
#define XBAR_SEL8_SEL17_SHIFT                    8
#define XBAR_SEL8_SEL17(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL8_SEL17_SHIFT))&XBAR_SEL8_SEL17_MASK)
/* SEL9 Bit Fields */
#define XBAR_SEL9_SEL18_MASK                     0x3Fu
#define XBAR_SEL9_SEL18_SHIFT                    0
#define XBAR_SEL9_SEL18(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL9_SEL18_SHIFT))&XBAR_SEL9_SEL18_MASK)
#define XBAR_SEL9_SEL19_MASK                     0x3F00u
#define XBAR_SEL9_SEL19_SHIFT                    8
#define XBAR_SEL9_SEL19(x)                       (((uint16_t)(((uint16_t)(x))<<XBAR_SEL9_SEL19_SHIFT))&XBAR_SEL9_SEL19_MASK)
/* SEL10 Bit Fields */
#define XBAR_SEL10_SEL20_MASK                    0x1Fu
#define XBAR_SEL10_SEL20_SHIFT                   0
#define XBAR_SEL10_SEL20(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL10_SEL20_SHIFT))&XBAR_SEL10_SEL20_MASK)
#define XBAR_SEL10_SEL21_MASK                    0x1F00u
#define XBAR_SEL10_SEL21_SHIFT                   8
#define XBAR_SEL10_SEL21(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL10_SEL21_SHIFT))&XBAR_SEL10_SEL21_MASK)
/* SEL11 Bit Fields */
#define XBAR_SEL11_SEL22_MASK                    0x3Fu
#define XBAR_SEL11_SEL22_SHIFT                   0
#define XBAR_SEL11_SEL22(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL11_SEL22_SHIFT))&XBAR_SEL11_SEL22_MASK)
#define XBAR_SEL11_SEL23_MASK                    0x3F00u
#define XBAR_SEL11_SEL23_SHIFT                   8
#define XBAR_SEL11_SEL23(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL11_SEL23_SHIFT))&XBAR_SEL11_SEL23_MASK)
/* SEL12 Bit Fields */
#define XBAR_SEL12_SEL24_MASK                    0x1Fu
#define XBAR_SEL12_SEL24_SHIFT                   0
#define XBAR_SEL12_SEL24(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL12_SEL24_SHIFT))&XBAR_SEL12_SEL24_MASK)
#define XBAR_SEL12_SEL25_MASK                    0x1F00u
#define XBAR_SEL12_SEL25_SHIFT                   8
#define XBAR_SEL12_SEL25(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL12_SEL25_SHIFT))&XBAR_SEL12_SEL25_MASK)
/* SEL13 Bit Fields */
#define XBAR_SEL13_SEL26_MASK                    0x3Fu
#define XBAR_SEL13_SEL26_SHIFT                   0
#define XBAR_SEL13_SEL26(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL13_SEL26_SHIFT))&XBAR_SEL13_SEL26_MASK)
#define XBAR_SEL13_SEL27_MASK                    0x3F00u
#define XBAR_SEL13_SEL27_SHIFT                   8
#define XBAR_SEL13_SEL27(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL13_SEL27_SHIFT))&XBAR_SEL13_SEL27_MASK)
/* SEL14 Bit Fields */
#define XBAR_SEL14_SEL28_MASK                    0x3Fu
#define XBAR_SEL14_SEL28_SHIFT                   0
#define XBAR_SEL14_SEL28(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL14_SEL28_SHIFT))&XBAR_SEL14_SEL28_MASK)
#define XBAR_SEL14_SEL29_MASK                    0x3F00u
#define XBAR_SEL14_SEL29_SHIFT                   8
#define XBAR_SEL14_SEL29(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL14_SEL29_SHIFT))&XBAR_SEL14_SEL29_MASK)
/* SEL15 Bit Fields */
#define XBAR_SEL15_SEL30_MASK                    0x1Fu
#define XBAR_SEL15_SEL30_SHIFT                   0
#define XBAR_SEL15_SEL30(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL15_SEL30_SHIFT))&XBAR_SEL15_SEL30_MASK)
#define XBAR_SEL15_SEL31_MASK                    0x1F00u
#define XBAR_SEL15_SEL31_SHIFT                   8
#define XBAR_SEL15_SEL31(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL15_SEL31_SHIFT))&XBAR_SEL15_SEL31_MASK)
/* SEL16 Bit Fields */
#define XBAR_SEL16_SEL32_MASK                    0x3Fu
#define XBAR_SEL16_SEL32_SHIFT                   0
#define XBAR_SEL16_SEL32(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_SEL16_SEL32_SHIFT))&XBAR_SEL16_SEL32_MASK)
/* CTRL0 Bit Fields */
#define XBAR_CTRL0_DEN0_MASK                     0x1u
#define XBAR_CTRL0_DEN0_SHIFT                    0
#define XBAR_CTRL0_IEN0_MASK                     0x2u
#define XBAR_CTRL0_IEN0_SHIFT                    1
#define XBAR_CTRL0_EDGE0_MASK                    0xCu
#define XBAR_CTRL0_EDGE0_SHIFT                   2
#define XBAR_CTRL0_EDGE0(x)                      (((uint16_t)(((uint16_t)(x))<<XBAR_CTRL0_EDGE0_SHIFT))&XBAR_CTRL0_EDGE0_MASK)
#define XBAR_CTRL0_STS0_MASK                     0x10u
#define XBAR_CTRL0_STS0_SHIFT                    4

/*!
 * @}
 */ /* end of group XBAR_Register_Masks */


/* XBAR - Peripheral instance base addresses */
/** Peripheral XBAR base pointer */
#define XBAR_BASE_PTR                            ((XBAR_MemMapPtr)0x40055000u)
/** Array initializer of XBAR peripheral base pointers */
#define XBAR_BASE_PTRS                           { XBAR_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- XBAR - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup XBAR_Register_Accessor_Macros XBAR - Register accessor macros
 * @{
 */


/* XBAR - Register instance definitions */
/* XBAR */
#define XBAR_SEL0                                XBAR_SEL0_REG(XBAR_BASE_PTR)
#define XBAR_SEL1                                XBAR_SEL1_REG(XBAR_BASE_PTR)
#define XBAR_SEL2                                XBAR_SEL2_REG(XBAR_BASE_PTR)
#define XBAR_SEL3                                XBAR_SEL3_REG(XBAR_BASE_PTR)
#define XBAR_SEL4                                XBAR_SEL4_REG(XBAR_BASE_PTR)
#define XBAR_SEL5                                XBAR_SEL5_REG(XBAR_BASE_PTR)
#define XBAR_SEL6                                XBAR_SEL6_REG(XBAR_BASE_PTR)
#define XBAR_SEL7                                XBAR_SEL7_REG(XBAR_BASE_PTR)
#define XBAR_SEL8                                XBAR_SEL8_REG(XBAR_BASE_PTR)
#define XBAR_SEL9                                XBAR_SEL9_REG(XBAR_BASE_PTR)
#define XBAR_SEL10                               XBAR_SEL10_REG(XBAR_BASE_PTR)
#define XBAR_SEL11                               XBAR_SEL11_REG(XBAR_BASE_PTR)
#define XBAR_SEL12                               XBAR_SEL12_REG(XBAR_BASE_PTR)
#define XBAR_SEL13                               XBAR_SEL13_REG(XBAR_BASE_PTR)
#define XBAR_SEL14                               XBAR_SEL14_REG(XBAR_BASE_PTR)
#define XBAR_SEL15                               XBAR_SEL15_REG(XBAR_BASE_PTR)
#define XBAR_SEL16                               XBAR_SEL16_REG(XBAR_BASE_PTR)
#define XBAR_CTRL0                               XBAR_CTRL0_REG(XBAR_BASE_PTR)

/*!
 * @}
 */ /* end of group XBAR_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group XBAR_Peripheral */


/*
** End of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #pragma pop
#elif defined(__CWCC__)
  #pragma pop
#elif defined(__GNUC__)
  /* leave anonymous unions enabled */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=default
#else
  #error Not supported compiler type
#endif

/*!
 * @}
 */ /* end of group Peripheral_defines */


/* ----------------------------------------------------------------------------
   -- Backward Compatibility
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Backward_Compatibility_Symbols Backward Compatibility
 * @{
 */

/* No backward compatibility issues. */

/*!
 * @}
 */ /* end of group Backward_Compatibility_Symbols */


#else /* #if !defined(MCU_MKM34Z5) && !defined(MCU_MKM38Z5) */
  /* There is already included the same memory map. Check if it is compatible (has the same major version) */
  #if (MCU_MEM_MAP_VERSION != 0x0100u)
    #if (!defined(MCU_MEM_MAP_SUPPRESS_VERSION_WARNING))
      #warning There are included two not compatible versions of memory maps. Please check possible differences.
    #endif /* (!defined(MCU_MEM_MAP_SUPPRESS_VERSION_WARNING)) */
  #endif /* (MCU_MEM_MAP_VERSION != 0x0100u) */
#endif  /* #if !defined(MCU_MKM34Z5) && !defined(MCU_MKM38Z5) */

/* MKM34Z5.h, eof. */
