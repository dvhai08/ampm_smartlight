#ifndef __UART4_H__
#define __UART4_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "lib/ringbuf.h"

void UART4_Setup(uint32_t BaudRate,                  /*!< This member configures the UART communication baud rate.
                                           The baud rate is computed using the following formula:
                                           - IntegerDivider = ((PCLKx) / (8 * (OVR8+1) * (huart->Init.BaudRate)))
                                           - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 8 * (OVR8+1)) + 0.5 
                                           Where OVR8 is the "oversampling by 8 mode" configuration bit in the CR1 register. */

  uint32_t WordLength,                /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

  uint32_t StopBits,                  /*!< Specifies the number of stop bits transmitted. This parameter can be a value of @ref UART_Stop_Bits */
  uint32_t Parity
);

void UART4_TxCpltCallback(UART_HandleTypeDef *huart);
uint8_t mUART4_PutChar (uint8_t c);
void mUART4_PutString (char *s) ;
extern RINGBUF UART4_TxRingBuff;
int32_t mUART4_GetChar (uint8_t *c);
uint32_t mUART4_GetFill (void);
uint32_t mUART4_GetRxBufSize(void);
#endif

