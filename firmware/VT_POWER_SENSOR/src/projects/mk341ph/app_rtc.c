
#include "app_rtc.h"

tTIME rTime,wTime;

void AppRTC_Init(void)
{
	//Init RTC
    BUILD_DATE_TIME (wTime); 
    IRTC_SetClock   (&wTime);
     // Init iRTC tamper pins if powered-up for the first time.                */  
    IRTC_Init (IRTC_MODULE_COMP_OFF_CONFIG(LFSR_CLK_64HZ,5),
               //IRTC_TAMPER_PIN_DI_CONFIG,
               IRTC_TAMPER_PIN_ACTIVE_INPMODE_CONFIG(TAMPER_FILT_CLK_64HZ,5),
               IRTC_TAMPER_PIN_PASSIVE_POL_LOW_CONFIG(TAMPER_FILT_CLK_64HZ,5),
               IRTC_TAMPER_PIN_PASSIVE_POL_LOW_CONFIG(TAMPER_FILT_CLK_64HZ,5),
               0);
    /* suppress initial "tamper" detect                                       */
    IRTC_ClrIsrFlag (IRTC_TAMPER0_MASK|IRTC_TAMPER1_MASK|IRTC_TAMPER2_MASK);
}


uint8_t AppRTC_Set(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{
	wTime.year = year;
	wTime.mon = month;
	wTime.day = day;
	wTime.hour = hour;
	wTime.min = min;
	wTime.sec = sec;
	IRTC_SetClock   (&wTime);
	return 0;
}


