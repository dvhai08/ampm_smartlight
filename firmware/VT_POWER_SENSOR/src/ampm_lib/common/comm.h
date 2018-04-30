/******************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.com.vn 
----------------------------------
AMPM ELECTRONICS EQUIPMENT TRADING COMPANY LIMITED.,
Add: 22 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
******************************************************************************/
#ifndef __COMM_H__
#define __COMM_H__
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "lib/ringbuf.h"
#include "main.h"
#include "drivers.h"

extern uint32_t  DbgCfgPrintf(const uint8_t *format, ...);

#define PPP_Info(...)		//DbgCfgPrintf(__VA_ARGS__)
#define PPP_Debug(...)	//DbgCfgPrintf(__VA_ARGS__)
#define AT_CMD_Debug(...)	//DbgCfgPrintf(__VA_ARGS__)
#define UIP_PPP_Info(...) //DbgCfgPrintf(__VA_ARGS__)
#define AMPM_GSM_LIB_DBG(...) //DbgCfgPrintf(__VA_ARGS__)
#define MODEM_Info(...)		//PrintfDebug(__VA_ARGS__)


extern RINGBUF USART1_TxRingBuff;
extern RINGBUF USART1_RxRingBuff;
//extern RINGBUF DLT645_RxRingBuff;
//extern RINGBUF Server_TxRingBuff;
extern RINGBUF *commTxRingBuf;
extern RINGBUF *commRxRingBuf;
extern void COMM_Init(void);
extern uint8_t USART0_PutChar(uint8_t ch);
extern void USART0_PutString (uint8_t *s);
extern void COMM_Putc(uint8_t c);
extern void COMM_Puts(uint8_t *s);
extern void uart_callback (UART_CALLBACK_SRC module, UART_CALLBACK_TYPE type, int8 status);
//extern uint8_t COMM_CarrierDetected(void);
//extern void MODEM_UartInit(uint32_t baudrate);
//extern void MODEM_DTR_Set(void);
//extern void MODEM_DTR_Clr(void);
//extern void MODEM_MOSFET_On(void);
//extern void MODEM_MOSFET_Off(void);
//extern void MODEM_POWER_Set(void);
//extern void MODEM_POWER_Clr(void);
//extern void MODEM_RESET_Set(void);
//extern void MODEM_RESET_Clr(void);

#endif
