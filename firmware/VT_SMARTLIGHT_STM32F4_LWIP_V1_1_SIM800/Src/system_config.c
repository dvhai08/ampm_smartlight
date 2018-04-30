#include "system_config.h"
#include "sst25.h"
#include "lib/sys_tick.h"
#include <stdlib.h>
#include "cmsis_os.h"

CONFIG_POOL sysCfg  __attribute__ ((__section__("osRam")));
DEVICE_SETTING_TYPE deviceCfg;//  __attribute__ ((__section__("osRam")));
DEVICE_IO_STATUS_TYPE deviceIoStatus  __attribute__ ((__section__("osRam")));

NOINIT_VARIABLE_TYPE variableNoInit  __attribute__((at(0x1000FF80)));

const char IOT_ROOT_CA [] = \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"\
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"\
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"\
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"\
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"\
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"\
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"\
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"\
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"\
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"\
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"\
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"\
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"\
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"\
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"\
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"\
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"\
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"\
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"\
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"\
"-----END CERTIFICATE-----\0";

static uint32_t GetSectorSize(uint32_t Sector);
static uint32_t GetSector(uint32_t Address);

FLASH_EraseInitTypeDef EraseInitStruct;
uint8_t sysResetMcuFlag = MCU_RESET_NONE;
uint8_t flagSystemStatus = 0;
uint8_t flagSystemStatusUpdate = 0;

osSemaphoreId config_sem_id;
osSemaphoreDef(CONFIG_SEM);
extern void Trigger_SendParamToServer(void);
void ResetMcuSet(uint8_t resetType)
{
	sysResetMcuFlag = resetType;
}

void ResetMcuTask(void)
{
		static Timeout_Type tMcuResetTimeout;
		switch(sysResetMcuFlag)
		{
			case MCU_RESET_NONE:

			break;
			case MCU_RESET_IMMEDIATELY:
				//DB_SaveAll();//thienhaiblue
				NVIC_SystemReset();
			break;
			case MCU_RESET_AFTER_10_SEC:
				InitTimeout(&tMcuResetTimeout,SYSTICK_TIME_SEC(10));
				sysResetMcuFlag = MCU_RESET_IS_WAITING;
			break; 
			case MCU_RESET_AFTER_30_SEC:
				InitTimeout(&tMcuResetTimeout,SYSTICK_TIME_SEC(30));	
				sysResetMcuFlag = MCU_RESET_IS_WAITING;
			break;
			case MCU_RESET_IS_WAITING:
				if(CheckTimeout(&tMcuResetTimeout) == SYSTICK_TIMEOUT)
				{
					//DB_SaveAll();//thienhaiblue
					NVIC_SystemReset();
				}
			break;
			default:
				//DB_SaveAll();//thienhaiblue
				NVIC_SystemReset();
				break;
		}
}

void CFG_Init(void)
{
	config_sem_id = osSemaphoreCreate(osSemaphore(CONFIG_SEM), 1);
}

void CFG_ReLoad(void)
{
	memcpy(&sysCfg, (void*)CONFIG_AREA_START, sizeof(CONFIG_POOL));
}

void DeviceIoStatus_Save(void)
{
	int16_t i;
	uint32_t u32Temp;
	uint8_t *u8Pt;
	u8Pt = (uint8_t *)&deviceIoStatus;
	u32Temp = 0;
	//if(osSemaphoreWait(config_sem_id, 500) == osOK)
	{
		for(i = 0;i < sizeof(DEVICE_IO_STATUS_TYPE)-sizeof(deviceIoStatus.crc);i++)
		{
			u32Temp += u8Pt[i];
		}
		deviceIoStatus.crc = u32Temp;
		
		OS_SST25_Write(DEVICE_IO_STATUS_ADDR,(uint8_t *)&deviceIoStatus,sizeof(DEVICE_IO_STATUS_TYPE));
		Trigger_SendParamToServer();
		//osSemaphoreRelease(config_sem_id);
	}
	
}

void OS_DeviceIoStatus_Save(void)
{
	int16_t i;
	uint32_t u32Temp;
	uint8_t *u8Pt;
	u8Pt = (uint8_t *)&deviceIoStatus;
	u32Temp = 0;
	if(osSemaphoreWait(config_sem_id, 500) == osOK)
	{
		for(i = 0;i < sizeof(DEVICE_IO_STATUS_TYPE)-sizeof(deviceIoStatus.crc);i++)
		{
			u32Temp += u8Pt[i];
		}
		deviceIoStatus.crc = u32Temp;
		
		OS_SST25_Write(DEVICE_IO_STATUS_ADDR,(uint8_t *)&deviceIoStatus,sizeof(DEVICE_IO_STATUS_TYPE));
		Trigger_SendParamToServer();
		osSemaphoreRelease(config_sem_id);
	}
	
}

