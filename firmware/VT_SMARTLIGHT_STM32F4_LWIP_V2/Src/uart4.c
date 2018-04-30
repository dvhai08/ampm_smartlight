
/**
* @file
*         uart4.c
* @author
*         Nguyen Van Hai <hainv75@viettel.com.vn>
* @brief  UART DMA ring buffer layer
*	@date	
*					2017-11-10
*/
#include "uart4.h"


uint8_t UART4_TxBuff[256];
RINGBUF UART4_TxRingBuff;
uint8_t UART4_DMATxBuff[256];
uint8_t UART4_DMARxBuff[600];
uint32_t UART4_RxTail = 0;
uint32_t UART4_Parity = 0;
static __IO uint8_t uart4_initialized = 0;

extern UART_HandleTypeDef huart4;

void UART4_Init(void)
{
	
	HAL_UART_Receive_DMA(&huart4,UART4_DMARxBuff,sizeof(UART4_DMARxBuff));
	RINGBUF_Init(&UART4_TxRingBuff,UART4_TxBuff,sizeof(UART4_TxBuff));
	uart4_initialized = 1;
}

/* UART4 init function */
void UART4_Setup(uint32_t BaudRate,                  /*!< This member configures the UART communication baud rate.
                                           The baud rate is computed using the following formula:
                                           - IntegerDivider = ((PCLKx) / (8 * (OVR8+1) * (huart->Init.BaudRate)))
                                           - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 8 * (OVR8+1)) + 0.5 
                                           Where OVR8 is the "oversampling by 8 mode" configuration bit in the CR1 register. */

  uint32_t WordLength,                /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

  uint32_t StopBits,                  /*!< Specifies the number of stop bits transmitted. This parameter can be a value of @ref UART_Stop_Bits */
  uint32_t Parity
)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = BaudRate;;
  huart4.Init.WordLength = WordLength;
  huart4.Init.StopBits = StopBits;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	UART4_Parity = Parity;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

uint32_t mUART4_GetRxBufSize(void) 
{
	return sizeof(UART4_DMARxBuff);
}

uint32_t mUART4_GetFill (void) 
{
	uint32_t head = sizeof(UART4_DMARxBuff) - huart4.hdmarx->Instance->NDTR;
	if(head >= UART4_RxTail)
	{
		return (head - UART4_RxTail);
	}
	else
	{
	   return( sizeof(UART4_DMARxBuff) - UART4_RxTail + head);
	}
}

int32_t mUART4_GetChar (uint8_t *c) 
{
	uint32_t s,even;
	if(!uart4_initialized)
	{
		UART4_Init();
	}
	uint32_t head = sizeof(UART4_DMARxBuff) - huart4.hdmarx->Instance->NDTR;
	if(UART4_RxTail == head) 
	{
		return -1;				// ring buffer is empty, this should be atomic operation
	}
	*c = UART4_DMARxBuff[UART4_RxTail];
	
	if(UART4_Parity == UART_PARITY_EVEN){
		//c &= 0x7F;
		s = *c;
		even = (s & 1) + (s>>1 & 1) + (s>>2 & 1) + (s>>3 & 1) + (s>>4 & 1) + (s>>5 & 1) + (s>>6 & 1);
		if(even & 1)
		{
			if(s & 0x80)
				*c = s & 0x7F;
		}
		else
		{
			if((s & 0x80) == 0)
				*c = s & 0x7F;
		}
	}
	
	UART4_RxTail++;
	if(UART4_RxTail >= sizeof(UART4_DMARxBuff)) 
	{
		UART4_RxTail = 0;
	}
	return 0;
}

void mUART4_PutString (char *s) 
{
   while(*s)
	{
		mUART4_PutChar(*s++);
	}
}



uint8_t mUART4_PutChar (uint8_t c) 
{
	uint32_t timeOut = 1000000;
	uint32_t s,even;
	if(!uart4_initialized)
	{
		UART4_Init();
	}
	while((RINGBUF_GetFill(&UART4_TxRingBuff) >= (UART4_TxRingBuff.size - 1)) && timeOut--);
	if(timeOut == 0xFFFFFFFF)
	{
		huart4.gState = HAL_UART_STATE_READY;
	}
	if(UART4_Parity == UART_PARITY_EVEN)
	{
		s = c;
		even = (s & 1) + (s>>1 & 1) + (s>>2 & 1) + (s>>3 & 1) + (s>>4 & 1) + (s>>5 & 1) + (s>>6 & 1);
		if(even & 1)
		{
			c |= 0x80;
		}
		else
		{
			c &= 0x7F;
		}
	}
	
	if(huart4.gState == HAL_UART_STATE_READY && (UART4->CR1 & USART_CR1_TXEIE) != USART_CR1_TXEIE)
	{
		UART4_DMATxBuff[0] = c;
		HAL_UART_Transmit_DMA(&huart4,UART4_DMATxBuff,1);
	}
	else
	{
		RINGBUF_Put(&UART4_TxRingBuff,c);
	}
	return 0;
}


void mUART4_IRQHandler(void) 
{
	uint8_t c;
	uint16_t len = 0;
	
	if(UART4->SR & USART_SR_TXE)
	{
		HAL_UART_AbortTransmit_IT(&huart4);
		if(RINGBUF_GetFill(&UART4_TxRingBuff))
		{
			while(RINGBUF_Get(&UART4_TxRingBuff,&c) == 0)
			{
				UART4_DMATxBuff[len] = c;
				len++;
				if(len >= sizeof(UART4_DMATxBuff))
					break;
			}
			/* Tx process is ended, restore huart->gState to Ready */
			HAL_UART_Transmit_DMA(&huart4,UART4_DMATxBuff,len);
		}
		else if(UART4->SR & USART_SR_TC)
		{
			UART4->SR &= ~USART_SR_TC;
		}
		if(UART4->SR & (USART_SR_FE | USART_SR_PE | USART_SR_NE | USART_SR_ORE))
		{
			c = UART4->DR;
		}
	}

}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/




