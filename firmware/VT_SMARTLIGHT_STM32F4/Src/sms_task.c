
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

#define SMS_DBG(...) printf(__VA_ARGS__)
#define smsScanf	sscanf
uint8_t inCallFlag = 0;
void process_sms(char *sender, char *sms,char *smsSendBuff,uint16_t smsLenBuf,uint16_t *dataOutLen) ;

const uint8_t smsReplyOk[] = "Cấu hình thành công";




void all_sms_init(void)
{
	
}

void io_process_action_task(void)
{
	
	if(incomingCall.state == CALL_INCOMING)
	{
		if(ampm_GotIncomingNumberFlag)
		{
			SMS_DBG("Incoming Call Number:%s\r\n",ampm_IncomingCallPhoneNo);
			if(inCallFlag == 0)
			{
				inCallFlag  = 1;
			}
			Ampm_VoiceCallSetAction(&incomingCall,CALL_HANGUP_NOW);
		}
   }
   else if(inCallFlag)
   {
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
	char tempBuff0[64] = {0};
	char tempBuff1[64] = {0};
	char tempBuff2[32] = {0};
	char *sms_res = pvPortMalloc(1024);
	if(sms_res == 0) return;
	for(i = 0; i < strlen(sms_in);i++)
	{
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
		pt = strstr(sms_in,(char *)"KHOI DONG LAI");
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
		pt = strstr(sms_in,(char *)"BAT CANH BAO MAT NGUON");
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
		
		pt = strstr(sms_in,(char *)"BAT CANH BAO MAT DONG");
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
		
		pt = strstr(sms_in,(char *)"BAT CANH BAO DONG RO");
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
		
		pt = strstr(sms_in,(char *)"BAT CANH BAO DONG THAP");
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
		
		pt = strstr(sms_in,(char *)"BAT CANH BAO DONG CAO");
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
		
		pt = strstr(sms_in,(char *)"BAT CANH BAO AP CAO");
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
		
		pt = strstr(sms_in,(char *)"BAT CANH BAO AP THAP");
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
		pt = strstr(sms_in,(char *)"TAT CANH BAO MAT NGUON");
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
		
		pt = strstr(sms_in,(char *)"TAT CANH BAO MAT DONG");
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
		
		pt = strstr(sms_in,(char *)"TAT CANH BAO DONG RO");
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
		
		pt = strstr(sms_in,(char *)"TAT CANH BAO DONG THAP");
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
		
		pt = strstr(sms_in,(char *)"TAT CANH BAO DONG CAO");
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
		
		pt = strstr(sms_in,(char *)"TAT CANH BAO AP CAO");
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
		
		pt = strstr(sms_in,(char *)"TAT CANH BAO AP THAP");
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
		
		pt = strstr(sms_in,(char *)"TAT TAT CA CANH BAO");
		if(pt != NULL)
		{
			deviceCfg.features = 0;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		//BAT SMS
		pt = strstr(sms_in,(char *)"BAT SMS");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"HEN GIO"))
				deviceCfg.features |= DEFAULT_FEATURE_SMS_SCHEDULE_REPORT;
			if(strstr(pt,(char *)"CANH BAO"))
				deviceCfg.features |= DEFAULT_FEATURE_SMS_WARNING_REPORT;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"TAT SMS");
		if(pt != NULL)
		{
			if(strstr(pt,(char *)"HEN GIO"))
				deviceCfg.features &= ~DEFAULT_FEATURE_SMS_SCHEDULE_REPORT;
			if(strstr(pt,(char *)"CANH BAO"))
				deviceCfg.features &= ~DEFAULT_FEATURE_SMS_WARNING_REPORT;
			DeviceCfg_Save();
			sms_res_len += sprintf(&sms_res[sms_res_len],"THANH CONG!");
			SMS_DBG("THANH CONG!");
		}
		
		pt = strstr(sms_in,(char *)"*HEN GIO#");
		if(pt != NULL)
		{
			//HEN GIO,10:00,6,0,ON:O1.O3,OFF:O2.O4,NAM:2018-2019,THANG:1-12,NGAY:1-31,THU:T2T3T4T5T6T7CN
			for(uint8_t i = 0;i < DEVICE_SCHEDULE_MAX; i++)
			{
				if(deviceCfg.schedules[i].hour != 0xff
				|| deviceCfg.schedules[i].min != 0xff)
				{
					//TIME
					sms_res_len += sprintf(&sms_res[sms_res_len],"HEN GIO,%d,0,%02d:%02d",i,deviceCfg.schedules[i].hour,deviceCfg.schedules[i].min);
						
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
						sms_res_len += sprintf(&sms_res[sms_res_len],",NAM:%04d:%04d",deviceCfg.schedules[i].fromYear,deviceCfg.schedules[i].toYear);
					}
					
					//THANG
					if(deviceCfg.schedules[i].fromMonth != 0xff && deviceCfg.schedules[i].toMonth != 0xff)
					{
						sms_res_len += sprintf(&sms_res[sms_res_len],",THANG:%d:%d",deviceCfg.schedules[i].fromMonth,deviceCfg.schedules[i].toMonth);
					}
					//NGAY
					if(deviceCfg.schedules[i].fromMonth != 0xff && deviceCfg.schedules[i].toMonth != 0xff)
					{
						sms_res_len += sprintf(&sms_res[sms_res_len],",NGAY:%d:%d",deviceCfg.schedules[i].fromMonth,deviceCfg.schedules[i].toMonth);
					}
					//NGAY
					if(deviceCfg.schedules[i].fromMday != 0xff && deviceCfg.schedules[i].toMday != 0xff)
					{
						sms_res_len += sprintf(&sms_res[sms_res_len],",NGAY:%d:%d",deviceCfg.schedules[i].fromMday,deviceCfg.schedules[i].toMday);
					}
					//WEEK
					if(deviceCfg.schedules[i].weekDay != 0xff)
					{
						sms_res_len += sprintf(&sms_res[sms_res_len],",THU:");
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
					sms_res_len += sprintf(&sms_res[sms_res_len],"\n");
				}
			}
		}
		
		pt = strstr(sms_in,(char *)"XOA HEN GIO,");
		if(pt != NULL)
		{
			uint32_t index;
			sscanf(pt,"XOA HEN GIO,%d",&index);
			
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
			pt = strstr(sms_in,(char *)"HEN GIO,");
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
				sscanf(pt,"HEN GIO,%02d:%02d,%d,%d,%[^ \r\n]",
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
						
						if(strlen(buffTemp0) >= 5)
						{
							RTC_DateTypeDef date;
							RTC_TimeTypeDef time;
							/* Get the RTC current Time */
							HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
							/* Get the RTC current Date */
							HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
							char *hengioPt;
							hengioPt = strstr(buffTemp0,(char *)",NAM:");
							if(hengioPt != NULL)
							{
								uint32_t fromYear,toYear;
								sscanf(hengioPt,",NAM:%04d-%04d",&fromYear,&toYear);
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
							
							hengioPt = strstr(buffTemp0,(char *)",THANG:");
							if(hengioPt != NULL)
							{
								uint32_t fromMonth,toMonth;
								sscanf(hengioPt,",THANG:%d-%d",&fromMonth,&toMonth);
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
							
							hengioPt = strstr(buffTemp0,(char *)",NGAY:");
							if(hengioPt != NULL)
							{
								uint32_t fromMday,toMday;
								sscanf(hengioPt,",NGAY:%d-%d",&fromMday,&toMday);
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
							
							
							hengioPt = strstr(buffTemp0,(char *)",THU:");
							if(hengioPt != NULL)
							{
								schedule.weekDay = 0;
								sscanf(hengioPt,",THU:%[^, \n\r]",buffTemp0);
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
							sms_res_len += sprintf(&sms_res[sms_res_len],DEV_SET_ALARM_OK_REPORT);
							sms_res_len += sprintf(&sms_res[sms_res_len],"HEN GIO,10:00,6,0,O1.O3,O2.O4,NAM:2018-2019,THANG:1-12,NGAY:1-31,THU:T2T3T4T5T6T7CN");
						}
						
					}
				}
				ampm_free(buffTemp0);
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






