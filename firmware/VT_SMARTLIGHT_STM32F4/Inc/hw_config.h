/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H__
#define __HW_CONFIG_H__

#include "system_config.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"


#define AN1_R1	10.0
#define AN1_R2	10.0
#define AN2_R1	10.0
#define AN2_R2	10.0

#define ADC_INPUT_CH1_SCALE	(3.3*(AN1_R1 + AN1_R2)/4096.0/AN1_R1)
#define ADC_INPUT_CH2_SCALE	(3.3*(AN2_R1 + AN2_R2)/4096.0/AN2_R1)

#define LED_GSM_TOGGLE 		HAL_GPIO_TogglePin(LED_GSM_GPIO_Port,LED_GSM_Pin)
#define LED_GSM_ON 				HAL_GPIO_WritePin(LED_GSM_GPIO_Port,LED_GSM_Pin,GPIO_PIN_SET)
#define LED_GSM_OFF 				HAL_GPIO_WritePin(LED_GSM_GPIO_Port,LED_GSM_Pin,GPIO_PIN_RESET)

#define LED_SYS_TOGGLE 		HAL_GPIO_TogglePin(LED_SYS_GPIO_Port,LED_SYS_Pin)
#define LED_SYS_ON 				HAL_GPIO_WritePin(LED_SYS_GPIO_Port,LED_SYS_Pin,GPIO_PIN_SET)
#define LED_SYS_OFF 				HAL_GPIO_WritePin(LED_SYS_GPIO_Port,LED_SYS_Pin,GPIO_PIN_RESET)

#define GSM_MOSFET_PORT
#define GSM_MOSFET_PIN
#define GSM_MOSFET_PIN_SET_OUTPUT
#define GSM_MOSFET_PIN_SET_INPUT
#define GSM_MOSFET_PIN_SET					HAL_GPIO_WritePin(GSM_PWR_ON_GPIO_Port, GSM_PWR_ON_Pin, GPIO_PIN_SET)
#define GSM_MOSFET_PIN_CLR					HAL_GPIO_WritePin(GSM_PWR_ON_GPIO_Port, GSM_PWR_ON_Pin, GPIO_PIN_RESET)

#define POWER_PORT
#define POWER_PIN
#define POWER_PIN_SET_OUTPUT
#define POWER_PIN_SET_INPUT
#define POWER_PIN_SET					HAL_GPIO_WritePin(GSM_PWR_KEY_GPIO_Port, GSM_PWR_KEY_Pin, GPIO_PIN_SET)
#define POWER_PIN_CLR					HAL_GPIO_WritePin(GSM_PWR_KEY_GPIO_Port, GSM_PWR_KEY_Pin, GPIO_PIN_RESET)


#define RESET_PORT
#define RESET_PIN
#define RESET_PIN_SET_OUTPUT
#define RESET_PIN_SET_INPUT
#define RESET_PIN_SET
#define RESET_PIN_CLR


#define RTS_PORT
#define RTS_PIN
#define RTS_PIN_SET_OUTPUT
#define RTS_PIN_SET_INPUT
#define RTS_PIN_SET
#define RTS_PIN_CLR


#define DTR_PORT
#define DTR_PIN
#define DTR_PIN_SET_OUTPUT
#define DTR_PIN_SET_INPUT
#define DTR_PIN_SET
#define DTR_PIN_CLR


#define DCD_PORT
#define DCD_PIN
#define DCD_PIN_SET_INPUT
#define MODEM_DCD()


#define RI_PORT
#define RI_PIN
#define GET_RI_PIN
#define RI_PIN_SET_INPUT


typedef struct {
	uint8_t bitOld;
	uint8_t bitNew;
	uint8_t highCnt;
	uint8_t lowCnt;
	uint8_t newUpdate;
}IOFilterType;

typedef struct {
	uint32_t updateCnt;
	IOFilterType din[5]; // {ACC,LOW_ACT,SOS,POWER_IN}
}IOstatusType;


extern IOstatusType  ioStatus;

typedef enum{
	SYS_INIT = 0xA5A5,
	SYS_DEINIT = 0x5A5A,
	SYS_OK = 0
}SYS_STATUS;


void GPS_Enable(void);
void GPS_Disable(void);
void IO_Control(uint32_t time);
void IO_Init(void);

#endif  /*__HW_CONFIG_H*/

