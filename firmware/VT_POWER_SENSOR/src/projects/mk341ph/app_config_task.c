
#include "app_config_task.h"
#include "system_config.h"
#include "smarthome_meter.h"
#include "smarthome_cfg.h"
#include "i2ceeprom.h"
#include "app_config_task.h"
#include "lib/protocol.h"
#include "lib/sys_tick.h"
#include "comm.h"
#include "aes128.h"
#include "auto_calib.h"

extern uint8_t calibChannel;
extern uint8_t calibFlag;
extern uint8_t flag_update_status_to_host;
extern void ReStartAFE(void);


extern uint8_t relay_state;
extern tSMARTHOME_RESULT result;

METER_STATUS_PACKAGE meterStatus;

CFG_PROTOCOL_TYPE cfgProtoRecv,cfgProtoSend;
uint8_t appSendBuf[64];
PROTO_PARSER appProtocol;
uint8_t appProtoBuf[64];

uint8_t U8CheckSum(uint8_t *buff, uint32_t length);
void AppConfigCallback(PROTO_PARSER *parser);

void AppConfigTaskInit(void)
{
	PROTO_InitParser(&appProtocol,(PROTO_PARSE_CALLBACK *)AppConfigCallback,appProtoBuf,sizeof(appProtoBuf));

}