void DeviceIoStatus_Load(void)
{
	uint32_t saveFlag = 0,u32Temp = 0;
	int16_t i;
	uint8_t *u8Pt;
	
	OS_SST25_Read(DEVICE_IO_STATUS_ADDR,(uint8_t *)&deviceIoStatus,sizeof(DEVICE_IO_STATUS_TYPE));

	u8Pt = (uint8_t *)&deviceIoStatus;
	u32Temp = 0;
	for(i = 0;i < sizeof(DEVICE_IO_STATUS_TYPE)-sizeof(deviceIoStatus.crc);i++)
	{
		u32Temp += u8Pt[i];
	}
	if(u32Temp != deviceIoStatus.crc)
	{
		memset((void*)&deviceIoStatus, 0xFF, sizeof deviceIoStatus);
		saveFlag = 1;
	}
	//size
	if(deviceIoStatus.size != sizeof(DEVICE_IO_STATUS_TYPE))
	{
		deviceIoStatus.size = sizeof(DEVICE_IO_STATUS_TYPE);
		saveFlag = 1;
	}
	
	for(i = 0 ;i < DEVICE_LINE_MAX;i++)
	{
		if(deviceIoStatus.line[i] == 0xff)
		{
			deviceIoStatus.line[i] = 0;
			saveFlag = 1;
		}
	}
	
	for(i = 0 ;i < DEVICE_LAMP_MAX;i++)
	{
		if(deviceIoStatus.lamp[i] == 0xff)
		{
			deviceIoStatus.lamp[i] = 0;
			saveFlag = 1;
		}
	}
	
	if(deviceIoStatus.systemStatus == 0xFFFFFFFF)
	{
		deviceIoStatus.systemStatus = 0;
		saveFlag = 1;
	}
	
	if(saveFlag)
	{
		DeviceIoStatus_Save();
	}

}

void DeviceCfg_Save(void)
{
	int16_t i;
	uint32_t u32Temp;
	uint8_t *u8Pt;
	u8Pt = (uint8_t *)&deviceCfg;
	u32Temp = 0;
	if(osSemaphoreWait(config_sem_id, 500) == osOK)
	{
		for(i = 0;i < sizeof(DEVICE_SETTING_TYPE)-sizeof(deviceCfg.crc);i++)
		{
			u32Temp += u8Pt[i];
		}
		deviceCfg.crc = u32Temp;
		
		OS_SST25_Write(DEVICE_PARAMETER_ADDR,(uint8_t *)&deviceCfg,sizeof(DEVICE_SETTING_TYPE));
		Trigger_SendParamToServer();
		osSemaphoreRelease(config_sem_id);
	}
	
}

