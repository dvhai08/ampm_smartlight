#include "led.h"
#include "io_control.h"

IO_TOGGLE_TYPE	io_network_led;
IO_TOGGLE_TYPE	io_debug_led;

void LedInit(void)
{
	IO_ControlInit();
	//HAL_GPIO_WritePin(LED_SYS_GPIO_Port, LED_SYS_Pin, GPIO_PIN_SET);
	IO_ToggleSetStatus(&io_debug_led,IO_STATUS_ON_TIME_DFG,0,IO_TOGGLE_ENABLE,0xffffffff);
	IO_ToggleSetStatus(&io_network_led,IO_STATUS_ON_TIME_DFG,IO_STATUS_OFF_TIME_DFG,IO_TOGGLE_ENABLE,0xffffffff);
}


void CtrLed(uint32_t time)
{
	if(IO_ToggleProcess(&io_network_led,time)){
		HAL_GPIO_WritePin(LED_GSM_GPIO_Port, LED_GSM_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(LED_GSM_GPIO_Port, LED_GSM_Pin, GPIO_PIN_RESET);
	}
//	if(IO_ToggleProcess(&io_debug_led,time)){
//		HAL_GPIO_WritePin(LED_SYS_GPIO_Port, LED_SYS_Pin, GPIO_PIN_SET);
//	}else{
//		HAL_GPIO_WritePin(LED_SYS_GPIO_Port, LED_SYS_Pin, GPIO_PIN_RESET);
//	}
}	
