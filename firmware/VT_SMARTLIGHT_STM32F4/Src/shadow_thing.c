/**
 *
 *
 */
/**
 * @file shadow_thing.c
 * @brief the use of Thing Shadow
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include "aws_iot_config.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"
#include "aws_iot_shadow_interface.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "lib/sys_tick.h"
#include "watchdog.h"
#include "ampm_gsm_main_task.h"
#include "relay.h"
#include "memory_buffer_alloc.h"
#include "alarm.h"
#include "power_sensor.h"

#define IOT_SHADOW_THING(...) printf(__VA_ARGS__)

#define SIZE_OF_JSON_DOC		2048
#define SHADOW_MAX_RETRY_TIMES	10
#define ADD_JSON_FIELD(callback, key, data, jtype) {\
														.cb		= callback, \
														.pKey	= key,\
														.pData	= data,\
														.type	= jtype\
													}

typedef struct{
	uint8_t status;
	int32_t P;
	int32_t Q;
	int32_t U;
	int32_t I;
	int32_t Ir;
	int32_t wh;
}LINE_OBJ_TYPE;		


typedef struct{
	uint8_t type;
	uint8_t valueOn;
	uint8_t valueOff;
}SCHEDULE_CMD_OBJ_TYPE;		


typedef struct{
	uint16_t len;
	char *cmds;
}CMD_OBJ_TYPE;		

typedef struct{
	char msg_type;
	char *msg;
}MSG_OBJ_TYPE;		

CMD_OBJ_TYPE cmds;					

extern uint32_t life_span;
uint8_t sendReportFlag = 0;
osSemaphoreId shadow_sem_id;
osSemaphoreDef(SHADOW_SEM);

uint32_t checkMemoryCnt = 0;
uint8_t *mbed_buf = NULL;
Timeout_Type tUpdateInterval;
uint32_t shadow_thing_fail_times = 0;
uint8_t shadowMsgBusyTimeout = 0;
uint32_t shadow_thing_need_a_system_restart_cnt = 0;
static int shadow_thing_init(AWS_IoT_Client *mqtt);
static void ShadowUpdateStatusCallback(const char *pThingName, ShadowActions_t action, Shadow_Ack_Status_t status,
								const char *pReceivedJsonDocument, void *pContextData);
static void shadow_thing_thread(void const *param);

void Line1_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t);
void Line2_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t);
void Line3_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t);
void Line4_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t);
void Cmds_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t);
void Schedules_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t);


static void ShadowUpdateStatusCallback(const char *pThingName, ShadowActions_t action, Shadow_Ack_Status_t status,
								const char *pReceivedJsonDocument, void *pContextData)
{
	IOT_UNUSED(pThingName);
	IOT_UNUSED(action);
	IOT_UNUSED(pReceivedJsonDocument);
	IOT_UNUSED(pContextData);

	if(SHADOW_ACK_TIMEOUT == status) {
		flagSystemStatus &= ~(SYS_SERVER_OK | SYS_SERVERS_MASH);
		IOT_SHADOW_THING("Update Timeout--");
		shadowMsgBusyTimeout = 0;
	} else if(SHADOW_ACK_REJECTED == status) {
		shadowMsgBusyTimeout = 0;
		IOT_SHADOW_THING("Update RejectedXX");
	} else if(SHADOW_ACK_ACCEPTED == status) {
		flagSystemStatus |= SYS_SERVER_OK;
		if(cmds.len && cmds.cmds)
		{
			cmds.len = 0;
			mbedtls_free(cmds.cmds);
			cmds.cmds = NULL;
		}
		shadowMsgBusyTimeout = 0;
		IOT_SHADOW_THING("Update Accepted !!");
		shadow_thing_fail_times = 0;
	}
}
static int shadow_thing_init(AWS_IoT_Client *mqtt)
{
	IoT_Error_t rc = FAILURE;
	ShadowInitParameters_t sp = ShadowInitParametersDefault;
	sp.pHost = (char *)AWS_IOT_MQTT_HOST;
	sp.port = AWS_IOT_MQTT_PORT;
	sp.enableAutoReconnect = false;
	sp.disconnectHandler = NULL;
	IOT_SHADOW_THING("Shadow Init");
	flagSystemStatus &= ~(SYS_SERVER_OK | SYS_SERVERS_MASH);
	rc = aws_iot_shadow_init(mqtt, &sp);
	if(AWS_SUCCESS != rc)
	{
		IOT_ERROR("Shadow Connection Error");
		//GPRS connection should be restart 
		shadow_thing_fail_times++;
		return rc;
	}
	ShadowConnectParameters_t scp = ShadowConnectParametersDefault;
	scp.pMyThingName	= AWS_IOT_MY_THING_NAME;
	scp.pMqttClientId	= AWS_IOT_MQTT_CLIENT_ID;
	scp.mqttClientIdLen = (uint16_t)strlen(AWS_IOT_MQTT_CLIENT_ID);
	scp.pUsername		= USERNAME;
	scp.usernameLen		= (uint16_t)strlen(USERNAME);
	scp.pPassword		= PASSWORD;
	scp.passwordLen		= (uint16_t)strlen(PASSWORD);

	IOT_SHADOW_THING("Shadow Connect");
	rc = aws_iot_shadow_connect(mqtt, &scp);
	if(AWS_SUCCESS != rc) {
		IOT_ERROR("Shadow Connection Error");
		shadow_thing_fail_times++;
		return rc;
	}
		/*
	 * Enable Auto Reconnect functionality. Minimum and Maximum time of Exponential backoff are set in aws_iot_config.h
	 *  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
	 *  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL
	 */
	rc = aws_iot_shadow_set_autoreconnect_status(mqtt, true);
	if(AWS_SUCCESS != rc) {
		shadow_thing_fail_times++;
		IOT_ERROR("Unable to set Auto Reconnect to true - %d", rc);
		return rc;
	}
	IOT_SHADOW_THING("Shadow connected as follow:");
	IOT_SHADOW_THING("\tthing name is : %s", scp.pMyThingName);
	IOT_SHADOW_THING("\tthing id is: %s", scp.pMqttClientId);
	IOT_SHADOW_THING("\tthing user name is: %s", scp.pUsername);
	IOT_SHADOW_THING("\tthing password is: %s", scp.pPassword);
	return rc;
}
extern void aws_task_lock(void);
static void shadow_thing_thread(void const *param)
{
	(void)param;
//	int16_t p_value = 200;
//	int8_t device_type_value = 0;
	int8_t battery = 100;
	char scheduleTimeBuf[5];
	char lineStatus[4][4];
	char weekDay[32];
	LINE_OBJ_TYPE line1;
	LINE_OBJ_TYPE line2;
	LINE_OBJ_TYPE line3;
	LINE_OBJ_TYPE line4;
	SCHEDULE_CMD_OBJ_TYPE scheduleCmds;
//	uint32_t timeoutResend = 0;
	uint16_t sizeOfScheduleJson;
	uint16_t sizeOfWarningsJson;
	uint16_t sizeOfAlarmsJson;
	IoT_Error_t rc = AWS_SUCCESS;
	AWS_IoT_Client mqttClient;
	memset(&line1,0,sizeof(LINE_OBJ_TYPE));
	memset(&line2,0,sizeof(LINE_OBJ_TYPE));
	memset(&line3,0,sizeof(LINE_OBJ_TYPE));
	memset(&line4,0,sizeof(LINE_OBJ_TYPE));
	memset(&scheduleCmds,0,sizeof(SCHEDULE_CMD_OBJ_TYPE));
	
	shadow_sem_id = osSemaphoreCreate(osSemaphore(SHADOW_SEM), 1);
	/* wait for unlock when modem was ready connection*/
	cmds.len = 0;
	cmds.cmds = NULL;
	mbed_buf = calloc(1,0xFF00);
	while(1)
	{
		
		mbedtls_memory_buffer_alloc_init( mbed_buf, 0xFF00 );
		char *pJsonDoccument = mbedtls_calloc(1,SIZE_OF_JSON_DOC * sizeof(char));
		if(pJsonDoccument == NULL)
		{
			IOT_ERROR("%s:%d malloc failed!", __FUNCTION__,__LINE__);
			vTaskDelete(NULL);
		}
		size_t sizeOfJsonDocument = SIZE_OF_JSON_DOC;
		
		while(!Ampm_GsmGPRS_IsWorking())
		{
			SysWatchdogFeed(WTD_SHADOW_LOOP);
			osDelay(3000);
		}
		SysWatchdogFeed(WTD_SHADOW_LOOP);
		InitTimeout(&tUpdateInterval,SYSTICK_TIME_MS(30000));
		
		/* initialize and connect to broker*/
		printf("OS HEAP FREE HEAP SIZE before shadow_thing_init:%d\r\n",(uint32_t)xPortGetFreeHeapSize());
		rc = shadow_thing_init(&mqttClient);
		printf("OS HEAP FREE HEAP SIZE after shadow_thing_init:%d\r\n",(uint32_t)xPortGetFreeHeapSize());
		if(shadow_thing_fail_times >= SHADOW_MAX_RETRY_TIMES)
		{
			printf("shadow_thing_fail_times\r\n");
			shadow_thing_fail_times = SHADOW_MAX_RETRY_TIMES-3;
			shadow_thing_need_a_system_restart_cnt++;
			if(shadow_thing_need_a_system_restart_cnt >= 3)
			{
				printf("shadow_thing_need_a_system_restart_cnt\r\n");
				shadow_thing_need_a_system_restart_cnt = 0;
				//reset here
			}
			Ampm_GsmInternetSetDisconnect();
		}
		
		/* reported json*/
		jsonStruct_t gsmSignalJson	= ADD_JSON_FIELD(NULL, "gsm_signal", &gsmSignal, SHADOW_JSON_INT8);
		jsonStruct_t mobileService	= ADD_JSON_FIELD(NULL, "gsm_service", gsmSimCPOSBuf, SHADOW_JSON_STRING);
		jsonStruct_t batteryPercentage	= ADD_JSON_FIELD(NULL, "battery", &battery, SHADOW_JSON_INT8);
		jsonStruct_t lifeSpan	= ADD_JSON_FIELD(NULL, "age", &life_span, SHADOW_JSON_INT32);
		
		jsonStruct_t schedules	= ADD_JSON_FIELD(Schedules_Status_DeltaCallback, "schedules", "null", SHADOW_JSON_ARRAY);
		jsonStruct_t schedulesFromYear			= ADD_JSON_FIELD(NULL, "fY", &deviceCfg.schedules[0].fromYear, SHADOW_JSON_INT8);
		jsonStruct_t schedulesToYear			= ADD_JSON_FIELD(NULL, "tY", &deviceCfg.schedules[0].toYear, SHADOW_JSON_INT8);
		jsonStruct_t schedulesFromMonth			= ADD_JSON_FIELD(NULL, "tM", &deviceCfg.schedules[0].fromMonth, SHADOW_JSON_INT8);
		jsonStruct_t schedulesToMonth			= ADD_JSON_FIELD(NULL, "tM", &deviceCfg.schedules[0].toMonth, SHADOW_JSON_INT8);
		jsonStruct_t schedulesFromDay		= ADD_JSON_FIELD(NULL, "tD", &deviceCfg.schedules[0].fromMday, SHADOW_JSON_INT8);
		jsonStruct_t schedulesToDay			= ADD_JSON_FIELD(NULL, "tD", &deviceCfg.schedules[0].toMday, SHADOW_JSON_INT8);
		jsonStruct_t schedulesWeekDay		= ADD_JSON_FIELD(NULL, "wD", &weekDay, SHADOW_JSON_STRING);
		sprintf(scheduleTimeBuf,"%02d:%02d",deviceCfg.schedules[0].hour,deviceCfg.schedules[0].min);
		jsonStruct_t schedulesTime			= ADD_JSON_FIELD(NULL, "time", scheduleTimeBuf, SHADOW_JSON_STRING);
		jsonStruct_t warningsArray	= ADD_JSON_FIELD(NULL, "warnings", "null", SHADOW_JSON_ARRAY);
		jsonStruct_t alarmsArray	= ADD_JSON_FIELD(NULL, "alarms", "null", SHADOW_JSON_ARRAY);
		jsonStruct_t warningsName			= ADD_JSON_FIELD(NULL, "name", "low_power", SHADOW_JSON_STRING);
		jsonStruct_t warningsMsg			= ADD_JSON_FIELD(NULL, "msg", "line1,line2,line,3,line4", SHADOW_JSON_STRING);
		jsonStruct_t alarmsName			= ADD_JSON_FIELD(NULL, "name", "no_power", SHADOW_JSON_STRING);
		jsonStruct_t alarmsMsg			= ADD_JSON_FIELD(NULL, "msg", "line1,line2,line,3,line4", SHADOW_JSON_STRING);
//		jsonStruct_t schedulesCmd			= ADD_JSON_FIELD(NULL, "cmd", &scheduleCmds, SHADOW_JSON_OBJECT);
		//if(cmds)
		//{
		jsonStruct_t cmdsObj	= ADD_JSON_FIELD(Cmds_Status_DeltaCallback, "cmds", &cmds, SHADOW_JSON_OBJECT);
		//}
		/* nested reported object json and its fields*/
		
		jsonStruct_t line1Obj	= ADD_JSON_FIELD(Line1_Status_DeltaCallback, "line1", &line1, SHADOW_JSON_OBJECT);
		jsonStruct_t line1Status		= ADD_JSON_FIELD(NULL, "status", lineStatus[0], SHADOW_JSON_STRING);
		jsonStruct_t line1P			= ADD_JSON_FIELD(NULL, "P", &line1.P, SHADOW_JSON_INT32);
		jsonStruct_t line1Q			= ADD_JSON_FIELD(NULL, "Q", &line1.Q, SHADOW_JSON_INT32);
		jsonStruct_t line1U			= ADD_JSON_FIELD(NULL, "U", &line1.U, SHADOW_JSON_INT32);
		jsonStruct_t line1I			= ADD_JSON_FIELD(NULL, "I", &line1.I, SHADOW_JSON_INT32);
		jsonStruct_t line1Ir			= ADD_JSON_FIELD(NULL, "Ir", &line1.Ir, SHADOW_JSON_INT32);
		jsonStruct_t line1Wh			= ADD_JSON_FIELD(NULL, "wh", &line1.wh, SHADOW_JSON_INT32);
		
		jsonStruct_t line2Obj	= ADD_JSON_FIELD(Line2_Status_DeltaCallback, "line2", &line2, SHADOW_JSON_OBJECT);
		jsonStruct_t line2Status		= ADD_JSON_FIELD(NULL, "status", lineStatus[1], SHADOW_JSON_STRING);
		jsonStruct_t line2P			= ADD_JSON_FIELD(NULL, "P", &line2.P, SHADOW_JSON_INT32);
		jsonStruct_t line2Q			= ADD_JSON_FIELD(NULL, "Q", &line2.Q, SHADOW_JSON_INT32);
		jsonStruct_t line2U			= ADD_JSON_FIELD(NULL, "U", &line2.U, SHADOW_JSON_INT32);
		jsonStruct_t line2I			= ADD_JSON_FIELD(NULL, "I", &line2.I, SHADOW_JSON_INT32);
		jsonStruct_t line2Ir			= ADD_JSON_FIELD(NULL, "Ir", &line2.Ir, SHADOW_JSON_INT32);
		jsonStruct_t line2Wh			= ADD_JSON_FIELD(NULL, "wh", &line2.wh, SHADOW_JSON_INT32);
		
		jsonStruct_t line3Obj	= ADD_JSON_FIELD(Line3_Status_DeltaCallback, "line3", &line3, SHADOW_JSON_OBJECT);
		jsonStruct_t line3Status		= ADD_JSON_FIELD(NULL, "status", lineStatus[2], SHADOW_JSON_STRING);
		jsonStruct_t line3P			= ADD_JSON_FIELD(NULL, "P", &line3.P, SHADOW_JSON_INT32);
		jsonStruct_t line3Q			= ADD_JSON_FIELD(NULL, "Q", &line3.Q, SHADOW_JSON_INT32);
		jsonStruct_t line3U			= ADD_JSON_FIELD(NULL, "U", &line3.U, SHADOW_JSON_INT32);
		jsonStruct_t line3I			= ADD_JSON_FIELD(NULL, "I", &line3.I, SHADOW_JSON_INT32);
		jsonStruct_t line3Ir			= ADD_JSON_FIELD(NULL, "Ir", &line3.Ir, SHADOW_JSON_INT32);
		jsonStruct_t line3Wh			= ADD_JSON_FIELD(NULL, "wh", &line3.wh, SHADOW_JSON_INT32);
		
		
		jsonStruct_t line4Obj	= ADD_JSON_FIELD(Line4_Status_DeltaCallback, "line4", &line4, SHADOW_JSON_OBJECT);
		jsonStruct_t line4Status		= ADD_JSON_FIELD(NULL, "status", lineStatus[3], SHADOW_JSON_STRING);
		jsonStruct_t line4P			= ADD_JSON_FIELD(NULL, "P", &line4.P, SHADOW_JSON_INT32);
		jsonStruct_t line4Q			= ADD_JSON_FIELD(NULL, "Q", &line4.Q, SHADOW_JSON_INT32);
		jsonStruct_t line4U			= ADD_JSON_FIELD(NULL, "U", &line4.U, SHADOW_JSON_INT32);
		jsonStruct_t line4I			= ADD_JSON_FIELD(NULL, "I", &line4.I, SHADOW_JSON_INT32);
		jsonStruct_t line4Ir			= ADD_JSON_FIELD(NULL, "Ir", &line4.Ir, SHADOW_JSON_INT32);
		jsonStruct_t line4Wh			= ADD_JSON_FIELD(NULL, "wh", &line4.wh, SHADOW_JSON_INT32);
		

		/*
		 * Register the jsonStruct object
		 */
		aws_iot_shadow_register_delta(&mqttClient, &line1Obj);
		aws_iot_shadow_register_delta(&mqttClient, &line2Obj);
		aws_iot_shadow_register_delta(&mqttClient, &line3Obj);
		aws_iot_shadow_register_delta(&mqttClient, &line4Obj);
		aws_iot_shadow_register_delta(&mqttClient, &cmdsObj);
		aws_iot_shadow_register_delta(&mqttClient, &schedules);
		if (AWS_SUCCESS != rc) {
			shadow_thing_fail_times++;
			IOT_ERROR("Shadow Register Delta Error");
		}
//		aws_iot_shadow_register_delta(&mqttClient, &line2Obj);
//		aws_iot_shadow_register_delta(&mqttClient, &line3Obj);
		IOT_SHADOW_THING("\n===========================================================================\n");
		rc = aws_iot_shadow_init_json_document(pJsonDoccument, sizeOfJsonDocument);
		if(AWS_SUCCESS == rc)
		{
			rc = aws_iot_shahow_add_reported_header(pJsonDoccument, sizeOfJsonDocument);
			if(AWS_SUCCESS == rc)
			{
					rc = aws_iot_shadow_add_fields(pJsonDoccument, sizeOfJsonDocument, 4,&gsmSignalJson,&mobileService,&batteryPercentage,&lifeSpan);
					if(AWS_SUCCESS == rc)
					{
						line1.status = deviceIoStatus.line[0];
						line2.status = deviceIoStatus.line[1];
						line3.status = deviceIoStatus.line[2];
						line4.status = deviceIoStatus.line[3];
						line1.I = powerLine1.IL*1000;
						line1.U = powerLine1.U;
						line1.Q = powerLine1.Q;
						line1.P = powerLine1.P;
						line1.Ir = powerLine1.Ir*1000;
						line1.wh = powerLine1.wh;
						if(line1.status)
							strcpy(lineStatus[0],"ON");
						else
							strcpy(lineStatus[0],"OFF");
						
						if(line2.status)
							strcpy(lineStatus[1],"ON");
						else
							strcpy(lineStatus[1],"OFF");
						
						if(line3.status)
							strcpy(lineStatus[2],"ON");
						else
							strcpy(lineStatus[2],"OFF");
						
						if(line4.status)
							strcpy(lineStatus[3],"ON");
						else
							strcpy(lineStatus[3],"OFF");
						
						rc = aws_iot_add_object(pJsonDoccument, sizeOfJsonDocument, &line1Obj, 7,
																				&line1Status,
																				&line1P,
																				&line1Q,
																				&line1U,
																				&line1I,
																				&line1Ir,
																				&line1Wh);
						rc = aws_iot_add_object(pJsonDoccument, sizeOfJsonDocument, &line2Obj, 7,
																				&line2Status,
																				&line2P,
																				&line2Q,
																				&line2U,
																				&line2I,
																				&line2Ir,
																				&line2Wh);
						rc = aws_iot_add_object(pJsonDoccument, sizeOfJsonDocument, &line3Obj, 7,
																				&line3Status,
																				&line3P,
																				&line3Q,
																				&line3U,
																				&line3I,
																				&line3Ir,
																				&line3Wh);
						rc = aws_iot_add_object(pJsonDoccument, sizeOfJsonDocument, &line4Obj, 7,
																				&line4Status,
																				&line4P,
																				&line4Q,
																				&line4U,
																				&line4I,
																				&line4Ir,
																				&line4Wh);
						//cmd
						if(cmds.len)
						{
							aws_iot_add_object_from_buffer(pJsonDoccument, sizeOfJsonDocument, &cmdsObj,cmds.cmds);
						}
						
						//Schedules
						sizeOfScheduleJson = (Alarm_GetSize()+1)*132; 
						char *pScheduleJson = mbedtls_calloc(1,sizeOfScheduleJson);
						if(pScheduleJson)
						{
							memset(pScheduleJson,0,sizeOfScheduleJson);
							for(uint8_t i = 0;i < DEVICE_SCHEDULE_MAX; i++)
							{
								if(deviceCfg.schedules[i].hour != 0xff
								|| deviceCfg.schedules[i].min != 0xff)
								{
									if(i == 0)
										sprintf(&pScheduleJson[strlen(pScheduleJson)],"{");
									else
										sprintf(&pScheduleJson[strlen(pScheduleJson)],",{");
									if(deviceCfg.schedules[i].fromYear != 0xff)
									{
										schedulesFromYear.pData = &deviceCfg.schedules[i].fromYear;
										aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesFromYear);
									}
									if(deviceCfg.schedules[i].toYear != 0xff)
									{
										schedulesToYear.pData = &deviceCfg.schedules[i].toYear;
										aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesToYear);
									}
									if(deviceCfg.schedules[i].fromMonth != 0xff)
									{
										schedulesFromMonth.pData = &deviceCfg.schedules[i].fromMonth;
										aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesFromMonth);
									}
									if(deviceCfg.schedules[i].toMonth != 0xff)
									{
										schedulesToMonth.pData = &deviceCfg.schedules[i].toMonth;
										aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesToMonth);
									}
									if(deviceCfg.schedules[i].fromMday != 0xff)
									{
										schedulesFromDay.pData = &deviceCfg.schedules[i].fromMday;
										aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesFromDay);
									}
									if(deviceCfg.schedules[i].toMday != 0xff)
									{
										schedulesToDay.pData = &deviceCfg.schedules[i].toMday;
										aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesToDay);
									}
									if(deviceCfg.schedules[i].weekDay != 0xff)
									{
										weekDay[0] = 0;
										for(uint8_t bit = 0;bit < 8; bit++)
										{
											switch(deviceCfg.schedules[i].weekDay & (1<<bit))
											{
												case 1<<SCHEDULE_WEEKDAY_MONDAY:
													strcat(weekDay,"T2");
												break;
												case 1<<SCHEDULE_WEEKDAY_TUESDAY:
													strcat(weekDay,"T3");
												break;
												case 1<<SCHEDULE_WEEKDAY_WEDNESDAY:
													strcat(weekDay,"T4");
												break;
												case 1<<SCHEDULE_WEEKDAY_THURSDAY:
													strcat(weekDay,"T5");
												break;
												case 1<<SCHEDULE_WEEKDAY_FRIDAY:
													strcat(weekDay,"T6");
												break;
												case 1<<SCHEDULE_WEEKDAY_SATURDAY:
													strcat(weekDay,"T7");
												break;
												case 1<<SCHEDULE_WEEKDAY_SUNDAY:
													strcat(weekDay,"CN");
												break;
												default:
													break;
											}
										}
										aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesWeekDay);
									}
									sprintf(scheduleTimeBuf,"%02d:%02d",deviceCfg.schedules[i].hour,deviceCfg.schedules[i].min);
									aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesTime);
									sprintf(&pScheduleJson[strlen(pScheduleJson)],"\"cmd\":{");
									for(uint8_t j = 0;j < 8; j++)
									{
										if(deviceCfg.schedules[i].valueOn & (1 << j))
										{
											sprintf(&pScheduleJson[strlen(pScheduleJson)],"\"line%d\":\"ON\",",j+1);
										}
										else if(deviceCfg.schedules[i].valueOff & (1 << j))
										{
											sprintf(&pScheduleJson[strlen(pScheduleJson)],"\"line%d\":\"OFF\",",j+1);
										}
									}
									if(pScheduleJson[strlen(pScheduleJson) - 1] == ',')
										sprintf(&pScheduleJson[strlen(pScheduleJson) - 1],"}}");
									else
										sprintf(&pScheduleJson[strlen(pScheduleJson)],"null}}");
								}
							}
							
							if(strlen(pScheduleJson) == 0)
							{
								sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"schedules\":null,");
							}
							else
							{
								schedules.pData = pScheduleJson;
								rc = aws_iot_shadow_add_fields(pJsonDoccument, sizeOfJsonDocument, 1, &schedules);
							}
							mbedtls_free(pScheduleJson);
						}
						//warnings
						sizeOfWarningsJson = 512; 
						char *pWarningsJson = mbedtls_calloc(1,sizeOfWarningsJson);
						if(pWarningsJson)
						{
							memset(pWarningsJson,0,sizeOfWarningsJson);
//									variableNoInit.deviceStatus = 0xffffffff;
							if(variableNoInit.deviceStatus & (LINE1_NO_POWER_ALARM | LINE2_NO_POWER_ALARM | LINE3_NO_POWER_ALARM |LINE4_NO_POWER_ALARM))
							{
								//sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"warnings\":null,");
								sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"warnings\":null,");
							}
							else if(variableNoInit.deviceStatus)
							{
								char *msg = mbedtls_calloc(1,64);
								warningsMsg.pData  = msg;
								//no_curent
								if(variableNoInit.deviceStatus & 
									(LINE1_NO_CURRENT_WARNING | LINE2_NO_CURRENT_WARNING | LINE3_NO_CURRENT_WARNING | LINE4_NO_CURRENT_WARNING)
								)
								{
									
									sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
									warningsName.pData = "no_curent";
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
									msg[0] = 0;
									if(variableNoInit.deviceStatus & LINE1_NO_CURRENT_WARNING)
									{
										strcat(msg, "line1;");
									}
									if(variableNoInit.deviceStatus & LINE2_NO_CURRENT_WARNING)
									{
										strcat(msg, "line2;");
									}
									if(variableNoInit.deviceStatus & LINE3_NO_CURRENT_WARNING)
									{
										strcat(msg, "line3;");
									}
									if(variableNoInit.deviceStatus & LINE4_NO_CURRENT_WARNING)
									{
										strcat(msg, "line4;");
									}
									msg[strlen(msg)-1] = 0; //remove ;
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
									sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
								}
								//leakage_curent
								if(variableNoInit.deviceStatus & 
									(LINE1_LEAKAGE_CURRENT_WARNING | LINE2_LEAKAGE_CURRENT_WARNING | LINE3_LEAKAGE_CURRENT_WARNING | LINE4_LEAKAGE_CURRENT_WARNING)
								)
								{
									sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
									warningsName.pData = "leakage_curent";
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
									msg[0] = 0;
									if(variableNoInit.deviceStatus & LINE1_LEAKAGE_CURRENT_WARNING)
									{
										strcat(msg, "line1;");
									}
									if(variableNoInit.deviceStatus & LINE2_LEAKAGE_CURRENT_WARNING)
									{
										strcat(msg, "line2;");
									}
									if(variableNoInit.deviceStatus & LINE3_LEAKAGE_CURRENT_WARNING)
									{
										strcat(msg, "line3;");
									}
									if(variableNoInit.deviceStatus & LINE4_LEAKAGE_CURRENT_WARNING)
									{
										strcat(msg, "line4;");
									}
									msg[strlen(msg)-1] = 0; //remove ;
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
									sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
								}
								
								//low_curent
								if(variableNoInit.deviceStatus & 
									(LINE1_LOW_CURRENT_WARNING | LINE2_LOW_CURRENT_WARNING | LINE3_LOW_CURRENT_WARNING | LINE4_LOW_CURRENT_WARNING)
								)
								{
									sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
									warningsName.pData = "low_curent";
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
									msg[0] = 0;
									if(variableNoInit.deviceStatus & LINE1_LOW_CURRENT_WARNING)
									{
										strcat(msg, "line1;");
									}
									if(variableNoInit.deviceStatus & LINE2_LOW_CURRENT_WARNING)
									{
										strcat(msg, "line2;");
									}
									if(variableNoInit.deviceStatus & LINE3_LOW_CURRENT_WARNING)
									{
										strcat(msg, "line3;");
									}
									if(variableNoInit.deviceStatus & LINE4_LOW_CURRENT_WARNING)
									{
										strcat(msg, "line4;");
									}
									msg[strlen(msg)-1] = 0; //remove ;
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
									sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
								}
								//high_curent
								if(variableNoInit.deviceStatus & 
									(LINE1_HIGH_CURRENT_WARNING | LINE2_HIGH_CURRENT_WARNING | LINE3_HIGH_CURRENT_WARNING | LINE4_HIGH_CURRENT_WARNING)
								)
								{
									sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
									warningsName.pData = "high_curent";
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
									msg[0] = 0;
									if(variableNoInit.deviceStatus & LINE1_HIGH_CURRENT_WARNING)
									{
										strcat(msg, "line1;");
									}
									if(variableNoInit.deviceStatus & LINE2_HIGH_CURRENT_WARNING)
									{
										strcat(msg, "line2;");
									}
									if(variableNoInit.deviceStatus & LINE3_HIGH_CURRENT_WARNING)
									{
										strcat(msg, "line3;");
									}
									if(variableNoInit.deviceStatus & LINE4_HIGH_CURRENT_WARNING)
									{
										strcat(msg, "line4;");
									}
									msg[strlen(msg)-1] = 0; //remove ;
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
									sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
								}
								//low_voltage
								if(variableNoInit.deviceStatus & 
									(LINE1_LOW_VOLTAGE_WARNING | LINE2_LOW_VOLTAGE_WARNING | LINE3_LOW_VOLTAGE_WARNING | LINE4_LOW_VOLTAGE_WARNING)
								)
								{
									sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
									warningsName.pData = "low_voltage";
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
									msg[0] = 0;
									if(variableNoInit.deviceStatus & LINE1_LOW_VOLTAGE_WARNING)
									{
										strcat(msg, "line1;");
									}
									if(variableNoInit.deviceStatus & LINE2_LOW_VOLTAGE_WARNING)
									{
										strcat(msg, "line2;");
									}
									if(variableNoInit.deviceStatus & LINE3_LOW_VOLTAGE_WARNING)
									{
										strcat(msg, "line3;");
									}
									if(variableNoInit.deviceStatus & LINE4_LOW_VOLTAGE_WARNING)
									{
										strcat(msg, "line4;");
									}
									msg[strlen(msg)-1] = 0; //remove ;
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
									sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
								}
								//high_voltage
								if(variableNoInit.deviceStatus & 
									(LINE1_HIGH_VOLTAGE_WARNING | LINE2_HIGH_VOLTAGE_WARNING | LINE3_HIGH_VOLTAGE_WARNING | LINE4_HIGH_VOLTAGE_WARNING)
								)
								{
									sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
									warningsName.pData = "high_voltage";
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
									msg[0] = 0;
									if(variableNoInit.deviceStatus & LINE1_HIGH_VOLTAGE_WARNING)
									{
										strcat(msg, "line1;");
									}
									if(variableNoInit.deviceStatus & LINE2_HIGH_VOLTAGE_WARNING)
									{
										strcat(msg, "line2;");
									}
									if(variableNoInit.deviceStatus & LINE3_HIGH_VOLTAGE_WARNING)
									{
										strcat(msg, "line3;");
									}
									if(variableNoInit.deviceStatus & LINE4_HIGH_VOLTAGE_WARNING)
									{
										strcat(msg, "line4;");
									}
									msg[strlen(msg)-1] = 0; //remove ;
									aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
									sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
								}
								if(strlen(pWarningsJson))
								{
									if(pWarningsJson[strlen(pWarningsJson) - 1] == ',')
										pWarningsJson[strlen(pWarningsJson) - 1] = '\0';
									mbedtls_free(msg);
									warningsArray.pData = pWarningsJson;
									rc = aws_iot_shadow_add_fields(pJsonDoccument, sizeOfJsonDocument, 1, &warningsArray);
								}
								else
								{
									sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"warnings\":null,");
									mbedtls_free(msg);
								}
							}
							mbedtls_free(pWarningsJson);
						}
						
						//Alarm
						sizeOfAlarmsJson = 512; 
						char *pAlarmsJson = mbedtls_calloc(1,sizeOfAlarmsJson);
						if(pAlarmsJson)
						{
							memset(pAlarmsJson,0,sizeOfAlarmsJson);
							//variableNoInit.deviceStatus = 0xffffffff;
							if(variableNoInit.deviceStatus & (LINE1_NO_POWER_ALARM | LINE2_NO_POWER_ALARM | LINE3_NO_POWER_ALARM |LINE4_NO_POWER_ALARM))
							{
								char *msg = mbedtls_calloc(1,64);
								alarmsMsg.pData  = msg;
								//no_curent
								if(variableNoInit.deviceStatus & 
									(LINE1_NO_POWER_ALARM | LINE2_NO_POWER_ALARM | LINE3_NO_POWER_ALARM | LINE4_NO_POWER_ALARM)
								)
								{
									
									sprintf(&pAlarmsJson[strlen(pAlarmsJson)],"{");
									alarmsName.pData = "no_power";
									aws_iot_shadow_add_fields(pAlarmsJson, sizeOfAlarmsJson, 1,&alarmsName);
									msg[0] = 0;
									if(variableNoInit.deviceStatus & LINE1_NO_POWER_ALARM)
									{
										strcat(msg, "line1;");
									}
									if(variableNoInit.deviceStatus & LINE2_NO_POWER_ALARM)
									{
										strcat(msg, "line2;");
									}
									if(variableNoInit.deviceStatus & LINE3_NO_POWER_ALARM)
									{
										strcat(msg, "line3;");
									}
									if(variableNoInit.deviceStatus & LINE4_NO_POWER_ALARM)
									{
										strcat(msg, "line4;");
									}
									msg[strlen(msg)-1] = 0; //remove ;
									aws_iot_shadow_add_fields(pAlarmsJson, sizeOfAlarmsJson, 1,&alarmsMsg);
									sprintf(&pAlarmsJson[strlen(pAlarmsJson) - 1],"},");
								}
								if(strlen(pAlarmsJson))
								{
									if(pAlarmsJson[strlen(pAlarmsJson) - 1] == ',')
										pAlarmsJson[strlen(pAlarmsJson) - 1] = '\0';
									mbedtls_free(msg);
									alarmsArray.pData = pAlarmsJson;
									rc = aws_iot_shadow_add_fields(pJsonDoccument, sizeOfJsonDocument, 1, &alarmsArray);
								}
								else
								{
									sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"alarms\":null,");
									mbedtls_free(msg);
								}
							}
							else
								sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"alarms\":null,");
							mbedtls_free(pAlarmsJson);
						}
						
						
						if(AWS_SUCCESS == rc)
						{
							rc = aws_shadow_footer_reported(pJsonDoccument, sizeOfJsonDocument);
							if(AWS_SUCCESS == rc)
							{
								rc = aws_iot_finalize_json_document(pJsonDoccument, sizeOfJsonDocument);
								if(AWS_SUCCESS == rc)
								{
									IOT_SHADOW_THING("Update Shadow: %s", pJsonDoccument);
									rc = aws_iot_shadow_update(&mqttClient, AWS_IOT_MY_THING_NAME,
																			pJsonDoccument,
																			ShadowUpdateStatusCallback,
																			NULL, 10, true);
								}
							}
						}
					}
			}
		}
		IOT_SHADOW_THING("*****************************************************************************************\n");
		if(AWS_SUCCESS != rc)
		{
			IOT_ERROR("failed!");
		}
		vTaskDelay(1000);
		while(NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc || AWS_SUCCESS == rc)
		{
			while(!Ampm_GsmGPRS_IsWorking())
			{
				osDelay(1000);
				SysWatchdogFeed(WTD_SHADOW_LOOP);
			}
			//IOT_SHADOW_THING("aws_iot_shadow_yield()-START");
			SysWatchdogFeed(WTD_SHADOW_LOOP);
			rc = aws_iot_shadow_yield(&mqttClient, 1000);
			if(NETWORK_ATTEMPTING_RECONNECT == rc) {
				vTaskDelay(3000);
				// If the client is attempting to reconnect we will skip the rest of the loop.
				IOT_ERROR("Reconnect!");
				shadow_thing_fail_times++;
				continue;
			}
			//IOT_SHADOW_THING("aws_iot_shadow_yield()-END");
			//vTaskDelay(500);
			osSemaphoreWait(shadow_sem_id, 1000);
			osSemaphoreRelease(shadow_sem_id);
			if(shadowMsgBusyTimeout)
			{
				shadowMsgBusyTimeout--;
			}
			else if(CheckTimeout(&tUpdateInterval) == SYSTICK_TIMEOUT || sendReportFlag)
			{
				shadowMsgBusyTimeout = 10; //10 sec
				sendReportFlag = 0;
//				if(checkMemoryCnt++ % 10 == 0)
//					mbedtls_memory_buffer_alloc_status();
				InitTimeout(&tUpdateInterval,SYSTICK_TIME_MS(30000));
				IOT_SHADOW_THING("\n===========================================================================\n");
				rc = aws_iot_shadow_init_json_document(pJsonDoccument, sizeOfJsonDocument);
				if(AWS_SUCCESS == rc)
				{
					rc = aws_iot_shahow_add_reported_header(pJsonDoccument, sizeOfJsonDocument);
					if(AWS_SUCCESS == rc)
					{
							rc = aws_iot_shadow_add_fields(pJsonDoccument, sizeOfJsonDocument, 4,&gsmSignalJson,&mobileService,&batteryPercentage,&lifeSpan);
							if(AWS_SUCCESS == rc)
							{
								line1.status = deviceIoStatus.line[0];
								line2.status = deviceIoStatus.line[1];
								line3.status = deviceIoStatus.line[2];
								line4.status = deviceIoStatus.line[3];
								
								line1.I = powerLine1.IL*1000;
								line1.U = powerLine1.U;
								line1.Q = powerLine1.Q;
								line1.P = powerLine1.P;
								line1.Ir = powerLine1.Ir*1000;
								line1.wh = powerLine1.wh;
								
								if(line1.status)
									strcpy(lineStatus[0],"ON");
								else
									strcpy(lineStatus[0],"OFF");
								
								if(line2.status)
									strcpy(lineStatus[1],"ON");
								else
									strcpy(lineStatus[1],"OFF");
								
								if(line3.status)
									strcpy(lineStatus[2],"ON");
								else
									strcpy(lineStatus[2],"OFF");
								
								if(line4.status)
									strcpy(lineStatus[3],"ON");
								else
									strcpy(lineStatus[3],"OFF");
								
								rc = aws_iot_add_object(pJsonDoccument, sizeOfJsonDocument, &line1Obj, 7,
																						&line1Status,
																						&line1P,
																						&line1Q,
																						&line1U,
																						&line1I,
																						&line1Ir,
																						&line1Wh);
								rc = aws_iot_add_object(pJsonDoccument, sizeOfJsonDocument, &line2Obj, 7,
																						&line2Status,
																						&line2P,
																						&line2Q,
																						&line2U,
																						&line2I,
																						&line2Ir,
																						&line2Wh);
								rc = aws_iot_add_object(pJsonDoccument, sizeOfJsonDocument, &line3Obj, 7,
																						&line3Status,
																						&line3P,
																						&line3Q,
																						&line3U,
																						&line3I,
																						&line3Ir,
																						&line3Wh);
								rc = aws_iot_add_object(pJsonDoccument, sizeOfJsonDocument, &line4Obj, 7,
																						&line4Status,
																						&line4P,
																						&line4Q,
																						&line4U,
																						&line4I,
																						&line4Ir,
																						&line4Wh);
								//cmd
								if(cmds.len)
								{
									aws_iot_add_object_from_buffer(pJsonDoccument, sizeOfJsonDocument, &cmdsObj,cmds.cmds);
								}
								
								//Schedules
								sizeOfScheduleJson = (Alarm_GetSize()+1)*132; 
								char *pScheduleJson = mbedtls_calloc(1,sizeOfScheduleJson);
								if(pScheduleJson)
								{
									memset(pScheduleJson,0,sizeOfScheduleJson);
									for(uint8_t i = 0;i < DEVICE_SCHEDULE_MAX; i++)
									{
										if(deviceCfg.schedules[i].hour != 0xff
										|| deviceCfg.schedules[i].min != 0xff)
										{
											if(i == 0)
												sprintf(&pScheduleJson[strlen(pScheduleJson)],"{");
											else
												sprintf(&pScheduleJson[strlen(pScheduleJson)],",{");
											if(deviceCfg.schedules[i].fromYear != 0xff)
											{
												schedulesFromYear.pData = &deviceCfg.schedules[i].fromYear;
												aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesFromYear);
											}
											if(deviceCfg.schedules[i].toYear != 0xff)
											{
												schedulesToYear.pData = &deviceCfg.schedules[i].toYear;
												aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesToYear);
											}
											if(deviceCfg.schedules[i].fromMonth != 0xff)
											{
												schedulesFromMonth.pData = &deviceCfg.schedules[i].fromMonth;
												aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesFromMonth);
											}
											if(deviceCfg.schedules[i].toMonth != 0xff)
											{
												schedulesToMonth.pData = &deviceCfg.schedules[i].toMonth;
												aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesToMonth);
											}
											if(deviceCfg.schedules[i].fromMday != 0xff)
											{
												schedulesFromDay.pData = &deviceCfg.schedules[i].fromMday;
												aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesFromDay);
											}
											if(deviceCfg.schedules[i].toMday != 0xff)
											{
												schedulesToDay.pData = &deviceCfg.schedules[i].toMday;
												aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesToDay);
											}
											if(deviceCfg.schedules[i].weekDay != 0xff)
											{
												weekDay[0] = 0;
												for(uint8_t bit = 0;bit < 8; bit++)
												{
													switch(deviceCfg.schedules[i].weekDay & (1<<bit))
													{
														case 1<<SCHEDULE_WEEKDAY_MONDAY:
															strcat(weekDay,"T2");
														break;
														case 1<<SCHEDULE_WEEKDAY_TUESDAY:
															strcat(weekDay,"T3");
														break;
														case 1<<SCHEDULE_WEEKDAY_WEDNESDAY:
															strcat(weekDay,"T4");
														break;
														case 1<<SCHEDULE_WEEKDAY_THURSDAY:
															strcat(weekDay,"T5");
														break;
														case 1<<SCHEDULE_WEEKDAY_FRIDAY:
															strcat(weekDay,"T6");
														break;
														case 1<<SCHEDULE_WEEKDAY_SATURDAY:
															strcat(weekDay,"T7");
														break;
														case 1<<SCHEDULE_WEEKDAY_SUNDAY:
															strcat(weekDay,"CN");
														break;
														default:
															break;
													}
												}
												aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesWeekDay);
											}
											sprintf(scheduleTimeBuf,"%02d:%02d",deviceCfg.schedules[i].hour,deviceCfg.schedules[i].min);
											aws_iot_shadow_add_fields(pScheduleJson, sizeOfScheduleJson, 1,&schedulesTime);
											sprintf(&pScheduleJson[strlen(pScheduleJson)],"\"cmd\":{");
											for(uint8_t j = 0;j < 8; j++)
											{
												if(deviceCfg.schedules[i].valueOn & (1 << j))
												{
													sprintf(&pScheduleJson[strlen(pScheduleJson)],"\"line%d\":\"ON\",",j+1);
												}
												else if(deviceCfg.schedules[i].valueOff & (1 << j))
												{
													sprintf(&pScheduleJson[strlen(pScheduleJson)],"\"line%d\":\"OFF\",",j+1);
												}
											}
											if(pScheduleJson[strlen(pScheduleJson) - 1] == ',')
												sprintf(&pScheduleJson[strlen(pScheduleJson) - 1],"}}");
											else
												sprintf(&pScheduleJson[strlen(pScheduleJson)],"null}}");
										}
									}
									
									if(strlen(pScheduleJson) == 0)
									{
										sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"schedules\":null,");
									}
									else
									{
										schedules.pData = pScheduleJson;
										rc = aws_iot_shadow_add_fields(pJsonDoccument, sizeOfJsonDocument, 1, &schedules);
									}
									mbedtls_free(pScheduleJson);
								}
								//warnings
								sizeOfWarningsJson = 512; 
								char *pWarningsJson = mbedtls_calloc(1,sizeOfWarningsJson);
								if(pWarningsJson)
								{
									memset(pWarningsJson,0,sizeOfWarningsJson);
//									variableNoInit.deviceStatus = 0xffffffff;
									if(variableNoInit.deviceStatus & (LINE1_NO_POWER_ALARM | LINE2_NO_POWER_ALARM | LINE3_NO_POWER_ALARM |LINE4_NO_POWER_ALARM))
									{
										//sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"warnings\":null,");
										sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"warnings\":null,");
									}
									else if(variableNoInit.deviceStatus)
									{
										char *msg = mbedtls_calloc(1,64);
										warningsMsg.pData  = msg;
										//no_curent
										if(variableNoInit.deviceStatus & 
											(LINE1_NO_CURRENT_WARNING | LINE2_NO_CURRENT_WARNING | LINE3_NO_CURRENT_WARNING | LINE4_NO_CURRENT_WARNING)
										)
										{
											
											sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
											warningsName.pData = "no_curent";
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
											msg[0] = 0;
											if(variableNoInit.deviceStatus & LINE1_NO_CURRENT_WARNING)
											{
												strcat(msg, "line1;");
											}
											if(variableNoInit.deviceStatus & LINE2_NO_CURRENT_WARNING)
											{
												strcat(msg, "line2;");
											}
											if(variableNoInit.deviceStatus & LINE3_NO_CURRENT_WARNING)
											{
												strcat(msg, "line3;");
											}
											if(variableNoInit.deviceStatus & LINE4_NO_CURRENT_WARNING)
											{
												strcat(msg, "line4;");
											}
											msg[strlen(msg)-1] = 0; //remove ;
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
											sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
										}
										//leakage_curent
										if(variableNoInit.deviceStatus & 
											(LINE1_LEAKAGE_CURRENT_WARNING | LINE2_LEAKAGE_CURRENT_WARNING | LINE3_LEAKAGE_CURRENT_WARNING | LINE4_LEAKAGE_CURRENT_WARNING)
										)
										{
											sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
											warningsName.pData = "leakage_curent";
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
											msg[0] = 0;
											if(variableNoInit.deviceStatus & LINE1_LEAKAGE_CURRENT_WARNING)
											{
												strcat(msg, "line1;");
											}
											if(variableNoInit.deviceStatus & LINE2_LEAKAGE_CURRENT_WARNING)
											{
												strcat(msg, "line2;");
											}
											if(variableNoInit.deviceStatus & LINE3_LEAKAGE_CURRENT_WARNING)
											{
												strcat(msg, "line3;");
											}
											if(variableNoInit.deviceStatus & LINE4_LEAKAGE_CURRENT_WARNING)
											{
												strcat(msg, "line4;");
											}
											msg[strlen(msg)-1] = 0; //remove ;
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
											sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
										}
										
										//low_curent
										if(variableNoInit.deviceStatus & 
											(LINE1_LOW_CURRENT_WARNING | LINE2_LOW_CURRENT_WARNING | LINE3_LOW_CURRENT_WARNING | LINE4_LOW_CURRENT_WARNING)
										)
										{
											sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
											warningsName.pData = "low_curent";
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
											msg[0] = 0;
											if(variableNoInit.deviceStatus & LINE1_LOW_CURRENT_WARNING)
											{
												strcat(msg, "line1;");
											}
											if(variableNoInit.deviceStatus & LINE2_LOW_CURRENT_WARNING)
											{
												strcat(msg, "line2;");
											}
											if(variableNoInit.deviceStatus & LINE3_LOW_CURRENT_WARNING)
											{
												strcat(msg, "line3;");
											}
											if(variableNoInit.deviceStatus & LINE4_LOW_CURRENT_WARNING)
											{
												strcat(msg, "line4;");
											}
											msg[strlen(msg)-1] = 0; //remove ;
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
											sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
										}
										//high_curent
										if(variableNoInit.deviceStatus & 
											(LINE1_HIGH_CURRENT_WARNING | LINE2_HIGH_CURRENT_WARNING | LINE3_HIGH_CURRENT_WARNING | LINE4_HIGH_CURRENT_WARNING)
										)
										{
											sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
											warningsName.pData = "high_curent";
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
											msg[0] = 0;
											if(variableNoInit.deviceStatus & LINE1_HIGH_CURRENT_WARNING)
											{
												strcat(msg, "line1;");
											}
											if(variableNoInit.deviceStatus & LINE2_HIGH_CURRENT_WARNING)
											{
												strcat(msg, "line2;");
											}
											if(variableNoInit.deviceStatus & LINE3_HIGH_CURRENT_WARNING)
											{
												strcat(msg, "line3;");
											}
											if(variableNoInit.deviceStatus & LINE4_HIGH_CURRENT_WARNING)
											{
												strcat(msg, "line4;");
											}
											msg[strlen(msg)-1] = 0; //remove ;
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
											sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
										}
										//low_voltage
										if(variableNoInit.deviceStatus & 
											(LINE1_LOW_VOLTAGE_WARNING | LINE2_LOW_VOLTAGE_WARNING | LINE3_LOW_VOLTAGE_WARNING | LINE4_LOW_VOLTAGE_WARNING)
										)
										{
											sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
											warningsName.pData = "low_voltage";
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
											msg[0] = 0;
											if(variableNoInit.deviceStatus & LINE1_LOW_VOLTAGE_WARNING)
											{
												strcat(msg, "line1;");
											}
											if(variableNoInit.deviceStatus & LINE2_LOW_VOLTAGE_WARNING)
											{
												strcat(msg, "line2;");
											}
											if(variableNoInit.deviceStatus & LINE3_LOW_VOLTAGE_WARNING)
											{
												strcat(msg, "line3;");
											}
											if(variableNoInit.deviceStatus & LINE4_LOW_VOLTAGE_WARNING)
											{
												strcat(msg, "line4;");
											}
											msg[strlen(msg)-1] = 0; //remove ;
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
											sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
										}
										//high_voltage
										if(variableNoInit.deviceStatus & 
											(LINE1_HIGH_VOLTAGE_WARNING | LINE2_HIGH_VOLTAGE_WARNING | LINE3_HIGH_VOLTAGE_WARNING | LINE4_HIGH_VOLTAGE_WARNING)
										)
										{
											sprintf(&pWarningsJson[strlen(pWarningsJson)],"{");
											warningsName.pData = "high_voltage";
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsName);
											msg[0] = 0;
											if(variableNoInit.deviceStatus & LINE1_HIGH_VOLTAGE_WARNING)
											{
												strcat(msg, "line1;");
											}
											if(variableNoInit.deviceStatus & LINE2_HIGH_VOLTAGE_WARNING)
											{
												strcat(msg, "line2;");
											}
											if(variableNoInit.deviceStatus & LINE3_HIGH_VOLTAGE_WARNING)
											{
												strcat(msg, "line3;");
											}
											if(variableNoInit.deviceStatus & LINE4_HIGH_VOLTAGE_WARNING)
											{
												strcat(msg, "line4;");
											}
											msg[strlen(msg)-1] = 0; //remove ;
											aws_iot_shadow_add_fields(pWarningsJson, sizeOfWarningsJson, 1,&warningsMsg);
											sprintf(&pWarningsJson[strlen(pWarningsJson) - 1],"},");
										}
										if(strlen(pWarningsJson))
										{
											if(pWarningsJson[strlen(pWarningsJson) - 1] == ',')
												pWarningsJson[strlen(pWarningsJson) - 1] = '\0';
											mbedtls_free(msg);
											warningsArray.pData = pWarningsJson;
											rc = aws_iot_shadow_add_fields(pJsonDoccument, sizeOfJsonDocument, 1, &warningsArray);
										}
										else
										{
											sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"warnings\":null,");
											mbedtls_free(msg);
										}
									}
									mbedtls_free(pWarningsJson);
								}
								
								//Alarm
								sizeOfAlarmsJson = 512; 
								char *pAlarmsJson = mbedtls_calloc(1,sizeOfAlarmsJson);
								if(pAlarmsJson)
								{
									memset(pAlarmsJson,0,sizeOfAlarmsJson);
									//variableNoInit.deviceStatus = 0xffffffff;
									if(variableNoInit.deviceStatus & (LINE1_NO_POWER_ALARM | LINE2_NO_POWER_ALARM | LINE3_NO_POWER_ALARM |LINE4_NO_POWER_ALARM))
									{
										char *msg = mbedtls_calloc(1,64);
										alarmsMsg.pData  = msg;
										//no_curent
										if(variableNoInit.deviceStatus & 
											(LINE1_NO_POWER_ALARM | LINE2_NO_POWER_ALARM | LINE3_NO_POWER_ALARM | LINE4_NO_POWER_ALARM)
										)
										{
											
											sprintf(&pAlarmsJson[strlen(pAlarmsJson)],"{");
											alarmsName.pData = "no_power";
											aws_iot_shadow_add_fields(pAlarmsJson, sizeOfAlarmsJson, 1,&alarmsName);
											msg[0] = 0;
											if(variableNoInit.deviceStatus & LINE1_NO_POWER_ALARM)
											{
												strcat(msg, "line1;");
											}
											if(variableNoInit.deviceStatus & LINE2_NO_POWER_ALARM)
											{
												strcat(msg, "line2;");
											}
											if(variableNoInit.deviceStatus & LINE3_NO_POWER_ALARM)
											{
												strcat(msg, "line3;");
											}
											if(variableNoInit.deviceStatus & LINE4_NO_POWER_ALARM)
											{
												strcat(msg, "line4;");
											}
											msg[strlen(msg)-1] = 0; //remove ;
											aws_iot_shadow_add_fields(pAlarmsJson, sizeOfAlarmsJson, 1,&alarmsMsg);
											sprintf(&pAlarmsJson[strlen(pAlarmsJson) - 1],"},");
										}
										if(strlen(pAlarmsJson))
										{
											if(pAlarmsJson[strlen(pAlarmsJson) - 1] == ',')
												pAlarmsJson[strlen(pAlarmsJson) - 1] = '\0';
											mbedtls_free(msg);
											alarmsArray.pData = pAlarmsJson;
											rc = aws_iot_shadow_add_fields(pJsonDoccument, sizeOfJsonDocument, 1, &alarmsArray);
										}
										else
										{
											sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"alarms\":null,");
											mbedtls_free(msg);
										}
									}
									else
										sprintf(&pJsonDoccument[strlen(pJsonDoccument)],"\"alarms\":null,");
									mbedtls_free(pAlarmsJson);
								}
								
								if(AWS_SUCCESS == rc)
								{
									rc = aws_shadow_footer_reported(pJsonDoccument, sizeOfJsonDocument);
									if(AWS_SUCCESS == rc)
									{
										rc = aws_iot_finalize_json_document(pJsonDoccument, sizeOfJsonDocument);
										if(AWS_SUCCESS == rc)
										{
											IOT_SHADOW_THING("Update Shadow: %s", pJsonDoccument);
											rc = aws_iot_shadow_update(&mqttClient, AWS_IOT_MY_THING_NAME,
																					pJsonDoccument,
																					ShadowUpdateStatusCallback,
																					NULL, 10, true);
										}
									}
								}
							}
					}
				}
				IOT_SHADOW_THING("*****************************************************************************************\n");
			}
			if(shadow_thing_fail_times >= SHADOW_MAX_RETRY_TIMES)
			{
				shadow_thing_fail_times = SHADOW_MAX_RETRY_TIMES-3;
				printf("shadow_thing_fail_times\r\n");
				shadow_thing_need_a_system_restart_cnt++;
				if(shadow_thing_need_a_system_restart_cnt >= 3)
				{
					printf("shadow_thing_need_a_system_restart_cnt\r\n");
					shadow_thing_need_a_system_restart_cnt = 0;
					//reset here
				}
				Ampm_GsmInternetSetDisconnect();
			}
		}
		if(AWS_SUCCESS != rc) {
			IOT_ERROR("An error occurred in the loop %d", rc);
		}

		IOT_SHADOW_THING("Disconnecting");
		rc = aws_iot_shadow_disconnect(&mqttClient);

		if(AWS_SUCCESS != rc) {
			IOT_ERROR("Disconnect error %d", rc);
		}
		
		mbedtls_memory_buffer_alloc_free( );
	}
	vTaskDelete(NULL);
}

