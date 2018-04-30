/**************************************************************************
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved                       
*************************************************************************** 
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF  
* THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************//*!
* @file      mk341ph.c
* @author    R55013
* @version   2.1.02.0
* @date      Feb-12-2014
* @brief     Main module of the MMK34 1-phase electricity meter.
* @notes  
* 2.1.02.0- Ported to KEIL uVision 5.1
* 2.1.01.0- Fixed re-calibration issue.
* 2.1.00.0- Using 2.2 driver's release. Fixed tamper detection issue. Contains
*           couple of conditional compilations required for compatibility with 
*           gcc compiler.
* 2.0.09.0- Using new metering library with Rogowski Coil extension. Use filter 
*           based metering algorithm configuration tool rev. 1.0.12 and above.
* 2.0.08.0- Updated metering library with APWR_OFS and RPWR_OFS; the power 
*           offsets we're introduced to compensate crosstalk between current and 
*           voltage channels - use configuration tool rev. 1.0.11 and above.
*         - Based on updated subfolder structure. 
* 2.0.07.0- Compacted filter-based metering library (the definition file can be 
*           updated by the configuration tool). 
*         - Ported to 2.0 silicon and based on 2.x software drivers.
*         - Added opto-coupler pulse output interface (routed to XBAR_OUT[6])
*         - Phase voltage scaled to Umax=410.0V 
* 2.0.06.0- LCD driver configurations, configuring NMI_B pin as LCD27.
* 2.0.05.0- On-chip peripherals accessed using Bit Manipulation Engine (BME).
* 2.0.04.0- Updated LCD driver and panel configuration.
* 2.0.03.0- Unused memory filled by "UDF #<imm8>" instruction. This defines a 
*           permanently-undefined opcode (0xde00) that generates an 
*           "undefined instruction exception" if execution is attempted.
*         - Energy counters and screen index stored in a non-initialized on-chip
*           RAM starting at address 0x20000000.
*         - Added hardfault_isr() handler.
*         - Reset pin configured as GPIO and driven low in normal operation to
*           improve EMC susceptibility.
* 2.0.02.0- MMA8491 forced to shutdown mode prior entering VLLS0_POR_ON mode. 
*         - added high-resolution imp/kWh and imp/kVARh pulse outputs  
* 2.0.01.0- ported to HW rev.B. Added FreeMASTER TSA via IR interface. Included 
*           tamper detection and additional small improvements in algorithm and 
*           firmware.
******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "drivers.h"
#include "lcd.h"
#include "fraclib.h"
#include "meterlib.h"
#include "appconfig.h"
#include "freemaster.h"
#include "config.h"
#include "meterlib1ph_cfg.h"      /* auto-generated algorithm definition file */
#include "mk341ph.h"              /* application defines and macros           */

/******************************************************************************
 * user data type defintions
 ******************************************************************************/  
typedef void (*MENU_FCN )(void);  /* definition menu functions data type      */
typedef void (*FMCMD_FCN)(void);  /* definition command functions data type   */

/******************************************************************************
 * public function prototypes
 ******************************************************************************/
/* hardfault exception handler prototype                                      */
void hardfault_isr (void);

/******************************************************************************
 * static function prototypes
 ******************************************************************************/
/* initialization function prototypes                                         */
static void norm_mode_hw_init (void);
static void stby_mode_hw_init (void);

/* menu function prototypes                                                   */
static void menu_disp_date   (void);
static void menu_disp_time   (void);
static void menu_disp_urms   (void);
static void menu_disp_irms   (void);
static void menu_disp_kW     (void);
static void menu_disp_kVAR   (void);
static void menu_disp_kVA    (void); 
static void menu_disp_kWh    (void);
static void menu_disp_kVARh  (void);
static void menu_disp_freq   (void);
static void menu_disp_vbat   (void);
static void menu_disp_snum   (void);
static void menu_disp_pnum1  (void);
static void menu_disp_pnum2  (void);
static void menu_disp_class  (void);

/* command function prototypes                                                */
static void fcmd_dummy_fcn   (void);
static void fcmd_sw_reset    (void);
static void fcmd_clr_counter (void);
static void fcmd_clr_tampers (void);
static void fcmd_set_clock   (void);
static void fcmd_set_pulse1  (void);
static void fcmd_set_pulse2  (void);
static void fcmd_calibrate   (void);
static void fcmd_save_config (void);

/******************************************************************************
 * callback function prototypes
 ******************************************************************************/
static void afech2_callback  (AFE_CH_CALLBACK_TYPE type, int32 result);
static void button_callback  (PORT_CALLBACK_SRC src, uint8 pin);
static void auxcalc_callback (void);
static void display_callback (void);

/******************************************************************************
 * static data definitions
 ******************************************************************************/
static volatile const Frac64 kwh_pulses[] = 
{
  KWH_PULS_NUM(1e2),KWH_PULS_NUM(2e2),KWH_PULS_NUM(5e2),KWH_PULS_NUM(1e3),
  KWH_PULS_NUM(2e3),KWH_PULS_NUM(5e3),KWH_PULS_NUM(1e4),KWH_PULS_NUM(2e4),
  KWH_PULS_NUM(5e4),KWH_PULS_NUM(1e5),KWH_PULS_NUM(2e5),KWH_PULS_NUM(5e5),
  KWH_PULS_NUM(1e6),KWH_PULS_NUM(2e6),KWH_PULS_NUM(4e6),KWH_PULS_NUM(6e6)
};

