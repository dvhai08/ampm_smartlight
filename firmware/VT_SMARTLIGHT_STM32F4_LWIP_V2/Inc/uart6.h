#ifndef __UART6_H__
#define __UART6_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "lib/ringbuf.h"

void USART6_TxCpltCallback(UART_HandleTypeDef *huart);
uint8_t mUSART6_PutChar (uint8_t c);
void mUSART6_PutString (char *s) ;
extern RINGBUF USART6_TxRingBuff;
int32_t mUSART6_GetChar (uint8_t *c);
uint32_t mUSART6_GetFill (void);
uint8_t USART6_IsOk(void);
#endif

