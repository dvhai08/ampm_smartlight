
#include "ampm_gsm_sms_api.h"
#include "lib/sys_tick.h"
#include "lib/encoding.h"

#define SMS_MSG_MAX_SIZE	250

uint16_t AmpmGsm_SendSmsTextPolling(char *phone,char *sms)
{
	return AmpmGsm_SendSmsPolling(phone,sms,strlen(sms),SMS_TEXT_MODE);
} 

uint16_t AmpmGsm_SendSmsPolling(char *phone,char *sms,uint8_t len,SMS_MODE_TYPE type)
{
	uint8_t *smsStatus = (uint8_t *)malloc(sizeof(uint8_t)); 
	uint16_t timeout = 60;//60s
	AMPM_GSM_LIB_DBG("\r\n AmpmGsm_SendSmsWait:Send Sms \r\n");
	if(AmpmGsm_SendSms(smsStatus,phone, sms,len,type,NULL) == SMS_OK)
	{
		while((*smsStatus != SMS_OK) && timeout)
		{
			osDelay(1000);
			if(timeout) timeout--;
			//AMPM_GSM_LIB_DBG("\r\n AmpmGsm_SendSmsWait:%d \r\n",timeout);
		}
	}
	if(*smsStatus == SMS_OK)
	{
		AMPM_GSM_LIB_DBG("\r\n AmpmGsm_SendSmsWait:Sms Sent\r\n");
		Ampm_UpdateStatus(smsStatus,0xff);
		return SMS_OK;
	}
	Ampm_UpdateStatus(smsStatus,0xff);
	AMPM_GSM_LIB_DBG("\r\n AmpmGsm_SendSmsWait:Sms Fail\r\n");
	return SMS_ERROR;
}



uint16_t AmpmGsm_SendSmsPDUWitchCallback(char *phone,char *sms,uint8_t len,void (*callback)(struct SMS_LIST_TYPE *msg))
{
	return AmpmGsm_SendSms(NULL,phone, sms,len,SMS_PDU16_MODE,callback);
}

uint16_t AmpmGsm_SendSmsPDU(char *phone,char *sms,uint8_t len)
{
	return AmpmGsm_SendSms(NULL,phone, sms,len,SMS_PDU16_MODE,NULL);
}

uint16_t AmpmGsm_SendSmsTextWithCallback(char *phone,char *sms,uint8_t len,void (*callback)(struct SMS_LIST_TYPE *msg))
{
	return AmpmGsm_SendSms(NULL,phone, sms,len,SMS_TEXT_MODE,callback);
}

uint16_t AmpmGsm_SendSmsText(char *phone,char *sms)
{
	return AmpmGsm_SendSms(NULL,phone, sms,strlen(sms),SMS_TEXT_MODE,NULL);
}



uint16_t AmpmGsm_SendSms(uint8_t *status,char *phone,char *sms,uint16_t sms_len,SMS_MODE_TYPE type,void (*callback)(struct SMS_LIST_TYPE *msg))
{
	uint16_t smsLen,res = SMS_OK,flag = 0;
	char *smsPt;
	if(type == SMS_PDU16_MODE)
	{
		uint8_t *smsBuf = (uint8_t *)malloc(strlen(sms)*2);
		smsLen = utf8s_to_ucs2s((int16_t *)smsBuf,(uint8_t *)sms);
		big2litel_endian((uint16_t *)smsBuf,unilen((uint16_t *)smsBuf));
		smsLen *= 2;
		res =  Ampm_Sms_SendMsg(status,callback,(uint8_t *)phone,(uint8_t *)smsBuf,smsLen,SMS_PDU16_MODE,2/*interval resend in sec*/,3/*resend times*/);
		ampm_free(smsBuf);
	}
	else if(sms_len < 1024) //max 3 sms
	{
		smsLen = 0;
		do
		{
			smsPt = sms + smsLen;
			if(sms_len > SMS_MSG_MAX_SIZE) 
			{
				smsLen += SMS_MSG_MAX_SIZE;
				sms_len -= SMS_MSG_MAX_SIZE;
			}
			else
			{
				smsLen = sms_len;
				sms_len = 0;
			}
			AMPM_GSM_LIB_DBG("\r\n AmpmGsm_SMS Sending:%d:%s\r\n",sms_len,sms);
			res = Ampm_Sms_SendMsg(status,callback,(uint8_t *)phone,(uint8_t *)smsPt,smsLen,SMS_TEXT_MODE,2/*interval resend in sec*/,3/*resend times*/);
		}while(sms_len);
	}
	return res;
}

