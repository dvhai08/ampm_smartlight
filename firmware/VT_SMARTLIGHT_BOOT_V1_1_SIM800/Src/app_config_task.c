

#include "app_config_task.h"
#include "uart_config_task.h"
#include "aes.h"
#include "flashprog.h"
#include "system_config.h"
#include "sst25.h"
#include "uart5.h"
#include "uart4.h"

#define FLASH_BUF_SIZE 512

uint32_t firmwareFileCrc,fileCrcCalc;
uint32_t firmwareFileSize;
uint32_t firmwareFileOffSet;
uint32_t firmwareSendFileOffSetFlag = 0;
uint32_t firmwareSaveFileOffsetCnt = 0;
uint32_t firmwareStatus = 0xFFFFFFFF;

uint8_t buff[2048];
uint8_t flashBuff[2048];
Timeout_Type bootLoginTimeout;
Timeout_Type bootExitTimeout;
uint32_t runApplication __attribute__((at(0x10000000)));//added by thienhaiblue@gmail.com

typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;

extern void SysDeInit(void);

uint8_t CfgCalcCheckSum(uint8_t *buff, uint32_t length);

const uint8_t key[16] = "^$&#(@*%%^^%^*^$";
const uint8_t my_iv[16] = "^$&#(@*%%^^%^*^$";
uint8_t last_iv[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t bootMode = 0;
void execute_user_code(void)
{
	runApplication = 0xA5A5A5A5;
	NVIC_SystemReset();
}

void run_app(void)
{
		//if (((*(uint32_t*)USER_FLASH_START) & 0x2FF00000 ) == 0x20000000)
    { /* Jump to user application */
			//SCB->VTOR = USER_FLASH_START;
      JumpAddress = *(uint32_t*) (USER_FLASH_START + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(uint32_t*) USER_FLASH_START);
      Jump_To_Application();
    }   
}

void FMC_DataDecrypt(const uint8_t *iv,uint8_t * data, uint32_t len)
{
	uint8_t this_iv[16];
	memcpy(this_iv,iv,16);
	mbedtls_aes_context ctx;
	mbedtls_aes_init( &ctx );
	mbedtls_aes_setkey_dec( &ctx, key, 128);
	if(len % 16) return;
	mbedtls_aes_crypt_cbc( &ctx, MBEDTLS_AES_DECRYPT, len, this_iv, data, data);
	mbedtls_aes_free( &ctx );
}


void AppCheck(void)
{
	if(runApplication == 0xA5A5A5A5)
	{
		runApplication = 0;
		run_app();
	}
}

void AppConfigTaskInit(void)
{
	SST25_Init();
	InitTimeout(&bootLoginTimeout,SYSTICK_TIME_SEC(1));
	InitTimeout(&bootExitTimeout,SYSTICK_TIME_SEC(2));
}


uint32_t AppConfigTask(void)
{
	uint8_t buf[16],i;
	LL_IWDG_ReloadCounter(IWDG);
	if(bootMode == 0)
	{
		if(CheckTimeout(&bootLoginTimeout) == SYSTICK_TIMEOUT)
		{
			buf[0] = 0xCA;
			buf[1] = 4; //length
			buf[3] = 0x88; //opcode
			buf[4] = 0xA5;
			buf[5] = 0xA5;
			buf[6] = 0xA5;
			buf[7] = 0xA5;
			buf[buf[1] + 4] = CfgCalcCheckSum((uint8_t *)&buf[4],buf[1]);
			for(i = 0;i < buf[1] + 4 + 1; i++)
			{
				mUART5_PutChar(buf[i]);
				mUART4_PutChar(buf[i]);
			}
			InitTimeout(&bootLoginTimeout,SYSTICK_TIME_MS(500));
		}
	}

	if(CheckTimeout(&bootExitTimeout) == SYSTICK_TIMEOUT)
	{
		AppCopy();
		execute_user_code();
	}

	return 0;
}


uint8_t CfgProcessData(CFG_PROTOCOL_TYPE *cfgProtoRecv,CFG_PROTOCOL_TYPE *cfgProtoSend,uint8_t *cfgSendDataBuff,uint32_t maxPacketSize,uint8_t logSendEnable)
{
	static uint8_t flagCfgProcessInUse = 0;
	uint8_t *u8pt,buf[18],temp_iv[16];
	uint32_t i,j,u32Temp;
	CFG_DATA_TYPE	pt;
	if(flagCfgProcessInUse) 
	{
		cfgProtoRecv->opcode = 0;
		return 0;
	}
	flagCfgProcessInUse = 1;
	InitTimeout(&bootExitTimeout,SYSTICK_TIME_SEC(10));
	switch(cfgProtoRecv->opcode)
	{
		case 0x12:
			pt.packetNo = cfgProtoRecv->dataPt[0];
			pt.packetNo |= (uint32_t)cfgProtoRecv->dataPt[1]<<8;
			pt.packetNo |= (uint32_t)cfgProtoRecv->dataPt[2]<<16;
			pt.packetNo |= (uint32_t)cfgProtoRecv->dataPt[3]<<24;
			pt.dataPt = (uint8_t *)cfgProtoRecv->dataPt + 4;
			if(pt.packetNo == 0xA5A5A5A5)
			{
				firmwareStatus = 0xA5A5A5A5;
				firmwareFileOffSet = 0;
				firmwareFileSize = pt.dataPt[0];
				firmwareFileSize |= pt.dataPt[1]<<8;
				firmwareFileSize |= pt.dataPt[2]<<16;
				firmwareFileSize |= pt.dataPt[3]<<24;
				 
				firmwareFileCrc = pt.dataPt[4];
				firmwareFileCrc |= pt.dataPt[5]<<8;
				firmwareFileCrc |= pt.dataPt[6]<<16;
				firmwareFileCrc |= pt.dataPt[7]<<24;
				
				cfgProtoSend->start = 0xCA;
				cfgProtoSend->opcode = 0x12;
				cfgProtoSend->length = 12;
				u8pt = (uint8_t *)&pt.packetNo;
				cfgSendDataBuff[0] = u8pt[0];
				cfgSendDataBuff[1] = u8pt[1];
				cfgSendDataBuff[2] = u8pt[2];
				cfgSendDataBuff[3] = u8pt[3];
				//Max file size
				i = firmwareFileSize;
				u8pt = (uint8_t *)&i;
				cfgSendDataBuff[4] = u8pt[0];
				cfgSendDataBuff[5] = u8pt[1];
				cfgSendDataBuff[6] = u8pt[2];
				cfgSendDataBuff[7] = u8pt[3];
				//Max packet size
				i = maxPacketSize;
				u8pt = (uint8_t *)&i;
				cfgSendDataBuff[8] = u8pt[0];
				cfgSendDataBuff[9] = u8pt[1];
				cfgSendDataBuff[10] = u8pt[2];
				cfgSendDataBuff[11] = u8pt[3];
				cfgProtoSend->dataPt = cfgSendDataBuff;
				cfgProtoSend->crc = CfgCalcCheckSum(cfgProtoSend->dataPt,cfgProtoSend->length);
				firmwareFileOffSet = 0;
				
			}
			else
			{
				if(pt.packetNo == firmwareFileOffSet)
				{
					firmwareFileOffSet += (uint32_t)cfgProtoRecv->length  - 4;
					u32Temp = cfgProtoRecv->length - 4;
					
					if((maxPacketSize == FLASH_BUF_SIZE) 
						&& (u32Temp == FLASH_BUF_SIZE)
						&& (firmwareFileOffSet % FLASH_BUF_SIZE == 0)
					)
					{
						
						if(firmwareFileOffSet == FLASH_BUF_SIZE)
						{
							HAL_FLASH_Unlock();
								FLASH_ErasePage(ADDR_FLASH_SECTOR_3);
							if((firmwareFileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_4)
								FLASH_ErasePage(ADDR_FLASH_SECTOR_4);
							if((firmwareFileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_5)
								FLASH_ErasePage(ADDR_FLASH_SECTOR_5);
							if((firmwareFileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_6)
								FLASH_ErasePage(ADDR_FLASH_SECTOR_6);
							if((firmwareFileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_7)
								FLASH_ErasePage(ADDR_FLASH_SECTOR_7);
							if((firmwareFileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_8)
								FLASH_ErasePage(ADDR_FLASH_SECTOR_8);
							if((firmwareFileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_9)
								FLASH_ErasePage(ADDR_FLASH_SECTOR_9);
							if((firmwareFileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_10)
								FLASH_ErasePage(ADDR_FLASH_SECTOR_11);
							if((firmwareFileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_11)
								FLASH_ErasePage(ADDR_FLASH_SECTOR_11);
							HAL_FLASH_Lock();
							
							fileCrcCalc = 0;
							for(i = 0; i < FLASH_BUF_SIZE;i++)
							{
								fileCrcCalc += pt.dataPt[i];
							}
							
							memcpy(last_iv,&pt.dataPt[FLASH_BUF_SIZE-16],16);
							FMC_DataDecrypt(my_iv,pt.dataPt,FLASH_BUF_SIZE);
						}
						else
						{
							for(i = 0; i < FLASH_BUF_SIZE;i++)
							{
								if((pt.packetNo + i) < firmwareFileSize)
									fileCrcCalc += pt.dataPt[i];
								else
									break;
							}
							
							memcpy(temp_iv,&pt.dataPt[FLASH_BUF_SIZE-16],16);
							FMC_DataDecrypt(last_iv,pt.dataPt,FLASH_BUF_SIZE);
							memcpy(last_iv,temp_iv,16);
						}
						__disable_irq();
						if(firmwareFileOffSet < firmwareFileSize)
						{
							if(FMC_ProgramData(pt.packetNo + USER_FLASH_START,(uint32_t *)pt.dataPt,FLASH_BUF_SIZE) != HAL_OK)
							{
								firmwareFileOffSet = 0;
							}
						}
						else
						{
							u32Temp = firmwareFileSize - pt.packetNo;
							//check padding
							u8pt = &pt.dataPt[u32Temp - 16];
							if(u8pt[15] > 16 || u8pt[15] == 0)
							{
								firmwareFileOffSet = 0;
							}
							else
							{
								for(i = u8pt[15];i >= 1;i--)
								{
									if(u8pt[16 - i] !=  u8pt[15])
									{
										firmwareFileOffSet = 0;
										break;
									}
									u8pt[16 - i] = 0xff;
								}
							}
							
							if(FMC_ProgramData(pt.packetNo + USER_FLASH_START,(uint32_t *)pt.dataPt,u32Temp) != HAL_OK)
							{
								firmwareFileOffSet = 0;
							}
						}
						__enable_irq();
					}
					else
					{
						NVIC_SystemReset();
					}
					
				}
				if(pt.packetNo <= firmwareFileOffSet)
					pt.packetNo = firmwareFileOffSet;
				else
				{
					firmwareStatus = 0xA5A5A5A5;
					pt.packetNo = 0;
					firmwareFileOffSet = 0;
				}
				if(firmwareFileOffSet >= firmwareFileSize)
				{
					if(fileCrcCalc == firmwareFileCrc)
					{
						InitTimeout(&bootExitTimeout,SYSTICK_TIME_SEC(1));
						firmwareStatus = 0x5A5A5A5A;
						pt.packetNo = 0x5A5A5A5A;
						firmwareFileOffSet = 0;
					}
					else
					{
						firmwareStatus = 0xA5A5A5A5;
						pt.packetNo = 0;
						firmwareFileOffSet = 0;
					}
				}
				
				cfgProtoSend->start = 0xCA;
				cfgProtoSend->opcode = 0x12;
				cfgProtoSend->length = 4;
				u8pt = (uint8_t *)&pt.packetNo;
				cfgSendDataBuff[0] = u8pt[0];
				cfgSendDataBuff[1] = u8pt[1];
				cfgSendDataBuff[2] = u8pt[2];
				cfgSendDataBuff[3] = u8pt[3];
				cfgProtoSend->dataPt = cfgSendDataBuff;
				cfgProtoSend->crc = CfgCalcCheckSum(cfgProtoSend->dataPt,cfgProtoSend->length);
			}
		break;
		case 0xA5:
			execute_user_code();
		break;
		case 0x88:
			bootMode = 1;
		break;
		case 0xFE:					// Reset device
			NVIC_SystemReset();
			break;
		default:
			break;
	}
	cfgProtoRecv->opcode = 0;
	flagCfgProcessInUse = 0;
	return 0;
}


void AppCopy(void)
{
	uint32_t *u32Pt,i,j,u32Temp;
	uint32_t fileCrc;
	uint8_t tryNum;
	uint32_t fileSize;
	uint8_t exit,*u8pt;
	uint32_t flashCheckSum,memAddr;
	u32Pt = (uint32_t *)buff;
	uint8_t temp_iv[16];
	//SST25_Init();
	SST25_Read(FIRMWARE_INFO_ADDR + FIRMWARE_STATUS_OFFSET*4,buff,4);
	if(*u32Pt != 0x5A5A5A5A)	
	{
		execute_user_code();
	}
	//USART3_PutString("\r\nPROGRAM NEW FIRMWARE*\r\n");
	tryNum = 10; 
	exit = 0;
	while(!exit && tryNum--)
	{
		SST25_Read(FIRMWARE_INFO_ADDR + FIRMWARE_CRC_OFFSET*4,buff,4);
		fileCrc = 	*u32Pt;
		SST25_Read(FIRMWARE_INFO_ADDR + FIRMWARE_FILE_SIZE_OFFSET*4,buff,4);
		fileSize = 	*u32Pt;
		//check file
		
		SST25_Read(fileSize + FIRMWARE_BASE_ADDR - 32,buff, 32);
		memcpy(temp_iv,buff,16);
		FMC_DataDecrypt(temp_iv,&buff[16],16);
		
		u8pt = &buff[16];
		if(u8pt[15] > 16 || u8pt[15] == 0)
		{
			exit = 1;
			continue;
		}
		else
		{
			for(i = u8pt[15];i >= 1;i--)
			{
				if(u8pt[16 - i] !=  u8pt[15])
				{
					exit = 1;
					break;
				}
				u8pt[16 - i] = 0xff;
			}
		}
		if(exit)
			continue;
		
		if((fileSize % 16 == 0) && (fileSize < FIRMWARE_MAX_SIZE))
		{
			LL_IWDG_ReloadCounter(IWDG);
			HAL_FLASH_Unlock();
//				FLASH_ErasePage(ADDR_FLASH_SECTOR_2);
//				if((firmwareFileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_3)
				FLASH_ErasePage(ADDR_FLASH_SECTOR_3);
			if((fileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_4)
				FLASH_ErasePage(ADDR_FLASH_SECTOR_4);
			if((fileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_5)
				FLASH_ErasePage(ADDR_FLASH_SECTOR_5);
			if((fileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_6)
				FLASH_ErasePage(ADDR_FLASH_SECTOR_6);
			if((fileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_7)
				FLASH_ErasePage(ADDR_FLASH_SECTOR_7);
			if((fileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_8)
				FLASH_ErasePage(ADDR_FLASH_SECTOR_8);
			if((fileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_9)
				FLASH_ErasePage(ADDR_FLASH_SECTOR_9);
			if((fileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_10)
				FLASH_ErasePage(ADDR_FLASH_SECTOR_11);
			if((fileSize + USER_FLASH_START) >= ADDR_FLASH_SECTOR_11)
				FLASH_ErasePage(ADDR_FLASH_SECTOR_11);
			HAL_FLASH_Lock();
			flashCheckSum = 0;
			for(i = 0; i < fileSize;i += FLASH_BUF_SIZE)
			{
				mUART5_PutChar('*');
				mUART4_PutChar('*');
				LL_IWDG_ReloadCounter(IWDG);
				SST25_Read(i + FIRMWARE_BASE_ADDR,buff, FLASH_BUF_SIZE);
				for(j = 0 ; j < FLASH_BUF_SIZE;j++)
				{
					if(i + j < fileSize)
						flashCheckSum += buff[j];
					else
						break;
				}
				if(i == 0)
				{
					memcpy(last_iv,&buff[FLASH_BUF_SIZE-16],16);
					FMC_DataDecrypt(my_iv,buff,FLASH_BUF_SIZE);
				}
				else
				{
					memcpy(temp_iv,&buff[FLASH_BUF_SIZE-16],16);
					FMC_DataDecrypt(last_iv,buff,FLASH_BUF_SIZE);
					memcpy(last_iv,temp_iv,16);
				}
				if(i + FLASH_BUF_SIZE >= fileSize)
				{
					u32Temp = fileSize - i;
					//check padding
					u8pt = &buff[u32Temp - 16];
					if(u8pt[15] > 16 || u8pt[15] == 0)
					{
						flashCheckSum = fileCrc + 1; //create a error
						break;
					}
					else
					{
						for(j = u8pt[15];j >= 1;j--)
						{
							if(u8pt[16 - j] !=  u8pt[15])
							{
								flashCheckSum = fileCrc + 1; //create a error
								break;
							}
							u8pt[16 - j] = 0xff;
						}
					}
					if(FMC_ProgramData(USER_FLASH_START + i,u32Pt,u32Temp) != HAL_OK)
					{
						flashCheckSum = fileCrc + 1; //create a error
						break;
					}
					memAddr = USER_FLASH_START + i;
					if(memcmp(buff, (void*)memAddr , u32Temp) != NULL)
					{
						flashCheckSum = fileCrc + 1;
						break;
					}
				}
				else
				{
					if(FMC_ProgramData(USER_FLASH_START + i,u32Pt,FLASH_BUF_SIZE) != HAL_OK)
					{
						flashCheckSum = fileCrc + 1; //create a error
						break;
					}
					memAddr = USER_FLASH_START + i;
					if(memcmp(buff, (void*)memAddr , FLASH_BUF_SIZE) != NULL)
					{
						flashCheckSum = fileCrc + 1;
						break;
					}
				}
			}
			if(flashCheckSum == fileCrc)
			{
					mUART4_PutString("\r\nFINISHED!\r\n");
					mUART5_PutString("\r\nFINISHED!\r\n");
					HAL_Delay(1000);
					exit = 1;
			}
		}
	}
	SST25_Erase(FIRMWARE_INFO_ADDR,block4k);
}


uint8_t CfgParserPacket(PARSER_PACKET_TYPE *parserPacket,CFG_PROTOCOL_TYPE *cfgProtoRecv,uint8_t c)
{
	switch(parserPacket->state)
	{
		case CFG_CMD_WAITING_SATRT_CODE:
			if(c == 0xCA)
			{
				parserPacket->state = CFG_CMD_GET_LENGTH;
				parserPacket->len = 0;
				parserPacket->crc = 0;
				parserPacket->cnt = 0;
			}
			break;
		case CFG_CMD_GET_LENGTH:
				parserPacket->len |= (uint16_t)c<<parserPacket->cnt;
				parserPacket->cnt += 8;
				if(parserPacket->cnt >= 16)
				{
					parserPacket->state = CFG_CMD_GET_OPCODE;
					parserPacket->cnt = 0;
				}
			break;
		case CFG_CMD_GET_OPCODE:		
					parserPacket->opcode = c;
					parserPacket->state = CFG_CMD_GET_DATA;
			break;
		case CFG_CMD_GET_DATA:
				if((parserPacket->cnt >= parserPacket->len) || (parserPacket->len > parserPacket->lenMax))
				{
					parserPacket->state = CFG_CMD_WAITING_SATRT_CODE;
				}
				else
				{
					parserPacket->crc += c;
					cfgProtoRecv->dataPt[parserPacket->cnt]= c;
					parserPacket->cnt++;
					if(parserPacket->cnt == parserPacket->len)
					{
						parserPacket->state = CFG_CMD_CRC_CHECK;
					}
				}
			break;
		case CFG_CMD_CRC_CHECK:
				parserPacket->state= CFG_CMD_WAITING_SATRT_CODE;
				if(parserPacket->crc  == c)
				{	
					cfgProtoRecv->length = parserPacket->len;
					cfgProtoRecv->opcode = parserPacket->opcode;
					return 0;
				}
			break;
		default:
			parserPacket->state = CFG_CMD_WAITING_SATRT_CODE;
			break;				
	}
	return 0xff;
}


uint8_t CfgCalcCheckSum(uint8_t *buff, uint32_t length)
{
	uint32_t i;
	uint8_t crc = 0;
	for(i = 0;i < length; i++)
	{
		crc += buff[i];
	}
	return crc;
}



