

#ifndef __CALIB_H__
#define __CALIB_H__ 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "appconfig.h"
#include "drivers.h"
#include "fraclib.h"
#include "meterlib.h"
#include "system_config.h" 
/*******************************************************************************
 * Self-calibration settings (can be slightly modified according to used HW)   *
 ******************************************************************************/  
#define U_NOMINAL     (double) 220.0      /* nominal or calibration value of the voltage [V] */
#define U_START_DIFF  (double) 8.0        /* U-tolerance [V] for starting-up the autocalibration */
#define U_END_DIFF    (double) 0.01      /* U-tolerance [V] for termination the autocalibration */
//#define U_END_DIFF    (double) 2      /* U-tolerance [V] for termination the autocalibration */
#define I_ZERO        (double) 0.015      /* max. I [A] for offset autocalibration */
#define I_NOMINAL     (double) 5.0        /* nominal or calibration value of the current [A] */
#define I_START_DIFF  (double) 0.2        /* I-tolerance for starting-up the autocalibration [A] */
#define I_END_DIFF    (double) 0.005     /* I-tolerance [A] for termination the autocalibration */
#define PF_NOMINAL    (double) 0.5 /* PF calibration value for 45 degree U-I phase shift */
#define PF_START_DIFF (double) 0.008      /* PF tolerance for starting-up the autocalibration */
#define PF_END_DIFF   (double) 0.0005    /* PF tolerance for termination the autocalibration */
#define PF_GAIN       (double) 26500.0    /* approx. PF to MODCLK conversion factor */
#define TIMEOUT_CALIB_U	(uint32) 12 //3 sec
#define TIMEOUT_CALIB_I	(uint32) 12 //3 sec
#define TIMEOUT_CALIB_PF	(uint32) 24 //6 sec
#define TIME_CALIB_STABLE	(uint32) 16//4sec
#define TIME_CALIB_SUM_STABLE	(uint32) 12//3sec
#define PF_CNT        (uint32) 10          /* number of total equalities of the PF value for terminating the autocalibration (>0) */
#define OFFS_AVRG     (uint32) 20         /* number of averaged samples for offset autocalibration (>0) */
#define F_START_DIFF  (double) 20.0       /* freq. tolerance [mHz] for starting-up the autocalibration */
#define F_NOMINAL     (double) 50000.0    /* nominal frequency [mHz] */  


#define AUTO_CALIB_TYPE 0xa5
#define MANUAL_CALIB_TYPE 0x5a

typedef enum 
{
	CALIB_NO    = 0xFFFF,
	CALIB_PREPARE = 0xA5FF,
	CALIB_OFFS_U  = 0xA500,
	CALIB_U       = 0xA501,
	CALIB_OFFS_I0 = 0xA010,
	CALIB_I0      = 0xA510,
	CALIB_OFFS_I1 = 0xA011,
	CALIB_I1      = 0xA511,
	CALIB_OFFS_I2 = 0xA012,
	CALIB_I2      = 0xA512,
	CALIB_OFFS_I3 = 0xA013,
	CALIB_I3      = 0xA513,
	CALIB_PH_I0   = 0xAAA0,
	CALIB_PH_I1   = 0xAAA1,
	CALIB_PH_I2   = 0xAAA2,
	CALIB_PH_I3   = 0xAAA3
} CALIB_STATES;

//typedef enum 
//{
//	CALIB_NO    = 0xFFFF,
//	CALIB_PREPARE = 0xA5FF,
//	CALIB_OFFSET  = 0xA500,
//	CALIB_U       = 0xA501,
//	CALIB_I      = 0xA510,
//	CALIB_PH_I   = 0xAAA0
//} CALIB_STATES;

/***************************************************************************//*!
*
* @brief    Self-calibration function.
*
* @param   val      non-billing instantaneous metering values structure
* @param   p        pointer to the metering library data structure (tMETERLIBFFT1PH_DATA)
* @param   p_flag   pointer to the calibration status flag
* @param   p_afe    pointer to the AFE calibration values
*         
* @return  TRUE     reseting and parameter saving is needed after calling this function
*          FALSE    without any reset after calling this function
*
* @note    Used for 1-ph AFE self-calibration.
*******************************************************************************/

extern void auto_calib (tCONFIG_FLASH_DATA *ptr,tSMARTHOME_RESULT val);

void CONFIG_UpdateOffsets (tCONFIG_FLASH_DATA *ptr, Frac32 u, Frac32* chan, int num);
void start_calib(uint8_t type);
void set_calib_state(CALIB_STATES new_state);
CALIB_STATES get_calib_state( void);
void auto_calib_init(void);
void auto_calib_task(void);
#endif /* __CALIB_H__ */