static volatile const Frac64 kvarh_pulses[] = 
{
  KVARH_PULS_NUM(1e2),KVARH_PULS_NUM(2e2),KVARH_PULS_NUM(5e2),KVARH_PULS_NUM(1e3),
  KVARH_PULS_NUM(2e3),KVARH_PULS_NUM(5e3),KVARH_PULS_NUM(1e4),KVARH_PULS_NUM(2e4),
  KVARH_PULS_NUM(5e4),KVARH_PULS_NUM(1e5),KVARH_PULS_NUM(2e5),KVARH_PULS_NUM(5e5),
  KVARH_PULS_NUM(1e6),KVARH_PULS_NUM(2e6),KVARH_PULS_NUM(4e6),KVARH_PULS_NUM(6e6)
};

static MENU_FCN  menu_fcn[]= 
{
  menu_disp_date,menu_disp_time,menu_disp_urms,menu_disp_irms,menu_disp_kW,
  menu_disp_kVAR,menu_disp_kVA,menu_disp_kWh,menu_disp_kVARh,menu_disp_freq,
  menu_disp_vbat,menu_disp_pnum1,menu_disp_pnum2,menu_disp_snum,menu_disp_class
};

static FMCMD_FCN fcmd_fcn[]= 
{ 
  fcmd_dummy_fcn,fcmd_sw_reset,fcmd_clr_counter,fcmd_clr_tampers,fcmd_set_clock,
  fcmd_set_pulse1,fcmd_set_pulse2,fcmd_calibrate,fcmd_save_config 
};

static volatile tMETERLIB1PH_DATA  mlib = METERLIB1PH_CFG;
static volatile double          wh, varh, urms, irms, w, var, va, umax, imax, freq;
static volatile Frac32          u24_sample, i24_sample;
static const    uint32          pulses[]= 
{ 
  100l,200l,500l,1000l,2000l,5000l,10000l,20000l,50000l,100000l,200000l,500000l,
  1000000l,2000000l,4000000l,6000000l 
};

static          char            buf[12];
static          tTIME           wTime, rTime;         
static volatile int8            fcmd_idx    = 0;
static volatile int8            pulse_index = 0;
static          uint16          password    = 0;
static          uint8           tamper_flags= 0;
static          uint16          reset_source= 0;

/******************************************************************************
 * FreeMASTER's TSA definitions - begin
 ******************************************************************************/
