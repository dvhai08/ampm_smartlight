
#include "alarm.h"
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "system_config.h"
#include "relay.h"
#include "lib/sys_time.h"
#include "ntp_server.h"
#include "sms_task.h"
#include "ampm_gsm_sms_api.h"

extern RTC_HandleTypeDef hrtc;

uint8_t alarmFlag = 0;

void RTC_UpdateNewTime(void)
{
	RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
	RTC_AlarmTypeDef sAlarm;
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
	
	HAL_RTC_GetAlarm(&hrtc,&sAlarm,RTC_ALARM_A,RTC_FORMAT_BIN);
	
	sysTime.year = 2000 + sdatestructureget.Year;
	sysTime.mday = sdatestructureget.Date;
	sysTime.month = sdatestructureget.Month;
	sysTime.hour = stimestructureget.Hours;
	sysTime.min = stimestructureget.Minutes;
	sysTime.sec = stimestructureget.Seconds;
}

void RTC_CalendarShow(void)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
	RTC_AlarmTypeDef sAlarm;
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
	
	HAL_RTC_GetAlarm(&hrtc,&sAlarm,RTC_ALARM_A,RTC_FORMAT_BIN);
	
	sysTime.year = 2000 + sdatestructureget.Year;
	sysTime.mday = sdatestructureget.Date;
	sysTime.month = sdatestructureget.Month;
	sysTime.hour = stimestructureget.Hours;
	sysTime.min = stimestructureget.Minutes;
	sysTime.sec = stimestructureget.Seconds;
  /* Display time Format : hh:mm:ss */
  printf("TIME NOW:%02d-%02d-%02d %02d:%02d:%02d \n\r", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year,
							stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  printf("ALARM:%02d:%02d\n\r",sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes);
	
	if(!(sysTime.year >= 2018 && sysTime.year <= 2099
		&& sysTime.month > 0 && sysTime.month <= 12
		&& sysTime.mday > 0 && sysTime.mday <= 31
		&& sysTime.hour > 0 && sysTime.hour < 24
		&& sysTime.min	> 0 && sysTime.min < 60
		&& sysTime.sec > 0 && sysTime.sec < 60)
	)
	{
		ntp_get_time();
	}

}


