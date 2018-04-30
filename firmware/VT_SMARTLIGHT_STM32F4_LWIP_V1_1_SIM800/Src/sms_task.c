
#include <string.h>
#include <ctype.h>
#include "lib/sparser.h"
#include "lib/sys_tick.h"
#include "lib/sys_time.h"
#include "lib/ampm_list.h"
#include "lib/encoding.h"
#include "ampm_gsm_sms.h"
#include "ampm_gsm_call.h"
#include "ampm_gsm_main_task.h"
#include "sms_task.h"
#include "ampm_gsm_sms_api.h"
#include "system_config.h"
#include "alarm.h"
#include "relay.h"
#define SMS_DBG(...) printf(__VA_ARGS__)
#define smsScanf	sscanf
uint8_t inCallFlag = 0;
void process_sms(char *sender, char *sms,char *smsSendBuff,uint16_t smsLenBuf,uint16_t *dataOutLen) ;

const uint8_t smsReplyOk[] = "Cấu hình thành công";
uint8_t incomingCallProcessTimeout = 0;
uint8_t smsSendRelaySetByCallFlag = 0;
extern void Trigger_SendParamToServer(void);
char imcommingPhoneNo[16];
int8_t smsSendingCallIndex = 0;

uint32_t power_good_low_cnt = 0;
uint32_t power_good_high_cnt = 0;

void all_sms_init(void)
{
	
}

void io_process_action_task(void)
{
		uint8_t sms_msg[160];
//	if(incomingCall.state == CALL_INCOMING)
//	{
//		if(ampm_GotIncomingNumberFlag)
//		{
//			SMS_DBG("Incoming Call Number:%s\r\n",ampm_IncomingCallPhoneNo);
//			if(inCallFlag == 0)
//			{
//				inCallFlag  = 1;
//			}
//			Ampm_VoiceCallSetAction(&incomingCall,CALL_HANGUP_NOW);
//		}
//   }
//   else if(inCallFlag)
//   {
//	   inCallFlag = 0;
//   }
	
	if(HAL_GPIO_ReadPin(P_GOOD_GPIO_Port,P_GOOD_Pin) == GPIO_PIN_SET)
	{
		power_good_high_cnt++;
		power_good_low_cnt = 0;
		if(power_good_high_cnt == 10 && ((deviceIoStatus.systemStatus & PGOOD_STATUS) == 0))//10s
		{
			deviceIoStatus.systemStatus |= PGOOD_STATUS;
			OS_DeviceIoStatus_Save();
			RTC_UpdateNewTime();
			if (variableNoInit.lineStatus & (1<<0)) {
				sprintf((char *)sms_msg, "%s%02d:%02d:%02d - %02d/%02d/%02d",
						DEVICE_AC_LINE_OUT_RES_ON_HP, sysTime.hour, sysTime.min,
						sysTime.sec,sysTime.mday,
						sysTime.month,sysTime.year-2000);
			} else {
				sprintf((char *)sms_msg, "%s%02d:%02d:%02d - %02d/%02d/%02d",
						DEVICE_AC_LINE_OUT_RES_OFF_HP, sysTime.hour, sysTime.min,
						sysTime.sec,sysTime.mday,
						sysTime.month,sysTime.year-2000);
			}
			
			if(strlen(sysCfg.bigBoss) >= 6 && strlen(sysCfg.bigBoss) <= 12)
				AmpmGsm_SendSmsText(sysCfg.bigBoss,(char *)sms_msg);
			for(uint8_t i = 0; i < MAX_OWNER - 1;i++)
			{
				if(sysCfg.ownerNumber[i][0]
					&& (strlen(sysCfg.ownerNumber[i]) > 6)
					&& (strlen(sysCfg.ownerNumber[i]) <= 12)
				)
				{
					AmpmGsm_SendSmsText(sysCfg.ownerNumber[i],(char *)sms_msg);
				}
			}
			
		}
	}
	else
	{
		power_good_high_cnt = 0;
		power_good_low_cnt++;
		if(power_good_low_cnt == 10 && (deviceIoStatus.systemStatus & PGOOD_STATUS))//10s
		{
			deviceIoStatus.systemStatus &= ~PGOOD_STATUS;
			OS_DeviceIoStatus_Save();
			RTC_UpdateNewTime();
			if (variableNoInit.lineStatus & (1<<0)) {
				sprintf((char *)sms_msg, "%s%02d:%02d:%02d - %02d/%02d/%02d",
						DEVICE_AC_LINE_OK_RES_ON_HP, sysTime.hour, sysTime.min,
						sysTime.sec,sysTime.mday,
						sysTime.month,sysTime.year-2000);
			} else {
				sprintf((char *)sms_msg, "%s%02d:%02d:%02d - %02d/%02d/%02d",
						DEVICE_AC_LINE_OK_RES_OFF_HP, sysTime.hour, sysTime.min,
						sysTime.sec,sysTime.mday,
						sysTime.month,sysTime.year-2000);
			}
			
			if(strlen(sysCfg.bigBoss) >= 6 && strlen(sysCfg.bigBoss) <= 12)
				AmpmGsm_SendSmsText(sysCfg.bigBoss,(char *)sms_msg);
			for(uint8_t i = 0; i < MAX_OWNER - 1;i++)
			{
				if(sysCfg.ownerNumber[i][0]
					&& (strlen(sysCfg.ownerNumber[i]) > 6)
					&& (strlen(sysCfg.ownerNumber[i]) <= 12)
				)
				{
					AmpmGsm_SendSmsText(sysCfg.ownerNumber[i],(char *)sms_msg);
				}
			}
			
		}
	}
	 if(incomingCallProcessTimeout)
	{
		incomingCallProcessTimeout--;
		if(incomingCallProcessTimeout == 1) inCallFlag = 0;
		SMS_DBG("incomingCallProcessTimeout:%d\r\n",incomingCallProcessTimeout);
	}
	else if(incomingCall.state == CALL_INCOMING)
	{
		if(ampm_GotIncomingNumberFlag)
		{
			SMS_DBG("Incoming Call Number:%s\r\n",ampm_IncomingCallPhoneNo);
			if(FindAllPhone((char *)ampm_IncomingCallPhoneNo))
			{
				if(inCallFlag == 0)
				{
					incomingCallProcessTimeout = 30;
					inCallFlag  = 1;
					smsSendRelaySetByCallFlag = 1;
					strcpy((char *)imcommingPhoneNo,(char *)ampm_IncomingCallPhoneNo);
					smsSendingCallIndex = 0;
					if (variableNoInit.lineStatus && (1<<0)) {
						sprintf((char *)sms_msg,DEV_BUMP_OFF_REPORT,ampm_IncomingCallPhoneNo);
						relay_off(1);
					} else {
						relay_on(1);
						sprintf((char *)sms_msg,DEV_DEVICE_ON_OK_REPORT,ampm_IncomingCallPhoneNo);
					}
					
					if(strlen(sysCfg.bigBoss) >= 6 && strlen(sysCfg.bigBoss) <= 12)
						AmpmGsm_SendSmsText(sysCfg.bigBoss,(char *)sms_msg);
					for(uint8_t i = 0; i < MAX_OWNER - 1;i++)
					{
						if(sysCfg.ownerNumber[i][0]
							&& (strlen(sysCfg.ownerNumber[i]) > 6)
							&& (strlen(sysCfg.ownerNumber[i]) <= 12)
						)
						{
							AmpmGsm_SendSmsText(sysCfg.ownerNumber[i],(char *)sms_msg);
						}
					}
				}
			}
			Ampm_VoiceCallSetAction(&incomingCall,CALL_HANGUP_NOW);
		}
   }
   if(incomingCall.state == CALL_ENDED && inCallFlag)
   {
   	   incomingCallProcessTimeout = 5;
	   inCallFlag = 0;
   }

	

}

