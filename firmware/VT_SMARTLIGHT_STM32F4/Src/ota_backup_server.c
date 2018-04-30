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
#include "lwip/netdb.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "lib/sys_tick.h"
#include "ota_backup_server.h"
#include "lwip_net_socket.h"
#include "sst25.h"
/* Platform specific implementation header files */
#include "network_interface.h"
#include "timer_interface.h"
#include "app_config_task.h"



#define OTA_DBG(...) printf(__VA_ARGS__)

#define GPRS_PACKET_SIZE 			516

osThreadId ota_backup_thread_id;
static int ota_backup_socket = -1;
extern uint32_t smsCheckTime;
Timeout_Type tFrimwareUpdateTimeout;
uint8_t fSendBuff[64] __attribute__ ((__section__("osRam")));
Timeout_Type fdummySendPacket;
uint8_t ota_buf[560] __attribute__ ((__section__("osRam")));
typedef struct __attribute__((packed))
{
	uint8_t imei[18];
	uint8_t id[18];
	uint8_t ver[16];
}INFO_FIRMWARE_TYPE;


static uint8_t CheckSum(uint8_t *buff, uint32_t length)
{
	uint32_t i;
	uint8_t crc = 0;
	for(i = 0;i < length; i++)
	{
		crc += buff[i];
	}
	return crc;
}


void vOTA_Backup_Check(void)
{
	if(firmwareStatus != 0xFFFFFFFF
		&& (CheckTimeout(&tFrimwareUpdateTimeout) == SYSTICK_TIMEOUT)
	)
	{
		OTA_DBG("OTA_DBG: Firmware update timeout\r\n");
		firmwareStatus = 0xFFFFFFFF;
		firmwareFileOffSet = 0;
		fSendBuff[0] = 0;
		InitTimeout(&fdummySendPacket,SYSTICK_TIME_SEC(300));
	}
}


