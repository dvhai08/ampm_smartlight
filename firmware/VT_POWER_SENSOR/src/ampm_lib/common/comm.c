/******************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.com.vn 
----------------------------------
AMPM ELECTRONICS EQUIPMENT TRADING COMPANY LIMITED.,
Add: 22 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
******************************************************************************/
#include "comm.h"
#include "main.h"
#include "drivers.h"
#include "system_config.h"
//#include "dlt645.h"

#define UARTTxCompIntEn() UART_TxCompIsrEnable(UART1)
#define UARTTxCompIntDis() UART_TxCompIsrDisable(UART1)
#define UARTTxIntEn() UART_TxIsrEnable(UART1)
#define UARTTxIntDis() UART_TxIsrDisable(UART1)
#define UARTSend(ch) UART_PutChar(UART1,ch)
#define UARTRecv() UART_GetChar(UART1)

uint8_t USART1_RxBuff[1024] = {0};
RINGBUF USART1_RxRingBuff;

uint8_t USART1_TxBuff[1024] = {0};
RINGBUF USART1_TxRingBuff;

RINGBUF *commTxRingBuf = &USART1_TxRingBuff;
RINGBUF *commRxRingBuf = &USART1_RxRingBuff;

void COMM_Init(void)
{
  RINGBUF_Init(&USART1_RxRingBuff,USART1_RxBuff,sizeof(USART1_RxBuff));
  RINGBUF_Init(&USART1_TxRingBuff,USART1_TxBuff,sizeof(USART1_TxBuff));
}

uint8_t USART1_PutChar(uint8_t c)
{
  uint32_t timeOut = 10000000;
	GPIO_Set(GPIOH,PIN7);
  while((RINGBUF_GetFill(&USART1_TxRingBuff) >= USART1_TxRingBuff.size) && timeOut--){}
  RINGBUF_Put(&USART1_TxRingBuff,c);
  if(!(UART1_C2 & UART_C2_TIE_MASK)){
    if(RINGBUF_Get(&USART1_TxRingBuff,&c) == 0)
    {
      UARTSend(c);
      UARTTxIntEn();
			UARTTxCompIntEn();
    }
  }
  return c;
}

void USART1_PutString(uint8_t *s)
{
  while(*s)
  {
    USART1_PutChar(*s++);
  }
}

void COMM_Putc(uint8_t c)
{
  USART1_PutChar(c);
}
void COMM_Puts(uint8_t *s)
{
  USART1_PutString(s);
}


uint8_t tx_cnt = 0;
void uart_callback (UART_CALLBACK_SRC module, UART_CALLBACK_TYPE type, int8 status)
{
  uint8_t ch;
  //GPIO_Tgl(GPIOF,PIN1);
   if (module == UART1_CALLBACK)
   {
    if (type == TX_CALLBACK) 
    {
     if(RINGBUF_GetFill(&USART1_TxRingBuff))
     {
      while(UART1_TCFIFO < 8)
      {
        if(RINGBUF_Get(&USART1_TxRingBuff,&ch) == 0){
          UARTSend(ch);
        }
        else {
        //  UARTTxIntDis();
          break;
        }
      }
     }
     else if(status & UART_S1_TC_MASK)
     {
			GPIO_Clr(GPIOH,PIN7);
			UARTTxCompIntDis();
      UARTTxIntDis();
     }
		 else if(status & UART_S1_TDRE_MASK)
		 {
			UARTTxIntDis();
		 }
    }
    if (type == RX_CALLBACK)
    {
      do{
        ch = UARTRecv();
        RINGBUF_Put(&USART1_RxRingBuff,ch);
//        if(!(ramappcfg.feature & FEATURE_GSM_ENABLE) && (!(meter_status & PASSWORD_OK)) && (!(app_status & PASSWORD_OK))){
//          host_uart_callback(ch);
//        }
      }while(UART1_RCFIFO);
    }
   }
}