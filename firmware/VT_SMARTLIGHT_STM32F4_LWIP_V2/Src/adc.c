/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f2xx_hal.h"
#include "cmsis_os.h"
#include "mbedtls.h"

__IO uint16_t uhADCxConvertedValue[2];


void AppAdcStart(void)
{
	if(HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&uhADCxConvertedValue, 2) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
}