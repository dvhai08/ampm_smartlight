#include "uart_config_task.h"
#include "uart5.h"
#include "uart4.h"


void CfgUART5SendData(CFG_PROTOCOL_TYPE *data);
void CfgUART4SendData(CFG_PROTOCOL_TYPE *data);

CFG_PROTOCOL_TYPE UART_ProtoRecv;
CFG_PROTOCOL_TYPE UART_ProtoSend;
uint8_t UART_DataBuff[UART_PACKET_SIZE];
PARSER_PACKET_TYPE UART_parserPacket;
Timeout_Type checkUart;
uint32_t uart5DataHead;
uint32_t uart4DataHead;

void UartConfigTaskInit(void)
{
	AppConfigTaskInit();
	InitTimeout(&checkUart,SYSTICK_TIME_MS(10));
	UART_ProtoRecv.dataPt = UART_DataBuff;
	UART_parserPacket.state = CFG_CMD_WAITING_SATRT_CODE;
	UART_parserPacket.lenMax = UART_PACKET_SIZE;
	
}


void UartConfigTask(void)
{
	uint8_t c;
	uint32_t u32Temp;
	//if(CheckTimeout(&checkUart) == SYSTICK_TIMEOUT)
	{
		//InitTimeout(&checkUart,SYSTICK_TIME_MS(1));	
		//UART5 process
		u32Temp = mUART5_GetFill();
		if((uart5DataHead != u32Temp || u32Temp == 0) 
			&& (u32Temp <= (mUART5_GetRxBufSize() - 10))
		)
		{
			uart5DataHead = u32Temp;
		}
		else
		{
			while(mUART5_GetChar(&c) == 0)
			{
					if(CfgParserPacket(&UART_parserPacket,&UART_ProtoRecv,c) == 0)
					{
						CfgProcessData(&UART_ProtoRecv,&UART_ProtoSend,UART_DataBuff,UART_parserPacket.lenMax - 4,1);
						CfgUART5SendData(&UART_ProtoSend);
					}
			}
		}
		//uart4 process
		u32Temp = mUART4_GetFill();
		if((uart4DataHead != u32Temp || u32Temp == 0) 
			&& (u32Temp <= (mUART4_GetRxBufSize() - 10))
		)
		{
			uart4DataHead = u32Temp;
		}
		else
		{
			while(mUART4_GetChar(&c) == 0)
			{
					//mUSART6_PutChar(c);
					if(CfgParserPacket(&UART_parserPacket,&UART_ProtoRecv,c) == 0)
					{
						CfgProcessData(&UART_ProtoRecv,&UART_ProtoSend,UART_DataBuff,UART_parserPacket.lenMax - 4,1);
						CfgUART4SendData(&UART_ProtoSend);
					}
			}
		}
		
//		while(mUSART6_GetChar(&c) == 0)
//		{
//			mUART4_PutChar(c);
//		}
	}
}

void CfgUART4SendData(CFG_PROTOCOL_TYPE *data)
{
	uint16_t i;
	if(data->length)
	{
		mUART4_PutChar(data->start);
		mUART4_PutChar(((uint8_t *)&data->length)[0]);
		mUART4_PutChar(((uint8_t *)&data->length)[1]);
		mUART4_PutChar(data->opcode);
		for(i = 0;i < data->length;i++)
			mUART4_PutChar(data->dataPt[i]);
		mUART4_PutChar(data->crc);
		data->length = 0;
	}
}

void CfgUART5SendData(CFG_PROTOCOL_TYPE *data)
{
	uint16_t i;
	if(data->length)
	{
		mUART5_PutChar(*(uint8_t *)&data->start);
		mUART5_PutChar(((uint8_t *)&data->length)[0]);
		mUART5_PutChar(((uint8_t *)&data->length)[1]);
		mUART5_PutChar(*(uint8_t *)&data->opcode);
		for(i = 0;i < data->length;i++)
			mUART5_PutChar(*(uint8_t *)&data->dataPt[i]);
		mUART5_PutChar(*(uint8_t *)&data->crc);
		data->length = 0;
	}
}

