/**********************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.com.vn 
----------------------------------
AMPM ELECTRONICS EQUIPMENT TRADING COMPANY LIMITED.,
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN

*********************************************************************/
#include "ampm_gsm_common.h"
#include "ampm_gsm_ring.h"
#include "main.h"

#define SIM800C	1
#define AMPM_CHECK_SMS_INTERVAL 1800 //30min
#define SMS_CHECK_TIMES	10
uint8_t ampm_MissCall = 0;
uint8_t ampm_NewSmsFlag = 0;
uint8_t ampm_RingingFlagTimes = 0;

uint8_t ampm_RingingFlag = 0;
uint8_t ampm_RingingTime = 0;
uint8_t ampm_RingingPauseTime = 0;

uint32_t smsCheckTimerCnt = 0;

uint32_t smsCheckTimeInterval = 60;
uint8_t smsCheckTimes = SMS_CHECK_TIMES;

AMPM_GSM_RING_PHASE ampm_RingingPhase;

void SmsResetIntervalCnt(void)
{	
	smsCheckTimerCnt = 0;
}
//Call 1 sec
void Ampm_RingingTimer(void)
{
	smsCheckTimeInterval = AMPM_CHECK_SMS_INTERVAL;
	if(smsCheckTimes || (gsmSignal == 0))
	{
		smsCheckTimeInterval = 60;
	}
	smsCheckTimerCnt++;
	if(smsCheckTimerCnt >= smsCheckTimeInterval)
	{
		if(smsCheckTimes)
			smsCheckTimes--;
		smsCheckTimerCnt = 0;
		ampm_NewSmsFlag = 1;
	}
	ampm_RingingTime++;
	if(ampm_RingingPauseTime) ampm_RingingPauseTime--;
}

AMPM_GSM_RING_PHASE Ampm_Ringing_GetPhase(void)
{
	return ampm_RingingPhase;
}

#include "stm32f4xx_hal.h"
uint32_t Ampm_Ringing_RI_isActive(void)
{
	if(HAL_GPIO_ReadPin(GSM_RI_GPIO_Port,GSM_RI_Pin))
		return 1;
	return 0;
}

void Ampm_RingingDisable(void)
{
	ampm_RingingPauseTime = 15; //stop detect ring pin in 30s
	ampm_RingingFlag = 0; 
	ampm_RingingPhase = AMPM_GSM_RING_IDLE_PHASE;
}

void Ampm_RingingEnable(void)
{
	ampm_RingingPauseTime = 0;
}


uint8_t Ampm_Is_RingingEnable(void)
{
	if( ampm_RingingPauseTime)
		return 0;
	return 1;
}

void Ampm_RingingReset(void)
{
	ampm_RingingFlag = 0; 
	ampm_RingingPhase = AMPM_GSM_RING_IDLE_PHASE;
	ampm_RingingPauseTime = 1;
}

void Ampm_SetRinging(void)
{
	//if(ampm_RingingPauseTime == 0)
	{
		ampm_RingingFlag |= AMPM_RINGING_FLAG;
		ampm_RingingFlagTimes++;
		ampm_RingingTime = 0;
	}
}

//void Ampm_ClearAllRingEvent(void)
//{
//	ampm_RingingFlag = 0; 
//	ampm_RingingPhase = AMPM_GSM_RING_IDLE_PHASE;
//	ampm_RingingPauseTime = 1;
//}


void Ampm_SetAtRing(void)
{
	ampm_RingingFlag |= AMPM_RING_GOT_AT_RING;
	ampm_RingingTime = 0;
}

void Ampm_SetAtClip(void)
{
	ampm_RingingFlag |= AMPM_RING_GOT_AT_CLIP;
	ampm_RingingTime = 0;
}

void Ampm_SetAtCmti(void)
{
	ampm_NewSmsFlag = 1;
	ampm_RingingFlag |= AMPM_RING_GOT_AT_CMTI;
	smsCheckTimes = SMS_CHECK_TIMES;
}

void Ampm_RingingProcess(void)
{

	switch(ampm_RingingPhase){
		case AMPM_GSM_RING_IDLE_PHASE:
			if(ampm_RingingFlag & (AMPM_RINGING_FLAG | AMPM_RING_GOT_AT_RING | AMPM_RING_GOT_AT_CLIP)) {
				ampm_RingingFlag = 0;
				ampm_RingingFlagTimes = 0;
				ampm_RingingTime = 0;
				AMPM_GSM_LIB_DBG("\r\n AMPM_GSM_RING:AMPM_GSM_RING_IS_DETECTING \r\n");
				ampm_RingingPhase = AMPM_GSM_RING_IS_DETECTING;
			}
		break;
		case AMPM_GSM_RING_IS_DETECTING:
			if(ampm_RingingFlag & AMPM_RING_GOT_AT_CLIP 
			|| ampm_RingingFlag & AMPM_RING_GOT_AT_RING
			|| ampm_RingingFlagTimes
			){
				ampm_RingingFlag = 0;
				ampm_RingingPhase = AMPM_GSM_RING_IS_A_CALL;
				AMPM_GSM_LIB_DBG("\r\n AMPM_GSM_RING:AMPM_GSM_RING_IS_A_CALL \r\n");
			}else if(ampm_RingingFlag & AMPM_RING_GOT_AT_CMTI
			){
				smsCheckTimes = SMS_CHECK_TIMES;
				ampm_RingingFlag = 0;
				ampm_RingingPhase = AMPM_GSM_RING_IS_A_SMS;
				AMPM_GSM_LIB_DBG("\r\n AMPM_GSM_RING:AMPM_GSM_RING_IS_A_SMS \r\n");
			}
			else if(ampm_RingingTime >= 6/*6 sec*/){
				ampm_RingingPhase = AMPM_GSM_RING_IS_A_SMS;
				AMPM_GSM_LIB_DBG("\r\n AMPM_GSM_RING:AMPM_GSM_RING_IS_A_SMS \r\n");
			}
		break;
		case AMPM_GSM_RING_IS_A_SMS:
			smsCheckTimes = SMS_CHECK_TIMES;
			ampm_NewSmsFlag = 1;
			ampm_RingingFlag = 0;
			ampm_RingingTime = 0;
			ampm_RingingPhase = AMPM_GSM_RING_IDLE_PHASE;
			AMPM_GSM_LIB_DBG("\r\n AMPM_GSM_RING:AMPM_GSM_RING_IDLE_PHASE \r\n");
		break;
		case AMPM_GSM_RING_IS_A_CALL:
			if(ampm_RingingTime >= 6)
			{
				ampm_MissCall = 1;
				ampm_RingingFlag = 0;
				ampm_RingingPhase = AMPM_GSM_RING_IDLE_PHASE;
				AMPM_GSM_LIB_DBG("\r\n AMPM_GSM_RING:AMPM_GSM_RING_IDLE_PHASE \r\n");
			}
		break;
		default:
			ampm_RingingFlag = 0;
			ampm_RingingPhase = AMPM_GSM_RING_IDLE_PHASE;
			AMPM_GSM_LIB_DBG("\r\n AMPM_GSM_RING:AMPM_GSM_RING_IDLE_PHASE \r\n");
		break;
	}

}

