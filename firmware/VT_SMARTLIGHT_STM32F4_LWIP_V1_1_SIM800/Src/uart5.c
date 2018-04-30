
/**
* @file
*         uart5.c
* @author
*         Nguyen Van Hai <hainv75@viettel.com.vn>
* @brief  UART DMA ring buffer layer
*	@date	
*					2017-11-10
*/
#include "uart5.h"


uint8_t UART5_TxBuff[256];
RINGBUF UART5_TxRingBuff;
uint8_t UART5_DMATxBuff[256];
uint8_t UART5_DMARxBuff[600];
uint32_t UART5_RxTail = 0;

static __IO uint8_t uart5_initialized = 0;

extern UART_HandleTypeDef huart5;

void UART5_Init(void)
{
	
	HAL_UART_Receive_DMA(&huart5,UART5_DMARxBuff,sizeof(UART5_DMARxBuff));
	RINGBUF_Init(&UART5_TxRingBuff,UART5_TxBuff,sizeof(UART5_TxBuff));
	uart5_initialized = 1;
}

//void UART5_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//		uint8_t c;
//		uint16_t len = 0;
//	if(RINGBUF_GetFill(&UART5_TxRingBuff))
//	{
//		while(RINGBUF_Get(&UART5_TxRingBuff,&c) == 0)
//		{
//			UART5_DMATxBuff[len] = c;
//			len++;
//			if(len >= sizeof(UART5_DMATxBuff))
//				break;
//		}
//		huart->hdmatx->Instance->NDTR = len;
//		HAL_UART_Transmit_DMA(huart,UART5_DMATxBuff,len);
//	}
//	else
//	{
//		HAL_UART_AbortTransmit_IT(huart);
//	}
//}

uint32_t mUART5_GetRxBufSize(void) 
{
	return sizeof(UART5_DMARxBuff);
}
uint32_t mUART5_GetFill (void) 
{
	uint32_t head = sizeof(UART5_DMARxBuff) - huart5.hdmarx->Instance->NDTR;
	if(head >= UART5_RxTail)
	{
		return (head - UART5_RxTail);
	}
	else
	{
	   return( sizeof(UART5_DMARxBuff) - UART5_RxTail + head);
	}
}

int32_t mUART5_GetChar (uint8_t *c) 
{
	
	if(!uart5_initialized)
	{
		UART5_Init();
	}
	uint32_t head = sizeof(UART5_DMARxBuff) - huart5.hdmarx->Instance->NDTR;
	if(UART5_RxTail == head) 
	{
		return -1;				// ring buffer is empty, this should be atomic operation
	}
	*c = UART5_DMARxBuff[UART5_RxTail];
	UART5_RxTail++;
	if(UART5_RxTail >= sizeof(UART5_DMARxBuff)) 
	{
		UART5_RxTail = 0;
	}
	return 0;
}

void mUART5_PutString (char *s) 
{
   while(*s)
	{
		mUART5_PutChar(*s++);
	}
}



uint8_t mUART5_PutChar (uint8_t c) 
{
	uint32_t timeOut = 1000000;
	if(!uart5_initialized)
	{
		UART5_Init();
	}
	while((RINGBUF_GetFill(&UART5_TxRingBuff) >= (UART5_TxRingBuff.size - 1)) && timeOut--);
	if(timeOut == 0xFFFFFFFF)
		huart5.gState = HAL_UART_STATE_READY;
	if(huart5.gState == HAL_UART_STATE_READY && (UART5->CR1 & USART_CR1_TXEIE) != USART_CR1_TXEIE)
	{
		UART5_DMATxBuff[0] = c;
		HAL_UART_Transmit_DMA(&huart5,UART5_DMATxBuff,1);
		UART5->CR1 |= USART_CR1_TCIE;
	}
	else
		RINGBUF_Put(&UART5_TxRingBuff,c);
	return 0;
}


void mUART5_IRQHandler(void) 
{
	uint8_t c;
	uint16_t len = 0;
	
	if(UART5->SR & USART_SR_TXE)
	{
		HAL_UART_AbortTransmit_IT(&huart5);
		if(RINGBUF_GetFill(&UART5_TxRingBuff))
		{
			while(RINGBUF_Get(&UART5_TxRingBuff,&c) == 0)
			{
//                if(c == '\n')
//                {
//                    UART5_DMATxBuff[len] = '\r';
//                    len++;
//                }
				UART5_DMATxBuff[len] = c;
				len++;
				if(len >= sizeof(UART5_DMATxBuff))
					break;
			}
			/* Tx process is ended, restore huart->gState to Ready */
			HAL_UART_Transmit_DMA(&huart5,UART5_DMATxBuff,len);
		}
		else if(UART5->SR & USART_SR_TC)
		{
			UART5->SR &= ~USART_SR_TC;
		}
		if(UART5->SR & (USART_SR_FE | USART_SR_PE | USART_SR_NE | USART_SR_ORE))
		{
			c = UART5->DR;
		}
	}

}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/




