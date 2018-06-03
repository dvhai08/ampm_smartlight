#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "lib/list.h"
#include "lib/sys_tick.h"
#include "lib/sys_time.h"
#include "system_config.h"
#include "lib/data_cmp.h"
//#include "ampm_ff/ampm_ff.h"
#include "lib/ringbuf.h"
#include "uart4.h"
#include "system_config.h"
#include "elster_tasks.h"
#define ELSTER_DBG(x)	//printf(x)
//extern RINGBUF App_TxRingBuf;
//extern RINGBUF UART_RxRingBuf;
//extern uint8_t ELSTER_Puts(const char *s);
#define NUMBER_OF_RETRIES	3
#define TIME_OUT_METER		1000	

#define ELSTER_RES_OK	1
#define ELSTER_RES_FAIL	0

static const uint8_t ACK = 0x06,SOH = 0x01,STX = 0x02,ETX = 0x03;
uint8_t m_buffer_rb[255];
uint8_t m_buffer_rb_len;
char m_buffer_tb[255];
uint8_t packetsHistory = 10;
uint32_t fre_sect;
uint8_t ampm_f_close_flag = 0;
//AMPM_FIL meterFile;
//AMPM_FIL meterFileBle;
DATE_TIME meterTime;
uint8_t dataSaveTo = SAVE_TO_FLASH;
uint8_t fileName = 0;
uint8_t readFromFlash = 0;
uint8_t powerOnReadCnt = 1;
typedef enum{
	METER_ELSTER_PHASE_IDLE = 0,
	METER_ELSTER_PHASE_START_INIT,
	METER_ELSTER_PHASE_START_READ,
	METER_ELSTER_PHASE_START_READING,

}METER_ELSTER_PHASE_TYPE;



METER_ELSTER_READ_TYPE meterReadTypeCmd;
uint8_t meterReadFlag = 0;
METER_ELSTER_PHASE_TYPE meterElsterPhase;
uint8_t meter_P2[9];
uint8_t meter_P0[9];
uint16_t pauseReadMeterTime = 0;

#define	ELSTER_WRITE_FILE		1

#define app_uart_puts mUART4_PutString

LIST(elsterCmdList);

typedef struct{
	void *appStruct;
	uint32_t (*send_data_callback)(void);
	uint32_t (*recv_data_callback)(uint8_t *buf,uint16_t len);
	uint8_t retryNum;
	uint16_t timeout;/*in mS*/
	uint16_t delay;/*delay when retry: in mS*/
}ELSTER_CMD_PACKET_TYPE;


	

typedef struct{
	uint8_t *opcode;
	uint8_t *str_command;
	uint8_t compare;
	uint8_t writefileFlag;
	uint8_t *str_printf;
}ELSTER_WRITE_CMD_TYPE;





char meterType[16];
METER_CURRENT_VALUE_TYPE currentValue;
METER_INSTANTANEOUS_VALUE_TYPE instantaneousValue;

uint16_t historyLength;

const char historyInfoBuf[16][16] = {
	"ImportkWh",
	"ExportkWh",
	"Q1kVarh",
	"Q2kVarh",
	"Q3kVarh",
	"Q4kVarh",
	"Varh",
	"CD1",
	"CD2",
	"CD3",
	"Im-Rate1",
  "Im-Rate2",
  "Im-Rate3",
  "Ex-Rate1",
  "Ex-Rate2",
	"Ex-Rate3"
};



typedef struct{
	uint8_t *cmd;
	uint8_t *res_cmp;
}ELSTER_WRITE_CMD_TYPE1;

typedef struct{
	uint8_t *opcode;
	uint8_t *str_command;
	uint8_t compare;
	uint8_t writefileFlag;
	uint8_t *str_printf;
	uint8_t cmdStartNum;
	uint8_t cmdEndNum;
}ELSTER_WRITE_CMD_TYPE2;

typedef enum{
	ELSTER_CMD_SEND = 0,
	ELSTER_CMD_WAIT,
	ELSTER_CMD_DELAY,
	ELSTER_CMD_ERROR,
	ELSTER_CMD_OK
} ELSTER_CMD_PHASE_TYPE;

typedef struct{
	struct ELSTER_CMD_LIST_TYPE *next;
	ELSTER_CMD_PACKET_TYPE *pt;
}ELSTER_CMD_LIST_TYPE;
void ElsterCmd_Task(void);
uint8_t ElsterCmdTask_IsIdle(void);
uint8_t elsterParse(uint8_t *cmd,char *data);
void ElsterCmd0Add(ELSTER_WRITE_CMD_TYPE1 *cmd,uint8_t *buf,uint8_t *cmp);
uint8_t ElsterCmdCheckList_IsEmpty(void);
uint8_t ElsterCmdTask_SendCmd(
	void *appStruct,
	uint32_t (*send_data_callback)(void),
	uint32_t (*recv_data_callback)(uint8_t *buf,uint16_t len),
	uint8_t retryNum,
	uint16_t timeout,	/*in mS*/
	uint16_t delay	/*delay when retry: in mS*/);
//Elster CMD
uint32_t ElsterWriteCmd(void);
uint32_t ElsterRecvData(uint8_t *buf,uint16_t len);

uint32_t ElsterSendCmd0(void);
uint32_t ElsterRecvCmd0(uint8_t *buf,uint16_t len);

uint32_t ElsterWriteCmd1(void);
uint32_t ElsterRecvData1(uint8_t *buf,uint16_t len);

uint32_t ElsterWriteCmd2(void);
uint32_t ElsterRecvData2(uint8_t *buf,uint16_t len);

uint32_t ElsterWriteCmdP2(void);
uint32_t ElsterRecvCmdP2(uint8_t *buf,uint16_t len);

uint32_t ElsterWriteCmdStart(void);
uint32_t ElsterRecvCmdStart(uint8_t *buf,uint16_t len);

uint32_t ElsterWriteCmdStop(void);
uint32_t ElsterRecvCmdStop(uint8_t *buf,uint16_t len);

uint32_t ElsterWriteCmdGetP0(void);
uint32_t ElsterRecvCmdGetP0(uint8_t *buf,uint16_t len);


ELSTER_CMD_PHASE_TYPE ElsterCmdTask_GetPhase(void);
void ElsterCmdAddToList(ELSTER_CMD_LIST_TYPE *cmd);

