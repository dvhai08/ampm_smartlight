
#include "spi1.h"


extern SPI_HandleTypeDef hspi1;
uint8_t halSpiWriteByte(uint8_t data)
{
	uint8_t rx;
	switch(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&data, (uint8_t *)&rx, 1, 5000))
	{
		case HAL_OK:  

    break;  
  case HAL_TIMEOUT:
//		printf("SPI_HAL_TIMEOUT\r\n");
    break;  
  case HAL_ERROR:
//    printf("SPI_HAL_ERROR\r\n");
    break;
  default:
    break;
	}
	return rx;
	
}