void RTC_SetTime(DATE_TIME time)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  if(time.year >= 2018 && time.year <= 2099
		&& time.month > 0 && time.month <= 12
		&& time.mday > 0 && time.mday <= 31
		&& time.hour > 0 && time.hour < 24
		&& time.min	> 0 && time.min < 60
		&& time.sec > 0 && time.sec < 60
	  && time.wday >= 1 && time.wday <= 7
	)
	{
		time.wday -= 1;
		if(time.wday == 0) time.wday = SCHEDULE_WEEKDAY_SUNDAY;
			
		sdatestructureget.Year = time.year - 2000;
		sdatestructureget.Date = time.mday;;
		sdatestructureget.Month = time.month;
		sdatestructureget.WeekDay = time.wday;
		stimestructureget.Hours = time.hour;
		stimestructureget.Minutes = time.min;
		stimestructureget.Seconds = time.sec;
		stimestructureget.TimeFormat = RTC_HOURFORMAT_24;
		stimestructureget.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		stimestructureget.StoreOperation = RTC_STOREOPERATION_RESET;
		
		/* Get the RTC current Time */
		HAL_RTC_SetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
		/* Get the RTC current Date */
		HAL_RTC_SetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
		/* Display time Format : hh:mm:ss */
		printf("TIME SET:%02d:%02d:%02d \n\r", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
		/* Display date Format : mm-dd-yy */
		printf("TIME SET:%02d-%02d-%02d \n\r", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
	}
}

void RTC_SetTimeTest(void)
{
	DATE_TIME time;
	uint32_t tempRtcSec;
	DATE_TIME timeTemp;
	timeTemp.year = 2018;
	timeTemp.month = 2;
	timeTemp.mday = 3;
	timeTemp.hour = 23;
	timeTemp.min = 59;
	timeTemp.sec = 30;
	for(int i = 1;i < 10;i++)
	{
		timeTemp.mday = i;
		tempRtcSec = TIME_GetSec(&timeTemp);
		TIME_FromSec(&time,tempRtcSec);
		RTC_SetTime(time);
	}
}

void Alarm_Set(uint8_t hour,uint8_t min)
{
	RTC_AlarmTypeDef sAlarm;
	sAlarm.AlarmTime.Hours = hour;
	sAlarm.AlarmTime.Minutes = min;
	sAlarm.AlarmTime.Seconds = 0;
	sAlarm.AlarmTime.SubSeconds = 0x0;
	sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_SECONDS;//RTC_ALARMMASK_SECONDS | RTC_ALARMMASK_MINUTES | RTC_ALARMMASK_HOURS;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 0x1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}



uint8_t testIo = 0;


uint8_t Alarm_GetSize(void)
{
	uint8_t alarmCnt = 0;
	for(uint8_t i = 0;i < DEVICE_SCHEDULE_MAX; i++)
	{
		if(deviceCfg.schedules[i].hour != 0xff
			|| deviceCfg.schedules[i].min != 0xff)
		{
			alarmCnt++;
		}
	}
	return alarmCnt;
}

uint8_t Alarm_FindNext(uint8_t *hour,uint8_t *min)
{
	uint8_t i,minHour = 0xff,minMinutes = 0xff;
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	//datetime format check
	if(time.Hours >= 24 
		|| time.Minutes >= 60
		|| time.Seconds >= 60
		|| date.Year > 99
		|| date.Month > 12
		|| date.Date > 31
		|| date.WeekDay > 7
	)
	{
		return 0;
	}
	
	for(i = 0;i < DEVICE_SCHEDULE_MAX; i++)
	{
		if(((deviceCfg.schedules[i].fromYear <= date.Year || deviceCfg.schedules[i].fromYear == 0xff) && deviceCfg.schedules[i].toYear >= date.Year)
			&& ((deviceCfg.schedules[i].fromMonth <= date.Month || deviceCfg.schedules[i].fromMonth == 0xff) && deviceCfg.schedules[i].toMonth >= date.Month)
			&& ((deviceCfg.schedules[i].fromMday <= date.Date || deviceCfg.schedules[i].fromMday == 0xff) && deviceCfg.schedules[i].toMday >= date.Date)
			&& (deviceCfg.schedules[i].weekDay & (1 << date.WeekDay))
		)
		{
			if((deviceCfg.schedules[i].hour >= time.Hours)
			&& ((deviceCfg.schedules[i].hour > time.Hours) || (deviceCfg.schedules[i].min > time.Minutes))
			)
			{
				if(deviceCfg.schedules[i].hour < minHour)
				{
					minHour = deviceCfg.schedules[i].hour;
					minMinutes = deviceCfg.schedules[i].min;
				}
				else if(deviceCfg.schedules[i].hour == minHour)
				{
					if(deviceCfg.schedules[i].min < minMinutes)
					{
						minMinutes = deviceCfg.schedules[i].min;
					}
				}
			}
		}
	}
	
	if(((minHour == time.Hours) && (minMinutes < time.Minutes)))
	{
		return 0;
	}
	else if((minHour < 24 && minHour >= time.Hours)
	&& (minMinutes < 60))
	{
		*hour = minHour;
		*min = minMinutes;
		return 1;
		//Alarm_Set(minHour,minMinutes);
	}
	return 0;
}

void Alarm_Check(void)
{
	RTC_AlarmTypeDef sAlarm;
	uint8_t alarmHour,alarmMin;
	if((alarmFlag == 0) && HAL_RTC_GetAlarm(&hrtc,&sAlarm,RTC_ALARM_A,RTC_FORMAT_BIN) == HAL_OK)
	{
		if(Alarm_FindNext(&alarmHour,&alarmMin))
		{
			if(sAlarm.AlarmTime.Hours != alarmHour
			|| (sAlarm.AlarmTime.Minutes != alarmMin)	
			)
			{
				Alarm_Set(alarmHour,alarmMin);
			}
		}
	}
}

void Alarm_Task(void)	
{
	if(alarmFlag)
	{
		RTC_AlarmTypeDef sAlarm;
		if(HAL_RTC_GetAlarm(&hrtc,&sAlarm,RTC_ALARM_A,RTC_FORMAT_BIN) == HAL_OK)
		{
			uint8_t i;
			RTC_DateTypeDef date;
			RTC_TimeTypeDef time;
			uint8_t alarmHour,alarmMin;
			alarmFlag = 0;
			/* Get the RTC current Time */
			HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
			/* Get the RTC current Date */
			HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
			//datetime format check
			if(time.Hours >= 24 
				|| time.Minutes >= 60
				|| time.Seconds >= 60
				|| date.Year > 99
				|| date.Month > 12
				|| date.Date > 31
				|| date.WeekDay > 7
			)
			{
				return;
			}
			uint32_t lineStatus = variableNoInit.lineStatus;
			for(i = 0;i < DEVICE_SCHEDULE_MAX; i++)
			{
				if(((deviceCfg.schedules[i].fromYear <= date.Year || deviceCfg.schedules[i].fromYear == 0xff) && deviceCfg.schedules[i].toYear >= date.Year)
					&& ((deviceCfg.schedules[i].fromMonth <= date.Month || deviceCfg.schedules[i].fromMonth == 0xff) && deviceCfg.schedules[i].toMonth >= date.Month)
					&& ((deviceCfg.schedules[i].fromMday <= date.Date || deviceCfg.schedules[i].fromMday == 0xff) && deviceCfg.schedules[i].toMday >= date.Date)
					&& (deviceCfg.schedules[i].weekDay & (1 << date.WeekDay))
				)
				{
					if((deviceCfg.schedules[i].hour == sAlarm.AlarmTime.Hours)
					&& (deviceCfg.schedules[i].min == sAlarm.AlarmTime.Minutes)
					)
					{
						if(deviceCfg.schedules[i].type == 0)
						{
							for(uint8_t j = 0;j < 4;j++)
							{
								if(deviceCfg.schedules[i].valueOn & (1<<j))
								{
									relay_on(j+1);
								}
								else if(deviceCfg.schedules[i].valueOff & (1<<j))
								{
									relay_off(j+1);
								}
							}
						}
					}
				}
			}
			
			if((lineStatus != variableNoInit.lineStatus) && (deviceCfg.features & DEFAULT_FEATURE_SMS_SCHEDULE_REPORT))
			{
				char *sms = pvPortMalloc(160);
				if(sms)
				{
					sms[0] = 0;
					if((deviceCfg.features & DEFAULT_FEATURE_1_RELAY))
					{
						if(variableNoInit.lineStatus & (1<<0))
							sprintf(sms,"Thiet bi da tu dong bat thanh cong\n");
						else
							sprintf(sms,"Thiet bi da tu dong tat\n");
					}
					else
					{
						sprintf(sms,"Thiet bi da tu dong\n");
					}
					for(i = 0;i < 4;i++)
					{
						if(((lineStatus & (1<<i)) != (variableNoInit.lineStatus & (1<<i)))
							&& ((deviceCfg.features & DEFAULT_FEATURE_1_RELAY) == 0)
						)
						{
							if(variableNoInit.lineStatus & (1<<i))
								sprintf(&sms[strlen(sms)],"BAT O%d\n",i+1);
							else
								sprintf(&sms[strlen(sms)],"TAT O%d\n",i+1);
						}
					}
					if(strlen(sysCfg.bigBoss) >= 6 && strlen(sysCfg.bigBoss) <= 12)
						AmpmGsm_SendSmsText((char *)sysCfg.bigBoss,sms);
					for(uint8_t index = 0;index < MAX_OWNER;index++)
					{
							if(strlen(sysCfg.ownerNumber[index]) >= 6 && strlen(sysCfg.ownerNumber[index]) <= 12)
								AmpmGsm_SendSmsText((char *)sysCfg.ownerNumber[index],sms);
					}
					vPortFree(sms);
				}
			}
			//nex alarm
			if(Alarm_FindNext(&alarmHour,&alarmMin))
			{
				if(sAlarm.AlarmTime.Hours != alarmHour
				|| (sAlarm.AlarmTime.Minutes != alarmMin)	
				)
				{
					Alarm_Set(alarmHour,alarmMin);
				}
			}
		}
	}
}

void Alarm_Callback(void)	
{
	alarmFlag = 1;
}
