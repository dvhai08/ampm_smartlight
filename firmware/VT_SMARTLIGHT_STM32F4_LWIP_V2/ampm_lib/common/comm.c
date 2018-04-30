
#include "comm.h"
#include "uart1.h"
#include "hw_config.h"
#include "lib/sys_tick.h"

uint8_t COMM_RxBuff[512];
RINGBUF COMM_RxRingBuff;
uint8_t comm_initialized = 0;
void COMM_Putc(uint8_t c)
{
	mUSART1_PutChar(c);
}
void COMM_Puts(uint8_t *s)
{
	mUSART1_PutString((char *)s);
}

int32_t COMM_Getc(uint8_t *c)
{
	if(!comm_initialized)
	{
		comm_initialized = 1;
		RINGBUF_Init(&COMM_RxRingBuff,COMM_RxBuff,sizeof(COMM_RxBuff));
	}
	return RINGBUF_Get(&COMM_RxRingBuff,c);
	// mUSART1_GetChar(c);
}

int32_t COMM_PutcRx(uint8_t c)
{
	if(!comm_initialized)
	{
		comm_initialized = 1;
		RINGBUF_Init(&COMM_RxRingBuff,COMM_RxBuff,sizeof(COMM_RxBuff));
	}
	return RINGBUF_Put(&COMM_RxRingBuff,c);
	// mUSART1_GetChar(c);
}

uint8_t COMM_CarrierDetected(void)
{
	return 0;
}

void MODEM_RTS_Set(void)
{
	//RTS_PIN_SET;
}
void MODEM_RTS_Clr(void)
{
	//RTS_PIN_CLR;
}

void MODEM_DTR_Set(void)
{
	//DTR_PIN_SET;
}
void MODEM_DTR_Clr(void)
{
	//DTR_PIN_CLR;
}

void COMM_ClearTx(void)
{
	
}
void MODEM_MOSFET_On(void)
{
	GSM_MOSFET_PIN_SET;
}

void MODEM_MOSFET_Off(void)
{
	GSM_MOSFET_PIN_CLR;
}

void MODEM_POWER_Set(void)
{
	POWER_PIN_SET;
}

void MODEM_POWER_Clr(void)
{
	POWER_PIN_CLR;
}

void MODEM_RESET_Set(void)
{
	//RESET_PIN_SET;
}

void MODEM_RESET_Clr(void)
{
//	RESET_PIN_CLR;
}

void MODEM_Wakeup(void)
{
	//MODEM_DTR_Clr();
	//SysTick_DelayMs(100);
}

void MODEM_Sleep(void)
{
	//MODEM_DTR_Set();
	//SysTick_DelayMs(100);
}
				
void MODEM_UartInit(uint32_t baudrate)
{
	//GSM_POWER_PIN_SET_OUTPUT;
	//POWER_PIN_SET_OUTPUT;
//	RESET_PIN_SET_OUTPUT;
	//DTR_PIN_SET_OUTPUT;
}