void SendReportSmsToUser(void)
{
		char *sms_res = pvPortMalloc(256);
		if(sms_res && (deviceCfg.features & DEFAULT_FEATURE_SMS_WARNING_REPORT))
		{
			memset(sms_res,0,256);
			//no_curent
			if(variableNoInit.deviceStatus & 
				(LINE1_NO_CURRENT_WARNING | LINE2_NO_CURRENT_WARNING | LINE3_NO_CURRENT_WARNING | LINE4_NO_CURRENT_WARNING)
			)
			{
				sprintf(&sms_res[strlen(sms_res)],"MAT DONG,");
				if(variableNoInit.deviceStatus & LINE1_NO_CURRENT_WARNING)
				{
					strcat(sms_res, "O1,");
				}
				if(variableNoInit.deviceStatus & LINE2_NO_CURRENT_WARNING)
				{
					strcat(sms_res, "O2,");
				}
				if(variableNoInit.deviceStatus & LINE3_NO_CURRENT_WARNING)
				{
					strcat(sms_res, "O3,");
				}
				if(variableNoInit.deviceStatus & LINE4_NO_CURRENT_WARNING)
				{
					strcat(sms_res, "O4,");
				}
				sprintf(&sms_res[strlen(sms_res) - 1],"\n");
			}
			//leakage_curent
			if(variableNoInit.deviceStatus & 
				(LINE1_LEAKAGE_CURRENT_WARNING | LINE2_LEAKAGE_CURRENT_WARNING | LINE3_LEAKAGE_CURRENT_WARNING | LINE4_LEAKAGE_CURRENT_WARNING)
			)
			{
				sprintf(&sms_res[strlen(sms_res)],"DONG RO,");
				if(variableNoInit.deviceStatus & LINE1_LEAKAGE_CURRENT_WARNING)
				{
					strcat(sms_res, "O1,");
				}
				if(variableNoInit.deviceStatus & LINE2_LEAKAGE_CURRENT_WARNING)
				{
					strcat(sms_res, "O2,");
				}
				if(variableNoInit.deviceStatus & LINE3_LEAKAGE_CURRENT_WARNING)
				{
					strcat(sms_res, "O3,");
				}
				if(variableNoInit.deviceStatus & LINE4_LEAKAGE_CURRENT_WARNING)
				{
					strcat(sms_res, "O4,");
				}
				sprintf(&sms_res[strlen(sms_res) - 1],"\n");
			}
			
			//low_curent
			if(variableNoInit.deviceStatus & 
				(LINE1_LOW_CURRENT_WARNING | LINE2_LOW_CURRENT_WARNING | LINE3_LOW_CURRENT_WARNING | LINE4_LOW_CURRENT_WARNING)
			)
			{
				sprintf(&sms_res[strlen(sms_res)],"DONG THAP,");
				if(variableNoInit.deviceStatus & LINE1_LOW_CURRENT_WARNING)
				{
					strcat(sms_res, "O1,");
				}
				if(variableNoInit.deviceStatus & LINE2_LOW_CURRENT_WARNING)
				{
					strcat(sms_res, "O2,");
				}
				if(variableNoInit.deviceStatus & LINE3_LOW_CURRENT_WARNING)
				{
					strcat(sms_res, "O3,");
				}
				if(variableNoInit.deviceStatus & LINE4_LOW_CURRENT_WARNING)
				{
					strcat(sms_res, "O4,");
				}
				sprintf(&sms_res[strlen(sms_res) - 1],"\n");
			}
			
			//high_curent
			if(variableNoInit.deviceStatus & 
				(LINE1_HIGH_CURRENT_WARNING | LINE2_HIGH_CURRENT_WARNING | LINE3_HIGH_CURRENT_WARNING | LINE4_HIGH_CURRENT_WARNING)
			)
			{
				sprintf(&sms_res[strlen(sms_res)],"DONG CAO,");
				if(variableNoInit.deviceStatus & LINE1_HIGH_CURRENT_WARNING)
				{
					strcat(sms_res, "O1,");
				}
				if(variableNoInit.deviceStatus & LINE2_HIGH_CURRENT_WARNING)
				{
					strcat(sms_res, "O2,");
				}
				if(variableNoInit.deviceStatus & LINE3_HIGH_CURRENT_WARNING)
				{
					strcat(sms_res, "O3,");
				}
				if(variableNoInit.deviceStatus & LINE4_HIGH_CURRENT_WARNING)
				{
					strcat(sms_res, "O4,");
				}
				sprintf(&sms_res[strlen(sms_res) - 1],"\n");
			}
			//low_voltage
			if(variableNoInit.deviceStatus & 
				(LINE1_LOW_VOLTAGE_WARNING | LINE2_LOW_VOLTAGE_WARNING | LINE3_LOW_VOLTAGE_WARNING | LINE4_LOW_VOLTAGE_WARNING)
			)
			{
				sprintf(&sms_res[strlen(sms_res)],"AP THAP,");
				if(variableNoInit.deviceStatus & LINE1_LOW_VOLTAGE_WARNING)
				{
					strcat(sms_res, "O1,");
				}
				if(variableNoInit.deviceStatus & LINE2_LOW_VOLTAGE_WARNING)
				{
					strcat(sms_res, "O2,");
				}
				if(variableNoInit.deviceStatus & LINE3_LOW_VOLTAGE_WARNING)
				{
					strcat(sms_res, "O3,");
				}
				if(variableNoInit.deviceStatus & LINE4_LOW_VOLTAGE_WARNING)
				{
					strcat(sms_res, "O4,");
				}
				sprintf(&sms_res[strlen(sms_res) - 1],"\n");
			}
			//high_voltage
			if(variableNoInit.deviceStatus & 
				(LINE1_HIGH_VOLTAGE_WARNING | LINE2_HIGH_VOLTAGE_WARNING | LINE3_HIGH_VOLTAGE_WARNING | LINE4_HIGH_VOLTAGE_WARNING)
			)
			{
				sprintf(&sms_res[strlen(sms_res)],"AP CAO,");
				if(variableNoInit.deviceStatus & LINE1_HIGH_VOLTAGE_WARNING)
				{
					strcat(sms_res, "O1,");
				}
				if(variableNoInit.deviceStatus & LINE2_HIGH_VOLTAGE_WARNING)
				{
					strcat(sms_res, "O2,");
				}
				if(variableNoInit.deviceStatus & LINE3_HIGH_VOLTAGE_WARNING)
				{
					strcat(sms_res, "O3,");
				}
				if(variableNoInit.deviceStatus & LINE4_HIGH_VOLTAGE_WARNING)
				{
					strcat(sms_res, "O4,");
				}
				sprintf(&sms_res[strlen(sms_res) - 1],"\n");
			}
			if(strlen(sms_res) >= 5)
			{
				if(strlen(sysCfg.bigBoss) >= 6 && strlen(sysCfg.bigBoss) <= 12)
					AmpmGsm_SendSmsText((char *)sysCfg.bigBoss,sms_res);
				for(uint8_t index = 0;index < MAX_OWNER;index++)
				{
						if(strlen(sysCfg.ownerNumber[index]) >= 6 && strlen(sysCfg.ownerNumber[index]) <= 12)
							AmpmGsm_SendSmsText((char *)sysCfg.ownerNumber[index],sms_res);
				}
			}
		}
		if(sms_res)
			vPortFree(sms_res);
}

