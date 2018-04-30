
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


uint8_t UART4_TxBuff[512];
RINGBUF UART4_TxRingBuff;
uint8_t UART4_DMATxBuff[512];
uint8_t UART4_DMARxBuff[1024];
uint32_t UART4_RxTail = 0;

static __IO uint8_t uart4_initialized = 0;

extern UART_HandleTypeDef huart4;

void UART4_Init(void)
{
	uart4_initialized = 1;
	HAL_UART_Receive_DMA(&huart4,UART4_DMARxBuff,sizeof(UART4_DMARxBuff));
	RINGBUF_Init(&UART4_TxRingBuff,UART4_TxBuff,sizeof(UART4_TxBuff));
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
	uint32_t head = sizeof(UART4_DMARxBuff) - huart4.hdmarx->Instance->NDTR;
	if(!uart4_initialized)
	{
		UART4_Init();
	}
	if(UART4_RxTail == head) 
	{
		return -1;				// ring buffer is empty, this should be atomic operation
	}
	*c = UART4_DMARxBuff[UART4_RxTail];
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
	if(!uart4_initialized)
	{
		UART4_Init();
	}
	while((RINGBUF_GetFill(&UART4_TxRingBuff) >= (UART4_TxRingBuff.size - 1)) && timeOut--);
	if(timeOut == 0xFFFFFFFF)
		huart4.gState = HAL_UART_STATE_READY;
	if(huart4.gState == HAL_UART_STATE_READY && (UART4->CR1 & USART_CR1_TXEIE) != USART_CR1_TXEIE)
	{
		UART4_DMATxBuff[0] = c;
		HAL_UART_Transmit_DMA(&huart4,UART4_DMATxBuff,1);
	}
	else
		RINGBUF_Put(&UART4_TxRingBuff,c);
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
		//huart4.gState = HAL_UART_STATE_READY;
		UART4->CR1 &= (~USART_CR1_TCIE);
		UART4->CR1 &= (~USART_CR1_TXEIE);
	}

}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/




