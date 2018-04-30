
/**
* @file
*         uart5.c
* @author
*         Nguyen Van Hai <hainv75@viettel.com.vn>
* @brief  UART DMA ring buffer layer
*	@date	
*					2017-11-10
*/
#include "uart1.h"


uint8_t USART1_TxBuff[512];
RINGBUF USART1_TxRingBuff;
uint8_t USART1_DMATxBuff[512];
uint8_t USART1_DMARxBuff[512];
uint32_t USART1_RxTail = 0;

static __IO uint8_t uart1_initialized = 0;

extern UART_HandleTypeDef huart1;

uint8_t USART1_IsOk(void)
{
	return uart1_initialized;
}

void USART1_Init(void)
{
	
	HAL_UART_Receive_DMA(&huart1,USART1_DMARxBuff,sizeof(USART1_DMARxBuff));
	RINGBUF_Init(&USART1_TxRingBuff,USART1_TxBuff,sizeof(USART1_TxBuff));
	uart1_initialized = 1;
}

//void USART1_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//		uint8_t c;
//		uint16_t len = 0;
//	if(RINGBUF_GetFill(&USART1_TxRingBuff))
//	{
//		while(RINGBUF_Get(&USART1_TxRingBuff,&c) == 0)
//		{
//			USART1_DMATxBuff[len] = c;
//			len++;
//			if(len >= sizeof(USART1_DMATxBuff))
//				break;
//		}
//		huart->hdmatx->Instance->NDTR = len;
//		HAL_UART_Transmit_DMA(huart,USART1_DMATxBuff,len);
//	}
//	else
//	{
//		HAL_UART_AbortTransmit_IT(huart);
//	}
//}

uint32_t mUSART1_GetFill (void) 
{
	uint32_t head = sizeof(USART1_DMARxBuff) - huart1.hdmarx->Instance->NDTR;
	if(head >= USART1_RxTail)
	{
		return (head - USART1_RxTail);
	}
	else
	{
	   return( sizeof(USART1_DMARxBuff) - USART1_RxTail + head);
	}
}

int32_t mUSART1_GetChar (uint8_t *c) 
{
	if(!uart1_initialized)
	{
		USART1_Init();
	}
	uint32_t head = sizeof(USART1_DMARxBuff) - huart1.hdmarx->Instance->NDTR;
	if(USART1_RxTail == head) 
	{
		return -1;				// ring buffer is empty, this should be atomic operation
	}
	*c = USART1_DMARxBuff[USART1_RxTail];
	USART1_RxTail++;
	if(USART1_RxTail >= sizeof(USART1_DMARxBuff)) 
	{
		USART1_RxTail = 0;
	}
	return 0;
}

void mUSART1_PutString (char *s) 
{
   while(*s)
	{
		mUSART1_PutChar(*s++);
	}
}



uint8_t mUSART1_PutChar (uint8_t c) 
{
	uint32_t timeOut = 1000000;
	if(!uart1_initialized)
	{
		USART1_Init();
	}
	while((RINGBUF_GetFill(&USART1_TxRingBuff) >= (USART1_TxRingBuff.size - 1)) && timeOut--);
	if(timeOut == 0xFFFFFFFF)
		huart1.gState = HAL_UART_STATE_READY;
	if(huart1.gState == HAL_UART_STATE_READY && (USART1->CR1 & USART_CR1_TXEIE) != USART_CR1_TXEIE)
	{
		USART1_DMATxBuff[0] = c;
		HAL_UART_Transmit_DMA(&huart1,USART1_DMATxBuff,1);
	}
	else
		RINGBUF_Put(&USART1_TxRingBuff,c);
	return 0;
}


void mUSART1_IRQHandler(void) 
{
	uint8_t c;
	uint16_t len = 0;
	
	if((USART1->SR & USART_SR_TXE))
	{
		HAL_UART_AbortTransmit_IT(&huart1);
		if(RINGBUF_GetFill(&USART1_TxRingBuff))
		{
			while(RINGBUF_Get(&USART1_TxRingBuff,&c) == 0)
			{
				USART1_DMATxBuff[len] = c;
				len++;
				if(len >= sizeof(USART1_DMATxBuff))
					break;
			}
			/* Tx process is ended, restore huart->gState to Ready */
			HAL_UART_Transmit_DMA(&huart1,USART1_DMATxBuff,len);
		}
		else if(USART1->SR & USART_SR_TC)
		{
			USART1->SR &= ~USART_SR_TC;
		}
		if(USART1->SR & (USART_SR_FE | USART_SR_PE | USART_SR_NE | USART_SR_ORE))
		{
			c = USART1->DR;
		}
	}

}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/




