#ifndef __SYSTEM_CONFIG_H__
#define __SYSTEM_CONFIG_H__
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"



/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */


#define CONFIG_AREA_START  ADDR_FLASH_SECTOR_2


#define MCU_RESET_IS_WAITING 4




#define MCU_RESET_NONE				0
#define MCU_RESET_IMMEDIATELY 1
#define MCU_RESET_AFTER_10_SEC 2
#define MCU_RESET_AFTER_30_SEC 3


#define FIRMWARE_VERSION	"VT_SML_V1.1.1"

#define __DATE_TIME__	__DATE__ "-"  __TIME__
#define __MY_VERSION_FULL__  FIRMWARE_VERSION "-" __DATE_TIME__

#define DEFAULT_ID					""
#define DEFAULT_BOST_NUMBER	"0978779222"
#define DEFAULT_SMS_PWD			"123456"
#define DEFAULT_GPSR_APN		"internet"
#define DEFAULT_GPRS_USR		"mms"
#define DEFAULT_GPRS_PWD		"mms"

#define DEFAULT_DSERVER_NAME	"platform.viettel.io"
#define DEFAULT_DSERVER_PORT	80


#define DEFAULT_FSERVER_NAME	"fw.m2m.vn"
#define DEFAULT_FSERVER_PORT	8092

#define DEFAULT_MQTT_HOST_NAME	"mqtt.viettel.io"
#define DEFAULT_MQTT_HOST_PORT	8883


#define DEFAULT_THING_NAME          "" ///< Thing Name of the Shadow this device is associated with
#define DEFAULT_ORGRANIZATION_ID		""
#define DEFAULT_USERNAME            ""
#define DEFAULT_PASSWORD            ""


#define LINE1_NO_POWER_ALARM					(1<<1)
#define LINE1_NO_CURRENT_WARNING			(1<<2)
#define LINE1_LEAKAGE_CURRENT_WARNING	(1<<3)

#define LINE2_NO_POWER_ALARM					 (1<<4)
#define LINE2_NO_CURRENT_WARNING			 (1<<5)
#define LINE2_LEAKAGE_CURRENT_WARNING	 (1<<6)

#define LINE3_NO_POWER_ALARM					 (1<<7)
#define LINE3_NO_CURRENT_WARNING	     (1<<8)
#define LINE3_LEAKAGE_CURRENT_WARNING	 (1<<9)

#define LINE4_NO_POWER_ALARM						(1<<10)
#define LINE4_NO_CURRENT_WARNING				(1<<11)
#define LINE4_LEAKAGE_CURRENT_WARNING	  (1<<12)

#define LINE1_LOW_CURRENT_WARNING	    (1<<13)
#define LINE1_HIGH_CURRENT_WARNING	  (1<<14)
#define LINE1_LOW_VOLTAGE_WARNING	  	(1<<15)
#define LINE1_HIGH_VOLTAGE_WARNING	  (1<<16)


#define LINE2_LOW_CURRENT_WARNING	    (1<<17)
#define LINE2_HIGH_CURRENT_WARNING	  (1<<18)
#define LINE2_LOW_VOLTAGE_WARNING	  	(1<<19)
#define LINE2_HIGH_VOLTAGE_WARNING	  (1<<20)

#define LINE3_LOW_CURRENT_WARNING	    (1<<21)
#define LINE3_HIGH_CURRENT_WARNING	  (1<<22)
#define LINE3_LOW_VOLTAGE_WARNING	  	(1<<23)
#define LINE3_HIGH_VOLTAGE_WARNING	  (1<<24)

#define LINE4_LOW_CURRENT_WARNING	    (1<<25)
#define LINE4_HIGH_CURRENT_WARNING	  (1<<26)
#define LINE4_LOW_VOLTAGE_WARNING	  	(1<<27)
#define LINE4_HIGH_VOLTAGE_WARNING	  (1<<28)
#define PGOOD_STATUS									(1<<29)


#define DEFAULT_FEATURE_LINE1_NO_POWER_ALARM					(1<<1)
#define DEFAULT_FEATURE_LINE1_NO_CURRENT_WARNING			(1<<2)
#define DEFAULT_FEATURE_LINE1_LEAKAGE_CURRENT_WARNING	(1<<3)

#define DEFAULT_FEATURE_LINE2_NO_POWER_ALARM					 (1<<4)
#define DEFAULT_FEATURE_LINE2_NO_CURRENT_WARNING			 (1<<5)
#define DEFAULT_FEATURE_LINE2_LEAKAGE_CURRENT_WARNING	 (1<<6)

#define DEFAULT_FEATURE_LINE3_NO_POWER_ALARM					 (1<<7)
#define DEFAULT_FEATURE_LINE3_NO_CURRENT_WARNING	     (1<<8)
#define DEFAULT_FEATURE_LINE3_LEAKAGE_CURRENT_WARNING	 (1<<9)

