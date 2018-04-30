
#ifndef __AMPM_GSM_SMS_API_H__
#define __AMPM_GSM_SMS_API_H__


#include "comm.h"
#include "ampm_gsm_sms.h"

uint16_t AmpmGsm_SendSms(uint8_t *status,char *phone,char *sms,uint16_t sms_len,SMS_MODE_TYPE type,void (*callback)(struct SMS_LIST_TYPE *msg));
uint16_t AmpmGsm_SendSmsTextPolling(char *phone,char *sms);
uint16_t AmpmGsm_SendSmsPolling(char *phone,char *sms,uint8_t len,SMS_MODE_TYPE type);
uint16_t AmpmGsm_SendSmsPDUWitchCallback(char *phone,char *sms,uint8_t len,void (*callback)(struct SMS_LIST_TYPE *msg));
uint16_t AmpmGsm_SendSmsPDU(char *phone,char *sms,uint8_t len);
uint16_t AmpmGsm_SendSmsTextWithCallback(char *phone,char *sms,uint8_t len,void (*callback)(struct SMS_LIST_TYPE *msg));
uint16_t AmpmGsm_SendSmsText(char *phone,char *sms);


#endif

