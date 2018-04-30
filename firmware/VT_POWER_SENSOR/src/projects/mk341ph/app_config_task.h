
#ifndef __APP_CONFIG_TASK_H__
#define __APP_CONFIG_TASK_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "lib/sys_tick.h"
#include "lib/ringbuf.h"
#include "system_config.h"
#include "proto_parser.h"

typedef struct __attribute__((packed)){
	uint8_t addr[6];
	uint8_t opcode;
	uint8_t data[9];
	uint32_t crc;
}ISMARTPACKAGE_TYPE_BLE;

typedef struct __attribute__((packed)){
	uint8_t handle; //not use to make crc
	ISMARTPACKAGE_TYPE_BLE packet;
}ISMARTPACKAGE;

typedef struct __attribute__((packed)){
	uint16_t u; //not use to make crc
	int16_t i;
	int16_t p;
	uint8_t energy[3];
}METER_STATUS_PACKAGE;

extern METER_STATUS_PACKAGE meterStatus;

void AppConfigTaskInit(void);
void App_PacketSendToUart(ISMARTPACKAGE *iSmartPacket);
void AppConfigTask(void);
void AppUpdateStatus(void);
#endif
