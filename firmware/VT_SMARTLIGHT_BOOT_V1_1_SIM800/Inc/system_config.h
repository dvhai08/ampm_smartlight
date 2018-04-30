#ifndef __SYSTEM_CONFIG_H__
#define __SYSTEM_CONFIG_H__
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"

#define CONFIG_AREA_START  ADDR_FLASH_SECTOR_2


#define MCU_RESET_IS_WAITING 4


#define MCU_RESET_NONE				0
#define MCU_RESET_IMMEDIATELY 1
#define MCU_RESET_AFTER_10_SEC 2
#define MCU_RESET_AFTER_30_SEC 3


#define FIRMWARE_VERSION	"VT_SML_BOOT_V0"

#define DEFAULT_ID					"SML001"
#define DEFAULT_BOST_NUMBER	"0978779222"
#define DEFAULT_SMS_PWD			"12345678"
#define DEFAULT_GPSR_APN		"internet"
#define DEFAULT_GPRS_USR		"mms"
#define DEFAULT_GPRS_PWD		"mms"

#define DEFAULT_DSERVER_NAME	"platform.viettel.io"
#define DEFAULT_DSERVER_PORT	11111


#define DEFAULT_FSERVER_NAME	"platform.viettel.io
#define DEFAULT_FSERVER_PORT	50000

#define DEFAULT_SLEEP_TIMER		60


// system definitions
#define CONFIG_HW_VERSION			1
#define CONFIG_FW_VERSION			1
#define CONFIG_RELEASE_DATE			__TIMESTAMP__

#define CONFIG_SIZE_GPRS_APN		16
#define CONFIG_SIZE_GPRS_USR		16
#define CONFIG_SIZE_GPRS_PWD		16
#define CONFIG_SIZE_SERVER_NAME		31
#define CONFIG_SIZE_SMS_PWD			16
#define CONFIG_SIZE_PHONE_NUMBER	16

#define DEFAULT_REPORT_INTERVAL				30
#define DEFAULT_STOP_REPORT_INTERVAL		600	/* report interval when vehicle stopped, second */

typedef struct __attribute__((packed))
{
	uint32_t size;
	int8_t imei[18];
	int8_t id[18];
	int8_t smsPwd[CONFIG_SIZE_SMS_PWD];					/**< SMS config password */
	int8_t whiteCallerNo[CONFIG_SIZE_PHONE_NUMBER];		/**< */
	// GPRS config
	int8_t gprsApn[CONFIG_SIZE_GPRS_APN];
	int8_t gprsUsr[CONFIG_SIZE_GPRS_USR];
	int8_t gprsPwd[CONFIG_SIZE_GPRS_PWD];
	uint16_t runReportInterval;
	uint16_t stopReportInterval;
	// primary server config
	uint8_t  priDserverName[CONFIG_SIZE_SERVER_NAME];	/**< domain name */
	uint16_t priDserverPort;	/**< port */
	// secondary server config
	uint8_t  secDserverName[CONFIG_SIZE_SERVER_NAME];	/**< domain name */
	uint16_t secDserverPort;	/**< port */
	// current firmware server config
	// primary server config
	uint8_t  priFserverName[CONFIG_SIZE_SERVER_NAME];	/**< domain name */
	uint16_t priFserverPort;	/**< port */
	// secondary server config
	uint8_t  secFserverName[CONFIG_SIZE_SERVER_NAME];	/**< domain name */
	uint16_t secFserverPort;	/**< port */
	uint16_t featureSet;
	uint8_t backup[100];
	uint32_t crc;
}CONFIG_POOL;

typedef struct
{
	uint32_t ramOk;
	uint32_t lineStatus;
}NOINIT_VARIABLE_TYPE;
extern NOINIT_VARIABLE_TYPE variableNoInit;
extern CONFIG_POOL sysCfg;
HAL_StatusTypeDef FLASH_ErasePage(uint32_t Page_Address);
void ResetMcuSet(uint8_t resetType);
void ResetMcuTask(void);
void CFG_ReLoad(void);
void CFG_Save(void);
void CFG_Load(void);

#endif