static void vOTA_Backup_Task(void const *param)
{
	int recbytes;
	int i;
	int tryCnt = 0;
	int ret;
	INFO_FIRMWARE_TYPE fInfo;
	uint8_t *u8pt;
	CFG_PROTOCOL_TYPE GPRS_ProtoRecv;
	CFG_PROTOCOL_TYPE GPRS_ProtoSend;
	PARSER_PACKET_TYPE GPRS_parserPacket;
	
	
	//InitTimeout(&tFrimwareUpdateTimeout,SYSTICK_TIME_SEC(300));
	
	//ota_buf = pvPortMalloc(560);
	while(1)
	{
		tryCnt++;
		vOTA_Backup_Check();
		while(!Ampm_GsmGPRS_IsWorking())
		{
			osDelay(1000);
		}
		if((ret = lwip_net_connect(&ota_backup_socket,"fw.m2m.vn","8092",LWIP_NET_PROTO_TCP)) != 0)
		{
			OTA_DBG("OTA_DBG:C > lwip_net_connect fail\n");
			switch(ret)
			{
				case LWIP_ERR_NET_SOCKET_FAILED:
					break;
				case LWIP_ERR_NET_UNKNOWN_HOST:
					break;
				case LWIP_ERR_NET_CONNECT_FAILED:
				default:
					break;
			}
			lwip_net_free(&ota_backup_socket);
			osDelay(10000);
			continue;
		}
		
		OTA_DBG("OTA_DBG:C > lwip_net_connect ok\n");
		OTA_DBG("OTA_DBG:C > send a login message\n");
		
		memcpy((char  *)fInfo.imei,(char  *)sysCfg.imei,18);
		memcpy((char  *)fInfo.id,(char  *)sysCfg.id,18);
		memcpy((char  *)fInfo.ver,FIRMWARE_VERSION,sizeof(FIRMWARE_VERSION));
		ota_buf[1] = sizeof(INFO_FIRMWARE_TYPE);
		ota_buf[0] = 0xCA;
		ota_buf[2] = 0;
		ota_buf[3] = 0x77;
		memcpy((char *)&ota_buf[4],(char  *)&fInfo,sizeof(INFO_FIRMWARE_TYPE));
		ota_buf[ota_buf[1] + 4] = CheckSum((uint8_t *)&fInfo,sizeof(INFO_FIRMWARE_TYPE));
		
		if (lwip_net_send(ota_backup_socket, (const unsigned char *)ota_buf, ota_buf[1] + 5) < 0) {
				lwip_net_free(&ota_backup_socket);
				OTA_DBG("OTA_DBG:C > send fail\n");
				osDelay(5000);
				continue;
		}
		if(firmwareStatus == 0xA5A5A5A5)
		{
			if((firmwareFileOffSet < firmwareFileSize) && (firmwareFileSize <= FIRMWARE_SIZE_MAX))
			{
				ota_buf[0] = 0xCA;
				ota_buf[1] = 4;
				ota_buf[2] = 0;
				ota_buf[3] = 0x12;
				u8pt = (uint8_t *)&firmwareFileOffSet;
				ota_buf[4] = u8pt[0];
				ota_buf[5] = u8pt[1];
				ota_buf[6] = u8pt[2];
				ota_buf[7] = u8pt[3];
				ota_buf[8] = CheckSum(&ota_buf[4],4);
			}
			if (lwip_net_send(ota_backup_socket, (const unsigned char *)ota_buf, ota_buf[1] + 5) < 0) {
				lwip_net_free(&ota_backup_socket);
				OTA_DBG("OTA_DBG:C > send fail\n");
				osDelay(5000);
				continue;
			}
		}
		while(1)
		{
			osDelay(10);
			vOTA_Backup_Check();
			recbytes = lwip_net_recv_timeout(ota_backup_socket , (unsigned char *)ota_buf, 560,3000);
			if(recbytes < 0 && recbytes != LWIP_ERR_SSL_TIMEOUT)
			{
				OTA_DBG("OTA_DBG:C > recv fail\n");
				osDelay(5000);
				break;
			}
			else if(recbytes > 0)
			{
				OTA_DBG("OTA_DBG:C > got %ld bytes\n",recbytes);
				GPRS_ProtoRecv.dataPt = ((uint8_t *)ota_buf);
				GPRS_parserPacket.lenMax = GPRS_PACKET_SIZE;
				GPRS_parserPacket.state = CFG_CMD_WAITING_SATRT_CODE;
				for(i = 0;i < recbytes;i++)
				{
					if(CfgParserPacket(&GPRS_parserPacket,&GPRS_ProtoRecv,((uint8_t *)ota_buf)[i]) == 0)
					{
						OTA_DBG("OTA_DBG: Parser Packet->OK\r\n");
						smsCheckTime = SysTick_Get();
						InitTimeout(&tFrimwareUpdateTimeout,SYSTICK_TIME_SEC(300));
						break;
					}

				}
				for(;i < recbytes;i++) 
				{
					((uint8_t *)ota_buf)[i] = 0;
				}
				CfgProcessData(&GPRS_ProtoRecv,&GPRS_ProtoSend,((uint8_t *)ota_buf),GPRS_parserPacket.lenMax - 4,0);
				
				if(GPRS_ProtoSend.length)
				{
					OTA_DBG("OTA_DBG: Send %d bytes to server\r\n",GPRS_ProtoSend.length);
					fSendBuff[0] = GPRS_ProtoSend.start;
					fSendBuff[1] = ((uint8_t *)&GPRS_ProtoSend.length)[0];
					fSendBuff[2] = ((uint8_t *)&GPRS_ProtoSend.length)[1];
					fSendBuff[3] = GPRS_ProtoSend.opcode;
					for(i = 0;i < GPRS_ProtoSend.length;i++)
						fSendBuff[i+4] = GPRS_ProtoSend.dataPt[i];
					fSendBuff[i+4] = GPRS_ProtoSend.crc;
					GPRS_ProtoSend.length = 0;
					InitTimeout(&tFrimwareUpdateTimeout,SYSTICK_TIME_SEC(300));
					
					if (lwip_net_send(ota_backup_socket, (const unsigned char *)fSendBuff, fSendBuff[1] + 5) < 0) {
						lwip_net_free(&ota_backup_socket);
						OTA_DBG("OTA_DBG:C > send fail\n");
						osDelay(3000);
						break;
					}
				}
				InitTimeout(&fdummySendPacket,SYSTICK_TIME_SEC(10));
				
			}
			else if(recbytes == LWIP_ERR_SSL_TIMEOUT)
			{
				//OTA_DBG("OTA_DBG:C > recv timeout\n");
			}
			
			if(fSendBuff[0] == 0xCA)
			{
				if((CheckTimeout(&fdummySendPacket) == SYSTICK_TIMEOUT))
				{
					OTA_DBG("OTA_DBG:C > send ota message interval\n");
					if (lwip_net_send(ota_backup_socket, (const unsigned char *)fSendBuff, fSendBuff[1] + 5) < 0) {
						lwip_net_free(&ota_backup_socket);
						OTA_DBG("OTA_DBG:C > send fail\n");
						osDelay(3000);
						break;
					}
					InitTimeout(&fdummySendPacket,SYSTICK_TIME_SEC(10));
				}
			}
			else if(CheckTimeout(&fdummySendPacket) == SYSTICK_TIMEOUT)
			{
				InitTimeout(&fdummySendPacket,SYSTICK_TIME_SEC(300)); //each 5 min
				OTA_DBG("OTA_DBG:C > send a login message\n");
				memcpy((char  *)fInfo.imei,(char  *)sysCfg.imei,18);
				memcpy((char  *)fInfo.id,(char  *)sysCfg.id,18);
				memcpy((char  *)fInfo.ver,FIRMWARE_VERSION,sizeof(FIRMWARE_VERSION));
				ota_buf[1] = sizeof(INFO_FIRMWARE_TYPE);
				ota_buf[0] = 0xCA;
				ota_buf[2] = 0;
				ota_buf[3] = 0x77;
				memcpy((char *)&ota_buf[4],(char  *)&fInfo,sizeof(INFO_FIRMWARE_TYPE));
				ota_buf[ota_buf[1] + 4] = CheckSum((uint8_t *)&fInfo,sizeof(INFO_FIRMWARE_TYPE));
				
				
				if (lwip_net_send(ota_backup_socket, (const unsigned char *)ota_buf, ota_buf[1] + 5) < 0) {
						lwip_net_free(&ota_backup_socket);
						OTA_DBG("OTA_DBG:C > send fail\n");
						osDelay(5000);
						continue;
				}
			}
			
		}
		lwip_net_free(&ota_backup_socket);
	}
	//vPortFree(ota_buf); ota_buf = NULL;
  ota_backup_thread_id = NULL;
	vTaskDelete( NULL );
}


void ota_backup_stop(void)
{
	OTA_DBG("OTA_DBG:C > ota_stop\n");
	if(ota_backup_thread_id)
	{
//		if(ota_buf)
//		{
//			vPortFree(ota_buf); ota_buf = NULL;
//		}
		OTA_DBG("OTA_DBG:C > ota kill task\n");
		osThreadTerminate(ota_backup_thread_id);
		if(ota_backup_socket > 0)
		{
			OTA_DBG("OTA_DBG:C > W kill socket %d\n",ota_backup_socket);
			lwip_net_free(&ota_backup_socket);
		}
	}
}

void ota_backup_start(void)
{
	ota_backup_stop();
	osThreadDef(ota_task, vOTA_Backup_Task, osPriorityNormal, 0, 1024);
	ota_backup_thread_id = osThreadCreate(osThread(ota_task), NULL);
}