#define DEFAULT_FEATURE_LINE4_NO_POWER_ALARM						(1<<10)
#define DEFAULT_FEATURE_LINE4_NO_CURRENT_WARNING				(1<<11)
#define DEFAULT_FEATURE_LINE4_LEAKAGE_CURRENT_WARNING	  (1<<12)


#define DEFAULT_FEATURE_LINE1_LOW_CURRENT_WARNING	    (1<<13)
#define DEFAULT_FEATURE_LINE1_HIGH_CURRENT_WARNING	  (1<<14)
#define DEFAULT_FEATURE_LINE1_LOW_VOLTAGE_WARNING	  	(1<<15)
#define DEFAULT_FEATURE_LINE1_HIGH_VOLTAGE_WARNING	  (1<<16)


#define DEFAULT_FEATURE_LINE2_LOW_CURRENT_WARNING	    (1<<17)
#define DEFAULT_FEATURE_LINE2_HIGH_CURRENT_WARNING	  (1<<18)
#define DEFAULT_FEATURE_LINE2_LOW_VOLTAGE_WARNING	  	(1<<19)
#define DEFAULT_FEATURE_LINE2_HIGH_VOLTAGE_WARNING	  (1<<20)

#define DEFAULT_FEATURE_LINE3_LOW_CURRENT_WARNING	    (1<<21)
#define DEFAULT_FEATURE_LINE3_HIGH_CURRENT_WARNING	  (1<<22)
#define DEFAULT_FEATURE_LINE3_LOW_VOLTAGE_WARNING	  	(1<<23)
#define DEFAULT_FEATURE_LINE3_HIGH_VOLTAGE_WARNING	  (1<<24)

#define DEFAULT_FEATURE_LINE4_LOW_CURRENT_WARNING	    (1<<25)
#define DEFAULT_FEATURE_LINE4_HIGH_CURRENT_WARNING	  (1<<26)
#define DEFAULT_FEATURE_LINE4_LOW_VOLTAGE_WARNING	  	(1<<27)
#define DEFAULT_FEATURE_LINE4_HIGH_VOLTAGE_WARNING	  (1<<28)

#define DEFAULT_FEATURE_SMS_SCHEDULE_REPORT	  				(1<<29)
#define DEFAULT_FEATURE_SMS_WARNING_REPORT	  				(1<<30)
#define DEFAULT_FEATURE_1_RELAY							  				(1<<31)

#define DEFAULT_FEATURES	(DEFAULT_FEATURE_LINE1_NO_POWER_ALARM | DEFAULT_FEATURE_SMS_SCHEDULE_REPORT | DEFAULT_FEATURE_1_RELAY)
// =================================================

#define DEFAULT_SLEEP_TIMER		60


// system definitions
#define CONFIG_HW_VERSION			1
#define CONFIG_FW_VERSION			1
#define CONFIG_RELEASE_DATE			__TIMESTAMP__

#define CONFIG_SIZE_GPRS_APN		16
#define CONFIG_SIZE_GPRS_USR		16
#define CONFIG_SIZE_GPRS_PWD		16
#define CONFIG_SIZE_SERVER_NAME		64
#define CONFIG_SIZE_SMS_PWD			16
#define CONFIG_SIZE_PHONE_NUMBER	16

#define CONFIG_SIZE_THING_NAME	64
#define CONFIG_SIZE_ORGRANIZATION	64
#define CONFIG_SIZE_MQTT_USERNAME	128
#define CONFIG_SIZE_MQTT_PASSWORD	128

#define DEFAULT_REPORT_INTERVAL				60
#define DEFAULT_STOP_REPORT_INTERVAL		600	/* report interval when vehicle stopped, second */

#define MAX_OWNER	10
#define MAX_ALARM_PROFILE	16

#define DEVICE_SCHEDULE_MAX 8

#define DEVICE_LINE_MAX	4
#define DEVICE_LAMP_MAX 128


#define SCHEDULE_WEEKDAY_MONDAY             ((uint8_t)0x01)
#define SCHEDULE_WEEKDAY_TUESDAY            ((uint8_t)0x02)
#define SCHEDULE_WEEKDAY_WEDNESDAY          ((uint8_t)0x03)
#define SCHEDULE_WEEKDAY_THURSDAY           ((uint8_t)0x04)
#define SCHEDULE_WEEKDAY_FRIDAY             ((uint8_t)0x05)
#define SCHEDULE_WEEKDAY_SATURDAY           ((uint8_t)0x06)
#define SCHEDULE_WEEKDAY_SUNDAY             ((uint8_t)0x07)


#define CONFIG_SIZE_ELSTER_PASS_NUMBER	16
#define DEFAULT_OUTSTATION_NUMBER	"001"
#define DEFAULT_ELSTER_PASSWORD		"FEDC0003"
#define CONFIG_SIZE_ELSTER_ID	32