FMSTR_TSA_TABLE_BEGIN(ctrl)
    FMSTR_TSA_RW_VAR(password,        FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(pulse_index,     FMSTR_TSA_SINT8)
    FMSTR_TSA_RW_VAR(fcmd_idx,        FMSTR_TSA_SINT8)
    FMSTR_TSA_RW_VAR(ramcnt.menu_idx, FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(ramcnt.pulse_out,FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(wTime, FMSTR_TSA_USERTYPE(tTIME))
    FMSTR_TSA_STRUCT(tTIME)
    FMSTR_TSA_MEMBER(tTIME, sec,      FMSTR_TSA_UINT8)
    FMSTR_TSA_MEMBER(tTIME, min,      FMSTR_TSA_UINT8)    
    FMSTR_TSA_MEMBER(tTIME, hour,     FMSTR_TSA_UINT8)    
    FMSTR_TSA_MEMBER(tTIME, day,      FMSTR_TSA_UINT8)    
    FMSTR_TSA_MEMBER(tTIME, wday,     FMSTR_TSA_UINT8)    
    FMSTR_TSA_MEMBER(tTIME, mon,      FMSTR_TSA_UINT8)     
    FMSTR_TSA_MEMBER(tTIME, year,     FMSTR_TSA_UINT16)
FMSTR_TSA_TABLE_END()  

FMSTR_TSA_TABLE_BEGIN(meas)
    FMSTR_TSA_RO_VAR(u24_sample,      FMSTR_TSA_FRAC32)
    FMSTR_TSA_RO_VAR(i24_sample,      FMSTR_TSA_FRAC32)
    FMSTR_TSA_RO_VAR(mlib.uDcb[1].yIIR,  FMSTR_TSA_SINT64)  
    FMSTR_TSA_RO_VAR(mlib.iDcb[1].yIIR,  FMSTR_TSA_SINT64)
    FMSTR_TSA_RO_VAR(wh,              FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RO_VAR(varh,            FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RO_VAR(urms,            FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RO_VAR(irms,            FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RO_VAR(w,               FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RO_VAR(var,             FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RO_VAR(va,              FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RO_VAR(freq,            FMSTR_TSA_DOUBLE)  
    FMSTR_TSA_RO_VAR(ramcnt.wh_cnt,   FMSTR_TSA_UINT32)
    FMSTR_TSA_RO_VAR(ramcnt.varh_cnt, FMSTR_TSA_UINT32)  
FMSTR_TSA_TABLE_END()    

FMSTR_TSA_TABLE_BEGIN(param)
    FMSTR_TSA_RO_VAR(ramcfg.wh_idx,   FMSTR_TSA_UINT16)
    FMSTR_TSA_RO_VAR(ramcfg.varh_idx, FMSTR_TSA_UINT16)
    FMSTR_TSA_RO_VAR(ramcfg.u_gain,   FMSTR_TSA_FRAC32)
    FMSTR_TSA_RO_VAR(ramcfg.i_gain,   FMSTR_TSA_FRAC32)
    FMSTR_TSA_RO_VAR(ramcfg.delay,    FMSTR_TSA_SINT16)
    FMSTR_TSA_RO_VAR(ramcfg.flag,     FMSTR_TSA_UINT16)  
FMSTR_TSA_TABLE_END()     

FMSTR_TSA_TABLE_BEGIN(other)
    FMSTR_TSA_RO_VAR(rTime, FMSTR_TSA_USERTYPE(tTIME))
    FMSTR_TSA_STRUCT(tTIME)
    FMSTR_TSA_MEMBER(tTIME, sec,      FMSTR_TSA_UINT8)
    FMSTR_TSA_MEMBER(tTIME, min,      FMSTR_TSA_UINT8)    
    FMSTR_TSA_MEMBER(tTIME, hour,     FMSTR_TSA_UINT8)    
    FMSTR_TSA_MEMBER(tTIME, day,      FMSTR_TSA_UINT8)    
    FMSTR_TSA_MEMBER(tTIME, wday,     FMSTR_TSA_UINT8)    
    FMSTR_TSA_MEMBER(tTIME, mon,      FMSTR_TSA_UINT8)     
    FMSTR_TSA_MEMBER(tTIME, year,     FMSTR_TSA_UINT16)  
    FMSTR_TSA_RO_VAR(CMP1_SCR,        FMSTR_TSA_UINT8)
    FMSTR_TSA_RO_VAR(TMR0_CNTR,       FMSTR_TSA_UINT16)
    FMSTR_TSA_RO_VAR(TMR0_CAPT,       FMSTR_TSA_UINT16)
    FMSTR_TSA_RO_VAR(tamper_flags,    FMSTR_TSA_UINT8)
    FMSTR_TSA_RO_VAR(reset_source,    FMSTR_TSA_UINT16)
FMSTR_TSA_TABLE_END()

FMSTR_TSA_TABLE_LIST_BEGIN()
    FMSTR_TSA_TABLE(ctrl)
    FMSTR_TSA_TABLE(meas) 
    FMSTR_TSA_TABLE(param)  
    FMSTR_TSA_TABLE(other)  
FMSTR_TSA_TABLE_LIST_END()
/******************************************************************************
 * FreeMASTER's TSA definitions - end
 ******************************************************************************/

/******************************************************************************
 * main function - device boots in run mode and uses external 1 MHz clock for 
 *                 flash calibration after reset.
 ******************************************************************************/ 
#if defined (__ICCARM__)
  #pragma diag_suppress=Pa082
#endif

uint64_t u64;
uint32_t u32_1 = 1000000,u32_2 = 1000000;
void main (void)
{    

	u32_1 = 1000000;
	u32_2 = 1000000;
	u64 = FRACLIB_SSMUL32(u32_1,u32_2);
	while(1);
  /* initialize watchdog - reset time 0.6s < tWDOG < 0.1s                     */
  WDOG_Init (WDOG_MODULE_WINDOW_STOP_DI_CONFIG(WDOG_LPO_CLK,WDOG_DIV1,600,100));
  
  /* reset pin filtered in all modes and filter clocked by LPO                */
  RCM_Init (RCM_MODULE_FILT_CLOCKED_BY_LPO_EN_IN_ALL_MODES_CONFIG);
  
  /* enable PMC buffer operation also in VLPx modes                           */      
  PMC_Init (PMC_MODULE_LVDRE_ON_LVDINT_OFF_LVWINT_OFF_CONFIG(PMC_LVDL,PMC_LVW1),
            PMC_INTREG_BGEN_ON_BGBE_LDON_CONFIG, PRI_LVL0, (PMC_CALLBACK)NULL);
  
  /* set PTA4 to analogue mode (LCD27)                                        */
  SIM_EnableModule (PORTA);
  PORT_Init (PORTA, PORT_MODULE_ALT0_MODE, PIN4); 
  
  /* initialize power input detect pin                                        */
  SIM_EnableModule (PORTC);
  PORT_Init (PORTC , PORT_MODULE_ALT1_EITHER_EDGE_IRQ_MODE, POWERON_MASK);
  GPIO_Init (FGPIOC, GPIO_INP_MODE, POWERON_MASK);
    
  /* IRTC init - call this function only if power-on reset occurs             */
  if (RCM_POR & (reset_source = (RCM_GetResetSrc()|(ramcnt.hardfault<<15))))  
  {
    IRTC_Init (IRTC_MODULE_COMP_OFF_CONFIG(LFSR_CLK_2HZ,5),
               IRTC_TAMPER_PIN_DI_CONFIG,
               IRTC_TAMPER_PIN_PASSIVE_POL_HIGH_CONFIG(TAMPER_FILT_CLK_2HZ,5),
               IRTC_TAMPER_PIN_PASSIVE_POL_HIGH_CONFIG(TAMPER_FILT_CLK_2HZ,5),
               0);
    /* suppress initial battery removal "tamper" detect                       */
    IRTC_ClrIsrFlag (IRTC_TAMPER1_MASK|IRTC_TAMPER2_MASK);
  }
  
  /* check validity of non-initialized variables in system ram and if data    */
  /* are not valid use default data stored in flash                           */
  if (ramcnt.flag != 0x5555) 
    memcpy ((void*)&ramcnt, (void*)&nvmcnt, sizeof(tCONFIG_NOINIT_DATA));
  
  /* read application setting from flash; if called for the first time        */
  /* flag=0xffff then initialize clock and update flag                        */
  CONFIG_ReadFlash ((tCONFIG_FLASH_DATA*)&ramcfg);
  if (ramcfg.flag == 0xffff)
  {
    BUILD_DATE_TIME (wTime); 
    IRTC_SetClock   (&wTime);
    CONFIG_SaveFlash ((tCONFIG_FLASH_DATA*)&ramcfg, 0xfff5);
  }
  
  /****************************************************************************
   * if power is applied execute normal mode 
   ****************************************************************************/
  if (GPIO_Get (FGPIOC) & POWERON_MASK)
  {
    /* initialize on-chip peripherals in normal mode                          */
    norm_mode_hw_init (); 
    
    /* release I/O states                                                     */
    if (PMC_GetACKISO()) { PMC_SetACKISO(); } 
    
    /* main software loop                                                     */
    while(1);
  }
  /****************************************************************************
   * if power is not applied execute standby mode 
   ****************************************************************************/
  else
  {
    /* initialize on-chip peripherals in standby mode                         */
    stby_mode_hw_init (); 
    
    /* release I/O states                                                     */
    if (PMC_GetACKISO()) { PMC_SetACKISO(); } 
    
    /* main software loop in standby mode - executes in ~0.3s                 */
    while(1) 
    {       
      IRTC_GetClock (&rTime);           /* update clock data structure        */      
      /* read and process tamper flags                                        */
      tamper_flags = IRTC_GetIsrFlag(IRTC_TAMPER1_MASK) | 
                     (IRTC_GetIsrFlag(IRTC_TAMPER2_MASK)<<1);
      if (tamper_flags) { LCD_SYM_WARN_ON;  } 
      else              { LCD_SYM_WARN_OFF; }  
      menu_fcn[ramcnt.menu_idx]();      /* store text string to buffer        */
      LCD_DISP_TARIF_SYM (ramcfg.tarif);/* display tarif information          */
      arch_delay (16000l);              /* wait 130ms                         */
      WDOG_WriteRefreshSeq ();          /* watchdog refresh                   */ 
      /* check LPTMR compare flag - if set go to sleep mode                   */
      if (LPTMR_GetCmpFlag())  
      { 
        /* Power down IR interface and 4kB EEPROM                             */
        PORT_Init (PORTF, PORT_MODULE_ALT0_MODE, POWERAUX_MASK);
        /* Force MMA8491 to shutdown mode                                     */
        GPIO_Clr  (FGPIOG, MMA_EN_MASK); 
        /* Disabled CAT25040 SS pin                                           */
        PORT_Init (PORTD, PORT_MODULE_ALT0_MODE, EEPROM_SS_MASK);
        PMC_ClrBGEN     ();             /* disable bandgap in VLPR modes      */
        LCD_Segments_OFF();             /* shut down LCD segments             */
        SMC_SetMode(VLLS2);             /* 4 kB of SRAM is powered in VLLS2   */ 
      }
    }
  }
}

/******************************************************************************
 * hardware init function definitions
 ******************************************************************************/ 
/* initializes RUN operation mode and necessary on-chip peripherals           */
static void norm_mode_hw_init (void)
{    
  /* SMC, SIM and PLL modules                                                 */ 
  SMC_Init (SMC_MODULE_VLPS_ON_VLLS_ON_CONFIG);
  SIM_Init (SIM_MODULE_NORM_MODE_SYSCLKMODE_SYSCLK_DIV1_CONFIG);
  PLL_Init (PLL_MODULE_PEE_12_288MHZ_CONFIG);               
    
  /* if calibration data were collected then calibration parameters are       */
  /* calculated and saved to flash                                            */
  if (CONFIG_CalcCalibData ((tCONFIG_FLASH_DATA*)&ramcfg) == TRUE)
    CONFIG_SaveFlash ((tCONFIG_FLASH_DATA*)&ramcfg, ramcfg.flag);
  
  /* VREF module must be initialized after SIM module                         */
  VREF_Init (VREF_MODULE_CHOP_ON_1V75_0V4_HPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_L_BUFF_OFF);
  VREFH_Trim(ramcfg.vrefh);             /* VREFH trimming                     */
  VREFL_Trim(ramcfg.vrefl);             /* VREFL trimming                     */
      
  /* RESET pin set as GPIO and driven low to improve EMC susceptibility       */
  PORT_Init (PORTE , PORT_MODULE_ALT1_MODE, RESET_PIN_MASK);
  GPIO_Init (FGPIOE, GPIO_OUT_LOGIC0_MODE , RESET_PIN_MASK);
  
  /* Power up IR interface and 4kB EEPROM                                     */
  PORT_Init (PORTF , PORT_MODULE_ALT1_MODE, POWERAUX_MASK);
  GPIO_Init (FGPIOF, GPIO_OUT_LOGIC1_MODE , POWERAUX_MASK);
  
  /* User push button                                                         */
  PORT_Init (PORTD , PORT_MODULE_BUTTON_IRQ_MODE, BUTTON_MASK);
  GPIO_Init (FGPIOD, GPIO_INP_MODE, BUTTON_MASK);
  PORT_InstallCallback (PRI_LVL3, button_callback);
  
  /* User LED                                                                 */
  PORT_Init (PORTH , PORT_MODULE_ALT1_MODE, LED_USER_MASK );
  GPIO_Init (FGPIOH,  GPIO_OUT_LOGIC1_MODE, LED_USER_MASK );
  
#ifdef FMSTR_UART3   
  /* 2.4GHz Wireless UART                                                     */
  PORT_Init (PORTC, PORT_MODULE_ALT2_MODE, UART3_TX_MASK|UART3_RX_MASK);
  UART_Init (UART3, UART_MODULE_INTRMODE_CONFIG(38400,12.288e6)); 
  UART_InstallCallback (UART2_UART3, PRI_LVL2, (UART_CALLBACK)FMSTR_Isr);
  FMSTR_Init();
#endif
#ifdef FMSTR_UART0  
  /* IR TERMINAL                                                              */
  PORT_Init (PORTC, PORT_MODULE_ALT2_MODE, UART0_TX_MASK);
  PORT_Init (PORTD, PORT_MODULE_ALT2_MODE, UART0_RX_MASK);
  UART_Init (UART0, UART_MODULE_INTRMODE_TXINV_CONFIG(4800,12.288e6/2)); 
  UART_InstallCallback (UART0_UART1, PRI_LVL2, (UART_CALLBACK)FMSTR_Isr);
  FMSTR_Init();
#endif
  
  /* MMA8491 3-axis accelerometer                                             */
  PORT_Init (PORTG , PORT_MODULE_ALT1_MODE, MMA_EN_MASK);
  GPIO_Init (FGPIOG,  GPIO_OUT_LOGIC0_MODE, MMA_EN_MASK);
  
  /* CAT25040 eeprom module                                                   */
  PORT_Init (PORTD , PORT_MODULE_ALT1_MODE, EEPROM_SS_MASK);
  GPIO_Init (FGPIOD,  GPIO_OUT_LOGIC1_MODE, EEPROM_SS_MASK);
    
  /* Software callbacks                                                       */
  SWISR_HandlerInit (0, PRI_LVL1, auxcalc_callback);
  SWISR_HandlerInit (1, PRI_LVL3, display_callback);
  
  /* LCD module                                                               */
  LCD_Init  (YDDC41919AATDRPN_VSUPPLY_OFF_CPSEL_OFF_RVEN_OFF_BACKPLANES_4,
             YDDC41919AATDRPN_LCD_FRONT_PLANE_PINS,
             YDDC41919AATDRPN_LCD_BACK_PLANE_PINS,
             YDDC41919AATDRPN_LCD_ASSIGN_BACK_PLANES);
  
  /* AFE module                                                               */
  SIM_SelAfePllClkSrc (SIM_MCG_PLL_CLK);
  AFE_ChanInit (CH0, 
                AFE_CH_SWTRG_CCM_PGAON_CONFIG(DEC_OSR1024,PGA_GAIN),      
                -ramcfg.delay, 
                PRI_LVL0, 
                (AFE_CH_CALLBACK)NULL);
  AFE_ChanInit (CH2, 
                AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR1024), 
                ramcfg.delay, 
                PRI_LVL0, 
                (AFE_CH_CALLBACK)afech2_callback);
  AFE_Init      (AFE_MODULE_RJFORMAT_CONFIG(AFE_PLL_CLK, AFE_DIV2, 12.288e6));
  
  /* ADC module                                                               */
  ADC_Init  (ADC_MODULE_16B_SWTRG_BGAP_LP_INTERNAL_CLK_CONFIG,
             HWAVG_OFF,
             ADC_CH_SE_POLL_CONFIG(AD1),
             ADC_CH_DISABLE_CONFIG,
             ADC_CH_DISABLE_CONFIG,
             ADC_CH_DISABLE_CONFIG,
             PRI_LVL0,
             (ADC_CALLBACK)NULL);
  
  /* XBAR module - all paths are setup in the code below                      */
  XBAR_Init (XBAR_MODULE_NO_EDGE_DETECT_CONFIG,PRI_LVL0,NULL);
  
  /* Fine pulse output control                                                */
  /* PTE0 (LED_KWH_MASK)  : T1 output routed via XBAR_OUT4                    */ 
  /* PTD1 (LED_KVARH_MASK): T2 output routed via XBAR_OUT3                    */
  /* PTF1 (PULSE_OUT):                routed via XBAR_OUT6                    */
  PORT_Init (PORTE, PORT_MODULE_ALT3_MODE, LED_KWH_MASK  );
  PORT_Init (PORTD, PORT_MODULE_ALT4_MODE, LED_KVARH_MASK);
  PORT_Init (PORTF, PORT_MODULE_ALT3_MODE, PULSE_OUT_MASK);
  TMR_Init  (CH1,
            TMR_PULSE_OUTPUT_ONE_SHOOT_MODE_CONFIG(SEC_CNTR1_INP,SET_ON_COMP),
            0,
            0,
            0,
            0,
            0,
            PRI_LVL0,
            (TMR_CH_CALLBACK)NULL);
  TMR_Init  (CH2,
            TMR_PULSE_OUTPUT_ONE_SHOOT_MODE_CONFIG(SEC_CNTR2_INP,SET_ON_COMP),
            0,
            0,
            0,
            0,
            0,
            PRI_LVL0,
            (TMR_CH_CALLBACK)NULL);
  XBAR_Path (XBAR_TMR1,XBAR_OUT4);
  XBAR_Path (XBAR_TMR2,XBAR_OUT3);
  
  /* Route selected pulse to pulse output                                     */
  if (ramcnt.pulse_out == 1) { XBAR_Path (XBAR_TMR2,XBAR_OUT6); }
  else                       { XBAR_Path (XBAR_TMR1,XBAR_OUT6); } 
  
  /* Frequency measurement                                                    */
  XBAR_Path (XBAR_CMP1OUT,XBAR_TMR0SEC);
  CMP_Init  (CMP1,CMP_FREQ_DETECT_MODE_CONFIG,IN2_SDADP2,IN3_SDADM2);
  TMR_Init  (CH0,
            TMR_FREQ_DETECT_MODE(BUS_CLK_DIV4,SEC_CNTR0_INP,LOAD_RISING_EDGE),
            0,
            0,
            0,
            0,
            0,
            PRI_LVL0,
            (TMR_CH_CALLBACK)NULL);
  
  EnableInterrupts();
  AFE_SwTrigger   (CH0|CH2);
}

/* initializes VLPR operation mode and necessary on-chip peripherals          */
static void stby_mode_hw_init (void)
{
  /* SMC and SIM modules                                                      */ 
  SMC_Init  (SMC_MODULE_VLPS_ON_VLLS_ON_CONFIG); 
  SIM_Init  (SIM_MODULE_STBY_MODE_SYSCLKMODE_SYSCLK_DIV16_CONFIG);
  
  /* switch to VLPR mode                                                      */
  SMC_SetMode(VLPR);
  
  /* Power up IR interface and 4kB EEPROM                                     */
  PORT_Init (PORTF , PORT_MODULE_ALT1_MODE, POWERAUX_MASK);
  GPIO_Init (FGPIOF, GPIO_OUT_LOGIC1_MODE , POWERAUX_MASK );

  /* User push button                                                         */
  PORT_Init (PORTD , PORT_MODULE_BUTTON_IRQ_MODE, BUTTON_MASK);
  GPIO_Init (FGPIOD, GPIO_INP_MODE, BUTTON_MASK);
  PORT_InstallCallback (PRI_LVL3, button_callback);
    
  /* MMA8491 3-axis accelerometer                                             */
  PORT_Init (PORTG , PORT_MODULE_ALT1_MODE, MMA_EN_MASK);
  GPIO_Init (FGPIOG, GPIO_OUT_LOGIC0_MODE , MMA_EN_MASK);
  
  /* CAT25040 eeprom module                                                   */
  PORT_Init (PORTD , PORT_MODULE_ALT1_MODE, EEPROM_SS_MASK);
  GPIO_Init (FGPIOD, GPIO_OUT_LOGIC1_MODE , EEPROM_SS_MASK);
  
  /* LLWU module                                                              */
  LLWU_Init (LLWU_MODULE_PTC5_RISING_EDGE_PTD7_FALLING_EDGE_FILT_ENABLED, 
             PRI_LVL0, 
             (LLWU_CALLBACK)NULL);
  
  /* LPTMR module - 5 seconds timeout                                         */
  LPTMR_Init(LPTMR_MODULE_TIMER_CNT_RST_ONTCF_MODE_CONFIG(0,LPTMR_LPOCLK), 2499);
  
  /* LCD module                                                               */
  LCD_Init  (YDDC41919AATDRPN_VSUPPLY_OFF_CPSEL_OFF_RVEN_OFF_BACKPLANES_4,
             YDDC41919AATDRPN_LCD_FRONT_PLANE_PINS,
             YDDC41919AATDRPN_LCD_BACK_PLANE_PINS,
             YDDC41919AATDRPN_LCD_ASSIGN_BACK_PLANES);
  
  /* ADC module                                                               */
  ADC_Init  (ADC_MODULE_16B_SWTRG_BGAP_LP_INTERNAL_CLK_CONFIG,
             HWAVG_OFF,
             ADC_CH_SE_POLL_CONFIG(AD1),
             ADC_CH_DISABLE_CONFIG,
             ADC_CH_DISABLE_CONFIG,
             ADC_CH_DISABLE_CONFIG,
             PRI_LVL0,
             (ADC_CALLBACK)NULL);
  
  LPTMR_Enable    ();
  EnableInterrupts();
}

/******************************************************************************
 * callback function definitions
 ******************************************************************************/
/* measurements callback @ 6000 Hz, duration 6.719 us, load 4.04 %            */
static void afech2_callback (AFE_CH_CALLBACK_TYPE type, int32 result)
{
  if (type == COC_CALLBACK) 
  {  
    /* read measurement samples                                               */
    u24_sample = result             << U_SCALE; /* up-scale phase voltage     */  
    i24_sample = AFE_ChanRead (CH0) << I_SCALE; /* up-scale phase current     */
    
    /* update offsets - needed for offset calculation during calibration      */
    CONFIG_UpdateOffsets((tCONFIG_FLASH_DATA*)&ramcfg,u24_sample,i24_sample);
    SWISR_HandlerFreqCall (0, 6000, 1200);   
  }
}

/* calculation callback @1200 Hz, duration 681 us, load 81.7 %                */        
static void auxcalc_callback (void)
{
  static int cnt_1=0, cnt_2=0;
 
  /* generate pulses on output LEDs                                           */
  PULSE_OUT (CH1,mlib.Wh  , cnt_1, PULSE_MAX_ONTIME(0.020), TMR_RES);
  PULSE_OUT (CH2,mlib.VARh, cnt_2, PULSE_MAX_ONTIME(0.020), TMR_RES);
  
  /* scale and process measured samples                                       */
  METERLIB1PH_RemoveDcBias ((tMETERLIB1PH_DATA*)&mlib, 
                          -L_mul ((u24_sample-ramcfg.u_offset), ramcfg.u_gain), 
                          -L_mul ((i24_sample-ramcfg.i_offset), ramcfg.i_gain));
  
  /* calculate active energy                                                  */
//  METERLIB1PH_CalcWattHours ((tMETERLIB1PH_DATA*)&mlib,(long*)&ramcnt.wh_cnt,
//                          kwh_pulses[ramcfg.wh_idx]);
//  
//  /* calculate reactive energy                                                */
//  METERLIB1PH_CalcVarHours ((tMETERLIB1PH_DATA*)&mlib, (long*)&ramcnt.varh_cnt,
//                          kvarh_pulses[ramcfg.varh_idx]);

  /* calculate remaining non-billing quantities                               */
  METERLIB1PH_CalcAuxiliary ((tMETERLIB1PH_DATA*)&mlib);
  FMSTR_Recorder ();
  SWISR_HandlerFreqCall (1, 1200, 4);
}

/* display and user interface callback @ 4 Hz, duration 7654 us, load 3.1 %   */
#if defined (__ICCARM__)
  #pragma diag_suppress=Pa082
#endif
static void display_callback (void)
{
  uint16 tmp16;

//  /* read data from metering engine - this is required for FreeMASTER         */
//  METERLIB1PH_ReadResults ((tMETERLIB1PH_DATA*)&mlib, 
//                        (double*)&urms, 
//                        (double*)&irms, 
//                        (double*)&w, 
//                        (double*)&var, 
//                        (double*)&va, 
//                        (double*)&umax, 
//                        (double*)&imax);  

  /* calibration - perform pre-processing conditionally and toggle user LED   */
  CONFIG_PreProcessing ((tCONFIG_FLASH_DATA*)&ramcfg, urms, irms, w, var);
  if (CONFIG_CAL_PENDING ()) { GPIO_Tgl (FGPIOH, LED_USER_MASK ); }
  else                       { GPIO_Set (FGPIOH, LED_USER_MASK ); } 
  
  /* update display and process commands                                      */
  IRTC_GetClock (&rTime);                 /* update clock data structure      */
  
  /* read and process tamper flags                                            */
  tamper_flags = IRTC_GetIsrFlag(IRTC_TAMPER1_MASK) | 
                 IRTC_GetIsrFlag(IRTC_TAMPER2_MASK)<<1;
  if (tamper_flags) { LCD_SYM_WARN_ON;  }
  else              { LCD_SYM_WARN_OFF; }

  /* read phase voltage frequency                                             */
  if (TMR_ReadClrCaptReg(CH0,&tmp16)) { freq = TMR2FREQ(tmp16); }
   
  menu_fcn[ramcnt.menu_idx]();            /* store text string to buffer      */
  LCD_DISP_TARIF_SYM  (ramcfg.tarif);     /* display tarif information        */
  fcmd_fcn[fcmd_idx]  ();                 /* process FreeMASTER command       */  
  /* refresh watchdog Timer                                                   */
  WDOG_WriteRefreshSeq ();
}

/* digital logic asynchronous processing                                      */
static void button_callback(PORT_CALLBACK_SRC src, uint8 pin)
{
  /* PTD7 user button                                                         */
  if (src == PORTD_CALLBACK)
  {
    if (pin & BUTTON_MASK)
    { 
      /* update menu index                                                    */
      if ((++ramcnt.menu_idx) >= DIM(menu_fcn)) { ramcnt.menu_idx = 0; }
      
      /* re-initialize LPTMR operation if power is not applied                */
      if (~GPIO_Get (FGPIOC) & POWERON_MASK) { LPTMR_Reset(); } 
    }
  }
  /* PTC5 power on signal                                                     */
  if (src == PORTC_CALLBACK) 
  { 
    if (pin & POWERON_MASK)                    
    { 
      DisableInterrupts ();   /* program terminates so interrupts not needed  */
      if (SMC_GetMode() == SMC_MODE_RUN) /* falling edge occurred - power off */
      { 
        /* write data gathered in the pre-calibration stage to the flash      */
        if (ramcfg.flag == 0xffa5) 
          CONFIG_SaveFlash ((tCONFIG_FLASH_DATA*)&ramcfg, ramcfg.flag);
      }
      else { /*do nothing*/ } /* rising edge occurred in VLPR mode - power up */
      LCD_Segments_OFF  ();   /* shut down LCD segments                       */
      SystemReset       ();   /* next boot will be to either RUN or VLPR mode */
    } 
  }
}

/******************************************************************************
 * Menu functions definition
 ******************************************************************************/
static void menu_disp_date  (void)
{
  sprintf((char*)buf," %02d%02d%04d",rTime.day,rTime.mon,rTime.year);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_KW_OFF; LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_COLONS_ON; LCD_SYM_OBIS_CODE(0,9,2);
}

static void menu_disp_time  (void)
{
  sprintf((char*)buf," %02d%02d%02d  ",rTime.hour, rTime.min, rTime.sec);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_KW_OFF; LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_COLONS_ON; LCD_SYM_OBIS_CODE(0,9,1);
}

static void menu_disp_urms  (void)
{
  sprintf((char*)buf,"U %6.1f U",urms);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_KW_OFF; LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_OBIS_CODE(-1,-1,-1);
}

static void menu_disp_irms  (void)
{
  sprintf((char*)buf,"I %8.3f",irms);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_KW_OFF; LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_OBIS_CODE(-1,-1,-1);
}

static void menu_disp_kW    (void)
{
  sprintf((char*)buf,"A %8.3f",w/1e3);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_KW_ON; LCD_SYM_OBIS_CODE(1,6,0);
}

static void menu_disp_kVAR  (void)
{
  sprintf((char*)buf,"R %8.3f",var/1e3);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_KW_ON; LCD_SYM_OBIS_CODE(-1,-1,-1);
}

static void menu_disp_kVA   (void)
{
  sprintf((char*)buf,"S %8.3f",va/1e3);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_KW_ON; LCD_SYM_OBIS_CODE(-1,-1,-1); 
}

static void menu_disp_kWh   (void)
{ 
  sprintf((char*)buf,"A%09.3f", (double)ramcnt.wh_cnt/1e5);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_KW_ON; LCD_SYM_H_ON; LCD_SYM_OBIS_CODE(1,8,0);  
}

static void menu_disp_kVARh (void)
{
  sprintf((char*)buf,"R%09.3f",(double)ramcnt.varh_cnt/1e5);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_KW_ON; LCD_SYM_H_ON; LCD_SYM_OBIS_CODE(-1,-1,-1);
}

static void menu_disp_freq (void)
{ 
  if (SMC_GetMode() == SMC_MODE_RUN) 
    sprintf((char*)buf,"F %6.1f F",       freq);
  else 
    sprintf((char*)buf,"F %6.1f F",(double)0.0);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_COLONS_OFF; LCD_SYM_BATT_OFF; LCD_SYM_KW_OFF; LCD_SYM_H_OFF;  
  LCD_SYM_OBIS_CODE(0,0,0); 
}

static void menu_disp_vbat  (void)
{
  register uint16 tmp16;
  static uint8 flg = 1;
  
  /* read battery voltage and initiate next conversion                        */
  while (!ADC_Ready(CHA));
  tmp16=(uint16)ADC_Read(CHA);
  sprintf((char*)buf,"     %4.2fU",ADC2VBAT(tmp16));
  ADC_Start (CHA,AD1);
 
  /* check battery voltage threshold                                          */  
  if (tmp16 < VBAT2ADC(3.5)) { flg = !flg; }
  else                       { flg =    1; } 
  LCD_WriteStr     (buf);                 /* display text on LCD              */
  LCD_SYM_KW_OFF; LCD_SYM_H_OFF; LCD_COLONS_OFF;
  LCD_DISP_BATT_SYM(flg); LCD_SYM_OBIS_CODE(-1,-1,-1);
}

static void menu_disp_pnum1 (void)
{
  sprintf((char*)buf,"P1%7d", pulses[ramcfg.wh_idx]);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_KW_OFF; LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_OBIS_CODE(-1,-1,-1);
}

static void menu_disp_pnum2 (void)
{
  sprintf((char*)buf,"P2%7d", pulses[ramcfg.varh_idx]);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_KW_OFF; LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_OBIS_CODE(-1,-1,-1);
}

static void menu_disp_snum  (void)
{
  sprintf((char*)buf,"R2110-%03d", ramcfg.id);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_KW_OFF; LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_OBIS_CODE(0,0,0);
}

static void menu_disp_class (void)
{
  sprintf((char*)buf, METER_CL);
  LCD_WriteStr    (buf);                  /* display text on LCD              */
  LCD_SYM_KW_OFF; LCD_SYM_H_OFF; LCD_COLONS_OFF; LCD_SYM_BATT_OFF; 
  LCD_SYM_OBIS_CODE(-1,-1,-1);
}

/******************************************************************************
 * FreeMASTER commands definition
 ******************************************************************************/
static void fcmd_dummy_fcn  (void) 
{                  
  fcmd_idx=0;             
}

static void fcmd_sw_reset   (void) 
{ 
  ramcnt.hardfault = FALSE; /* clear hard fault notification                  */
  SystemReset (); 
}

static void fcmd_clr_counter(void)
{  
  ramcnt.wh_cnt     = 0l;
  mlib.Wh.dispRem   = 0ll;
  ramcnt.varh_cnt   = 0l;
  mlib.VARh.dispRem = 0ll;  
  fcmd_idx = 0;
}

static void fcmd_clr_tampers(void)
{  
  IRTC_ClrIsrFlag (IRTC_TAMPER1_MASK|IRTC_TAMPER2_MASK);
  fcmd_idx = 0;  
}

static void fcmd_set_clock  (void) 
{ 
  IRTC_SetClock (&wTime);
  fcmd_idx=0;
}

static void fcmd_set_pulse1 (void)
{
  ramcfg.wh_idx     = pulse_index;
  mlib.Wh.pulsRem   = 0ll;
  fcmd_idx = 0;
}

static void fcmd_set_pulse2 (void)
{
  ramcfg.varh_idx   = pulse_index;
  mlib.VARh.pulsRem = 0ll;
  fcmd_idx = 0;
}

static void fcmd_calibrate(void)
{  
  if (password == PASSWORD) 
  { 
    DisableInterrupts ();
    /* updated default configuration                                          */
    ramcfg.u_msrmax = FRAC32(-1.0);
    ramcfg.u_msrmin = FRAC32( 1.0); 
    ramcfg.i_msrmax = FRAC32(-1.0);
    ramcfg.i_msrmin = FRAC32( 1.0);
    ramcfg.delay    = 0;
    ramcfg.i_offset = 0l;
    ramcfg.i_gain   = FRAC32(-1.0);
    ramcfg.u_offset = 0l;
    ramcfg.u_gain   = FRAC32(-1.0);
    CONFIG_SaveFlash  ((tCONFIG_FLASH_DATA*)&ramcfg, 0xfff5); 
    SystemReset       (); 
  }
  password = 0;
  fcmd_idx = 0;
}

static void fcmd_save_config (void) 
{ 
  if (password == PASSWORD) 
  {
    DisableInterrupts ();
    CONFIG_SaveFlash  ((tCONFIG_FLASH_DATA*)&ramcfg, ramcfg.flag); 
    SystemReset       (); 
  }
  password = 0;
  fcmd_idx = 0;
}

/******************************************************************************
 * HardFault handler
 ******************************************************************************/
void hardfault_isr (void)
{
  ramcnt.hardfault = TRUE;  /* mark occurence of the hard fault exception     */
  for(;;)                   /* enter software loop to wait for watchdog reset */
  {
  }
}
/******************************************************************************
 * End of module
 ******************************************************************************/