void AppConfigCallback(PROTO_PARSER *parser)
{
	uint8_t i;
	uint8_t len,*u8Pt,*u8Pt1;
	uint16_t u16Temp;
	uint32_t u32Temp1;
	int32_t i32Temp1;
	ISMARTPACKAGE iSmartPacket;
	uint8_t answerCmd = 1;
	uint8_t opcode, channel;
	if(parser->dataLen == sizeof(ISMARTPACKAGE))
	{
		memcpy(&iSmartPacket,parser->buf,sizeof(ISMARTPACKAGE));
		opcode = iSmartPacket.packet.opcode & 0x1F;
		switch(opcode)
		{
			case 0x01://SET RELAY STATE
				flag_update_status_to_host = 0;
				channel = iSmartPacket.packet.data[0];
				switch(channel)
				{
					case 0:
					case 1:
					case 2:
					case 3:
						iSmartPacket.packet.opcode = 0x04;
						meterStatus.u = (uint16_t)(result.urms * 100);
						meterStatus.i = (int16_t)(result.irms[channel] * 100);
						meterStatus.p = (uint16_t)(result.p[channel] * 10);
						u32Temp1 = (ramcnt.wh_cnt[channel]/100);
						u8Pt1 = (uint8_t *)&u32Temp1;
						meterStatus.energy[0] = u8Pt1[0];
						meterStatus.energy[1] = u8Pt1[1];
						meterStatus.energy[2] = (u8Pt1[2] & 0x1F);
						memcpy(iSmartPacket.packet.data,&meterStatus,sizeof(METER_STATUS_PACKAGE));
					break;
						default:
								iSmartPacket.packet.opcode = 0x80;//ERROR
								iSmartPacket.packet.data[0] = 0x02;
						break;
				}
				break;
			case 0x02://GET RELAY STATE
				relay_state = iSmartPacket.packet.data[0];
			break;
			case 0x05://GET METER STATUS
			case 0x06:
			case 0x07:
			case 0x08:
					meterStatus.u = (uint16_t)(result.urms * 100);
					meterStatus.i = (int16_t)(result.irms[opcode-0x05] * 1000);
					meterStatus.p = (uint16_t)(result.p[opcode-0x05] * 10);
					u32Temp1 = ramcnt.wh_cnt[opcode-0x05];
					//u32Temp1 = 0x00AABBCC;
					u8Pt1 = (uint8_t *)&u32Temp1;
					meterStatus.energy[0] = u8Pt1[0];
					meterStatus.energy[1] = u8Pt1[1];
					meterStatus.energy[2] = u8Pt1[2];
				//	meterStatus.energy[3] = u8Pt1[3];
                    // Leave relay status for BLE chip
					memcpy(iSmartPacket.packet.data,&meterStatus,sizeof(METER_STATUS_PACKAGE));
				break;
			case 0x03://METER CONFIG
				calibFlag = 1;
				PORT_Init(PORTH,PORT_MODULE_LED_MODE,PIN7);
				GPIO_Init(GPIOH,GPIO_OUT_LOGIC0_MODE,PIN7);
				u8Pt = (uint8_t *)&i32Temp1;
				u8Pt[0] = iSmartPacket.packet.data[1];
				u8Pt[1] = iSmartPacket.packet.data[2];
				u8Pt[2] = iSmartPacket.packet.data[3];
				u8Pt[3] = iSmartPacket.packet.data[4];
				switch(iSmartPacket.packet.data[0])
				{
					case /*error u*/	0x01:
						ramcfg.correct.u_gain = (uint32_t)(((double)ramcfg.correct.u_gain) * (1 + ((double)i32Temp1)/10000));
						if(ramcfg.correct.u_gain < 0) ramcfg.correct.u_gain = nvmcfg.correct.u_gain;
						u8Pt[0] = iSmartPacket.packet.data[5];
						u8Pt[1] = iSmartPacket.packet.data[6];
						u8Pt[2] = iSmartPacket.packet.data[7];
						u8Pt[3] = iSmartPacket.packet.data[8];
						ramcfg.delayu = (int16_t)((double)i32Temp1*PHASE_CORRECT_TO_MKM+0.5); 
					break;
					case /*	error I0*/	0x02:
						calibChannel = 0;
						ramcfg.correct.i_gain[0] = ((double)ramcfg.correct.i_gain[0]) * (1 + ((double)i32Temp1)/10000);
						if(ramcfg.correct.i_gain[0] < 0) ramcfg.correct.i_gain[0] = nvmcfg.correct.i_gain[0];
						u8Pt[0] = iSmartPacket.packet.data[5];
						u8Pt[1] = iSmartPacket.packet.data[6];
						u8Pt[2] = iSmartPacket.packet.data[7];
						u8Pt[3] = iSmartPacket.packet.data[8];
						ramcfg.delay[0] = (int16_t)((double)i32Temp1*PHASE_CORRECT_TO_MKM+0.5); 
					break;
					case /*	error I1*/	0x03:
						calibChannel = 1;
						ramcfg.correct.i_gain[1] = ((double)ramcfg.correct.i_gain[1]) * (1 + ((double)i32Temp1)/10000);
						if(ramcfg.correct.i_gain[1] < 0) ramcfg.correct.i_gain[1] = nvmcfg.correct.i_gain[1];
						u8Pt[0] = iSmartPacket.packet.data[5];
						u8Pt[1] = iSmartPacket.packet.data[6];
						u8Pt[2] = iSmartPacket.packet.data[7];
						u8Pt[3] = iSmartPacket.packet.data[8];
						ramcfg.delay[1] = (int16_t)((double)i32Temp1*PHASE_CORRECT_TO_MKM+0.5); 
					break;
					case /*	error I2*/	0x04:
						calibChannel = 2;
						ramcfg.correct.i_gain[2] = ((double)ramcfg.correct.i_gain[2]) * (1 + ((double)i32Temp1)/10000);
						if(ramcfg.correct.i_gain[2] < 0) ramcfg.correct.i_gain[2] = nvmcfg.correct.i_gain[2];
						u8Pt[0] = iSmartPacket.packet.data[5];
						u8Pt[1] = iSmartPacket.packet.data[6];
						u8Pt[2] = iSmartPacket.packet.data[7];
						u8Pt[3] = iSmartPacket.packet.data[8];
						ramcfg.delay[2] = (int16_t)((double)i32Temp1*PHASE_CORRECT_TO_MKM+0.5); 
					break;
					case /*	error I3*/	0x05:
						calibChannel = 3;
						ramcfg.correct.i_gain[3] = ((double)ramcfg.correct.i_gain[2]) * (1 + ((double)i32Temp1)/10000);
						if(ramcfg.correct.i_gain[3] < 0) ramcfg.correct.i_gain[2] = nvmcfg.correct.i_gain[3];
						u8Pt[0] = iSmartPacket.packet.data[5];
						u8Pt[1] = iSmartPacket.packet.data[6];
						u8Pt[2] = iSmartPacket.packet.data[7];
						u8Pt[3] = iSmartPacket.packet.data[8];
						ramcfg.delay[3] = (int16_t)((double)i32Temp1*PHASE_CORRECT_TO_MKM+0.5); 
					break;
					case 	0x06:
						calibChannel = 0;
						set_calib_state(CALIB_OFFS_I0);
					break;
					case 	0x07:
						calibChannel = 1;
						set_calib_state(CALIB_OFFS_I1);
					break;
					case 	0x08:
						calibChannel = 2;
						set_calib_state(CALIB_OFFS_I2);
					break;
					case 	0x09:
						calibChannel = 3;
						set_calib_state(CALIB_OFFS_I3);
					break;
					case 	0x0C:
						
					break;
					case 	0x0D:
						SystemReset();
					break;
					case 	0x0F:
					break;
					case /*	Default factory reset*/	0xFF:
						start_calib();
//						CONFIG_ReadFlash((void *)&nvmcfg,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA));
//						ramcfg.flag = 0xfff5;
//						CONFIG_SaveFlash((void *)__CONFIG_FLASH_BLOCK__,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA)); 
					break;
					default:
						iSmartPacket.packet.opcode = 0x80;
						iSmartPacket.packet.data[0] = 0x02;
						break;
				}
				CONFIG_SaveFlash((void *)__CONFIG_FLASH_BLOCK__,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA));
				CONFIG_ReadFlash((void *)__CONFIG_FLASH_BLOCK__,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA));
				ReStartAFE();
				break;
			case 0x04:
				answerCmd = 0;
				u16Temp = iSmartPacket.packet.data[6];
				u16Temp <<= 8;
				u16Temp |= iSmartPacket.packet.data[5];
				AppRTC_Set(u16Temp
					,iSmartPacket.packet.data[4]
					,iSmartPacket.packet.data[3]
					,iSmartPacket.packet.data[2]
					,iSmartPacket.packet.data[1]
					,iSmartPacket.packet.data[0]);
			break;
			case 0x0F:
				flag_update_status_to_host = 0;
			break;
			default:
				iSmartPacket.packet.opcode = 0x80;
				iSmartPacket.packet.data[0] = 0x02;
				break;
		}