typedef struct __attribute__((packed))
{
	uint32_t size;
	char imei[18];
	char ccid[32];	/**< ccid**/
	char cimi[32];	/**< imsi**/
	char id[18];
	char firmwareVersion[16];
	char smsPwd[CONFIG_SIZE_SMS_PWD];					/**< SMS config password */
	char ownerNumber[MAX_OWNER][CONFIG_SIZE_PHONE_NUMBER];
	char bigBoss[CONFIG_SIZE_PHONE_NUMBER];
	char myNumber[CONFIG_SIZE_PHONE_NUMBER];
	char smsBurner[CONFIG_SIZE_PHONE_NUMBER];
	// GPRS config
	char gprsApn[CONFIG_SIZE_GPRS_APN];
	char gprsUsr[CONFIG_SIZE_GPRS_USR];
	char gprsPwd[CONFIG_SIZE_GPRS_PWD];
	uint16_t reportInterval;
	//server ota
	char  otaMainServerName[CONFIG_SIZE_SERVER_NAME];	/**< domain name */
	uint16_t otaMainServerPort;	/**< port */
	char mqttServerDomain[CONFIG_SIZE_SERVER_NAME];
	uint16_t mqttServerPort;
	char mqttUsername[CONFIG_SIZE_MQTT_USERNAME];
	char mqttPassword[CONFIG_SIZE_MQTT_PASSWORD];
	char thingName[CONFIG_SIZE_THING_NAME];
	char thingOrgranization[CONFIG_SIZE_ORGRANIZATION];
	uint16_t featureSet;
	uint8_t backup[512];
	uint32_t crc;
}CONFIG_POOL;


typedef void (*alarm_callback_t)(void);
typedef struct  {
	uint8_t fromYear;
	uint8_t toYear;
	uint8_t fromMonth;
	uint8_t toMonth;
	uint8_t fromMday;
	uint8_t toMday;
	uint8_t weekDay; //bit 1 = monday ,bit 2 = tuesday....bit 7 = sunday 
	uint8_t hour; 
	uint8_t min;
	uint8_t type;//bit 7 = 1 mean lamp, bit 7 = 0 mean line
	uint8_t valueOn;//if bit 7 =  1 mean using bits(0-6) for all device's IO, else if bit 7 = 0 mean using bit 1 for on/off
	uint8_t valueOff;//if bit 7 =  1 mean using bits(0-6) for all device's IO, else if bit 7 = 0 mean using bit 1 for on/off
}alarmParam_t;

typedef struct  {
	uint32_t baudRate;
	int8_t parity;
	int8_t dataSize;
	int8_t stop;
}comPort_t;

typedef struct __attribute__((packed))
{
	uint32_t size;
	alarmParam_t schedules[DEVICE_SCHEDULE_MAX];
	float highVoltageLevel;
	float lowVoltageLevel;
	float highCurrentLevel;
	float lowCurrentLevel;
	uint8_t meterType;
	uint8_t currentSensorType;
	comPort_t rs232;
	comPort_t rs485;
	uint16_t leakageCurentLevel;
	uint8_t outstationNumber[4];
	uint8_t elsterPassword[CONFIG_SIZE_ELSTER_PASS_NUMBER];
	int8_t meterID[CONFIG_SIZE_ELSTER_ID];
	float meterVoltageScale;
	uint32_t features;
	uint8_t backup[442];
	uint32_t crc;
}DEVICE_SETTING_TYPE; 


typedef struct __attribute__((packed))
{
	uint32_t size;
	uint8_t line[DEVICE_LINE_MAX];
	uint8_t lamp[DEVICE_LAMP_MAX];
	uint32_t systemStatus;
	uint32_t crc;
}DEVICE_IO_STATUS_TYPE;

typedef struct
{
	uint32_t ramOk;
	uint32_t lineStatus;
	uint32_t deviceStatus;
}NOINIT_VARIABLE_TYPE;

extern DEVICE_IO_STATUS_TYPE deviceIoStatus;
extern DEVICE_SETTING_TYPE deviceCfg;
extern CONFIG_POOL sysCfg;
extern NOINIT_VARIABLE_TYPE variableNoInit;
HAL_StatusTypeDef FLASH_ErasePage(uint32_t Page_Address);
void ResetMcuSet(uint8_t resetType);
void ResetMcuTask(void);
void CFG_Init(void);
void CFG_ReLoad(void);
void CFG_Save(void);
void CFG_Load(void);
void DeviceCfg_Save(void);
void DeviceCfg_Load(void);
void DeviceIoStatus_Load(void);
void DeviceIoStatus_Save(void);
void OS_DeviceIoStatus_Save(void);
#endif
