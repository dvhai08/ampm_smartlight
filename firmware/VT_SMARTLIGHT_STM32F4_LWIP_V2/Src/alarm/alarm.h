
#ifndef __ALARM_H__
#define __ALARM_H__

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "lib/sys_time.h"
extern RTC_HandleTypeDef hrtc;
void Alarm_Callback(void)	;
void Set_Alarm(uint8_t hour,uint8_t min);
void Alarm_Check(void);
void Alarm_Task(void);
uint8_t Alarm_GetSize(void);
void RTC_CalendarShow(void);
void RTC_SetTime(DATE_TIME time);
void RTC_SetTimeTest(void);
void RTC_UpdateNewTime(void);
#endif /* __ALARM_H__ */
