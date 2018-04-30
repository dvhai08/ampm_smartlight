/*****************************************************************************
 * (c) Copyright 2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * @file      config.c
 * @author    R55013
 * @version   1.0.1.0
 * @date      Sep-12-2011
 * @brief     Configuration data management.
 ******************************************************************************/
#include <string.h>
#include <math.h>
#include "drivers.h"
#include "fraclib.h"
#include "meterlib.h"
#include "system_config.h" 

/******************************************************************************
 * public data definitions
 ******************************************************************************/
/* this variables is stored in parameter section of the flash  addr=0x001fc00 */
#if defined (__ICCARM__) /* IAR   */
  #pragma location = ".config"
  const tCONFIG_FLASH_DATA nvmcfg =
#elif defined(__GNUC__) || defined(__CC_ARM)/* CW GCC */
  const tCONFIG_FLASH_DATA nvmcfg __attribute__ ((section(".config"))) = 
#endif
{
    CAL_VOLT,
    {CAL_CURR, CAL_CURR, CAL_CURR, CAL_CURR},
	0,
	{0,	0,	0,	0},
	{0, 0,  0,  0},
    /* basic power meter configuration data                                   */
    FRAC32(-1.0),         /* measured maximum voltage                         */
    FRAC32( 1.0),         /* measured minimum voltage                         */
    {FRAC32(-1.0),FRAC32(-1.0),FRAC32(-1.0),FRAC32(-1.0)},         
	/* measured maximum current                       */
    {FRAC32( 1.0),FRAC32( 1.0),FRAC32( 1.0),FRAC32( 1.0)},         
	/* measured minimum  current                       */  
    /* post-calibration data - calculated phase delay, offsets and gains      */
    {1,1,1,1},            /* ch1 delay in modulator clocks                    */                   
    1,                    /* u delay                                          */ 
    {0l,0l,0l,0l},         /* current measurement offset (AFE ch0)             */
		0l,
		{0,0,0,0},
    /* configuration flag                                                     */
    0xffff,                /* 0xffff=read default configuration data           */
    {FRAC32(1.0),{FRAC32(1.0),FRAC32(1.0),FRAC32(1.0),FRAC32(1.0)}},
};


/* this variables is stored in flash                                          */
const tCONFIG_NOINIT_DATA nvmcnt = 
{
    /* basic power meter configuration data                                   */
    {0l,0l,0l,0l},                   /* active energy counter                  */ 
		{0l,0l,0l,0l},                   /* reactive energy counter                */ 
    3,                    /* menu index                                       */
    0,                    /* 0= active energy (T1) => XBAR_OUT6               */
    FALSE,                /* hardfault didn't occur                           */
    /* configuration flag                                                     */
    0x5555                /* 0x5555=configuration valid                       */
};

/* these variables are stored in non-initialized ram                          */
#if defined (__ICCARM__) /* IAR   */
  __no_init volatile tCONFIG_NOINIT_DATA  ramcnt;
  __no_init volatile tCONFIG_FLASH_DATA   ramcfg;
  __no_init volatile tCONFIG_APP_FLASH_DATA   ramappcfg;
#elif defined(__GNUC__)/* CW GCC */
  volatile tCONFIG_NOINIT_DATA  ramcnt __attribute__ ((section(".noinit")));
  volatile tCONFIG_FLASH_DATA   ramcfg __attribute__ ((section(".noinit")));
#elif defined(__CC_ARM)
  volatile tCONFIG_NOINIT_DATA  ramcnt __attribute__ ((section(".noinit")));
  volatile tCONFIG_FLASH_DATA   ramcfg;
 //volatile tCONFIG_APP_FLASH_DATA  ramappcfg;
#endif

void SystemConfigInit(void)
{
  /* read application setting from flash; if called for the first time        */
  /* flag=0xffff then initialize clock and update flag                        */
  CONFIG_ReadFlash((void *)__CONFIG_FLASH_BLOCK__,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA));
	
  if (ramcfg.flag == 0xffff)
  {
    CONFIG_ReadFlash((void *)&nvmcfg,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA));
		ramcfg.flag = 0xfff5;
    CONFIG_SaveFlash((void *)__CONFIG_FLASH_BLOCK__,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA)); 
  }
}

/******************************************************************************
 * public function definitions
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   Reads configuration data from NVM memory conditionally.
 * @param   ptr   - pointer to tCONFIG_DATA to be read
 * @note    Implemented as a function call.
 ******************************************************************************/
void CONFIG_ReadFlash (void *addr,void *ptr,int len)
{
  memcpy (ptr, addr, len); 
}

/***************************************************************************//*!
 * @brief   Writes configuration data to NVM memory.
 * @param   ptr   - pointer to tCONFIG_DATA to be saved
 * @param   flag  - configuration flag 
 * @note    Implemented as a function call.
 ******************************************************************************/
void CONFIG_SaveFlash (void *addr,void *ptr,int len)
{ 
  DisableInterrupts();
  FTFA_EraseSector (addr);
  //FTFA_WriteArray  (addr, ptr, sizeof(tCONFIG_FLASH_DATA)); 
  FTFA_WriteArray (addr, ptr, len);
  EnableInterrupts();
}


/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