void DeviceCfg_Load(void)
{
	uint32_t saveFlag = 0,u32Temp = 0;
	int16_t i;
	uint8_t *u8Pt;
	OS_SST25_Read(DEVICE_PARAMETER_ADDR,(uint8_t *)&deviceCfg,sizeof(DEVICE_SETTING_TYPE));

	u8Pt = (uint8_t *)&deviceCfg;
	u32Temp = 0;
	for(i = 0;i < sizeof(DEVICE_SETTING_TYPE)-sizeof(deviceCfg.crc);i++)
	{
		u32Temp += u8Pt[i];
	}
	if(u32Temp != deviceCfg.crc)
	{
		memset((void*)&deviceCfg, 0xFF, sizeof deviceCfg);
		saveFlag = 1;
	}
	//size
	if(deviceCfg.size != sizeof(DEVICE_SETTING_TYPE))
	{
		deviceCfg.size = sizeof(DEVICE_SETTING_TYPE);
		saveFlag = 1;
	}
	
	if(deviceCfg.currentSensorType == 0xff)
	{
		deviceCfg.currentSensorType = 0;
		deviceCfg.highVoltageLevel = 400;
		deviceCfg.lowVoltageLevel = 195;
		deviceCfg.highCurrentLevel = 40;
		deviceCfg.lowCurrentLevel = 0.2;
		deviceCfg.meterType = 0;
		saveFlag = 1;
	}
	if(deviceCfg.rs232.baudRate > 256000)
	{
		deviceCfg.rs232.baudRate = 9600;
		deviceCfg.rs232.dataSize = 8;
		deviceCfg.rs232.parity = 0;//NONE
		deviceCfg.rs232.stop = 1;
		saveFlag = 1;
	}
	if(deviceCfg.rs485.baudRate > 256000)
	{
		deviceCfg.rs485.baudRate = 9600;
		deviceCfg.rs485.dataSize = 8;
		deviceCfg.rs485.parity = 0;//NONE
		deviceCfg.rs485.stop = 1;
		saveFlag = 1;
	}
	
	if(deviceCfg.leakageCurentLevel == 0xffff)
	{
		deviceCfg.leakageCurentLevel = 100;//100mA
		saveFlag = 1;
	}
	if(deviceCfg.features == 0xffffffff)
	{
		deviceCfg.features = DEFAULT_FEATURES;
		saveFlag = 1;
	}
	
	for(i = 0;i < DEVICE_SCHEDULE_MAX; i++)
	{
		if((deviceCfg.schedules[i].fromYear >= 99 && deviceCfg.schedules[i].fromYear != 0xff)
		|| (deviceCfg.schedules[i].toYear >= 99 && deviceCfg.schedules[i].toYear != 0xff)
		|| (deviceCfg.schedules[i].fromMonth > 12 && deviceCfg.schedules[i].fromMonth != 0xff)
		|| (deviceCfg.schedules[i].toMonth > 12 && deviceCfg.schedules[i].toMonth != 0xff)
		|| (deviceCfg.schedules[i].fromMday > 31 && deviceCfg.schedules[i].fromMday != 0xff)
		|| (deviceCfg.schedules[i].toMday > 31 && deviceCfg.schedules[i].toMday != 0xff)
		|| (deviceCfg.schedules[i].hour >= 24 && deviceCfg.schedules[i].hour != 0xff)
		|| (deviceCfg.schedules[i].min >= 60 && deviceCfg.schedules[i].min != 0xff)
		)
		{
			deviceCfg.schedules[i].fromYear = 0xff;
			deviceCfg.schedules[i].toYear = 0xff;
			deviceCfg.schedules[i].fromMonth = 0xff;
			deviceCfg.schedules[i].toMonth = 0xff;
			deviceCfg.schedules[i].fromMday = 0xff;
			deviceCfg.schedules[i].toMday = 0xff;
			deviceCfg.schedules[i].weekDay = 0xff;
			deviceCfg.schedules[i].hour = 0xff;
			deviceCfg.schedules[i].min = 0xff;
			deviceCfg.schedules[i].type = 0;
			deviceCfg.schedules[i].valueOn = 0;
			deviceCfg.schedules[i].valueOff = 0;
		}
	}
	
	if(deviceCfg.outstationNumber[0] == 0xFF 
		|| (deviceCfg.outstationNumber[0] < '0')
		|| (deviceCfg.outstationNumber[0] > '9')
	){
		strcpy((char *)deviceCfg.outstationNumber, DEFAULT_OUTSTATION_NUMBER);
		saveFlag = 1;
	}
	
	if(deviceCfg.elsterPassword[0] == 0xff)
	{
		strcpy((char *)deviceCfg.elsterPassword, DEFAULT_ELSTER_PASSWORD);
		saveFlag = 1;
	}
	
	if(*((uint32_t *)&deviceCfg.meterVoltageScale) == 0xFFFFFFFF || deviceCfg.meterVoltageScale < 0)
	{
		deviceCfg.meterVoltageScale = 382.0;
		saveFlag = 1;
	}
	
	if((deviceCfg.meterID[0] == 0xff) 
		&& (deviceCfg.meterID[1] == 0xff)
		&& (deviceCfg.meterID[2] == 0xff)
		&& (deviceCfg.meterID[3] == 0xff)
	)
	{
		strcpy((char *)deviceCfg.meterID, "METER_NO_ID");
		saveFlag = 1;
	}
	
	if(saveFlag)
	{
		DeviceCfg_Save();
	}

}