//		iSmartPacket.packet.opcode |= 0x80;
//		iSmartPacket.packet.crc = AppMakeCrc((uint8_t *)&iSmartPacket.packet,sysCfg.customerId,my_addr.addr,(rtcTimeSec)>>4);
		if(answerCmd)
		{
			len = PROTO_AddUnFcs(appSendBuf,(uint8_t *)&iSmartPacket,sizeof(appSendBuf),sizeof(ISMARTPACKAGE));
			for (i = 0; i < len; i++)
			{
				COMM_Putc(appSendBuf[i]);
			}
		}
	}
}

void App_PacketSendToUart(ISMARTPACKAGE *iSmartPacket)
{
		uint8_t buf[48];
		uint8_t len = PROTO_AddUnFcs(buf,(uint8_t *)iSmartPacket,sizeof(buf),sizeof(ISMARTPACKAGE));
		for (uint8_t i = 0; i < len; i++)
		{
				COMM_Putc(buf[i]);
		}
}
	
void AppUpdateStatus(void)
{
	uint8_t i;
	uint8_t len;
	ISMARTPACKAGE iSmartPacket;
	memset(&iSmartPacket,0,sizeof(ISMARTPACKAGE));
	iSmartPacket.packet.opcode = 0x8F;
	iSmartPacket.handle = 0xff;
	
	len = PROTO_AddUnFcs(appSendBuf,(uint8_t *)&iSmartPacket,sizeof(appSendBuf),sizeof(ISMARTPACKAGE));
	for (i = 0; i < len; i++)
	{
		COMM_Putc(appSendBuf[i]);
	}
}
void AppConfigTask(void)
{
	uint8_t c;
	while(RINGBUF_Get(&USART1_RxRingBuff,&c)==0)
	{
		PROTO_ParseByteUnFcs(&appProtocol,c);
	}
}


uint8_t U8CheckSum(uint8_t *buff, uint32_t length)
{
	uint32_t i;
	uint8_t crc = 0;
	for(i = 0;i < length; i++)
	{
		crc += buff[i];
	}
	return crc;
}