const ELSTER_WRITE_CMD_TYPE elsteWriterCmd0 = {"P2","(0000000000000000)",ACK,NULL,NULL};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd1 = {"R1","798001(10)",ETX,ELSTER_WRITE_FILE,"798001"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd2 = {"R1","795001(08)",ETX,ELSTER_WRITE_FILE,"795001"};

const ELSTER_WRITE_CMD_TYPE elsteWriterCmdTime = {"R1","861001(07)",ETX,ELSTER_WRITE_FILE,"861001"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd3 = {"R1","507001(40)",ETX,ELSTER_WRITE_FILE,"507001"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd4 = {"R1","507002(10)",ETX,ELSTER_WRITE_FILE,"507002"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd5 = {"R1","508001(40)",ETX,ELSTER_WRITE_FILE,"508001"};
//Dong, Ap.Goc, tan so
//
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd6 = {"W1","605001(1A2A4A00)",ACK,NULL,NULL};
ELSTER_WRITE_CMD_TYPE2 elsteWriterCmd7 = {"R1","605001(04)",ETX,NULL,NULL,0,10};
const ELSTER_WRITE_CMD_TYPE2 elsteWriterCmd7Reload = {"R1","605001(04)",ETX,NULL,NULL,0,10};
//A-> Current
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd8 = {"R1","606001(1C)",ETX,ELSTER_WRITE_FILE,"60600A"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd9 = {"W1","605001(1B2B4B00)",ACK,NULL,NULL};
//B->Voltage
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd10 = {"R1","606001(1C)",ETX,ELSTER_WRITE_FILE,"60600B"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd11 = {"W1","605001(18284800)",ACK,NULL,NULL};
//8->frequency
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd12 = {"R1","606001(1C)",ETX,ELSTER_WRITE_FILE,"606008"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd13 = {"W1","605001(19294900)",ACK,NULL,NULL};
//9->angle
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd14 = {"R1","606001(1C)",ETX,ELSTER_WRITE_FILE,"606009"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd14_1 = {"W1","605001(13234303)",ACK,NULL,NULL};
//3->factor
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd14_2 = {"R1","606001(1C)",ETX,ELSTER_WRITE_FILE,"606003"};
//Secutity
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd15 = {"R1","701001(2E)",ETX,ELSTER_WRITE_FILE,"701001"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd16 = {"R1","455001(2E)",ETX,ELSTER_WRITE_FILE,"455001"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd17 = {"R1","693001(3F)",ETX,ELSTER_WRITE_FILE,"693001"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd18 = {"R1","694001(2E)",ETX,ELSTER_WRITE_FILE,"694001"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd19 = {"R1","695001(16)",ETX,ELSTER_WRITE_FILE,"695001"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd20 = {"R1","702001(2E)",ETX,ELSTER_WRITE_FILE,"702001"};
//moi them
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd21 = {"R1","680001(16)",ETX,ELSTER_WRITE_FILE,"680001"};
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd22 = {"R1","700001(16)",ETX,ELSTER_WRITE_FILE,"700001"};
//Chot Thang
ELSTER_WRITE_CMD_TYPE2 elsteWriterCmd23 = {"R1","543001(40)",ETX,ELSTER_WRITE_FILE,"543001",2,10};
const ELSTER_WRITE_CMD_TYPE2 elsteWriterCmd23Reload = {"R1","543001(40)",ETX,ELSTER_WRITE_FILE,"543001",2,10};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd24 = {"R1","543002(40)",ETX,ELSTER_WRITE_FILE,"543002"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd25 = {"R1","543003(40)",ETX,ELSTER_WRITE_FILE,"543003"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd26 = {"R1","543004(40)",ETX,ELSTER_WRITE_FILE,"543004"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd27 = {"R1","543005(40)",ETX,ELSTER_WRITE_FILE,"543005"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd28 = {"R1","543006(40)",ETX,ELSTER_WRITE_FILE,"543006"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd29 = {"R1","543007(40)",ETX,ELSTER_WRITE_FILE,"543007"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd30 = {"R1","543008(40)",ETX,ELSTER_WRITE_FILE,"543008"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd31 = {"R1","543009(40)",ETX,ELSTER_WRITE_FILE,"543009"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd32 = {"R1","54300a(40)",ETX,ELSTER_WRITE_FILE,"54300a"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd33 = {"R1","54300b(40)",ETX,ELSTER_WRITE_FILE,"54300b"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd34 = {"R1","54300c(40)",ETX,ELSTER_WRITE_FILE,"54300c"};
//const ELSTER_WRITE_CMD_TYPE elsteWriterCmd35 = {"R1","54300d(40)",ETX,ELSTER_WRITE_FILE,"54300d"};
//load profile
const ELSTER_WRITE_CMD_TYPE elsteWriterCmd36 = {"W1","551001(0200)",ACK,NULL,NULL};
ELSTER_WRITE_CMD_TYPE2 elsteWriterCmd37 = {"R1","551001(02)",ETX,NULL,NULL,1,32};

const ELSTER_WRITE_CMD_TYPE2 elsteWriterCmd37Reload = {"R1","551001(02)",ETX,NULL,NULL,1,32};


ELSTER_CMD_PACKET_TYPE cmdElsterSend;

const ELSTER_CMD_PACKET_TYPE elsterCmdRead0[] = {
	NULL,ElsterWriteCmdStop,NULL,0,1000,250,
	NULL,ElsterWriteCmdStart,ElsterRecvCmdStart,3,1000,250,
	NULL,ElsterWriteCmdGetP0,ElsterRecvCmdGetP0,3,3000,250,
	(void *)&elsteWriterCmd0,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	NULL,ElsterWriteCmdP2,ElsterRecvCmdP2,3,3000,250,
	(void *)&elsteWriterCmdTime,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd3,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd4,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd5,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	NULL,ElsterWriteCmdStop,NULL,0,3000,250,
};

const ELSTER_CMD_PACKET_TYPE elsterCmdRead1[] = {
	NULL,ElsterWriteCmdStop,NULL,0,1000,250,
	NULL,ElsterWriteCmdStart,ElsterRecvCmdStart,3,1000,250,
	NULL,ElsterWriteCmdGetP0,ElsterRecvCmdGetP0,3,3000,250,
	(void *)&elsteWriterCmd0,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	NULL,ElsterWriteCmdP2,ElsterRecvCmdP2,3,3000,250,
	(void *)&elsteWriterCmdTime,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd3,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd4,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd5,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd6,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd8,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd9,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd10,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd11,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd12,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd13,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd14,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	(void *)&elsteWriterCmd14_1,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd14_2,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	NULL,ElsterWriteCmdStop,NULL,0,3000,250,
};

const ELSTER_CMD_PACKET_TYPE elsterCmdRead2[] = {
	NULL,ElsterWriteCmdStop,NULL,0,1000,250,
	NULL,ElsterWriteCmdStart,ElsterRecvCmdStart,3,1000,250,
	NULL,ElsterWriteCmdGetP0,ElsterRecvCmdGetP0,3,3000,250,
	(void *)&elsteWriterCmd0,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	NULL,ElsterWriteCmdP2,ElsterRecvCmdP2,3,3000,250,
	(void *)&elsteWriterCmdTime,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd3,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd4,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd5,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd6,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd8,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd9,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd10,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd11,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd12,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd13,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd14,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	(void *)&elsteWriterCmd14_1,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd14_2,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	(void *)&elsteWriterCmd23,ElsterWriteCmd2,ElsterRecvData2,3,3000,300,
	NULL,ElsterWriteCmdStop,NULL,0,3000,250,
};

//Dang nhap
const ELSTER_CMD_PACKET_TYPE elsterCmdGroup0[] = {
	NULL,ElsterWriteCmdStop,NULL,0,1000,250,
	NULL,ElsterWriteCmdStart,ElsterRecvCmdStart,3,1000,250,
	NULL,ElsterWriteCmdGetP0,ElsterRecvCmdGetP0,3,3000,250,
	(void *)&elsteWriterCmd0,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd1,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd2,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	NULL,ElsterWriteCmdP2,ElsterRecvCmdP2,3,3000,250,
	(void *)&elsteWriterCmdTime,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd3,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd4,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd5,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd6,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd8,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd9,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd10,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd11,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd12,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd13,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd14,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	(void *)&elsteWriterCmd14_1,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	(void *)&elsteWriterCmd7,ElsterWriteCmd1,ElsterRecvData1,3,3000,300,
	(void *)&elsteWriterCmd14_2,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	(void *)&elsteWriterCmd15,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd16,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd17,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd18,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd19,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd20,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd21,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd22,ElsterWriteCmd,ElsterRecvData,3,3000,100,
	(void *)&elsteWriterCmd23,ElsterWriteCmd2,ElsterRecvData2,3,3000,300,
	(void *)&elsteWriterCmd36,ElsterWriteCmd,ElsterRecvData,3,3000,300,
	(void *)&elsteWriterCmd37,ElsterWriteCmd2,ElsterRecvData2,3,3000,300,
	NULL,ElsterWriteCmdStop,NULL,0,3000,250,
};

ELSTER_CMD_LIST_TYPE elsterCmdListGroup0[sizeof(elsterCmdGroup0)/sizeof(ELSTER_CMD_PACKET_TYPE)];
ELSTER_CMD_LIST_TYPE elsterCmdListRead0[sizeof(elsterCmdRead0)/sizeof(ELSTER_CMD_PACKET_TYPE)];
ELSTER_CMD_LIST_TYPE elsterCmdListRead1[sizeof(elsterCmdRead1)/sizeof(ELSTER_CMD_PACKET_TYPE)];
ELSTER_CMD_LIST_TYPE elsterCmdListRead2[sizeof(elsterCmdRead2)/sizeof(ELSTER_CMD_PACKET_TYPE)];


ELSTER_CMD_PHASE_TYPE elsterCmdPhase = ELSTER_CMD_OK;
ELSTER_WRITE_CMD_TYPE1 elsterSendCmd;
Timeout_Type tElsterTimeout;
uint8_t elsterCmdTimeoutCount = 0;
Timeout_Type tElsterCmdTimeout;
Timeout_Type tElsterReadParaTimeout;
Timeout_Type tElsterPhaseTimeout;
uint8_t meterElsterReadOk = 0;
void JsonSave(char *s);

void creat_P2(uint8_t *remote_password,uint8_t *meter_P0,uint8_t *meter_P2)
{	uint8_t remote_password_length = 8,i,j;
	for (i=0; i < remote_password_length; i++) {
		meter_P2[i] = meter_P0[remote_password_length-1] ^ remote_password[remote_password_length-1]; 
		for (j=0; j<i+1; j++) {
			meter_P2[i] += meter_P0[j] ^ remote_password[j]; //data[j]; 
		}															
	}
	meter_P0[8] = '\0';
	meter_P2[8] = '\0';
	remote_password[8] = '\0';
}

uint8_t calc_bcc_data(uint8_t *buffer,uint8_t len)
{
	uint8_t i;
	uint8_t data_out = 0;
	for(i = 0 ; i < len; i++)
	{
		data_out = data_out ^ (uint8_t)buffer[i];
	}
	return data_out;
}

uint8_t  calc_bcc_funs (const  char *format, ...)
{
	static char buffer[255];
	va_list     vArgs;		    
	unsigned int	len,i;
	uint8_t data_out = 0;
	va_start(vArgs, format);
	vsprintf((char *)buffer, (char const *)format, vArgs);
	va_end(vArgs);
	len =  strlen(buffer);
	if(len < 255)
	{
		for(i = 0 ; i < len; i++)
		{
			data_out = data_out ^ (uint8_t)buffer[i];
		}
	}
	return data_out;
}


uint8_t elsterResOk[4];

uint32_t MeterElterTasks_IsIdle(void)
{
	if(meterElsterPhase == METER_ELSTER_PHASE_IDLE)
		return 1;
	return 0;
}

uint8_t elsterReadParaTryNum = 0;

void MeterElterRead(METER_ELSTER_READ_TYPE cmd)
{
	meterElsterPhase = METER_ELSTER_PHASE_IDLE;
	UART4_Setup(115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE);
	//if(MeterElterTasks_IsIdle())
	{
		meterReadTypeCmd = cmd;
		meterReadFlag = 1;
		meterElsterReadOk = 0;
	}
}

void MeterElterTasksInit(void)
{
	if(sysCfg.reportInterval <= 300)
		InitTimeout(&tElsterReadParaTimeout,SYSTICK_TIME_SEC(300)); 
	else
		InitTimeout(&tElsterReadParaTimeout,SYSTICK_TIME_SEC((uint32_t)sysCfg.reportInterval)); 
	UART4_Setup(9600,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_EVEN);
}
void MeterElterTasksStart(void)
{
	//InitTimeout(&tElsterReadParaTimeout,SYSTICK_TIME_SEC((uint32_t)deviceCfg.meterReadInterval*60)); 
	InitTimeout(&tElsterReadParaTimeout,SYSTICK_TIME_MS(1)); 

}

void MeterElsterTasksClose(void)
{
	meterElsterPhase = METER_ELSTER_PHASE_IDLE;
	UART4_Setup(115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE);
}


void MeterElterTasks(void)
{
	
	char buf[256];
	//uint8_t *u8Pt0,*u8Pt1;
	uint16_t i;
	uint32_t u32T;
//	UART4_Setup(9600,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_EVEN);
//	while(1)
//	{
//		SysTick_DelayMs(1000);
//		sprintf((char *)m_buffer_tb,"%cB0%cq",SOH,ETX);
//				app_uart_puts(m_buffer_tb);
//	}
//	
//	return;
	switch(meterElsterPhase)
	{
		case METER_ELSTER_PHASE_IDLE:
		{
			if(ampm_f_close_flag)
			{
				ELSTER_DBG("Close:");
				SysTick_DelayMs(100);
				sprintf((char *)m_buffer_tb,"%cB0%cq",SOH,ETX);
				app_uart_puts(m_buffer_tb);
				SysTick_DelayMs(100);
				sprintf((char *)m_buffer_tb,"%cB0%cq",SOH,ETX);
				app_uart_puts(m_buffer_tb);
				ELSTER_DBG(buf);
				ELSTER_DBG("\n");
			}
			
			if(elsterReadParaTryNum)
			{
				elsterReadParaTryNum--;
				meterElsterPhase = METER_ELSTER_PHASE_START_INIT;
			}
			
			if(meterReadFlag)
			{
				meterReadFlag = 0;
				elsterReadParaTryNum = 1;
				meterElsterPhase = METER_ELSTER_PHASE_START_INIT;
			}
			else if(CheckTimeout(&tElsterReadParaTimeout) == SYSTICK_TIMEOUT)
			{
				if(powerOnReadCnt)
				{
					powerOnReadCnt--;
					InitTimeout(&tElsterReadParaTimeout,SYSTICK_TIME_SEC(10)); 
				}
				else
				{
					if(sysCfg.reportInterval <= 300)
						InitTimeout(&tElsterReadParaTimeout,SYSTICK_TIME_SEC(300)); 
					else
						InitTimeout(&tElsterReadParaTimeout,SYSTICK_TIME_SEC((uint32_t)sysCfg.reportInterval)); 
				}
				//InitTimeout(&tElsterReadParaTimeout,SYSTICK_TIME_SEC(60)); 
				elsterReadParaTryNum = 1;
				readFromFlash = 0;
				dataSaveTo = SAVE_TO_FLASH;
				meterReadTypeCmd = METER_READ_1;
				meterElsterPhase = METER_ELSTER_PHASE_START_INIT;
			}
	
			
			break;
		}
		case METER_ELSTER_PHASE_START_INIT:
		{
			UART4_Setup(9600,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_EVEN);
			list_init(elsterCmdList);
			meterElsterPhase = METER_ELSTER_PHASE_START_READ;
			break;
		}
		case METER_ELSTER_PHASE_START_READ:
		{
			{
				if(ElsterCmdTask_IsIdle())
				{
					elsteWriterCmd7 = elsteWriterCmd7Reload;
					elsteWriterCmd37 = elsteWriterCmd37Reload;
					elsteWriterCmd23 = elsteWriterCmd23Reload;
					if(meterReadTypeCmd == METER_READ_0)
					{
						for(i = 0;i < sizeof(elsterCmdRead0)/sizeof(ELSTER_CMD_PACKET_TYPE);i++)
						{
							elsterCmdListRead0[i].pt = (ELSTER_CMD_PACKET_TYPE *)&elsterCmdRead0[i];
							ElsterCmdAddToList(&elsterCmdListRead0[i]);
						}
					}
					else if(meterReadTypeCmd == METER_READ_1)
					{
						for(i = 0;i < sizeof(elsterCmdRead1)/sizeof(ELSTER_CMD_PACKET_TYPE);i++)
						{
							elsterCmdListRead1[i].pt = (ELSTER_CMD_PACKET_TYPE *)&elsterCmdRead1[i];
							ElsterCmdAddToList(&elsterCmdListRead1[i]);
						}
					}
					else if(meterReadTypeCmd == METER_READ_2)
					{
						for(i = 0;i < sizeof(elsterCmdRead2)/sizeof(ELSTER_CMD_PACKET_TYPE);i++)
						{
							elsterCmdListRead2[i].pt = (ELSTER_CMD_PACKET_TYPE *)&elsterCmdRead2[i];
							ElsterCmdAddToList(&elsterCmdListRead2[i]);
						}
					}
					else
					{
						for(i = 0;i < sizeof(elsterCmdGroup0)/sizeof(ELSTER_CMD_PACKET_TYPE);i++)
						{
							elsterCmdListGroup0[i].pt = (ELSTER_CMD_PACKET_TYPE *)&elsterCmdGroup0[i];
							ElsterCmdAddToList(&elsterCmdListGroup0[i]);
						}
					}
					elsterCmdPhase = ELSTER_CMD_OK;
					InitTimeout(&tElsterCmdTimeout,SYSTICK_TIME_SEC(120));
					meterElsterPhase = METER_ELSTER_PHASE_START_READING;
				}
			}
			break;
		}
		case METER_ELSTER_PHASE_START_READING:
		{
			{
				if(ElsterCmdCheckList_IsEmpty() &&  ElsterCmdTask_GetPhase() == ELSTER_CMD_OK)
				{
					elsterParse("END",NULL);
					ampm_f_close_flag = 0;
					meterElsterPhase = METER_ELSTER_PHASE_IDLE;
					meterElsterReadOk = 1;
					UART4_Setup(115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE);
					elsterReadParaTryNum = 0;
					fileName ^= 1;
				}
				else if(ElsterCmdTask_GetPhase() == ELSTER_CMD_ERROR)
				{
					ampm_f_close_flag = 0;
					meterElsterPhase = METER_ELSTER_PHASE_IDLE;
					UART4_Setup(115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE);
				}
				if(CheckTimeout(&tElsterCmdTimeout) == SYSTICK_TIMEOUT)
				{
					ampm_f_close_flag = 0;
					meterElsterPhase = METER_ELSTER_PHASE_IDLE;
					UART4_Setup(115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE);
				}
			}
			break;
		}
	}
	ElsterCmd_Task();
}

void ElsterCmdAddToList(ELSTER_CMD_LIST_TYPE *cmd)
{
	list_add(elsterCmdList, cmd);
}

uint8_t ElsterCmdCheckList_IsEmpty(void)
{
	if(elsterCmdList[0] == NULL)
		return 1;
	return 0;
}

uint8_t ElsterCmdTask_IsIdle(void)
{
	switch(elsterCmdPhase)
	{
		case ELSTER_CMD_ERROR:
		case ELSTER_CMD_OK:
			return 1;
		default:
			return 0;
	}
}

ELSTER_CMD_PHASE_TYPE ElsterCmdTask_GetPhase(void)
{
	return elsterCmdPhase;
}



//Send Start
uint32_t ElsterWriteCmdStart(void)
{
	
	sprintf((char *)m_buffer_tb,"/?%s!\r\n",deviceCfg.outstationNumber);
	app_uart_puts(m_buffer_tb);
	return 1;
}
uint32_t ElsterRecvCmdStart(uint8_t *buf,uint16_t len)
{
	if((buf[len-1] == '\n') && (buf[len - 2] == '\r'))
	{
		buf[len - 2] = '\0';
		meterTime = sysTime; 
		ampm_f_close_flag = 1;
		elsterParse("Type",(char *)&buf[1]);
		return 1;
	}
	return 0;
}


//Send Stop
uint32_t ElsterWriteCmdStop(void)
{
	
	sprintf((char *)m_buffer_tb,"%cB0%cq",SOH,ETX);
	app_uart_puts(m_buffer_tb);
	return 1;
}
uint32_t ElsterRecvCmdStop(uint8_t *buf,uint16_t len)
{
	return 1;
}
//Get P0
uint32_t ElsterWriteCmdGetP0(void)
{
	sprintf((char *)m_buffer_tb,"%c051\r\n",ACK);
	app_uart_puts(m_buffer_tb);
	return 1;
}
uint32_t ElsterRecvCmdGetP0(uint8_t *buf,uint16_t len)
{
	uint8_t *temp_pt,i;
	uint8_t buffer_temp[32];
	if(buf[len-2] == ETX)
	{
		if(calc_bcc_data(&buf[1],len - 2) ==  buf[len - 1])
		{
			temp_pt = (uint8_t *)strstr((const char *)buf,"(");
			if(temp_pt != NULL)
			{
				for(i = 0 ; i < 16 ; i++)
				{
					if((temp_pt[i+1] >= '0') && ((temp_pt[i+1] <= '9')))
					{
						buffer_temp[i] = temp_pt[i+1] - '0';
					}
					else 
					{
						buffer_temp[i] = temp_pt[i+1] - 'A' + 10;
					}
				}
				for(i = 0 ; i < 8 ; i++)
				{
					meter_P0[i] = buffer_temp[i*2]<<4 | buffer_temp[i*2+1];
				}
				meter_P0[i] = '\0';
				return 1;
			}
		}
		
	}
	return 0;
}
//Send P2
uint32_t ElsterWriteCmdP2(void)
{
	uint8_t calc_bcc = 0;
	creat_P2(deviceCfg.elsterPassword,meter_P0,meter_P2);
	calc_bcc = 	calc_bcc_funs("P2%c(%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X)%c",STX,meter_P2[0],meter_P2[1],meter_P2[2],meter_P2[3],meter_P2[4],meter_P2[5],meter_P2[6],meter_P2[7],ETX);
	sprintf((char *)m_buffer_tb,"%cP2%c(%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X)%c%c",SOH,STX,meter_P2[0],meter_P2[1],meter_P2[2],meter_P2[3],meter_P2[4],meter_P2[5],meter_P2[6],meter_P2[7],ETX,calc_bcc);
	app_uart_puts(m_buffer_tb);
	return 1;
}
uint32_t ElsterRecvCmdP2(uint8_t *buf,uint16_t len)
{
	if((buf[0] == ACK)) //ACK
	{
		return 1;
	}
	return 0;
}
//Send a command with ElsterCmdTask_SendCmd
uint8_t ElsterCmdTask_SendCmd(
	void *appStruct,
	uint32_t (*send_data_callback)(void),
	uint32_t (*recv_data_callback)(uint8_t *buf,uint16_t len),
	uint8_t retryNum,
	uint16_t timeout,	/*in mS*/
	uint16_t delay	/*delay when retry: in mS*/)
{
	if(ElsterCmdTask_IsIdle())
	{
		cmdElsterSend.appStruct = appStruct;
		cmdElsterSend.send_data_callback = send_data_callback;
		cmdElsterSend.recv_data_callback = recv_data_callback;
		cmdElsterSend.retryNum = retryNum;
		cmdElsterSend.timeout = timeout;
		cmdElsterSend.delay = delay;
		InitTimeout(&tElsterTimeout, SYSTICK_TIME_MS(cmdElsterSend.delay = delay));
		elsterCmdPhase = ELSTER_CMD_SEND;
		return ELSTER_RES_OK;
	}
	return ELSTER_RES_FAIL;
}
COMPARE_TYPE cmpElsterCmpOk;
void ElsterCmd0Add(ELSTER_WRITE_CMD_TYPE1 *cmd,uint8_t *buf,uint8_t *cmp)
{
	cmd->cmd = buf;
	cmd->res_cmp = cmp;
}
uint32_t ElsterSendCmd0(void)
{
	ELSTER_WRITE_CMD_TYPE1 *cmd;
	cmd = (ELSTER_WRITE_CMD_TYPE1 *)cmdElsterSend.appStruct;
	if(cmd->res_cmp)
		InitFindData(&cmpElsterCmpOk,cmd->res_cmp);
	app_uart_puts((char *)cmd->cmd);
	return 1;
}
uint32_t ElsterRecvCmd0(uint8_t *buf,uint16_t len)
{
	ELSTER_WRITE_CMD_TYPE1 *cmd;
	cmd = (ELSTER_WRITE_CMD_TYPE1 *)cmdElsterSend.appStruct;
	if(len){
		if(cmd->res_cmp == NULL)
			return 1;
		if (FindData(&cmpElsterCmpOk, buf[len-1]) == 0)
		{
			return 1;
		}
	}
	return 0;
}
//Write common command
uint32_t ElsterWriteCmd(void)
{
	uint8_t calc_bcc = 0;
	ELSTER_WRITE_CMD_TYPE *cmd;
	cmd = (ELSTER_WRITE_CMD_TYPE *)cmdElsterSend.appStruct;
	calc_bcc = calc_bcc_funs("%s%c%s%c",cmd->opcode,STX,cmd->str_command,ETX);
	sprintf((char *)m_buffer_tb,"%c%s%c%s%c%c",SOH,cmd->opcode,STX,cmd->str_command,ETX,calc_bcc);	
	app_uart_puts(m_buffer_tb);
	return 1;
}
uint32_t ElsterRecvData(uint8_t *buf,uint16_t len)
{
	ELSTER_WRITE_CMD_TYPE *cmd;
	cmd = (ELSTER_WRITE_CMD_TYPE *)cmdElsterSend.appStruct;
	if((buf[0] == cmd->compare)) //ACK
	{
		return 1;
	}
	else if(buf[len - 2] == cmd->compare)
	{
		if(calc_bcc_data(&buf[1],len - 2) == buf[len - 1])
		{
			buf[len - 3] = '\0';	// Clear ETX , calc_bcc and )
			if(cmd->writefileFlag)
			{
				//ampm_f_printf(&meterFile,"\r\n%s",cmd->str_printf);															
				//ampm_f_printf(&meterFile,"    %s",&buf[2]);			
				elsterParse(cmd->str_printf,(char *)&buf[2]);
			}
			return 1;
		}
	}
	return 0;
}

//Write common command
uint32_t ElsterWriteCmd1(void)
{
	uint8_t calc_bcc = 0;
	ELSTER_WRITE_CMD_TYPE2 *cmd;
	cmd = (ELSTER_WRITE_CMD_TYPE2 *)cmdElsterSend.appStruct;
	calc_bcc = calc_bcc_funs("%s%c%s%c",cmd->opcode,STX,cmd->str_command,ETX);
	sprintf((char *)m_buffer_tb,"%c%s%c%s%c%c",SOH,cmd->opcode,STX,cmd->str_command,ETX,calc_bcc);	
	app_uart_puts(m_buffer_tb);
	return 1;
}
uint32_t ElsterRecvData1(uint8_t *buf,uint16_t len)
{
	ELSTER_WRITE_CMD_TYPE2 *cmd;
	cmd = (ELSTER_WRITE_CMD_TYPE2 *)cmdElsterSend.appStruct;
	if((buf[0] == cmd->compare)) //ACK
	{
		return 1;
	}
	else if(buf[len - 2] == cmd->compare)
	{
		if(calc_bcc_data(&buf[1],len - 2) == buf[len - 1])
		{
			buf[len - 3] = '\0';	// Clear ETX , calc_bcc and )
			if(cmd->writefileFlag)
			{
				//ampm_f_printf(&meterFile,"\r\n%s",cmd->str_printf);															
				//ampm_f_printf(&meterFile,"    %s",&buf[2]);
				elsterParse(cmd->str_printf,(char *)&buf[2]);				
			}	
			if(buf[len - 5] == '8' && buf[len - 4] == '0')
			{
				elsteWriterCmd7 = elsteWriterCmd7Reload;
				return 1;
			}
			cmd->cmdStartNum++;
			if(cmd->cmdStartNum >= cmd->cmdEndNum)
			{
				elsteWriterCmd7 = elsteWriterCmd7Reload;
				return 1;
			}
			cmdElsterSend.retryNum = 3;
			InitTimeout(&tElsterTimeout, SYSTICK_TIME_MS(cmdElsterSend.delay));
			elsterCmdPhase = ELSTER_CMD_SEND;
			return 0;
		}
	}
	return 0;
}


uint8_t r1_xxxxxxCmd[16];
uint8_t r1_xxxxxxWriteFlash[16];
ELSTER_WRITE_CMD_TYPE elsterWriteCmd2;
//Write common command
uint32_t ElsterWriteCmd2(void)
{
	uint8_t calc_bcc = 0;
	ELSTER_WRITE_CMD_TYPE2 *cmd;
	cmd = (ELSTER_WRITE_CMD_TYPE2 *)cmdElsterSend.appStruct;
	calc_bcc = calc_bcc_funs("%s%c%s%c",cmd->opcode,STX,cmd->str_command,ETX);
	sprintf((char *)m_buffer_tb,"%c%s%c%s%c%c",SOH,cmd->opcode,STX,cmd->str_command,ETX,calc_bcc);	
	app_uart_puts(m_buffer_tb);
	return 1;
}
uint32_t ElsterRecvData2(uint8_t *buf,uint16_t len)
{
	uint32_t u32Temp;
	ELSTER_WRITE_CMD_TYPE2 *cmd;
	cmd = (ELSTER_WRITE_CMD_TYPE2 *)cmdElsterSend.appStruct;
	if((buf[0] == cmd->compare)) //ACK
	{
		return 1;
	}
	else if(buf[len - 2] == cmd->compare)
	{
		if(calc_bcc_data(&buf[1],len - 2) == buf[len - 1])
		{
			buf[len - 3] = '\0';	// Clear ETX , calc_bcc and )
			if(cmd->writefileFlag)
			{
				//ampm_f_printf(&meterFile,"\r\n%s",(char *)cmd->str_printf);															
				//ampm_f_printf(&meterFile,"    %s",(char *)&buf[2]);		
				elsterParse(cmd->str_printf,(char *)&buf[2]);					
			}
			
			if(strstr((char *)cmd->str_command,"55"))
			{
				if(cmd->writefileFlag == 0)
				{
					cmd->writefileFlag  = 1;
					buf[6] = '\0';
					sscanf((char *)&buf[2],"%04X",&u32Temp);
					if(u32Temp <= 32)
						cmd->cmdEndNum = u32Temp + 1;
				}
				sprintf((char *)r1_xxxxxxCmd,"55%0.4x(40)",cmd->cmdStartNum);
				sprintf((char *)r1_xxxxxxWriteFlash,"55%0.4x",cmd->cmdStartNum);
			}
			else if(strstr((char *)cmd->str_command,"543"))
			{
				cmd->cmdEndNum = packetsHistory + 1;
				sprintf((char *)r1_xxxxxxCmd,"543%0.3x(40)",cmd->cmdStartNum);
				sprintf((char *)r1_xxxxxxWriteFlash,"543%0.3x",cmd->cmdStartNum);
			}
			else
			{
				return 1;
			}
			
			
			cmd->str_command = r1_xxxxxxCmd;
			cmd->str_printf = r1_xxxxxxWriteFlash;
			cmd->cmdStartNum++;
			if(cmd->cmdStartNum > cmd->cmdEndNum)
			{
				return 1;
			}
			
			cmdElsterSend.retryNum = 3;
			InitTimeout(&tElsterTimeout, SYSTICK_TIME_MS(cmdElsterSend.delay));
			elsterCmdPhase = ELSTER_CMD_SEND;
			return 0;
		}
	}
	return 0;
}


void ElsterCmd_Task(void)
{
	uint8_t c;
	ELSTER_CMD_LIST_TYPE *cmdList;
	
	switch(elsterCmdPhase)
	{
		case ELSTER_CMD_SEND:

				if(CheckTimeout(&tElsterTimeout) == SYSTICK_TIMEOUT)
				{
						if(cmdElsterSend.send_data_callback != NULL)
						{
							while (mUART4_GetChar(&c) == 0);
							cmdElsterSend.send_data_callback();
							InitTimeout(&tElsterTimeout, SYSTICK_TIME_MS(cmdElsterSend.timeout));
							m_buffer_rb_len = 0;
							elsterCmdPhase = ELSTER_CMD_WAIT;
						}
						else
						{
							elsterCmdPhase = ELSTER_CMD_OK;
						}
				}
			break;
			
		case ELSTER_CMD_WAIT:
			if(cmdElsterSend.recv_data_callback == NULL)
			{
				elsterCmdPhase = ELSTER_CMD_DELAY;
				break;	
			}
			if(CheckTimeout(&tElsterTimeout) == SYSTICK_TIMEOUT)
			{
				if (cmdElsterSend.retryNum)
				{
					cmdElsterSend.retryNum--;
					InitTimeout(&tElsterTimeout, SYSTICK_TIME_MS(cmdElsterSend.delay));
					elsterCmdPhase = ELSTER_CMD_SEND;
				}
				else
				{
					elsterCmdPhase = ELSTER_CMD_ERROR;
				}
			}
			else
			{
				while (mUART4_GetChar(&c) == 0)
				{
					if(m_buffer_rb_len <  sizeof(m_buffer_rb))
					{
						m_buffer_rb[m_buffer_rb_len] = c;
						m_buffer_rb_len++;
					}
					if(cmdElsterSend.recv_data_callback(m_buffer_rb,m_buffer_rb_len))
					{
						InitTimeout(&tElsterTimeout, SYSTICK_TIME_MS(cmdElsterSend.delay));
						elsterCmdPhase = ELSTER_CMD_DELAY;
						break;	
					}
					if(elsterCmdPhase != ELSTER_CMD_WAIT)
					{
						break;
					}
				}
			}
			break;
		case ELSTER_CMD_DELAY:	
			if(CheckTimeout(&tElsterTimeout) == SYSTICK_TIMEOUT)
			{
				while (mUART4_GetChar(&c) == 0)
				{
					if(m_buffer_rb_len <  sizeof(m_buffer_rb))
					{
						m_buffer_rb[m_buffer_rb_len] = c;
						m_buffer_rb_len++;
					}
				}
				elsterCmdPhase = ELSTER_CMD_OK;
			}
		break;
		case ELSTER_CMD_ERROR:		
		case ELSTER_CMD_OK:
			if(elsterCmdList[0] != NULL)
			{
				if(elsterCmdPhase != ELSTER_CMD_ERROR)
				{
					cmdList = list_pop(elsterCmdList);
					if(cmdList != NULL)
					{
						cmdElsterSend = *cmdList->pt;
						InitTimeout(&tElsterTimeout, SYSTICK_TIME_MS(cmdElsterSend.delay));
						elsterCmdPhase = ELSTER_CMD_SEND;
					}
				}
				else
					list_init(elsterCmdList);
			}
			break;
		default:
			elsterCmdPhase = ELSTER_CMD_ERROR;
			break;
	}
}


int8_t data_reverse(char *out,char *in,uint8_t length)
{
	uint8_t i;
	if ((length % 2) == 0)
	{
		for (i = 0; i < length; i += 2)
		{
				out[i] = in[length-i-2];
				out[i+1] = in[length-i-1];
		}
		return 0;
	}
	return -1;
}

uint8_t elsterConvertData(char *out,char *in,uint8_t offset)
{
	uint64_t u64Temp;
	char buf1[16],buf[16],*pt;
	if(offset)
	{
		memcpy(buf1,&in[16-offset],offset);
		memcpy(buf,in,16-offset);
		buf[16-offset] = 0;
		buf1[offset] = 0;
		u64Temp = atoll((char *)buf);
		sprintf(out,"%llu.%s",u64Temp,buf1);
	}
	else
	{
		for(pt = in;(pt[0] == '0' || pt[0] > '9' || pt[0] < '0') && pt[1] != '.';pt++);
		strcpy(out,pt);
	}
	//remove zero
	pt = &out[strlen(out) - 1];
	while(*pt == '0')
	{
		pt--;
		if(*pt == '.')
		{
			pt++;
			break;
		}
	}
	pt++;
	*pt = '\0';
	return 0;
}

int8_t data_remove_zero(char *out,char *in,uint8_t length)
{
	uint8_t i;
	out[0] = '0';
	for (i = 0; i < length; )
	{
		if(in[i] != '0')
			break;
		i++;
	}
	for(;i < length;i++)
	{
		*out = in[i];
		out++;
	}
	out[i] = 0;;
	return 0;
}

uint32_t endian_convert_u32(uint32_t in)
{
  uint32_t out;
  ((uint8_t *)&out)[0] = ((uint8_t *)&in)[3];
  ((uint8_t *)&out)[1] = ((uint8_t *)&in)[2];
  ((uint8_t *)&out)[2] = ((uint8_t *)&in)[1];
  ((uint8_t *)&out)[3] = ((uint8_t *)&in)[0];
  return out;
}

int8_t cmd606_process(char *out,char *data)
{
		uint8_t ss;        // sign and scaling byte (ss)
		char mvalue[16];
		char buf[16];
		{
			memset(mvalue,0,16);
			memset(buf,0,16);
			mvalue[0] = data[0];
			mvalue[1] = data[1];
			mvalue[2] = 0;
			ss = atoi((char *)mvalue);
			memcpy(mvalue,&data[2],12);
				//*'*/ get scale
			switch (ss & 0x07)
			{
					case 0:
							//mvalue /= 10000
							memcpy(buf,mvalue,8);
							buf[8] = '.';
							memcpy(&buf[9],&mvalue[8],4);
							break;
					case 1:
							//mvalue /= 1000
							//mvalue = mvalue.Insert(9, ".");
							memcpy(buf,mvalue,9);
							buf[9] = '.';
							memcpy(&buf[10],&mvalue[9],3);
							break;
					case 2:
							//mvalue /= 100
							//mvalue = mvalue.Insert(10, ".");
							memcpy(buf,mvalue,10);
							buf[10] = '.';
							memcpy(&buf[11],&mvalue[10],2);
							break;
					case 3:
							//mvalue /= 10
							//mvalue = mvalue.Insert(11, ".");
							memcpy(buf,mvalue,11);
							buf[11] = '.';
							memcpy(&buf[12],&mvalue[11],1);
							break;
					case 4:
							break;
					case 5:
					case 6:
					case 7:
							strcpy(buf,"99999999999.9");
							break;
			}
			// get sign
			if ((ss & 0x80) == 0x80) 
			{
				mvalue[0] = '-';
				strcpy(&mvalue[1],buf);
			}
			else
			{
				strcpy(mvalue,buf);
			}
			strcpy(out,mvalue);
		}
		return 0;
}

uint8_t get_packetforhis(void)
{
	char cloai[32];
	char type[32];
	///090100301013
	memcpy(type,&meterType[6],8);
	cloai[0] = ',';
	memcpy(&cloai[1],type,8);
	cloai[9] = '\0';
	if (strstr(",010002,010015,010024,010007,010003,010008,010014,01000103",cloai) == NULL)
	{
			if (strstr(",010025,010012,010017,010010,01002001,01002002,01002003,",cloai) != NULL)
			{
					return 12;
			}
			if (strstr(",010030,010021,010026,010028,010029,01002004,",cloai)  != NULL)
			{
					return 14;
			}
			cloai[strlen(cloai) - 2] = '\0';
			if (strstr(",010002,010015,010024,010007,010003,010008,010014,01000103",cloai)  != NULL)
			{
					return 10;
			}
			if (strstr(",010025,010012,010017,010010,01002001,01002002,01002003,",cloai)  != NULL)
			{
					return 12;
			}
			if (strstr(",010030,010021,010026,010028,010029,01002004,",cloai)  != NULL)
			{
					return 14;
			}
	}
	return 10;
}

void JsonSave(char *s)
{
	uint32_t len;
	ELSTER_DBG(s);
//	if(dataSaveTo == SAVE_TO_FLASH)
//	{
//		ampm_f_write(&meterFile,s,strlen(s),&len);
//	}
//	else if(dataSaveTo == SAVE_TO_BLE)
//	{
//		ELSTER_Puts(s);
//	}
//	else if(dataSaveTo == (SAVE_TO_FLASH + SAVE_TO_BLE))
//	{
//		ampm_f_write(&meterFile,s,strlen(s),&len);
//		ELSTER_Puts(s);
//	}
}

uint8_t elsterParse(uint8_t *cmd,char *data)
{
	char buf[128],buf1[32],buf2[32],*pt,u8Temp;
	uint16_t len,i;
	uint32_t u32Temp;
	DATE_TIME time;

	if(strstr((char *)cmd,"Type"))
	{
		JsonSave("***BEGIN***\n");
		JsonSave("{\n");
		JsonSave("    \"Meter Type\":\"Elster\",\n");
		JsonSave("    \"Type\":");
		JsonSave("\"");
		JsonSave(data);
		JsonSave("\"");
		JsonSave(",\n");
		if(strlen(data) < sizeof(meterType));
		{
			memcpy(meterType,data,strlen(data));
			packetsHistory =  get_packetforhis();
			historyLength = (packetsHistory - 1) * 128 + (packetsHistory == 14 ? 31 * 2 : 39 * 2);
		}
	}
	else if(strstr((char *)cmd,"798001"))
	{
		uint32_t b0,b1,b2,b3,b4,b5,b6,b7;
		sscanf(&data[16],"%02X%02X%02X%02X%02X%02X%02X%02X",&b0,&b1,&b2,&b3,&b4,&b5,&b6,&b7);
		buf[0] = b0;
		buf[1] = b1;
		buf[2] = b2;
		buf[3] = b3;
		buf[4] = b4;
		buf[5] = b5;
		buf[6] = b6;
		buf[7] = b7;
		buf[8] = 0;
		if(strcmp(deviceCfg.meterID,buf))
		{
			strcpy(deviceCfg.meterID,buf);
			DeviceCfg_Save();
		}
		
		JsonSave("    \"Meter ID\":");
		JsonSave("\"");
		JsonSave(buf);
		JsonSave("\"");
		JsonSave(",\n");
		
	}
	else if(strstr((char *)cmd,"861001"))
	{
		uint32_t b0,b1,b2,b3,b4,b5,b6;
		sscanf(data,"%02d%02d%02d%02X%02X%02d%02d",&b0,&b1,&b2,&b3,&b4,&b5,&b6);
		time.sec = b0;
		time.min = b1;
		time.hour = b2;
		time.mday = (b3 & 0x0f) + ((b3 & 0x3F)>>4)*10;
		time.month = (b4 & 0x0f) + ((b4 & 0x1F)>>4)*10;
		time.year = b6 + 2000;
		JsonSave("    \"Time\":{\n");
		sprintf(buf,"        \"year\":%d,\n        \"month\":%d,\n        \"day\":%d,\n",time.year,time.month,time.mday);
		JsonSave(buf);
		sprintf(buf,"        \"hour\":%d,\n        \"min\":%d,\n        \"sec\":%d\n",time.hour,time.min,time.sec);
		JsonSave(buf);
		JsonSave("    },\n");
	}
	else if(strstr((char *)cmd,"507001"))
	{
		JsonSave("    \"Current Value\": {\n");
		buf[16] = 0;
		pt = data;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Import\":");
		JsonSave(buf1);
		JsonSave(",\n");
		
		currentValue.Import_Wh =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Export\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Export_Wh =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Q1\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Q1_varh =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Q2\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Q2_varh =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Q3\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Q3_varh =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Q4\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Q4_varh =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"VAh\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.VAh_vah =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"CD1\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.CD1_varh =  atoll((char *)buf);
	}
	else if(strstr((char *)cmd,"507002"))
	{
		buf[16] = 0;
		pt = data;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"CD2\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.CD2_varh =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"CD3\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.CD3_varh =  atoll((char *)buf);
	}
	else if(strstr((char *)cmd,"508001"))
	{
		buf[16] = 0;
		pt = data;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Rates1\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Rates1 =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Rates2\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Rates2 =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Rates3\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Rates3 =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Rates4\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Rates4 =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Rates5\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Rates5 =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Rates6\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Rates6 =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Rates7\":");
		JsonSave(buf1);
		JsonSave(",\n");
		currentValue.Rates7 =  atoll((char *)buf);
		pt += 16;
		data_reverse(buf,pt,16);
		elsterConvertData(buf1,buf,6);
		JsonSave("        \"Rates8\":");
		JsonSave(buf1);
		if(meterReadTypeCmd == METER_READ_0)
			JsonSave("\n    }\n");
		else
			JsonSave("\n    },\n");
		currentValue.Rates8 =  atoll((char *)buf);
	}
	else if(strstr((char *)cmd,"605001"))
	{
		
	}
	else if(strstr((char *)cmd,"60600A"))
	{
		
		JsonSave("    \"Instantaneous Value\": {\n");
		len = strlen(data);
		if(len == 56)
		{
			pt = data;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.current_phase_A);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.current_phase_B);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.current_phase_C);
		}
	}
	else if(strstr((char *)cmd,"60600B"))
	{
		
		len = strlen(data);
		if(len == 56)
		{
			pt = data;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.voltage_phase_A);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.voltage_phase_B);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.voltage_phase_C);
		}
	}
	else if(strstr((char *)cmd,"606009"))
	{
		len = strlen(data);
		if(len == 56)
		{
			pt = data;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.angle_phase_A);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.angle_phase_B);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.angle_phase_C);
	
		}
	}
	else if(strstr((char *)cmd,"606008"))
	{
		len = strlen(data);
		if(len == 56)
		{
			pt = data;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.frequency_phase_A);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.frequency_phase_B);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.frequency_phase_C);
		}
		len = 0;
	}
	else if(strstr((char *)cmd,"606003"))
	{
		len = strlen(data);
		if(len == 56)
		{
			pt = data;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.factor_phase_A);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.factor_phase_B);
			pt += 14;
			cmd606_process((char *)buf2,pt);
			elsterConvertData(buf1,buf2,0);
			sscanf(buf2,"%f",&instantaneousValue.factor_phase_C);
		}
		len = 0;
	}
	else if(strstr((char *)cmd,"701001"))
	{
		
	}
	else if(strstr((char *)cmd,"455001"))
	{
		
	}
	else if(strstr((char *)cmd,"693001"))
	{
		
	}
	else if(strstr((char *)cmd,"694001"))
	{
		
	}
	else if(strstr((char *)cmd,"695001"))
	{
		
	}
	else if(strstr((char *)cmd,"702001"))
	{
		
	}
	else if(strstr((char *)cmd,"680001"))
	{
		
	}
	else if(strstr((char *)cmd,"700001"))
	{
		
	}
	else if(strstr((char *)cmd,"54300"))
	{
		if(packetsHistory >= 10)
		{
			u8Temp = packetsHistory - 10 + 'a';
		}
		else
		{
			u8Temp = packetsHistory + '0';
		}
		if(cmd[5] == u8Temp) //mean this is last packet
		{
			memcpy(buf,&data[(historyLength % 128) - 8],8);
			buf[9] = 0;
			sscanf(buf,"%08X",&u32Temp);
			u32Temp = endian_convert_u32(u32Temp);
			TIME_FromSec(&time,u32Temp);
			//startTime;
			memcpy(buf,&data[(historyLength % 128) - 26],8);
			buf[9] = 0;
			sscanf(buf,"%08X",&u32Temp);
			u32Temp = endian_convert_u32(u32Temp);
			TIME_FromSec(&time,u32Temp);
			JsonSave("        \"Start Time\":{\n");
			sprintf(buf,"            \"year\":%d,\n            \"month\":%d,\n            \"day\":%d,\n",time.year,time.month,time.mday);
			JsonSave(buf);
			sprintf(buf,"            \"hour\":%d,\n            \"min\":%d,\n            \"sec\":%d\n",time.hour,time.min,time.sec);
			JsonSave(buf);
			JsonSave("        },\n");
			//EndTime;
			memcpy(buf,&data[(historyLength % 128) - 18],8);
			buf[9] = 0;
			sscanf(buf,"%08X",&u32Temp);
			u32Temp = endian_convert_u32(u32Temp);
			TIME_FromSec(&time,u32Temp);
			JsonSave("        \"End Time\":{\n");
			sprintf(buf,"            \"year\":%d,\n            \"month\":%d,\n            \"day\":%d,\n",time.year,time.month,time.mday);
			JsonSave(buf);
			sprintf(buf,"            \"hour\":%d,\n            \"min\":%d,\n            \"sec\":%d\n",time.hour,time.min,time.sec);
			JsonSave(buf);
			JsonSave("        }\n");
			JsonSave("    }\n");
		}
		switch(cmd[5])
		{
			case '1':
				JsonSave("    \"History\": {\n");
				buf[16] = 0;
				for(i = 0;i < 8;i++)
				{
					
					data_reverse(buf,&data[i*16],16);		
					elsterConvertData(buf1,buf,6);
					
					JsonSave("        \"");
					JsonSave((char *)historyInfoBuf[i]);
					JsonSave("\":");
					JsonSave(buf1);
					JsonSave(",\n");
				}
			break;
			case '2':
				buf[16] = 0;
				for(i = 0;i < 8;i++)
				{
					data_reverse(buf,&data[i*16],16);		
					elsterConvertData(buf1,buf,6);
					
					JsonSave("        \"");
					JsonSave((char *)historyInfoBuf[i+8]);
					JsonSave("\":");
					JsonSave(buf1);
					JsonSave(",\n");
				}
			break;
			case '3':
				
			break;
			case '4':
				
			break;
			case '5':
				
			break;
			case '6':
				
			break;
			case '7':
				
			break;
			case '8':
				
			break;
			case '9':
				
			break;
			case 'a':
				
			break;
			case 'b':
				
			break;
			case 'c':
				
			break;
			case 'd':
				
			break;
			case 'e':
				
			break;
		}
	}
	else if(strstr((char *)cmd,"550001"))
	{
		len = 0;
	}
	else if(strstr((char *)cmd,"END"))
	{
		JsonSave("}\n");
		JsonSave("***END***\n");
		return 1;
	}
	return 0;
}


