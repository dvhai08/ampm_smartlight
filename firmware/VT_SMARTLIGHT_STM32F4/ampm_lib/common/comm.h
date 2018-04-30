
#ifndef __COMM_H__
#define __COMM_H__
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "lib/ringbuf.h"
#ifdef CMSIS_OS_RTX
#include "cmsis_os.h"

#define ampm_malloc	pvPortMalloc
#define ampm_free	vPortFree
#endif    


#define		AMPM_NO_LOG  0
#define		AMPM_TCP_IP_LOG	1
#define		AMPM_GSM_AT_CMD_LOG 2



#define PPP_Info(...)		printf(__VA_ARGS__)

#define PPP_Debug(...)	printf(__VA_ARGS__)
#define AT_CMD_Debug(...)	printf(__VA_ARGS__)
#define UIP_PPP_Info(...) printf(__VA_ARGS__)
#define AMPM_GSM_LIB_DBG(...) printf(__VA_ARGS__)
#define MODEM_Info(...)		printf(__VA_ARGS__)

extern RINGBUF USART2_TxRingBuff;
extern RINGBUF USART2_RxRingBuff;


extern RINGBUF *commTxRingBuf;
extern RINGBUF *commRxRingBuf;

extern uint8_t USART2_PutChar(uint8_t ch);
extern void USART2_PutString (uint8_t *s);
extern void COMM_Putc(uint8_t c);
extern  int32_t COMM_Getc(uint8_t *c);
extern  void COMM_ClearTx(void);
extern void COMM_Puts(uint8_t *s);
extern int32_t COMM_PutcRx(uint8_t c);
extern uint8_t COMM_CarrierDetected(void);
extern void MODEM_UartInit(uint32_t baudrate);
extern void MODEM_DTR_Set(void);
extern void MODEM_DTR_Clr(void);
extern void MODEM_MOSFET_On(void);
extern void MODEM_MOSFET_Off(void);
extern void MODEM_POWER_Set(void);
extern void MODEM_POWER_Clr(void);
extern void MODEM_RESET_Set(void);
extern void MODEM_RESET_Clr(void);
extern  void MODEM_Wakeup(void);
#endif
