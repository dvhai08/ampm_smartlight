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
#include "elster_tasks.h"


extern void Trigger_SendReportToServer(void);

POWER_SENSOR_OBJ_TYPE powerLine1;
POWER_SENSOR_OBJ_TYPE powerLine2;
POWER_SENSOR_OBJ_TYPE powerLine3;
POWER_SENSOR_OBJ_TYPE powerLine4;

uint32_t powerSensorTimeCnt = 0;
uint8_t sentFlag = 0;
uint8_t flagSendToServer = 0;

osThreadId sensor_thread_id;

uint8_t checkPowerSensorTimeout = 0;

void PowerSensor_Init(void)
{
	//osThreadDef(sensor_task, PowerSensor_Task, osPriorityNormal, 0, 512);
	//sensor_thread_id = osThreadCreate(osThread(sensor_task), NULL);
	
	MeterElterTasksInit();
}

void PowerSensorPause(uint8_t sec)
{
	checkPowerSensorTimeout = sec/2;
}

void PowerSensor_Task(void const *param)
{
	uint8_t c;
	
	char data[128];
	uint8_t len = 0;
	
	//for(;;)
	{
		powerSensorTimeCnt++;
		MeterElterTasks();
		if(powerSensorTimeCnt % 1000 == 0)
		{
			if(sentFlag == 0)
			{
				sentFlag = 1;
				#ifndef _UART2RS485
				mUSART6_PutString("AT+READALL=?\n");
				#endif
			}
			else
			{
				sentFlag = 0;
				len = 0;
				while(mUSART6_GetChar(&c) == 0)
				{
					data[len++] = c;
					if(len >= 127) break;
				}
				data[len] = 0;
				//printf("%s\r\n",data);
				sscanf(data,"AT+READALL=%fV,%fA,%fA,%fA,%fW,%ldwh,%fPF",&powerLine1.U,&powerLine1.IL,&powerLine1.IN,&powerLine1.Ir,&powerLine1.P,&powerLine1.wh,&powerLine1.PF);
				powerLine2 = powerLine1;
				powerLine3 = powerLine1;
				powerLine4 = powerLine1;
			}

		//variableNoInit.deviceStatus = 0;
			//LINE 1
			if(checkPowerSensorTimeout)
			{
				checkPowerSensorTimeout--;
				if(checkPowerSensorTimeout == 0)
				{
					MeterElterRead(METER_READ_1);
					flagSendToServer = 1;
					//Trigger_SendReportToServer();
				}
			}
			else if(MeterElterTasks_IsIdle())
			{
				if(flagSendToServer)
				{
					if(meterElsterReadOk)
					{
						meterElsterReadOk = 0;
						powerLine1.U = instantaneousValue.voltage_phase_A/deviceCfg.meterVoltageScale;
						powerLine2.U = instantaneousValue.voltage_phase_B/deviceCfg.meterVoltageScale;
						powerLine3.U = instantaneousValue.voltage_phase_C/deviceCfg.meterVoltageScale;
						
						powerLine1.IL = instantaneousValue.current_phase_A/123.7;
						powerLine2.IL = instantaneousValue.current_phase_B/123.7;
						powerLine3.IL = instantaneousValue.current_phase_C/123.7;
						
						powerLine1.IN = instantaneousValue.current_phase_A;
						powerLine2.IN = instantaneousValue.current_phase_B;
						powerLine3.IN = instantaneousValue.current_phase_C;
						
						powerLine1.Ir = 0;
						powerLine2.Ir = 0;
						powerLine3.Ir = 0;
						
						powerLine1.PF = instantaneousValue.factor_phase_A/10;
						powerLine2.PF = instantaneousValue.factor_phase_B/10;
						powerLine3.PF = instantaneousValue.factor_phase_C/10;
						
						powerLine1.P = powerLine1.U*powerLine1.IL*powerLine1.PF;
						powerLine2.P = powerLine2.U*powerLine2.IL*powerLine2.PF;
						powerLine3.P = powerLine3.U*powerLine3.IL*powerLine3.PF;
						
					}
					flagSendToServer = 0;
					Trigger_SendReportToServer();
				}
					
				if(deviceCfg.features & DEFAULT_FEATURE_LINE1_NO_POWER_ALARM)
				{
					if(powerLine1.U <= 50 && (variableNoInit.lineStatus & (1<<0))) variableNoInit.deviceStatus |= LINE1_NO_POWER_ALARM;
					else	variableNoInit.deviceStatus &= ~LINE1_NO_POWER_ALARM;
				}
				else	variableNoInit.deviceStatus &= ~LINE1_NO_POWER_ALARM;
					
				
				if(deviceCfg.features & DEFAULT_FEATURE_LINE1_NO_CURRENT_WARNING)
				{
					if((powerLine1.IL < 0.01f || powerLine1.P == 0) && (variableNoInit.lineStatus & (1<<0))) variableNoInit.deviceStatus |= LINE1_NO_CURRENT_WARNING;
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
					if((powerLine2.IL < 0.01f || powerLine2.P == 0)  && (variableNoInit.lineStatus & (1<<1))) variableNoInit.deviceStatus |= LINE2_NO_CURRENT_WARNING;
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
					if((powerLine3.IL < 0.01f || powerLine3.P == 0)  && (variableNoInit.lineStatus & (1<<2))) variableNoInit.deviceStatus |= LINE3_NO_CURRENT_WARNING;
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
					if((powerLine4.IL < 0.01f || powerLine4.P == 0)  && (variableNoInit.lineStatus & (1<<3))) variableNoInit.deviceStatus |= LINE4_NO_CURRENT_WARNING;
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
	}
}


