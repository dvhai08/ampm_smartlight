/*****************************************************************************
 * (c) Copyright 2010, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      appconfig.h
 * @version   1.0.4.0
 * @date      May-14-2012
 * @brief     Application configuration and interrupt vector definitions.
 ******************************************************************************/
#ifndef __APPCONFIG_H
#define __APPCONFIG_H

/***************************************************************************//*!
 * @brief   Define processor device.
 * @param   Supported devices - MCU_MKM34Z50  
 ******************************************************************************/
#define MCU_MKM34Z50

/***************************************************************************//*!
 * @brief   Reserve trace buffer in RAM for Micro Trace Buffer (MTB) trace 
 *          capabilities.  
 * @param   MTB_RAM_RESERVE - this define reserves trace buffer in RAM for use 
 *          by Micro Trace Buffer (MTB). The trace buffer starts at 0x1FFFF000 
 *          and it has size of 64-bytes. You can modify size of the trace buffer 
 *          for example to 256-bytes by the following definition: 
 *          #define MTB_RAM_SIZE 0x100
 * @note    Tracing capabilities reduces performance of the application.
 ******************************************************************************/
//#define MTB_RAM_RESERVE

/***************************************************************************//*!
 * @brief   Force read-by-write accesses to on-chip peripheral registers via 
 *	        core's instruction set instead of using Bit Manipulation Engine (BME). 
 *	        Using BME results in a better code density.
 * @param   EMULATE_BME_IO_ACCESS - define this parameter in order to force 
 *          read-by-write accesses to on-chip peripheral registers via core's 
 *	        instruction set (BME read-by-write accesses enabled by default)
 ******************************************************************************/
//#define EMULATE_BME_IO_ACCESS

/***************************************************************************//*!
 * @brief   Modify default boot options. The boot options allows the MCU to 
 *          modify default operations from NVM at reset. 
 *          Supported options:
 * @param   BOOT_FLASH_SECURE - MCU Secured (MCU is unsecured by default; secure
 *                              command is blocked in IAR 6.40.2 flash loader)
 * @param   BOOT_DIV_CLK_BY_8 - Divide clock by 8 (MCU boots up with system clock 
 *                              divide by 1 by default).   
 * @param   BOOT_NMI_DISABLE  - Disable NMI (NMI is enabled by default)
 * @param   BOOT_EXT_CLK      - Device uses external slower clock for flash  
 *                              calibration after reset. Internal 25 MHz flash 
 *                              clock is disabled.
 * @param   BOOT_HWVLPR_MODE  - Device boots in VLPR mode starting next VDD power 
 *                              cycle provided VBAT power is retained (forced by HW).
 * @param   BOOT_SWVLPR_MODE  - Device boots in RUN mode however device enters 
 *                              VLPR by first software instructions of the device
 *                              startup (forced by SW).
 * @note    If neither BOOT_EXE_MODE, BOOT_VLPR_MODE nor BOOT_EXT_FLASH_CLK 
 *          constants are defined then device boots in default mode which is RUN 
 *          mode with internal 25 MHz clock enabled and used for flash calibration.
 ******************************************************************************/
#define BOOT_NMI_DISABLE  
#define BOOT_EXT_CLK
//#define BOOT_FLASH_SECURE
   
/***************************************************************************//*!
 * @brief   Define specific peripheral configuration structures.
 * @example 
 *          #define PIT_CH_CONFIG                                              \
 *          (tPIT_CH){                                                         \
 *            SET(PIT_TCTRL_TIE_MASK)|SET(PIT_TCTRL_TEN_MASK),                 \
 *          } 
 ******************************************************************************/
 
 