void CFG_Load(void)
{
	uint32_t saveFlag = 0,u32Temp = 0;
	int16_t i;
	uint8_t *u8Pt;
	
	memcpy(&sysCfg, (void*)CONFIG_AREA_START, sizeof(CONFIG_POOL));
	u8Pt = (uint8_t *)&sysCfg;
	u32Temp = 0;
	for(i = 0;i < sizeof(CONFIG_POOL)-sizeof(sysCfg.crc);i++)
	{
		u32Temp += u8Pt[i];
	}
	if(u32Temp != sysCfg.crc)
	{
		memset((void*)&sysCfg, 0xFF, sizeof sysCfg);
		saveFlag = 1;
	}
	//size
	if(sysCfg.size != sizeof(CONFIG_POOL))
	{
		sysCfg.size = sizeof(CONFIG_POOL);
		saveFlag = 1;
	}
	//imei
	if((uint8_t)sysCfg.imei[0] == 0xFF)
	{
		saveFlag = 1;
		sysCfg.imei[0] = 0;
	}
	//ccid
	if((uint8_t)sysCfg.ccid[0] == 0xFF)
	{
		saveFlag = 1;
		sysCfg.ccid[0] = 0;
	}
	//cimi
	if((uint8_t)sysCfg.cimi[0] == 0xFF)
	{
		saveFlag = 1;
		sysCfg.cimi[0] = 0;
	}
	//id
	if((uint8_t)sysCfg.id[0] == 0xFF || strlen((char*)sysCfg.id) >= sizeof(sysCfg.id))
	{
		strcpy((char *)sysCfg.id,DEFAULT_ID);
		saveFlag = 1;
	}
	//firmwareVersion
	if((uint8_t)sysCfg.firmwareVersion[0] == 0xff || strlen((char*)sysCfg.firmwareVersion) >= sizeof(sysCfg.firmwareVersion))
	{
		strcpy((char *)sysCfg.firmwareVersion,FIRMWARE_VERSION);
		saveFlag = 1;
	}
	// load default SMS config password
	if((uint8_t)sysCfg.smsPwd[0] == 0xFF || sysCfg.smsPwd[0] == 0  || strlen((char*)sysCfg.smsPwd) >= sizeof(sysCfg.smsPwd))
	{
		saveFlag = 1;
		strcpy((char *)sysCfg.smsPwd, DEFAULT_SMS_PWD);
	}
	//owner
	for(i = 0;i <= MAX_OWNER;i++)
	{
		if((uint8_t)sysCfg.ownerNumber[i][0] == 0xFF || strlen((char*)sysCfg.ownerNumber[i]) >= CONFIG_SIZE_PHONE_NUMBER)
		{
			saveFlag = 1;
			strcpy((char *)sysCfg.ownerNumber[i], "");
		}
	}
	//big_boss
	if((uint8_t)sysCfg.bigBoss[0] == 0xFF || strlen((char*)sysCfg.bigBoss) >= CONFIG_SIZE_PHONE_NUMBER)
	{
		saveFlag = 1;
		strcpy((char *)sysCfg.bigBoss, DEFAULT_BOST_NUMBER);
	}
	
	//password
	if((uint8_t)sysCfg.myNumber[0] == 0xFF || sysCfg.myNumber[0] == 0  || strlen((char*)sysCfg.myNumber) >= CONFIG_SIZE_PHONE_NUMBER)
	{
		saveFlag = 1;
		strcpy((char *)sysCfg.myNumber, "");
	}
	//sms_burner
	if((uint8_t)sysCfg.smsBurner[0] == 0xFF || strlen((char*)sysCfg.smsBurner) >= CONFIG_SIZE_PHONE_NUMBER)
	{
		saveFlag = 1;
		strcpy((char *)sysCfg.smsBurner, "");
	}
	
	// load default GPRS config
	if((uint8_t)sysCfg.gprsApn[0] == 0xFF || sysCfg.gprsApn[0] == 0 || strlen((char*)sysCfg.gprsApn) >= CONFIG_SIZE_GPRS_APN)
	{
		strcpy((char *)sysCfg.gprsApn, DEFAULT_GPSR_APN);
		strcpy((char *)sysCfg.gprsUsr, DEFAULT_GPRS_USR);
		strcpy((char *)sysCfg.gprsPwd, DEFAULT_GPRS_PWD);
		saveFlag = 1;
	}
	//server ota
	// load default data server name
	if((uint8_t)sysCfg.otaMainServerName[0] == 0xFF || strlen((char*)sysCfg.otaMainServerName) >= CONFIG_SIZE_SERVER_NAME)
	{
		strcpy((char *)sysCfg.otaMainServerName, DEFAULT_DSERVER_NAME);
		sysCfg.otaMainServerPort = DEFAULT_DSERVER_PORT;
		saveFlag = 1;
	}
	
	//mqttHost
	if((uint8_t)sysCfg.mqttServerDomain[0] == 0xFF || strlen((char*)sysCfg.mqttServerDomain) >= CONFIG_SIZE_SERVER_NAME)
	{
		strcpy((char *)sysCfg.mqttServerDomain, DEFAULT_MQTT_HOST_NAME);
		sysCfg.mqttServerPort = DEFAULT_MQTT_HOST_PORT;
		saveFlag = 1;
	}
	//mqttUsername
	if((uint8_t)sysCfg.mqttUsername[0] == 0xFF || strlen((char*)sysCfg.mqttUsername) >= CONFIG_SIZE_MQTT_USERNAME)
	{
		strcpy((char *)sysCfg.mqttUsername, DEFAULT_USERNAME);
		saveFlag = 1;
	}
	//mqttPassword
	if((uint8_t)sysCfg.mqttPassword[0] == 0xFF || strlen((char*)sysCfg.mqttPassword) >= CONFIG_SIZE_MQTT_PASSWORD)
	{
		strcpy((char *)sysCfg.mqttPassword, DEFAULT_PASSWORD);
		saveFlag = 1;
	}
	//thingName
	if((uint8_t)sysCfg.thingName[0] == 0xFF || strlen((char*)sysCfg.thingName) >= CONFIG_SIZE_THING_NAME)
	{
		strcpy((char *)sysCfg.thingName, DEFAULT_THING_NAME);
		saveFlag = 1;
	}
	//thingOrgranization
	if((uint8_t)sysCfg.thingOrgranization[0] == 0xFF || strlen((char*)sysCfg.thingOrgranization) >= CONFIG_SIZE_ORGRANIZATION)
	{
		strcpy((char *)sysCfg.thingOrgranization, DEFAULT_ORGRANIZATION_ID);
		saveFlag = 1;
	}
	if(sysCfg.featureSet == 0xFFFF)
	{
		saveFlag = 1;
		sysCfg.featureSet = 0;
	}
	
	// report interval
	if(sysCfg.reportInterval == 0xFFFF)
	{
		sysCfg.reportInterval = DEFAULT_REPORT_INTERVAL;
		saveFlag = 1;
	}

	if(saveFlag)
	{
		CFG_Save();
	}

}


