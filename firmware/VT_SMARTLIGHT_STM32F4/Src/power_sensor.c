#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "uart6.h"
#include "lib/sys_time.h"
#include "system_config.h"
#include "ampm_gsm_main_task.h"
#include "hw_config.h"
#include "lib/sys_time.h"
#include "watchdog.h"
#include "cmsis_os.h"
#include "aws_iot_config.h"
#include "led.h"
#include "uart_config_task.h"
#include "alarm.h"
#include "relay.h"
#include "power_sensor.h"

static void PowerSensor_Task(void const *param);



POWER_SENSOR_OBJ_TYPE powerLine1;
POWER_SENSOR_OBJ_TYPE powerLine2;
POWER_SENSOR_OBJ_TYPE powerLine3;
POWER_SENSOR_OBJ_TYPE powerLine4;

osThreadId sensor_thread_id;



void PowerSensor_Init(void)
{
	osThreadDef(sensor_task, PowerSensor_Task, osPriorityNormal, 0, 512);
	sensor_thread_id = osThreadCreate(osThread(sensor_task), NULL);
}

static void PowerSensor_Task(void const *param)
{
	uint8_t c;
	char data[128];
	uint8_t len = 0;
	
	for(;;)
	{
		osDelay(1000);
		mUSART6_PutString("AT+READALL=?\n");
		osDelay(1000);
		len = 0;
		while(mUSART6_GetChar(&c) == 0)
		{
			data[len++] = c;
			if(len >= 127) break;
		}
		data[len] = 0;
		sscanf(data,"AT+READALL=%fV,%fA,%fA,%fA,%fW,%ldwh,%fPF",&powerLine1.U,&powerLine1.IL,&powerLine1.IN,&powerLine1.Ir,&powerLine1.P,&powerLine1.wh,&powerLine1.PF);
		
		powerLine2 = powerLine1;
		powerLine3 = powerLine1;
		powerLine4 = powerLine1;
		//variableNoInit.deviceStatus = 0;
		//LINE 1
		if(deviceCfg.features & DEFAULT_FEATURE_LINE1_NO_POWER_ALARM)
		{
			if(powerLine1.U <= 50 && (variableNoInit.lineStatus & (1<<0))) variableNoInit.deviceStatus |= LINE1_NO_POWER_ALARM;
			else	variableNoInit.deviceStatus &= ~LINE1_NO_POWER_ALARM;
		}
		else	variableNoInit.deviceStatus &= ~LINE1_NO_POWER_ALARM;
			
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE1_NO_CURRENT_WARNING)
		{
			if((powerLine1.IL <= 0.01 || powerLine1.P == 0) && (variableNoInit.lineStatus & (1<<0))) variableNoInit.deviceStatus |= LINE1_NO_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE1_NO_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE1_NO_CURRENT_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE1_LEAKAGE_CURRENT_WARNING)
		{
			if(powerLine1.IL >= deviceCfg.leakageCurentLevel) variableNoInit.deviceStatus |= LINE1_LEAKAGE_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE1_LEAKAGE_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE1_LEAKAGE_CURRENT_WARNING;
		
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING)
		{
			if(powerLine1.IL <= deviceCfg.lowCurrentLevel) variableNoInit.deviceStatus |= LINE1_LOW_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE1_LOW_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE1_LOW_CURRENT_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING)
		{
			if(powerLine1.IL >= deviceCfg.highCurrentLevel) variableNoInit.deviceStatus |= LINE1_HIGH_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE1_HIGH_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE1_HIGH_CURRENT_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING)
		{
			if(powerLine1.U <= deviceCfg.lowVoltageLevel) variableNoInit.deviceStatus |= LINE1_LOW_VOLTAGE_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE1_LOW_VOLTAGE_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE1_LOW_VOLTAGE_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING)
		{
			if(powerLine1.U >= deviceCfg.highVoltageLevel) variableNoInit.deviceStatus |= LINE1_HIGH_VOLTAGE_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE1_HIGH_VOLTAGE_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE1_HIGH_VOLTAGE_WARNING;
		
		//LINE 2
		if(deviceCfg.features & DEFAULT_FEATURE_LINE2_LOW_CURRENT_WARNING)
		{
			if(powerLine2.IL <= deviceCfg.lowCurrentLevel) variableNoInit.deviceStatus |= LINE2_LOW_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE2_LOW_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE2_LOW_CURRENT_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE2_HIGH_CURRENT_WARNING)
		{
			if(powerLine2.IL >= deviceCfg.highCurrentLevel) variableNoInit.deviceStatus |= LINE2_HIGH_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE2_HIGH_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE2_HIGH_CURRENT_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE2_LOW_VOLTAGE_WARNING)
		{
			if(powerLine2.U <= deviceCfg.lowVoltageLevel) variableNoInit.deviceStatus |= LINE2_LOW_VOLTAGE_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE2_LOW_VOLTAGE_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE2_LOW_VOLTAGE_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE2_HIGH_VOLTAGE_WARNING)
		{
			if(powerLine2.U >= deviceCfg.highVoltageLevel) variableNoInit.deviceStatus |= LINE2_HIGH_VOLTAGE_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE2_HIGH_VOLTAGE_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE2_HIGH_VOLTAGE_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE2_NO_POWER_ALARM)
		{
			if(powerLine2.U <= 50 && (variableNoInit.lineStatus & (1<<1))) variableNoInit.deviceStatus |= LINE2_NO_POWER_ALARM;
			else	variableNoInit.deviceStatus &= ~LINE2_NO_POWER_ALARM;
		}
		else	variableNoInit.deviceStatus &= ~LINE2_NO_POWER_ALARM;
		if(deviceCfg.features & DEFAULT_FEATURE_LINE2_NO_CURRENT_WARNING)
		{
			if((powerLine2.IL <= 0.01 || powerLine2.P == 0)  && (variableNoInit.lineStatus & (1<<1))) variableNoInit.deviceStatus |= LINE2_NO_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE2_NO_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE2_NO_CURRENT_WARNING;
		if(deviceCfg.features & DEFAULT_FEATURE_LINE2_LEAKAGE_CURRENT_WARNING)
		{
			if(powerLine2.IL >= deviceCfg.leakageCurentLevel) variableNoInit.deviceStatus |= LINE2_LEAKAGE_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE2_LEAKAGE_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE2_LEAKAGE_CURRENT_WARNING;
		
		//LINE 3
		if(deviceCfg.features & DEFAULT_FEATURE_LINE3_LOW_CURRENT_WARNING)
		{
			if(powerLine3.IL <= deviceCfg.lowCurrentLevel) variableNoInit.deviceStatus |= LINE3_LOW_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE3_LOW_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE3_LOW_CURRENT_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE3_HIGH_CURRENT_WARNING)
		{
			if(powerLine3.IL >= deviceCfg.highCurrentLevel) variableNoInit.deviceStatus |= LINE3_HIGH_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE3_HIGH_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE3_HIGH_CURRENT_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE3_LOW_VOLTAGE_WARNING)
		{
			if(powerLine3.U <= deviceCfg.lowVoltageLevel) variableNoInit.deviceStatus |= LINE3_LOW_VOLTAGE_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE3_LOW_VOLTAGE_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE3_LOW_VOLTAGE_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE3_HIGH_VOLTAGE_WARNING)
		{
			if(powerLine3.U >= deviceCfg.highVoltageLevel) variableNoInit.deviceStatus |= LINE3_HIGH_VOLTAGE_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE3_HIGH_VOLTAGE_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE3_HIGH_VOLTAGE_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE3_NO_POWER_ALARM)
		{
			if(powerLine3.U <= 50 && (variableNoInit.lineStatus & (1<<2))) variableNoInit.deviceStatus |= LINE3_NO_POWER_ALARM;
			else	variableNoInit.deviceStatus &= ~LINE3_NO_POWER_ALARM;
		}
		else	variableNoInit.deviceStatus &= ~LINE3_NO_POWER_ALARM;
		if(deviceCfg.features & DEFAULT_FEATURE_LINE3_NO_CURRENT_WARNING)
		{
			if((powerLine3.IL <= 0.01 || powerLine3.P == 0)  && (variableNoInit.lineStatus & (1<<2))) variableNoInit.deviceStatus |= LINE3_NO_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE3_NO_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE3_NO_CURRENT_WARNING;
		if(deviceCfg.features & DEFAULT_FEATURE_LINE3_LEAKAGE_CURRENT_WARNING)
		{
			if(powerLine3.IL >= deviceCfg.leakageCurentLevel) variableNoInit.deviceStatus |= LINE3_LEAKAGE_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE3_LEAKAGE_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE3_LEAKAGE_CURRENT_WARNING;
		
		//LINE 4
		if(deviceCfg.features & DEFAULT_FEATURE_LINE4_LOW_CURRENT_WARNING)
		{
			if(powerLine4.IL <= deviceCfg.lowCurrentLevel) variableNoInit.deviceStatus |= LINE4_LOW_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE4_LOW_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE4_LOW_CURRENT_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE4_HIGH_CURRENT_WARNING)
		{
			if(powerLine4.IL >= deviceCfg.highCurrentLevel) variableNoInit.deviceStatus |= LINE4_HIGH_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE4_HIGH_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE4_HIGH_CURRENT_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE4_LOW_VOLTAGE_WARNING)
		{
			if(powerLine4.U <= deviceCfg.lowVoltageLevel && (variableNoInit.lineStatus & (1<<0))) variableNoInit.deviceStatus |= LINE4_LOW_VOLTAGE_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE4_LOW_VOLTAGE_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE4_LOW_VOLTAGE_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE4_HIGH_VOLTAGE_WARNING)
		{
			if(powerLine4.U >= deviceCfg.highVoltageLevel) variableNoInit.deviceStatus |= LINE4_HIGH_VOLTAGE_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE4_HIGH_VOLTAGE_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE4_HIGH_VOLTAGE_WARNING;
		
		if(deviceCfg.features & DEFAULT_FEATURE_LINE4_NO_POWER_ALARM)
		{
			if(powerLine4.U <= 50 && (variableNoInit.lineStatus & (1<<3))) variableNoInit.deviceStatus |= LINE4_NO_POWER_ALARM;
			else	variableNoInit.deviceStatus &= ~LINE4_NO_POWER_ALARM;
		}
		else	variableNoInit.deviceStatus &= ~LINE4_NO_POWER_ALARM;
		if(deviceCfg.features & DEFAULT_FEATURE_LINE4_NO_CURRENT_WARNING)
		{
			if((powerLine4.IL <= 0.01 || powerLine4.P == 0)  && (variableNoInit.lineStatus & (1<<3))) variableNoInit.deviceStatus |= LINE4_NO_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE4_NO_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE4_NO_CURRENT_WARNING;
		if(deviceCfg.features & DEFAULT_FEATURE_LINE4_LEAKAGE_CURRENT_WARNING)
		{
			if(powerLine4.IL >= deviceCfg.leakageCurentLevel) variableNoInit.deviceStatus |= LINE4_LEAKAGE_CURRENT_WARNING;
			else	variableNoInit.deviceStatus &= ~LINE4_LEAKAGE_CURRENT_WARNING;
		}
		else	variableNoInit.deviceStatus &= ~LINE4_LEAKAGE_CURRENT_WARNING;
		

	}
}


