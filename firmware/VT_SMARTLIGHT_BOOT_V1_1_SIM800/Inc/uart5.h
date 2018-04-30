#ifndef __UART5_H__
#define __UART5_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "ringbuf.h"

void UART5_TxCpltCallback(UART_HandleTypeDef *huart);
uint8_t mUART5_PutChar (uint8_t c);
void mUART5_PutString (char *s) ;
extern RINGBUF UART5_TxRingBuff;
int32_t mUART5_GetChar (uint8_t *c);
uint32_t mUART5_GetFill (void);
uint32_t mUART5_GetRxBufSize(void);

#endif

