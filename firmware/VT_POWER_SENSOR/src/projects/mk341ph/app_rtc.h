

#ifndef __APP_RTC_H__
#define __APP_RTC_H__

#include <stdint.h>
#include "drivers.h"


/* Buid data and time formatting                                              */
#define _SEC_   ((__TIME__[6]==' '?0:__TIME__[6]-'0')*10+(__TIME__[7]-'0'))
#define _MIN_   ((__TIME__[3]==' '?0:__TIME__[3]-'0')*10+(__TIME__[4]-'0'))
#define _HOUR_  ((__TIME__[0]==' '?0:__TIME__[0]-'0')*10+(__TIME__[1]-'0'))
#define _DAY_   ((__DATE__[4]==' '?0:__DATE__[4]-'0')*10+(__DATE__[5]-'0'))
#define _MON_   (__DATE__[2] == 'n' ? (__DATE__[1] == 'a' ? 1 : 6)            \
                :__DATE__[2] == 'b' ? 2                                       \
                :__DATE__[2] == 'r' ? (__DATE__[0] == 'M' ? 3 : 4)            \
                :__DATE__[2] == 'y' ? 5                                       \
                :__DATE__[2] == 'l' ? 7                                       \
                :__DATE__[2] == 'g' ? 8                                       \
                :__DATE__[2] == 'p' ? 9                                       \
                :__DATE__[2] == 't' ? 10                                      \
                :__DATE__[2] == 'v' ? 11 : 12)
#define _YEAR_  ((((__DATE__[7]-'0')*10+(__DATE__[8] -'0'))*10+               \
                   (__DATE__[9]-'0'))*10+(__DATE__[10]-'0'))
#define M_TAB   (_MON_ == 1 ? 6: _MON_ == 2 ? 2: _MON_ == 3 ? 2:              \
                 _MON_ == 4 ? 5: _MON_ == 5 ? 0: _MON_ == 6 ? 3:              \
                 _MON_ == 7 ? 5: _MON_ == 8 ? 1: _MON_ == 9 ? 4:              \
                 _MON_ == 10? 6: _MON_ == 11? 2: 4)
#define Y2000   (_YEAR_ % 2000)
#define _WDAY_  (((!(Y2000%4)) && (_MON_<3))?(Y2000+Y2000/4+M_TAB+_DAY_-1)%7: \
                                             (Y2000+Y2000/4+M_TAB+_DAY_)%7)

#define BUILD_DATE_TIME(t)                                                    \
{                                                                             \
  t.sec = _SEC_; t.min = _MIN_; t.hour = _HOUR_; t.day = _DAY_;               \
  t.mon = _MON_; t.year= _YEAR_; t.wday = _WDAY_;                             \
}


#define AppRTC_Get IRTC_GetClock
extern tTIME rTime,wTime;
void AppRTC_Init(void);
uint8_t AppRTC_Set(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);

#endif
