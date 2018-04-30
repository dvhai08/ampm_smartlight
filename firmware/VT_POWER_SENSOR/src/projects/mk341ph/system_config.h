/*****************************************************************************
 * (c) Copyright 2010, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      config.h
 * @author    R55013
 * @version   1.0.1.0
 * @date      Sep-12-2011
 * @brief     Header file.
 ******************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H
#include "common.h"
#include "fraclib.h"
#include "smarthome_meter.h"

#define __CONFIG_DAILY_FLASH_BLOCK__   0x0001f800
#define __CONFIG_FLASH_BLOCK__   0x0001f400
#define __CONFIG_APP_FLASH_BLOCK__   0x0001ec00


/******************************************************************************
 * Macro definitions                                                          *
 ******************************************************************************/
#define CONFIG_CAL_PENDING()    (((ramcfg.flag&0x00f0)==0x00f0)?TRUE:FALSE)  
#define CONFIG_CAL_DONE()       (((ramcfg.flag&0xffff)==0xffa5)?TRUE:FALSE)  
/******************************************************************************
 * application password - must be entered via FreeMASTER prior recalibration
 * and/or saving parameters into flash
 ******************************************************************************/  
#define PASSWORD  11111                     /*!< default password             */   

 /*****************************************************************************
  * Default calibration structure definition
  *****************************************************************************/
#define PGA_GAIN  PGA_GAIN2                /*!< PGA gain for phase current   */ 
#define I_SCALE   0                         /*!< Up-scaling phase current     */
#define U_SCALE   1                         /*!< Up-scaling phase voltage     */           
#define METER_CL  " C 5-120A"               /*!< Meter class shown on LCD     */ 
#define METER_SN  95                        /*!< Meter serial number          */
#define CAL_CURR  5.0                       /*!< Calibration point - voltage  */                 
#define CAL_VOLT  230.0                     /*!< Calibration point - current  */
#define PHASE_CORRECT_TO_MKM (6000.0*1024.0/256.0/4096.0)
/******************************************************************************
 * configuration data structure definition														        *
 ******************************************************************************/

typedef struct
{
	double urms_cal;  /* preset calibration voltage [Vrms]                */
	double irms_cal[NUM_CHAN]; /* preset calibration current [Arms]                */
	double u_msr;
	double i_msr[NUM_CHAN];
	double p_msr[NUM_CHAN];
	/* basic power meter configuration data                                     */
	Frac32      u_msrmax;   /* measured maximum voltage                         */
	Frac32      u_msrmin;   /* measured minimum voltage                         */
	Frac32      i_msrmax[NUM_CHAN];   /* measured maximum current                        */
	Frac32      i_msrmin[NUM_CHAN];   /* measured minimum current                        */
  
	/* post-calibration data - calculated phase delay, offsets and gains        */
	int16_t     delay[NUM_CHAN];      /* ch1 delay in modulator clocks                   */
	int16_t     delayu;      /* u delay                                         */
	Frac32      i_offset[NUM_CHAN];   /* current measurement offset (AFE ch0)            */
  Frac32			u_offset;
  
	uint16_t relay_delay[NUM_CHAN];
  /* configuration flag                                                       */
  uint16_t      flag;
  tCORRECT_DATA correct;
} tCONFIG_FLASH_DATA;

typedef struct
{
  long        wh_cnt[NUM_CHAN];     /* active energy counter                            */
  long        varh_cnt[NUM_CHAN];   /* reactive energy counter                          */ 
  uint16      menu_idx;   /* menu index                                       */
  uint16      pulse_out;  /* 0=T1 => XBAR_OUT6, 1=T2 => XBAR_OUT6             */
  uint16      hardfault;  /* hardfault flag:TRUE-occurred, FALSE-didn't occur */                                     
  uint16      flag;       /* 0x5555= valid data                               */
                          /* 0x----= not valid data - initialization needed   */
} tCONFIG_NOINIT_DATA;

/******************************************************************************
 * exported data declarations                                                 *
 ******************************************************************************/
#if defined (__ICCARM__) /* IAR   */
  extern const              tCONFIG_NOINIT_DATA nvmcnt;
  extern __no_init volatile tCONFIG_NOINIT_DATA ramcnt; 
  extern __no_init volatile tCONFIG_FLASH_DATA  ramcfg;
  extern const tCONFIG_FLASH_DATA nvmcfg;
#elif defined(__GNUC__) || defined(__CC_ARM)/* CW GCC */
  extern const    tCONFIG_NOINIT_DATA nvmcnt;
  extern volatile tCONFIG_NOINIT_DATA ramcnt; 
  extern volatile tCONFIG_FLASH_DATA  ramcfg;
  extern const tCONFIG_FLASH_DATA nvmcfg;
#endif
  
/******************************************************************************
 * exported function prototypes														                    *
 ******************************************************************************/

extern void CONFIG_ReadFlash (void *addr,void *ptr,int len);
extern void CONFIG_SaveFlash (void *addr,void *ptr,int len);
extern void SystemConfigInit(void);

#endif /* __CONFIG_H */