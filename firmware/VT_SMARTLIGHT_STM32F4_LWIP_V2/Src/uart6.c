
/**
* @file
*         uart6.c
* @author
*         Nguyen Van Hai <hainv75@viettel.com.vn>
* @brief  UART DMA ring buffer layer
*	@date	
*					2017-11-10
*/
#include "uart6.h"


uint8_t USART6_TxBuff[256];
RINGBUF USART6_TxRingBuff;
uint8_t USART6_DMATxBuff[256];
uint8_t USART6_DMARxBuff[256];
uint32_t USART6_RxTail = 0;

static __IO uint8_t uart6_initialized = 0;

extern UART_HandleTypeDef huart6;

uint8_t USART6_IsOk(void)
{
	return uart6_initialized;
}

void USART6_Init(void)
{
	
	HAL_UART_Receive_DMA(&huart6,USART6_DMARxBuff,sizeof(USART6_DMARxBuff));
	RINGBUF_Init(&USART6_TxRingBuff,USART6_TxBuff,sizeof(USART6_TxBuff));
	uart6_initialized = 1;
}

//void USART6_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//		uint8_t c;
//		uint16_t len = 0;
//	if(RINGBUF_GetFill(&USART6_TxRingBuff))
//	{
//		while(RINGBUF_Get(&USART6_TxRingBuff,&c) == 0)
//		{
//			USART6_DMATxBuff[len] = c;
//			len++;
//			if(len >= sizeof(USART6_DMATxBuff))
//				break;
//		}
//		huart->hdmatx->Instance->NDTR = len;
//		HAL_UART_Transmit_DMA(huart,USART6_DMATxBuff,len);
//	}
//	else
//	{
//		HAL_UART_AbortTransmit_IT(huart);
//	}
//}

uint32_t mUSART6_GetFill (void) 
{
	uint32_t head = sizeof(USART6_DMARxBuff) - huart6.hdmarx->Instance->NDTR;
	if(head >= USART6_RxTail)
	{
		return (head - USART6_RxTail);
	}
	else
	{
	   return( sizeof(USART6_DMARxBuff) - USART6_RxTail + head);
	}
}

int32_t mUSART6_GetChar (uint8_t *c) 
{
	
	if(!uart6_initialized)
	{
		USART6_Init();
	}
	uint32_t head = sizeof(USART6_DMARxBuff) - huart6.hdmarx->Instance->NDTR;
	if(USART6_RxTail == head) 
	{
		return -1;				// ring buffer is empty, this should be atomic operation
	}
	*c = USART6_DMARxBuff[USART6_RxTail];
	USART6_RxTail++;
	if(USART6_RxTail >= sizeof(USART6_DMARxBuff)) 
	{
		USART6_RxTail = 0;
	}
	return 0;
}

void mUSART6_PutString (char *s) 
{
   while(*s)
	{
		mUSART6_PutChar(*s++);
	}
}



uint8_t mUSART6_PutChar (uint8_t c) 
{
	uint32_t timeOut = 1000000;
	if(!uart6_initialized)
	{
		USART6_Init();
	}
	HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_SET);
	while((RINGBUF_GetFill(&USART6_TxRingBuff) >= (USART6_TxRingBuff.size - 1)) && timeOut--);
	if(timeOut == 0xFFFFFFFF)
		huart6.gState = HAL_UART_STATE_READY;
	if(huart6.gState == HAL_UART_STATE_READY && (USART6->CR1 & USART_CR1_TXEIE) != USART_CR1_TXEIE)
	{
		USART6_DMATxBuff[0] = c;
		HAL_UART_Transmit_DMA(&huart6,USART6_DMATxBuff,1);
		USART6->CR1 |= USART_CR1_TCIE;
	}
	else
		RINGBUF_Put(&USART6_TxRingBuff,c);
	return 0;
}


void mUSART6_IRQHandler(void) 
{
	uint8_t c;
	uint16_t len = 0;
	
	if((USART6->SR & USART_SR_TXE))
	{
		HAL_UART_AbortTransmit_IT(&huart6);
		if(RINGBUF_GetFill(&USART6_TxRingBuff))
		{
			while(RINGBUF_Get(&USART6_TxRingBuff,&c) == 0)
			{
				USART6_DMATxBuff[len] = c;
				len++;
				if(len >= sizeof(USART6_DMATxBuff))
					break;
			}
			/* Tx process is ended, restore huart->gState to Ready */
			HAL_UART_Transmit_DMA(&huart6,USART6_DMATxBuff,len);
			//USART6->CR1 |= USART_CR1_TCIE;
		}
		else if(USART6->SR & USART_SR_TC)
		{
			USART6->SR &= ~USART_SR_TC;
			HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);
			//HAL_GPIO_TogglePin(LED_SYS_GPIO_Port, LED_SYS_Pin);
		}
		if(USART6->SR & (USART_SR_FE | USART_SR_PE | USART_SR_NE | USART_SR_ORE))
		{
			c = USART6->DR;
		}
	}
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/




