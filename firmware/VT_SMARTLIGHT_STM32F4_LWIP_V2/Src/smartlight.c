

#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "uart1.h"
#include "uart5.h"
#include "lib/sys_time.h"
#include "system_config.h"
#include "ampm_gsm_main_task.h"
#include "hw_config.h"
#include "lib/sys_time.h"
#include "ntp_server.h"
#include "ota_server.h"
#include "ota_backup_server.h"
#include "watchdog.h"
#include "cmsis_os.h"
#include "aws_iot_config.h"
#include "led.h"
#include "test_spiffs.h"
#include "uart_config_task.h"
#include "memory_buffer_alloc.h"
#include "alarm.h"
#include "relay.h"
#include "power_sensor.h"
#include "lwip/stats.h"
#include "lib/ampm_filestream.h"
//ntp_start();
//
extern void iot_shadow_thing_start(void);
extern void RTC_CalendarShow(void);
extern void Trigger_SendReportToServer(void);
extern void SendReportSmsToUser(void);
#include "lwip/sockets.h"
#include "stdio.h"
FILE *test_stream;
char buf[10],buf1[64];
int res;
uint32_t smartLightMainCnt = 0;

AMPM_FILEHANDLE fileTest;
void vSmartLightMainTask()
{
		uint32_t deviceStatus,smsSendCnt = 0;
		LedInit();
		SysWatchdogDisable(WTD_MAIN_LOOP);
		SST25_Init();
		//spiffs_setup();
		SysWatchdogEnable(WTD_MAIN_LOOP);
		AMPM_GSM_Init("internet","mms","mms",NULL,NULL);
		Ampm_GsmSetMode(AMPM_GSM_GPRS_ENABLE);
		
		//load device config here
		DeviceCfg_Load();
		DeviceIoStatus_Load();
	
		for(uint8_t i = 0;i < DEVICE_LINE_MAX;i++)
		{
			if(deviceIoStatus.line[i])
				relay_on(i + 1);
			else
				relay_off(i + 1);
		}
		iot_shadow_thing_start();
		UartConfigTaskInit();
    printf("Hello,I'm SMARTLIGHT! Ver = %s\n\r",FIRMWARE_VERSION);
		ota_backup_start();
		PowerSensor_Init();
		//ota_start();
		ntp_start();
		//mbedtls_memory_buffer_alloc_self_test(1);
	  Alarm_Callback();
//		RTC_SetTimeTest();
		deviceStatus = variableNoInit.deviceStatus;
		
		
		
    while (1) {
			SysWatchdogFeed(WTD_MAIN_LOOP);
			//SysWatchdogFeed(WTD_SHADOW_LOOP);
			ResetMcuTask();
			UartConfigTask();
			PowerSensor_Task(NULL);
			osDelay(1);
			smartLightMainCnt++;
			Alarm_Task();
			if(smartLightMainCnt % 10000 == 0)
			{
				RTC_CalendarShow();
				printf("OS HEAP FREE HEAP SIZE:%d\r\n",(uint32_t)xPortGetFreeHeapSize());
				MEM_STATS_DISPLAY();
			}
			if(smartLightMainCnt % 120000 == 0)
			{
				stats_display();
			}
			if(smartLightMainCnt % 15000 == 0)
			{
				Alarm_Check();
			}
			if(flagSystemStatus >= SYS_SERVER_OK)
			{
				IO_ToggleSetStatus(&io_network_led,100,3000,IO_TOGGLE_ENABLE,IO_MAX_VALUE);
			}
			else if(flagSystemStatus >= SYS_GPRS_OK)
			{
				IO_ToggleSetStatus(&io_network_led,IO_STATUS_ON_TIME_DFG,IO_STATUS_OFF_TIME_DFG,IO_TOGGLE_ENABLE,IO_MAX_VALUE);
			}
			else if(flagSystemStatus >= SYS_GSM_OK)
			{
				IO_ToggleSetStatus(&io_network_led,200,1000,IO_TOGGLE_ENABLE,IO_MAX_VALUE);
			}
			else
			{
				IO_ToggleSetStatus(&io_network_led,0,0,IO_TOGGLE_DISABLE,IO_MAX_VALUE);
			}
			if(deviceStatus != variableNoInit.deviceStatus)
			{
				deviceStatus = variableNoInit.deviceStatus;
				Trigger_SendReportToServer();
				smsSendCnt = 5000;
			}
			
			if(smsSendCnt)
			{
				smsSendCnt--;
				if(smsSendCnt == 1)
					SendReportSmsToUser();
			}
    }
}

