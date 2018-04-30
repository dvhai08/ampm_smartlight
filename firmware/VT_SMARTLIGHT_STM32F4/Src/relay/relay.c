/*
 * relay.c
 *
 *  Created on: Oct 16, 2016
 *      Author: trunghv3
 */

#include "main.h"
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "system_config.h"
#include "cmsis_os.h"

osSemaphoreId relay_sem_id;
osSemaphoreDef(REALAY_SEM);

void relay_init(void) {
	
	relay_sem_id = osSemaphoreCreate(osSemaphore(REALAY_SEM), 1);
	
}


void update_relay_status(uint8_t num,bool status) {
	if(num == 0 || num > DEVICE_LINE_MAX)
		return;
	num--;
	if(status)
		variableNoInit.lineStatus |= (1<<num);
	else
		variableNoInit.lineStatus &= ~(1<<num);
	if(deviceIoStatus.line[num] != status)
	{
		deviceIoStatus.line[num] = status;
		DeviceIoStatus_Save();
	}
}


void relay_on(uint8_t num) {
	if(osSemaphoreWait(relay_sem_id, 100) == osOK)
	{
		update_relay_status(num,true);
		switch(num)
		{
			case 1:
				HAL_GPIO_WritePin(GPIOC, RELAY1_Pin, GPIO_PIN_SET);
			break;
			case 2:
				HAL_GPIO_WritePin(GPIOC, RELAY2_Pin, GPIO_PIN_SET);
			break;
			case 3:
				HAL_GPIO_WritePin(GPIOC, RELAY3_Pin, GPIO_PIN_SET);
			break;
			case 4:
				HAL_GPIO_WritePin(GPIOC, RELAY4_Pin, GPIO_PIN_SET);
			break;
		}
	}
	osSemaphoreRelease(relay_sem_id);
}

void relay_off(uint8_t num) {
	if(osSemaphoreWait(relay_sem_id, 100) == osOK)
	{
		update_relay_status(num,false);
		switch(num)
		{
			case 1:
				HAL_GPIO_WritePin(GPIOC, RELAY1_Pin, GPIO_PIN_RESET);
			break;
			case 2:
				HAL_GPIO_WritePin(GPIOC, RELAY2_Pin, GPIO_PIN_RESET);
			break;
			case 3:
				HAL_GPIO_WritePin(GPIOC, RELAY3_Pin, GPIO_PIN_RESET);
			break;
			case 4:
				HAL_GPIO_WritePin(GPIOC, RELAY4_Pin, GPIO_PIN_RESET);
			break;
		}
	}
	osSemaphoreRelease(relay_sem_id);
}