void iot_shadow_thing_start(void)
{
	osThreadDef(shadow_thing, shadow_thing_thread, osPriorityNormal, 0, 8*1024);
	osThreadCreate(osThread(shadow_thing), NULL);
}

void Line1_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t) {
	//IOT_UNUSED(pJsonStruct_t);
	LINE_OBJ_TYPE *line = pJsonStruct_t->pData;
	char *buf;
	buf = mbedtls_calloc(1,valueLength + 1);
	memcpy(buf,pJsonValueBuffer,valueLength);
	buf[valueLength] = 0;
	if(strstr(buf,"\"status\":\"ON\""))
	{
		line->status = 1;
		relay_on(1);
	}
	else if(strstr(buf,"\"status\":\"OFF\""))
	{
		line->status = 0;
		relay_off(1);
	}
	IOT_SHADOW_THING("Received Delta Line1 message %.*s", valueLength, pJsonValueBuffer);
	mbedtls_free(buf);
	sendReportFlag = 1;
	osSemaphoreRelease(shadow_sem_id);
}

void Line2_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t) {
	//IOT_UNUSED(pJsonStruct_t);
	LINE_OBJ_TYPE *line = pJsonStruct_t->pData;
	char *buf;
	buf = mbedtls_calloc(1,valueLength + 1);
	memcpy(buf,pJsonValueBuffer,valueLength);
	buf[valueLength] = 0;
	if(strstr(buf,"\"status\":\"ON\""))
	{
		line->status = 1;
		relay_on(2);
	}
	else if(strstr(buf,"\"status\":\"OFF\""))
	{
		line->status = 0;
		relay_off(2);
	}
	IOT_SHADOW_THING("Received Delta Line2 message %.*s", valueLength, pJsonValueBuffer);
	mbedtls_free(buf);
	sendReportFlag = 1;
	osSemaphoreRelease(shadow_sem_id);
}

