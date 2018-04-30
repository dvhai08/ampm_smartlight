
#ifndef __UART_CONFIG_TASK_H__
#define __UART_CONFIG_TASK_H__

#include "app_config_task.h"
#include "uart4.h"
#include "uart5.h"
#define  UART_PACKET_SIZE		516

extern CFG_PROTOCOL_TYPE UART_ProtoRecv;
extern CFG_PROTOCOL_TYPE UART_ProtoSend;
extern uint8_t UART_DataBuff[UART_PACKET_SIZE];
extern PARSER_PACKET_TYPE UART_parserPacket;
extern Timeout_Type checkUart;
extern uint32_t uart1DataHead;
void CfgUART2SendData(CFG_PROTOCOL_TYPE *data);
void CfgUARTSendData(CFG_PROTOCOL_TYPE *data);
void UartConfigTaskInit(void);
void UartConfigTask(void);


#endif