#define BOARD_PERIPH_ON_CONFIG                                         \
(tSIM){                                                                         \
/* SOPT1    */ SET(SIM_SOPT1_OSC32KSEL(0)),                                     \
/* SOPT1_CFG*/ CLR(SIM_SOPT1_CFG_RAMBPEN_MASK)|CLR(SIM_SOPT1_CFG_RAMSBDIS_MASK)|\
/* .........*/ CLR(SIM_SOPT1_CFG_CMPOLPTMR0SEL_MASK)|                           \
/* .........*/ SET(SIM_SOPT1_CFG_LPTMR3SEL(0))|SET(SIM_SOPT1_CFG_LPTMR2SEL(0))| \
/* .........*/ SET(SIM_SOPT1_CFG_LPTMR1SEL(0)),                                 \
/* CTRL_REG */ SET(SIM_CTRL_REG_CLKOUTSEL(0))|                                  \
/* ........ */ SET(SIM_CTRL_REG_SAR_TRG_CLK_SEL(0))|                            \
/* ........ */ CLR(SIM_CTRL_REG_PTC2_HD_EN_MASK)|                               \
/* ........ */ CLR(SIM_CTRL_REG_PLL_VLP_EN_MASK)|SET(SIM_CTRL_REG_NMIDIS_MASK), \
/* SCGC4    */ CLR(SIM_SCGC4_SPI1_MASK)|CLR(SIM_SCGC4_SPI0_MASK)|               \
/* .....    */ SET(SIM_SCGC4_CMP1_MASK)|SET(SIM_SCGC4_CMP0_MASK)|               \
/* .....    */ SET(SIM_SCGC4_VREF_MASK)|CLR(SIM_SCGC4_UART3_MASK)|              \
/* .....    */ CLR(SIM_SCGC4_UART2_MASK)|SET(SIM_SCGC4_UART1_MASK)|             \
/* .....    */ CLR(SIM_SCGC4_UART0_MASK)|SET(SIM_SCGC4_I2C1_MASK)|              \
/* .....    */ CLR(SIM_SCGC4_I2C0_MASK)|SET(SIM_SCGC4_OSC_MASK)|                \
/* .....    */ SET(SIM_SCGC4_MCG_MASK)|CLR(SIM_SCGC4_EWM_MASK),                 \
/* SCGC5    */ CLR(SIM_SCGC5_TMR3_MASK)|CLR(SIM_SCGC5_TMR2_MASK)|               \
/* .....    */ SET(SIM_SCGC5_TMR1_MASK)|SET(SIM_SCGC5_TMR0_MASK)|               \
/* .....    */ SET(SIM_SCGC5_XBAR_MASK)|SET(SIM_SCGC5_WDOG_MASK)|               \
/* .....    */ SET(SIM_SCGC5_IRTCREGFILE_MASK)|SET(SIM_SCGC5_IRTC_MASK)|        \
/* .....    */ SET(SIM_SCGC5_PORTI_MASK)|SET(SIM_SCGC5_PORTH_MASK)|             \
/* .....    */ SET(SIM_SCGC5_PORTG_MASK)|SET(SIM_SCGC5_PORTF_MASK)|             \
/* .....    */ SET(SIM_SCGC5_PORTE_MASK)|CLR(SIM_SCGC5_PORTD_MASK)|             \
/* .....    */ CLR(SIM_SCGC5_PORTC_MASK)|CLR(SIM_SCGC5_PORTB_MASK)|             \
/* .....    */ SET(SIM_SCGC5_PORTA_MASK)|CLR(SIM_SCGC5_SLCD_MASK),              \
/* SCGC6    */ SET(SIM_SCGC6_SIM_LP_MASK)|                                      \
/* .....    */ SET(SIM_SCGC6_LPTMR_MASK)|CLR(SIM_SCGC6_CRC_MASK)|               \
/* .....    */ SET(SIM_SCGC6_AFE_MASK)|CLR(SIM_SCGC6_PIT1_MASK)|                \
/* .....    */ CLR(SIM_SCGC6_PIT0_MASK)|SET(SIM_SCGC6_ADC_MASK)|                \
/* .....    */ CLR(SIM_SCGC6_RNGA_MASK)|CLR(SIM_SCGC6_DMAMUX3_MASK)|            \
/* .....    */ CLR(SIM_SCGC6_DMAMUX2_MASK)|CLR(SIM_SCGC6_DMAMUX1_MASK)|         \
/* .....    */ CLR(SIM_SCGC6_DMAMUX0_MASK)|SET(SIM_SCGC6_FTFA_MASK),            \
/* SCGC7    */ CLR(SIM_SCGC7_DMA_MASK)|SET(SIM_SCGC7_MPU_MASK),                 \
/* CLKDIV1  */ SET(SIM_CLKDIV1_SYSDIV(SYSCLK_DIV1))|                            \
/* .......  */ SET(SIM_CLKDIV1_SYSCLKMODE_MASK),                                \
/* FCCFG1   */ CLR(SIM_FCFG1_FLASHDOZE_MASK)|CLR(SIM_FCFG1_FLASHDIS_MASK),      \
/* MISC_CTL */ CLR(SIM_MISC_CTL_VREFBUFPD_MASK)|                                \
/* ........ */ CLR(SIM_MISC_CTL_VREFBUFINSEL_MASK)|                             \
/* ........ */ CLR(SIM_MISC_CTL_VREFBUFOUTEN_MASK)|                             \
/* ........ */ CLR(SIM_MISC_CTL_RTCCLKSEL_MASK)|                                \
/* ........ */ SET(SIM_MISC_CTL_TMR3PCSSEL(0))|                                 \
/* ........ */ SET(SIM_MISC_CTL_TMR2PCSSEL(0))|                                 \
/* ........ */ SET(SIM_MISC_CTL_TMR1PCSSEL(0))|                                 \
/* ........ */ SET(SIM_MISC_CTL_TMR0PCSSEL(0))|                                 \
/* ........ */ SET(SIM_MISC_CTL_TMR3SCSSEL_MASK)|                               \
/* ........ */ SET(SIM_MISC_CTL_TMR2SCSSEL_MASK)|                               \
/* ........ */ SET(SIM_MISC_CTL_TMR1SCSSEL_MASK)|                               \
/* ........ */ SET(SIM_MISC_CTL_TMR0SCSSEL_MASK)|                               \
/* ........ */ CLR(SIM_MISC_CTL_TMR0PLLCLKSEL_MASK)|                            \
/* ........ */ CLR(SIM_MISC_CTL_EWMINSEL_MASK)|                                 \
/* ........ */ SET(SIM_MISC_CTL_XBARPITOUTSEL(0))|                              \
/* ........ */ CLR(SIM_MISC_CTL_UART3IRSEL_MASK)|                               \
/* ........ */ CLR(SIM_MISC_CTL_UART2IRSEL_MASK)|                               \
/* ........ */ CLR(SIM_MISC_CTL_UART1IRSEL_MASK)|                               \
/* ........ */ CLR(SIM_MISC_CTL_UART0IRSEL_MASK)|                               \
/* ........ */ CLR(SIM_MISC_CTL_UARTMODTYPE_MASK)|                              \
/* ........ */ CLR(SIM_MISC_CTL_AFECLKPADDIR_MASK)|                             \
/* ........ */ SET(SIM_MISC_CTL_AFECLKSEL(0))|                                  \
/* ........ */ SET(SIM_MISC_CTL_DMADONESEL(0))|                                 \
/* ........ */ SET(SIM_MISC_CTL_XBARAFEMODOUTSEL(0)),                           \
}
/***************************************************************************//*!
 * @brief   Define interrupt vectors.
 * @param   Supported devices - MCU_MKM34Z5
 * @example 
 *          extern void UART2_UART3_isr (void);
 *          #define VECTOR_036 UART2_UART3_isr
 ******************************************************************************/
extern void AFE_CH3_isr     (void);
#define VECTOR_040 AFE_CH3_isr
//extern void AFE_CH2_isr     (void);
//#define VECTOR_039 AFE_CH2_isr
extern void hardfault_isr (void);
#define VECTOR_003 hardfault_isr

extern void swisr0_isr      (void);
#define VECTOR_029 swisr0_isr

extern void CMP0_CMP1_isr      (void);
#define VECTOR_030 CMP0_CMP1_isr

extern void swisr1_isr      (void);
#define VECTOR_043 swisr1_isr

extern void swisr2_isr      (void);
#define VECTOR_045 swisr2_isr

extern void PTx_isr (void);
#define VECTOR_033 PTx_isr

extern void lptmr_isr (void);
#define VECTOR_046 lptmr_isr 

extern void TMR0_isr (void) ; 
#define VECTOR_023 TMR0_isr

extern void UART0_UART1_isr (void);
#define VECTOR_035 UART0_UART1_isr
/******************************************************************************/
#endif /* __APPCONFIG_ */