uint8_t CFG_CheckSum(CONFIG_POOL *sysCfg)
{
	uint32_t u32Temp = 0;
	int16_t i;
	uint8_t *u8Pt;
	u8Pt = (uint8_t *)sysCfg;
	u32Temp = 0;
	for(i = 0;i < sizeof(CONFIG_POOL)- sizeof(sysCfg->crc);i++)
	{
		u32Temp += u8Pt[i];
	}
	if(u32Temp != sysCfg->crc)
	{
		return 0;
	}
	return 1;
}

HAL_StatusTypeDef FLASH_ErasePage(uint32_t Page_Address)
{
	uint32_t SectorError = 0;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = GetSector(Page_Address);
  EraseInitStruct.NbSectors = 1;
	return HAL_FLASHEx_Erase(&EraseInitStruct,&SectorError);
}


void CFG_Save(void)
{
	int16_t i;
	uint32_t u32Temp;
	uint8_t *u8Pt;
	uint32_t *cfgdest;
	uint16_t  FlashStatus;
	uint32_t pageError = 0;
	uint8_t reTry = 0;
	if(osSemaphoreWait(config_sem_id, 500) == osOK)
	{
		if(memcmp((void *)CONFIG_AREA_START,&sysCfg,sizeof(CONFIG_POOL)) == NULL)
		{
			osSemaphoreRelease(config_sem_id);
			return;
		}
		u8Pt = (uint8_t *)&sysCfg;
		u32Temp = 0;
		for(i = 0;i < sizeof(CONFIG_POOL)-sizeof(sysCfg.crc);i++)
		{
			u32Temp += u8Pt[i];
		}
		sysCfg.crc = u32Temp;
		HAL_FLASH_Unlock();
		while(reTry++ <= 3)
		{
			if(FLASH_ErasePage(CONFIG_AREA_START) != HAL_OK)
			{
				continue;
			}
			cfgdest = (uint32_t*)&sysCfg;
			for(i=0; i < sizeof(CONFIG_POOL); i+=4) 
			{
				FlashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,CONFIG_AREA_START + i, *(uint32_t*)(cfgdest + i/4));
				if(*(uint32_t*)(cfgdest + i/4) != *(uint32_t*)(CONFIG_AREA_START + i)) //check wrote data
				{
					continue;
				}
			}
		}
		HAL_FLASH_Lock();
		Trigger_SendParamToServer();
		osSemaphoreRelease(config_sem_id);
	}
	
}


/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
  {
    sector = FLASH_SECTOR_11;  
  }

  return sector;
}

/**
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  */
static uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;
  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) ||\
     (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
  {
    sectorsize = 16 * 1024;
  }
  else if((Sector == FLASH_SECTOR_4))
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }  
  return sectorsize;
}
