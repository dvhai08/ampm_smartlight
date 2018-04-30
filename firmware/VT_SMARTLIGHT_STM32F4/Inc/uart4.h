#ifndef __UART4_H__
#define __UART4_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "lib/ringbuf.h"

void UART4_TxCpltCallback(UART_HandleTypeDef *huart);
uint8_t mUART4_PutChar (uint8_t c);
void mUART4_PutString (char *s) ;
extern RINGBUF UART4_TxRingBuff;
int32_t mUART4_GetChar (uint8_t *c);
uint32_t mUART4_GetFill (void);
uint32_t mUART4_GetRxBufSize(void);
#endif

