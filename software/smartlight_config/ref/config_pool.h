#ifndef _CONFIG_POOL_H_
#define _CONFIG_POOL_H_

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "sbl_config.h"
#include "sbl_iap.h"


#define ISUSB3G 1
#define USB_CDC_DBG	2
#define NOT_USE	0xff
#define USB_MODE	USB_CDC_DBG

#define FIRMWARE_VERSION	"V1.600"


#define CONFIG_SIZE_GPRS_APN		16
#define CONFIG_SIZE_GPRS_USR		16
#define CONFIG_SIZE_GPRS_PWD		16
#define CONFIG_SIZE_DSERVER_NAME	32
#define CONFIG_SIZE_IP				16
#define CONFIG_SIZE_FSERVER_NAME	32
#define CONFIG_SIZE_SMS_PWD			16
#define CONFIG_SIZE_PHONE_NUMBER	16
#define CONFIG_SIZE_MODEM_NAME		32
#define CONFIG_SIZE_OUTSTATION_NUMBER	4
#define CONFIG_SIZE_OUTSTATION_REMODE_PASSWORD	16

#define FEATURE_SUPPORT_BATTERY		0x0001

typedef struct __attribute__((packed)){
	uint8_t min; 		/*!< Minutes Register */
	uint8_t hour; 		/*!< Hours Register */
} RTC_Type;

typedef struct __attribute__((packed)){
	uint32_t status;
	uint32_t fileSize;
	uint32_t offset;
	uint32_t saveSector;
	uint32_t crc;
}FW_UPDATE_TYPE;

typedef struct __attribute__((packed)){
	uint32_t size;
	FW_UPDATE_TYPE firmware;
	uint8_t imei[18];	/**<IMEI>**/
	uint8_t id[18];
	uint8_t fwVersion[16];	/**< firmware vertion>**/
	uint8_t smsPwd[CONFIG_SIZE_SMS_PWD];	/**< SMS config password */	
	// GPRS config
	uint8_t gprsApn[CONFIG_SIZE_GPRS_APN];	/**<>**/
	uint8_t gprsUsr[CONFIG_SIZE_GPRS_USR];	/**<>**/
	uint8_t gprsPwd[CONFIG_SIZE_GPRS_PWD];	/**<>**/
	// data server config
	uint16_t dserverIp[2];	/**< ip addr */
	uint8_t dserverName[CONFIG_SIZE_DSERVER_NAME];	/**< domain name */
	uint8_t dserverUseIp;	/**<>**/
	uint16_t dserverPort;	/**< port */	
	// firmware server config
	uint16_t fserverIp[2];	/**< ip addr */
	uint8_t fserverName[CONFIG_SIZE_FSERVER_NAME];	/**< domain name */
	uint8_t fserverUseIp;	/**<>**/
	uint16_t fserverPort;	/**< port */
	
	uint16_t cserverIp[2];	/**< ip addr */
	uint8_t cserverName[CONFIG_SIZE_FSERVER_NAME];	/**< domain name */
	uint8_t cserverUseIp;	/**<>**/
	uint16_t cserverPort;	/**< port */
	
	uint32_t comBaudRate;
	uint8_t comParity;
	uint8_t comDatabits;
	uint8_t comStopbits;
	uint8_t powerWarning;
	uint8_t	bossPhoneNum[10][CONFIG_SIZE_PHONE_NUMBER];	/**<>**/
	uint32_t resetCnt;
	uint8_t debugEnable;
	uint32_t crc;
}CONFIG_POOL;

extern CONFIG_POOL sysCfg;

void CFG_Init(void);
uint32_t CFG_Save(void);
uint32_t CFG_Load(void);
void CFG_LoadDefault(void);

#endif
