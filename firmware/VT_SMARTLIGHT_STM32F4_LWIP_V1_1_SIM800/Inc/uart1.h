#ifndef __UART1_H__
#define __UART1_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "lib/ringbuf.h"

void USART1_TxCpltCallback(UART_HandleTypeDef *huart);
uint8_t mUSART1_PutChar (uint8_t c);
void mUSART1_PutString (char *s) ;
extern RINGBUF USART1_TxRingBuff;
int32_t mUSART1_GetChar (uint8_t *c);
uint32_t mUSART1_GetFill (void);
uint8_t USART1_IsOk(void);
#endif