void Line3_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t) {
	//IOT_UNUSED(pJsonStruct_t);
	LINE_OBJ_TYPE *line = pJsonStruct_t->pData;
	char *buf;
	buf = mbedtls_calloc(1,valueLength + 1);
	memcpy(buf,pJsonValueBuffer,valueLength);
	buf[valueLength] = 0;
	if(strstr(buf,"\"status\":\"ON\""))
	{
		line->status = 1;
		relay_on(3);
	}
	else if(strstr(buf,"\"status\":\"OFF\""))
	{
		line->status = 0;
		relay_off(3);
	}
	IOT_SHADOW_THING("Received Delta Line3 message %.*s", valueLength, pJsonValueBuffer);
	mbedtls_free(buf);
	sendReportFlag = 1;
	osSemaphoreRelease(shadow_sem_id);
}

void Line4_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t) {
	//IOT_UNUSED(pJsonStruct_t);
	LINE_OBJ_TYPE *line = pJsonStruct_t->pData;
	char *buf;
	buf = mbedtls_calloc(1,valueLength + 1);
	memcpy(buf,pJsonValueBuffer,valueLength);
	buf[valueLength] = 0;
	if(strstr(buf,"\"status\":\"ON\""))
	{
		line->status = 1;
		relay_on(4);
	}
	else if(strstr(buf,"\"status\":\"OFF\""))
	{
		line->status = 0;
		relay_off(4);
	}
	IOT_SHADOW_THING("Received Delta Line4 message %.*s", valueLength, pJsonValueBuffer);
	mbedtls_free(buf);
	sendReportFlag = 1;
	osSemaphoreRelease(shadow_sem_id);
}


void Cmds_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t) {
	//IOT_UNUSED(pJsonStruct_t);
	CMD_OBJ_TYPE *cmds = pJsonStruct_t->pData;
	if(cmds->len < valueLength + 1)
	{
		mbedtls_free(cmds->cmds);
		cmds->cmds = mbedtls_calloc(1,valueLength + 1);
	}
	memcpy(cmds->cmds,pJsonValueBuffer,valueLength);
	cmds->cmds[valueLength] = 0;
	cmds->len = valueLength + 1;
	IOT_SHADOW_THING("Received Delta Cmds message %.*s", valueLength, pJsonValueBuffer);
	sendReportFlag = 1;
	osSemaphoreRelease(shadow_sem_id);
}

void Schedules_Status_DeltaCallback(const char *pJsonValueBuffer, uint32_t valueLength, jsonStruct_t *pJsonStruct_t) {
//	alarmParam_t *schedule = pJsonStruct_t->pData;
	
	osSemaphoreRelease(shadow_sem_id);
}

void Trigger_SendReportToServer(void)
{
	sendReportFlag = 1;
}