void process_sms(char *sender, char *sms,char *smsSendBuff,uint16_t smsLenBuf,uint16_t *dataOutLen) {
	
}


void Ampm_MainSmsRecvCallback(uint8_t *buf)
{
	uint16_t len,i,passOk = 0,sms_res_len = 0,flagCfgSave = 0;
	char *pt,*sms_in = (char *)buf;
	char tempBuff0[160] = {0};
	char tempBuff1[64] = {0};
	char tempBuff2[32] = {0};
	char *sms_res = pvPortMalloc(1024);
	if(sms_res == 0) return;
	for(i = 0; i < strlen(sms_in);)
	{
		if(sms_in[i] == ' ')
		{
			sms_in[i] = 0;
			strcat(sms_in,&sms_in[i+1]);
		}
		else i++;
		if(islower(sms_in[i]))
		 sms_in[i] = toupper(sms_in[i]);
	}
	
	pt = strstr(sms_in,(char *)"START");
	if(pt != NULL)
	{
		sscanf(pt,"START,%[^,],%[^,],%s",tempBuff0,tempBuff1,tempBuff2);
		pt = strstr(tempBuff0,sysCfg.smsPwd);
		if(pt != NULL)
		{
			strcpy(sysCfg.smsPwd,tempBuff1);
			strcpy((char *)sysCfg.bigBoss,(char *)smsSender);
			strcpy(sysCfg.myNumber,tempBuff2);
			sms_res_len += sprintf((char *)&sms_res[sms_res_len],DEV_AUTH_OK_REPORT);
			flagCfgSave = 1;
			passOk = 1;
			SMS_DBG(DEV_AUTH_OK_REPORT);
		}
		else
		{
			sms_res_len += sprintf((char *)&sms_res[sms_res_len],DEV_AUTH_ERR_REPORT);
			SMS_DBG(DEV_AUTH_ERR_REPORT);
		}
	}
	
	pt = strstr(sms_in,(char *)"THEM");
	if(pt != NULL)
	{
		sscanf(pt,"THEM,%[^,],%s",tempBuff0,tempBuff1);
		pt = strstr(tempBuff0,sysCfg.smsPwd);
		if(pt != NULL)
		{
			if(FindAllPhone(tempBuff1))
			{
				sms_res_len += sprintf((char *)&sms_res[sms_res_len],DEV_ADD_OWNER_EXIST_REPORT, tempBuff1);
				SMS_DBG(DEV_ADD_OWNER_EXIST_REPORT,tempBuff1);
			}
			else
			{
				for(i = 0; i < MAX_OWNER - 1;i++)
				{
					strcpy(sysCfg.ownerNumber[i],sysCfg.ownerNumber[i + 1]);
				}
				strcpy(sysCfg.ownerNumber[MAX_OWNER - 1],tempBuff1);
				flagCfgSave = 1;
				sms_res_len += sprintf(&sms_res[sms_res_len],DEV_ADD_OWNER_REPORT, tempBuff1);
				SMS_DBG(DEV_ADD_OWNER_REPORT,tempBuff1);
			}
		}
	}
	
	pt = strstr(sms_in,(char *)"IDCONGTO,");
	if(pt != NULL)
	{
		sscanf(pt,"IDCONGTO,%s",tempBuff0);
		if(strlen(tempBuff0) < 4)
		{
			strcpy((char *)deviceCfg.outstationNumber,tempBuff0);
			sms_res_len += sprintf(&sms_res[sms_res_len],"ID CONG TO=%s", tempBuff0);
			// compare with saved password here
			SMS_DBG("ID CONG TO=%s",tempBuff0);
			DeviceCfg_Save();
		}
	}
	
	
	pt = strstr(sms_in,(char *)"XOA");
	if(pt != NULL)
	{
		sscanf(pt,"XOA,%[^,],%s",tempBuff0,tempBuff1);
		pt = strstr(tempBuff0,(char *)sysCfg.smsPwd);
		if(pt != NULL)
		{
			if(FindAllPhone(tempBuff1) == NULL)
			{
				sms_res_len += sprintf((char *)&sms_res[sms_res_len],DEV_RM_OWNER_NOT_EXIST_REPORT, tempBuff1);
				SMS_DBG(DEV_RM_OWNER_NOT_EXIST_REPORT,tempBuff1);
			}
			else
			{
				for(i = 0; i < MAX_OWNER - 1;i++)
				{
					pt = strstr(sysCfg.ownerNumber[i],tempBuff1);
					if(pt != NULL)
					{
						sysCfg.ownerNumber[i][0] = 0;
					}
				}
				flagCfgSave = 1;
				sms_res_len += sprintf(&sms_res[sms_res_len],DEV_RM_OWNER_REPORT, tempBuff1);
				// compare with saved password here
				SMS_DBG(DEV_RM_OWNER_REPORT,tempBuff1);
			}
		}
	}
	
	if(FindAllPhone((char *)smsSender))
	{
		
		//check monney
		pt = strstr(sms_in,"*101#");
		if(pt != NULL)
		{
			uint8_t c;
			char *replySmsBuf = ampm_malloc(200);
			while(COMM_Getc(&c)==0);
			if(Ampm_SendCommand("OK",modemError,1000,3,"AT\r")== AMPM_GSM_RES_OK)
			{
				//if(Ampm_SendCommand("OK",modemError,1000,3,"AT+CSCS=\"UCS2\"\r")== AMPM_GSM_RES_OK)
				{
					Ampm_SendCommand("OK",modemError,1000,3,"AT+CUSD=1\r");
					if(Ampm_SendCommand("+CUSD: ",modemError,30000,1,"ATD*101#;\r") == AMPM_GSM_RES_OK)
					{
						osDelay(1500);
						replySmsBuf[0] = 0;
						while(COMM_Getc(&c)==0)
						{
							if(c == ',')
							{
								break;
							}
						}
						while(COMM_Getc(&c)==0)
						{
							if(c == '"')
							{
								break;
							}
						}
						//SMS_DBG("Get Data\n");
						i  = 0;
						while(COMM_Getc(&c)==0)
						{
							if(c == '"')
								break;
							//SMS_DBG("%c",c);
							replySmsBuf[i] = c;
							i++;
							if(i >= 160)
								break;
						}
						replySmsBuf[i] = 0;
						sms_res_len += sprintf(&sms_res[sms_res_len],"%s",replySmsBuf);
					}
				}
			}
		}
		
		pt = strstr(sms_in,"*102#");
		if(pt != NULL)
		{
			uint8_t c;
			char *replySmsBuf = ampm_malloc(200);
			while(COMM_Getc(&c)==0);
			if(Ampm_SendCommand("OK",modemError,1000,3,"AT\r")== AMPM_GSM_RES_OK)
			{
				//if(Ampm_SendCommand("OK",modemError,1000,3,"AT+CSCS=\"UCS2\"\r")== AMPM_GSM_RES_OK)
				{
					Ampm_SendCommand("OK",modemError,1000,3,"AT+CUSD=1\r");
					if(Ampm_SendCommand("+CUSD: ",modemError,30000,1,"ATD*102#;\r") == AMPM_GSM_RES_OK)
					{
						osDelay(1500);
						replySmsBuf[0] = 0;
						while(COMM_Getc(&c)==0)
						{
							if(c == ',')
							{
								break;
							}
						}
						while(COMM_Getc(&c)==0)
						{
							if(c == '"')
							{
								break;
							}
						}
						//SMS_DBG("Get Data\n");
						i  = 0;
						while(COMM_Getc(&c)==0)
						{
							if(c == '"')
								break;
							//SMS_DBG("%c",c);
							replySmsBuf[i] = c;
							i++;
							if(i >= 160)
								break;
						}
						replySmsBuf[i] = 0;
						sms_res_len += sprintf(&sms_res[sms_res_len],"%s",replySmsBuf);
					}
				}
			}
		}
		
		pt = strstr(sms_in,(char *)"TANSUAT,");
		if(pt != NULL)
		{
			uint32_t interval;
			sscanf(pt,"TANSUAT,%d",&interval);
			sysCfg.reportInterval = interval;
			sms_res_len += sprintf(&sms_res[sms_res_len],DEV_INTERVAL_REPORT,sysCfg.reportInterval);
			SMS_DBG(DEV_INTERVAL_REPORT,sysCfg.reportInterval);
			flagCfgSave  = 1;
		}
		//highVoltageLevel
		pt = strstr(sms_in,(char *)"MUCAPCAO,");
		if(pt != NULL)
		{
			float value;
			sscanf(pt,"MUCAPCAO,%f",&value);
			deviceCfg.highVoltageLevel = value;
			sms_res_len += sprintf(&sms_res[sms_res_len],DEV_HIGH_VOLTAGE_LEVEL_REPORT,value);
			SMS_DBG(DEV_HIGH_VOLTAGE_LEVEL_REPORT,value);
			DeviceCfg_Save();
		}
		
		pt = strstr(sms_in,(char *)"MUCAPTHAP,");
		if(pt != NULL)
		{
			float value;
			sscanf(pt,"MUCAPTHAP,%f",&value);
			deviceCfg.lowVoltageLevel = value;
			sms_res_len += sprintf(&sms_res[sms_res_len],DEV_LOW_VOLTAGE_LEVEL_REPORT,value);
			SMS_DBG(DEV_LOW_VOLTAGE_LEVEL_REPORT,value);
			DeviceCfg_Save();
		}
		
		pt = strstr(sms_in,(char *)"MUCDONGCAO,");
		if(pt != NULL)
		{
			float value;
			sscanf(pt,"MUCDONGCAO,%f",&value);
			deviceCfg.highCurrentLevel = value;
			sms_res_len += sprintf(&sms_res[sms_res_len],DEV_HIGH_CURRENT_LEVEL_REPORT,value);
			SMS_DBG(DEV_HIGH_CURRENT_LEVEL_REPORT,value);
			DeviceCfg_Save();
		}
		
		pt = strstr(sms_in,(char *)"MUCAPTHAP,");
		if(pt != NULL)
		{
			float value;
			sscanf(pt,"MUCAPTHAP,%f",&value);
			deviceCfg.lowCurrentLevel = value;
			sms_res_len += sprintf(&sms_res[sms_res_len],DEV_LOW_CURRENT_LEVEL_REPORT,value);
			SMS_DBG(DEV_LOW_CURRENT_LEVEL_REPORT,value);
			DeviceCfg_Save();
		}
		
		pt = strstr(sms_in,(char *)"DONGRO,");
		if(pt != NULL)
		{
			uint32_t value;
			sscanf(pt,"DONGRO,%d",&value);
			deviceCfg.leakageCurentLevel = value;
			sms_res_len += sprintf(&sms_res[sms_res_len],DEV_CURRENT_LEAKAGE_LEVEL_REPORT,value);
			SMS_DBG(DEV_CURRENT_LEAKAGE_LEVEL_REPORT,value);
			DeviceCfg_Save();
		}
		
		
		
		pt = strstr(sms_in,(char *)"REBOOT");
		if(pt != NULL)
		{
			ResetMcuSet(MCU_RESET_AFTER_30_SEC);
			sms_res_len += sprintf(&sms_res[sms_res_len],DEV_RESET_REPORT);
			SMS_DBG(DEV_RESET_REPORT);
		}
		pt = strstr(sms_in,(char *)"RESET");
		if(pt != NULL)
		{
			ResetMcuSet(MCU_RESET_AFTER_30_SEC);
			sms_res_len += sprintf(&sms_res[sms_res_len],DEV_RESET_REPORT);
			SMS_DBG(DEV_RESET_REPORT);
		}

		pt = strstr(sms_in,(char *)"KHOIDONGLAI");
		if(pt != NULL)
		{
			ResetMcuSet(MCU_RESET_AFTER_30_SEC);
			sms_res_len += sprintf(&sms_res[sms_res_len],DEV_RESET_REPORT);
			SMS_DBG(DEV_RESET_REPORT);
		}
		//BAT CANH BAO
		pt = strstr(sms_in,(char *)"BATCANHBAOMATNGUON");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_NO_POWER_ALARM;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE2_NO_POWER_ALARM;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE3_NO_POWER_ALARM;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE4_NO_POWER_ALARM;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"BATCANHBAOMATDONG");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_NO_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE2_NO_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE3_NO_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE4_NO_CURRENT_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"BATCANHBAODONGRO");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_LEAKAGE_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE2_LEAKAGE_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE3_LEAKAGE_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE4_LEAKAGE_CURRENT_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"BATCANHBAODONGTHAP");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"BATCANHBAODONGCAO");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"BATCANHBAOAPCAO");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"BATCANHBAOAPTHAP");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features |= DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		//TAT CANH BAO
		pt = strstr(sms_in,(char *)"TATCANHBAOMATNGUON");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_NO_POWER_ALARM;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE2_NO_POWER_ALARM;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE3_NO_POWER_ALARM;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE4_NO_POWER_ALARM;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"TATCANHBAOMATDONG");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_NO_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE2_NO_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE3_NO_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE4_NO_CURRENT_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"TATCANHBAODONGRO");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_LEAKAGE_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE2_LEAKAGE_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE3_LEAKAGE_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE4_LEAKAGE_CURRENT_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		if((deviceCfg.features & DEFAULT_FEATURE_1_RELAY) == 0)
		{
			pt = strstr(sms_in,(char *)"BAT,");
			if(pt != NULL)
			{
				if(strstr(pt,(char *)"LINE1"))
					relay_on(1);
				if(strstr(pt,(char *)"LINE2"))
					relay_on(2);
				if(strstr(pt,(char *)"LINE3"))
					relay_on(3);
				if(strstr(pt,(char *)"LINE4"))
					relay_on(4);
				sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
				SMS_DBG("THANH CONG!");
			}
		}
		else
		{
			if(memcmp(sms_in,(char *)"BAT",3) == NULL)
			{
				relay_on(1);
				sprintf((char *)tempBuff0,DEV_DEVICE_ON_OK_REPORT,smsSender);
				if(strlen(sysCfg.bigBoss) >= 6 && strlen(sysCfg.bigBoss) <= 12)
					AmpmGsm_SendSmsText(sysCfg.bigBoss,tempBuff0);
				for(i = 0; i < MAX_OWNER - 1;i++)
				{
					if(sysCfg.ownerNumber[i][0]
						&& (strlen(sysCfg.ownerNumber[i]) > 6)
						&& (strlen(sysCfg.ownerNumber[i]) <= 12)
					)
					{
						AmpmGsm_SendSmsText(sysCfg.ownerNumber[i],tempBuff0);
					}
				}
			}
		}
		
		
		if((deviceCfg.features & DEFAULT_FEATURE_1_RELAY) == 0)
		{
			pt = strstr(sms_in,(char *)"TAT,");
			if(pt != NULL)
			{
				if(strstr(pt,(char *)"LINE1"))
					relay_off(1);
				if(strstr(pt,(char *)"LINE2"))
					relay_off(2);
				if(strstr(pt,(char *)"LINE3"))
					relay_off(3);
				if(strstr(pt,(char *)"LINE4"))
					relay_off(4);
				sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
				SMS_DBG("THANH CONG!");
			}
		}
		else
		{
			if(memcmp(sms_in,(char *)"TAT",3) == NULL)
			{
				relay_off(1);
				sprintf((char *)tempBuff0,DEV_BUMP_OFF_REPORT,smsSender);
				if(strlen(sysCfg.bigBoss) >= 6 && strlen(sysCfg.bigBoss) <= 12)
					AmpmGsm_SendSmsText(sysCfg.bigBoss,tempBuff0);
				for(i = 0; i < MAX_OWNER - 1;i++)
				{
					if(sysCfg.ownerNumber[i][0]
						 && (strlen(sysCfg.ownerNumber[i]) > 6)
					)
					{
						AmpmGsm_SendSmsText(sysCfg.ownerNumber[i],tempBuff0);
					}
				}
			}
		}
		
		pt = strstr(sms_in,(char *)"*TRANGTHAI#");
		if(pt != NULL)
		{
			if((deviceCfg.features & DEFAULT_FEATURE_1_RELAY) == 0)
			{
				for(i = 0;i < 4;i++)
				{
					if(variableNoInit.lineStatus & (1<<i))
					{
						if(variableNoInit.lineStatus & (1<<i))
							sms_res_len += sprintf(&sms_res[sms_res_len],"O%d BAT\n",i+1);
						else
							sms_res_len += sprintf(&sms_res[sms_res_len],"O%d TAT\n",i+1);
					}
				}
			}
			else
			{
				if(variableNoInit.lineStatus & 1)
				{
					sms_res_len += sprintf(&sms_res[sms_res_len],DEV_GET_RELAY_STATUS_RES,"Bat");
				}
				else
				{
					sms_res_len += sprintf(&sms_res[sms_res_len],DEV_GET_RELAY_STATUS_RES,"Tat");
				}
			}
		}
		
		pt = strstr(sms_in,(char *)"*NGAYGIO#");
		if(pt != NULL)
		{
			RTC_UpdateNewTime();
			sms_res_len += sprintf(&sms_res[sms_res_len], "%02d gio %02d phut, %02d/%02d/%02d", sysTime.hour, sysTime.min, sysTime.mday, sysTime.month, sysTime.year - 2000);
		}
		
		pt = strstr(sms_in,(char *)"TATCANHBAODONGTHAP");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"TATCANHBAODONGCAO");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"TATCANHBAOAPCAO");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"TATCANHBAOAPTHAP");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"LINE1"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE2"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE3"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING;
			if(strstr(pt,(char *)"LINE4"))
				deviceCfg.features &= ~DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"TATTATCACANHBAO");
		if(pt != NULL)
		{
			deviceCfg.features = 0;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		//BAT SMS
		pt = strstr(sms_in,(char *)"BATSMS");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"HENGIO"))
				deviceCfg.features |= DEFAULT_FEATURE_SMS_SCHEDULE_REPORT;
			if(strstr(pt,(char *)"CANHBAO"))
				deviceCfg.features |= DEFAULT_FEATURE_SMS_WARNING_REPORT;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"TATSMS");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"HENGIO"))
				deviceCfg.features &= ~DEFAULT_FEATURE_SMS_SCHEDULE_REPORT;
			if(strstr(pt,(char *)"CANHBAO"))
				deviceCfg.features &= ~DEFAULT_FEATURE_SMS_WARNING_REPORT;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"*HENGIO#");
		if(pt != NULL)
		{
			uint8_t i;
			//HEN GIO,10:00,6,0,ON:O1.O3,OFF:O2.O4,NAM:2018-2019,THANG:1-12,NGAY:1-31,THU:T2T3T4T5T6T7CN
			for(i = 0;i < DEVICE_SCHEDULE_MAX; i++)
			{
				if(deviceCfg.schedules[i].hour != 0xff
				|| deviceCfg.schedules[i].min != 0xff)
				{
					sms_res_len += sprintf(&sms_res[sms_res_len],"%s",ALARM_INFO);
					break;
				}
			}
			if(i >= DEVICE_SCHEDULE_MAX)
			{
				sms_res_len += sprintf(&sms_res[sms_res_len],"%s",NO_ALARM_INFO);
			}
			else
			{
				for(i = 0;i < DEVICE_SCHEDULE_MAX; i++)
				{
					if(deviceCfg.schedules[i].hour != 0xff
					|| deviceCfg.schedules[i].min != 0xff)
					{
						sms_res_len += sprintf(&sms_res[sms_res_len],"%d-",i+1);
						if((deviceCfg.features & DEFAULT_FEATURE_1_RELAY))
						{

							if(deviceCfg.schedules[i].fromMday != 0xFF
								&& deviceCfg.schedules[i].toMday != 0xFF
								&& deviceCfg.schedules[i].fromMonth != 0xFF
								&& deviceCfg.schedules[i].toMonth != 0xFF
								&& deviceCfg.schedules[i].fromYear != 0xFF
								&& deviceCfg.schedules[i].toYear != 0xFF
							)
							{
								if(deviceCfg.schedules[i].valueOn & 0x01)
									sms_res_len += sprintf(&sms_res[sms_res_len],"Bat 1 lan %02d:%02d",deviceCfg.schedules[i].hour,deviceCfg.schedules[i].min);
								else
									sms_res_len += sprintf(&sms_res[sms_res_len],"Tat 1 lan %02d:%02d",deviceCfg.schedules[i].hour,deviceCfg.schedules[i].min);
							}
							else
							{
								if(deviceCfg.schedules[i].valueOn & 0x01)
									sms_res_len += sprintf(&sms_res[sms_res_len],"Bat hang ngay %02d:%02d",deviceCfg.schedules[i].hour,deviceCfg.schedules[i].min);
								else
									sms_res_len += sprintf(&sms_res[sms_res_len],"Tat hang ngay %02d:%02d",deviceCfg.schedules[i].hour,deviceCfg.schedules[i].min);
							}
						}
						else
						{
							//TIME
							sms_res_len += sprintf(&sms_res[sms_res_len],"%d,0,%02d:%02d",i,deviceCfg.schedules[i].hour,deviceCfg.schedules[i].min);
								
							//ON
							if(deviceCfg.schedules[i].valueOn )
								sms_res_len += sprintf(&sms_res[sms_res_len],",ON:");
							for(uint8_t j = 0;j < 4; j++)
							{
								if(deviceCfg.schedules[i].valueOn & (1 << j))
								{
									sms_res_len += sprintf(&sms_res[sms_res_len],"O%d",j+1);
								}
							}
							//OFF
							if(deviceCfg.schedules[i].valueOff )
								sms_res_len += sprintf(&sms_res[sms_res_len],",OFF:");
							for(uint8_t j = 0;j < 4; j++)
							{
								if(deviceCfg.schedules[i].valueOff & (1 << j))
								{
									sms_res_len += sprintf(&sms_res[sms_res_len],"O%d",j+1);
								}
							}
							//NAM
							if(deviceCfg.schedules[i].fromYear != 0xff && deviceCfg.schedules[i].toYear != 0xff)
							{
								sms_res_len += sprintf(&sms_res[sms_res_len],",N:%04d-%04d",2000 + (uint16_t)deviceCfg.schedules[i].fromYear,2000 + (uint16_t)deviceCfg.schedules[i].toYear);
							}
							
							//THANG
							if(deviceCfg.schedules[i].fromMonth != 0xff && deviceCfg.schedules[i].toMonth != 0xff)
							{
								sms_res_len += sprintf(&sms_res[sms_res_len],",T:%d-%d",deviceCfg.schedules[i].fromMonth,deviceCfg.schedules[i].toMonth);
							}
							//NGAY
							if(deviceCfg.schedules[i].fromMonth != 0xff && deviceCfg.schedules[i].toMonth != 0xff)
							{
								sms_res_len += sprintf(&sms_res[sms_res_len],",N:%d-%d",deviceCfg.schedules[i].fromMonth,deviceCfg.schedules[i].toMonth);
							}
							//WEEK
							if(deviceCfg.schedules[i].weekDay != 0xff)
							{
								sms_res_len += sprintf(&sms_res[sms_res_len],",U:");
								for(uint8_t bit = 0;bit < 8; bit++)
								{
									switch(deviceCfg.schedules[i].weekDay & (1<<bit))
									{
										case 1<<SCHEDULE_WEEKDAY_MONDAY:
											sms_res_len += sprintf(&sms_res[sms_res_len],"T2");
										break;
										case 1<<SCHEDULE_WEEKDAY_TUESDAY:
											sms_res_len += sprintf(&sms_res[sms_res_len],"T3");
										break;
										case 1<<SCHEDULE_WEEKDAY_WEDNESDAY:
											sms_res_len += sprintf(&sms_res[sms_res_len],"T4");
										break;
										case 1<<SCHEDULE_WEEKDAY_THURSDAY:
											sms_res_len += sprintf(&sms_res[sms_res_len],"T5");
										break;
										case 1<<SCHEDULE_WEEKDAY_FRIDAY:
											sms_res_len += sprintf(&sms_res[sms_res_len],"T6");
										break;
										case SCHEDULE_WEEKDAY_SATURDAY:
											sms_res_len += sprintf(&sms_res[sms_res_len],"T7");
										break;
										case 1<<SCHEDULE_WEEKDAY_SUNDAY:
											sms_res_len += sprintf(&sms_res[sms_res_len],"CN");
										break;
										default:
											break;
									}
								}
							}
						}
						sms_res_len += sprintf(&sms_res[sms_res_len],"\n");
					}
				}
			}
		}
		
		pt = strstr(sms_in,(char *)"XOAHENGIO,");
		if(pt != NULL)
		{
			uint32_t index;
			sscanf(pt,"XOAHENGIO,%d",&index);
			
			if(index == 0) //xoa tat ca hen gio
			{
				for(i = 0;i < DEVICE_SCHEDULE_MAX; i++)
				{
						deviceCfg.schedules[i].fromYear = 0xff;
						deviceCfg.schedules[i].toYear = 0xff;
						deviceCfg.schedules[i].fromMonth = 0xff;
						deviceCfg.schedules[i].toMonth = 0xff;
						deviceCfg.schedules[i].fromMday = 0xff;
						deviceCfg.schedules[i].toMday = 0xff;
						deviceCfg.schedules[i].weekDay = 0xff;
						deviceCfg.schedules[i].hour = 0xff;
						deviceCfg.schedules[i].min = 0xff;
						deviceCfg.schedules[i].type = 0;
						deviceCfg.schedules[i].valueOn = 0;
						deviceCfg.schedules[i].valueOff = 0;
				}
				DeviceCfg_Save();
				sms_res_len += sprintf(&sms_res[sms_res_len],DEV_DEL_ALARM_OK_REPORT);
				SMS_DBG(DEV_DEL_ALARM_OK_REPORT);
			}
			else if(index <= DEVICE_SCHEDULE_MAX) 
			{
				i = index -1;
				deviceCfg.schedules[i].fromYear = 0xff;
				deviceCfg.schedules[i].toYear = 0xff;
				deviceCfg.schedules[i].fromMonth = 0xff;
				deviceCfg.schedules[i].toMonth = 0xff;
				deviceCfg.schedules[i].fromMday = 0xff;
				deviceCfg.schedules[i].toMday = 0xff;
				deviceCfg.schedules[i].weekDay = 0xff;
				deviceCfg.schedules[i].hour = 0xff;
				deviceCfg.schedules[i].min = 0xff;
				deviceCfg.schedules[i].type = 0;
				deviceCfg.schedules[i].valueOn = 0;
				deviceCfg.schedules[i].valueOff = 0;
				sms_res_len += sprintf(&sms_res[sms_res_len],DEV_DEL_ALARM_OK_REPORT);
				SMS_DBG(DEV_DEL_ALARM_OK_REPORT);
			}
			else
			{
				sms_res_len += sprintf(&sms_res[sms_res_len],"Thiet bi ho tro toi da %d thoi diem",DEVICE_SCHEDULE_MAX);
				SMS_DBG("Thiet bi ho tro toi da %d thoi diem",DEVICE_SCHEDULE_MAX);
			}
		}
		else
		{
			//HEN GIO,10:00,1,0,O1.O3,O2.O4  = hen hang ngay
			//HEN GIO,10:00,3,0,O1.O3,O2.O4,THU:T2T3T4T5T6T7CN
			//HEN GIO,10:00,4,0,O1.O3,O2.O4,NGAY:1-31,THU:T2T3T4T5T6T7CN
			//HEN GIO,10:00,5,0,O1.O3,O2.O4,THANG:1-12,NGAY:1-31,THU:T2T3T4T5T6T7CN
			//HEN GIO,10:00,6,0,O1.O3,O2.O4,NAM:2018-2019,THANG:1-12,NGAY:1-31,THU:T2T3T4T5T6T7CN
			if((deviceCfg.features & DEFAULT_FEATURE_1_RELAY))
			{
				pt = strstr(sms_in,(char *)"HENGIO,");
				if(pt != NULL)
				{
					uint32_t index,outputType,hour,min,scheduleConfigOk = 0;
					alarmParam_t schedule;
					schedule.fromYear = 0xff;
					schedule.toYear = 0xff;
					schedule.fromMonth = 0xff;
					schedule.toMonth = 0xff;
					schedule.fromMday = 0xff;
					schedule.toMday = 0xff;
					schedule.weekDay = 0xff;
					schedule.hour = 0xff;
					schedule.min = 0xff;
					schedule.type = 0;
					schedule.valueOn = 0;
					schedule.valueOff = 0;
					char *buffTemp0 = ampm_malloc(128);
					sscanf(pt,"HENGIO,%d,%[^ \r\n,],%[^ \r\n,],%02d:%02d",
							&index,buffTemp0,tempBuff0,&hour,&min);
					if(index)
					{
						index--;
						if(index < DEVICE_SCHEDULE_MAX)
						{
							if(hour < 24 && min < 60)
							{
								schedule.hour = hour;
								schedule.min = min;
								scheduleConfigOk = 1;
							}
							
							schedule.type &= ~(1<<7);
							pt = strstr(buffTemp0,(char *)"BAT");
							if(pt != NULL)
							{
								schedule.valueOn |= (1<<0);
							}
							else
							{
								schedule.valueOff |= (1<<0);
							}

							if(schedule.valueOn == 0 && schedule.valueOff == 0)
							{
								scheduleConfigOk = 0;
							}
							
							RTC_DateTypeDef date;
							RTC_TimeTypeDef time;
							/* Get the RTC current Time */
							HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
							/* Get the RTC current Date */
							HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
							
							pt = strstr(tempBuff0,(char *)"1LAN");
							if(pt != NULL)
							{
								schedule.fromYear = date.Year;
								schedule.toYear = date.Year;
								
								schedule.fromMonth = date.Month;
								schedule.toMonth = date.Month;
								
								schedule.fromMday = date.Date;
								schedule.toMday = date.Date;
							}

							if(scheduleConfigOk)
							{
								deviceCfg.schedules[index] = schedule;
								SMS_DBG(DEV_SET_ALARM_OK_REPORT);
								sms_res_len += sprintf(&sms_res[sms_res_len],"%s\n%s",sms_in,DEV_SET_ALARM_OK_REPORT);
								DeviceCfg_Save();
							}
							else
							{
								SMS_DBG(DEV_SET_ALARM_ERR_REPORT);
								sms_res_len += sprintf(&sms_res[sms_res_len],"%s\n%s",sms_in,DEV_SET_ALARM_ERR_REPORT);
							}
						}
					}
					else
					{
						SMS_DBG(DEV_SET_ALARM_ERR_REPORT);
						sms_res_len += sprintf(&sms_res[sms_res_len],"%s\n%s",sms_in,DEV_SET_ALARM_ERR_REPORT);
					}
					ampm_free(buffTemp0);
				}
			}
			else
			{
				pt = strstr(sms_in,(char *)"HENGIO,");
				if(pt != NULL)
				{
					uint32_t index,outputType,hour,min,scheduleConfigOk = 0;
					alarmParam_t schedule;
					schedule.fromYear = 0xff;
					schedule.toYear = 0xff;
					schedule.fromMonth = 0xff;
					schedule.toMonth = 0xff;
					schedule.fromMday = 0xff;
					schedule.toMday = 0xff;
					schedule.weekDay = 0xff;
					schedule.hour = 0xff;
					schedule.min = 0xff;
					schedule.type = 0;
					schedule.valueOn = 0;
					schedule.valueOff = 0;
					char *buffTemp0 = ampm_malloc(128);
					sscanf(pt,"HENGIO,%02d:%02d,%d,%d,%[^ \r\n]",
							&hour,&min,&index,&outputType,buffTemp0);
					if(index)
					{
						index--;
						if(index < DEVICE_SCHEDULE_MAX)
						{
							if(hour < 24 && min < 60)
							{
								schedule.hour = hour;
								schedule.min = min;
								scheduleConfigOk = 1;
							}
							if(outputType == 0/*ouput is LINE*/)
							{
								schedule.type &= ~(1<<7);
								pt = strstr(buffTemp0,(char *)"BAT:");
								if(pt != NULL)
								{
									sscanf(pt,"BAT:%[^, \r\n]",tempBuff0);
									for(i = 1;i <= 4;i++)
									{
										sprintf(tempBuff1,"O%d",i);
										if(strstr(tempBuff0,tempBuff1))
										{
											schedule.valueOn |= (1<<(i-1));
										}
									}
								}
								
								pt = strstr(buffTemp0,(char *)"TAT:");
								if(pt != NULL)
								{
									sscanf(pt,"TAT:%[^, \r\n]",tempBuff0);
									for(i = 1;i <= 4;i++)
									{
										sprintf(tempBuff1,"O%d",i);
										if(strstr(tempBuff0,tempBuff1))
										{
											schedule.valueOff |= (1<<(i-1));
										}
									}
								}
							}
							
							if(schedule.valueOn == 0 && schedule.valueOff == 0)
							{
								scheduleConfigOk = 0;
							}
							else if(strlen(buffTemp0) >= 5)
							{
								RTC_DateTypeDef date;
								RTC_TimeTypeDef time;
								/* Get the RTC current Time */
								HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
								/* Get the RTC current Date */
								HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
								char *hengioPt;
								hengioPt = strstr(buffTemp0,(char *)",N:");
								if(hengioPt != NULL)
								{
									uint32_t fromYear,toYear;
									sscanf(hengioPt,",N:%04d-%04d",&fromYear,&toYear);
									if((fromYear <= toYear)
										&& toYear >= ((uint32_t)date.Year + 2000)
										&& fromYear >= ((uint32_t)date.Year + 2000)
									)
									{
										schedule.fromYear = fromYear - 2000;
										schedule.toYear = toYear - 2000;
									}
									else
									{
										scheduleConfigOk = 0;
									}
								}
								
								hengioPt = strstr(buffTemp0,(char *)",T:");
								if(hengioPt != NULL)
								{
									uint32_t fromMonth,toMonth;
									sscanf(hengioPt,",T:%d-%d",&fromMonth,&toMonth);
									if((fromMonth <= toMonth)
										&& toMonth <= 12
										&& fromMonth <= 12
									)
									{
										schedule.fromMonth = fromMonth;
										schedule.toMonth = toMonth;
									}
									else
									{
										scheduleConfigOk = 0;
									}
								}
								
								hengioPt = strstr(buffTemp0,(char *)",N:");
								if(hengioPt != NULL)
								{
									uint32_t fromMday,toMday;
									sscanf(hengioPt,",N:%d-%d",&fromMday,&toMday);
									if((fromMday <= toMday)
										&& toMday <= 31
										&& fromMday <= 31
									)
									{
										schedule.fromMday = fromMday;
										schedule.toMday = toMday;
									}
									else
									{
										scheduleConfigOk = 0;
									}
								}
								
								
								hengioPt = strstr(buffTemp0,(char *)",U:");
								if(hengioPt != NULL)
								{
									schedule.weekDay = 0;
									sscanf(hengioPt,",U:%[^, \n\r]",buffTemp0);
									hengioPt = strstr(buffTemp0,(char *)"T2");
									if(hengioPt)
									{
										schedule.weekDay |= 1<<SCHEDULE_WEEKDAY_MONDAY;
									}
									hengioPt = strstr(buffTemp0,(char *)"T3");
									if(hengioPt)
									{
										schedule.weekDay |= 1<<SCHEDULE_WEEKDAY_TUESDAY;
									}
									hengioPt = strstr(buffTemp0,(char *)"T4");
									if(hengioPt)
									{
										schedule.weekDay |= 1<<SCHEDULE_WEEKDAY_WEDNESDAY;
									}
									hengioPt = strstr(buffTemp0,(char *)"T5");
									if(hengioPt)
									{
										schedule.weekDay |= 1<<SCHEDULE_WEEKDAY_THURSDAY;
									}
									hengioPt = strstr(buffTemp0,(char *)"T6");
									if(hengioPt)
									{
										schedule.weekDay |= 1<<SCHEDULE_WEEKDAY_FRIDAY;
									}
									hengioPt = strstr(buffTemp0,(char *)"T7");
									if(hengioPt)
									{
										schedule.weekDay |= 1<<SCHEDULE_WEEKDAY_SATURDAY;
									}
									hengioPt = strstr(buffTemp0,(char *)"CN");
									if(hengioPt)
									{
										schedule.weekDay |= 1<<SCHEDULE_WEEKDAY_SUNDAY;
									}
								}
							}
							if(scheduleConfigOk)
							{
								deviceCfg.schedules[index] = schedule;
								SMS_DBG(DEV_SET_ALARM_OK_REPORT);
								sms_res_len += sprintf(&sms_res[sms_res_len],DEV_SET_ALARM_OK_REPORT);
								DeviceCfg_Save();
							}
							else
							{
								SMS_DBG(DEV_SET_ALARM_ERR_REPORT);
								sms_res_len += sprintf(&sms_res[sms_res_len],DEV_SET_ALARM_ERR_REPORT);
								sms_res_len += sprintf(&sms_res[sms_res_len],"HEN GIO,10:00,6,0,BAT:O1.O3,TAT:O2.O4,N:2018-2019,T:1-12,N:1-31,U:T2T3T4T5T6T7CN");
							}
						}
					}
					else
					{
						SMS_DBG(DEV_SET_ALARM_ERR_REPORT);
						sms_res_len += sprintf(&sms_res[sms_res_len],DEV_SET_ALARM_OK_REPORT);
						sms_res_len += sprintf(&sms_res[sms_res_len],"HEN GIO,10:00,1,0,BAT:O1.O3,TAT:O2.O4,N:2018-2019,T:1-12,N:1-31,U:T2T3T4T5T6T7CN");
					}
					ampm_free(buffTemp0);
				}
			}
		}
	}
	
	if(flagCfgSave)
	{
		CFG_Save();
		
	}
	
	if(sms_res_len)
	{
		AmpmGsm_SendSmsText((char *)smsSender,sms_res);
	}
	
	vPortFree(sms_res);
}




char * FindBossPhone(char *phone)
{
		if(Ampm_ComparePhoneNumber(phone,sysCfg.bigBoss) != NULL)
		{
			return sysCfg.bigBoss;
		}
		return NULL;
}

char * FindUserPhone(char *phone)
{	
		uint8_t i;
		for(i = 0; i < MAX_OWNER;i++)
		{
			if(Ampm_ComparePhoneNumber(phone,sysCfg.ownerNumber[i]) != NULL)
			{
				return sysCfg.ownerNumber[i];
			}
		}
		return NULL;
}

char * FindAllPhone(char *phone)
{
		if(FindBossPhone(phone))
			return FindBossPhone(phone);
		else if(FindUserPhone(phone))
			return FindUserPhone(phone);
		return NULL;
}






