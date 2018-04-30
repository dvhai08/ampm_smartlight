
#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include "drivers.h"


#define HAL_IO_GDO0_PIN 0
#define HAL_IO_GDO2_PIN 2
#define HAL_GDO0_PIN PIN0
#define HAL_GDO2_PIN PIN2
#define HAL_IO_GDO0_PORT PORT_F
#define HAL_IO_GDO2_PORT PORT_F

#define HAL_SPI_FLASH_CS_DEASSERT GPIO_Set(GPIOD,PIN4)
#define HAL_SPI_FLASH_CS_ASSERT GPIO_Clr(GPIOD,PIN4)

#define HAL_SPI_CS_PORT GPIOD
#define HAL_SPI_CS_PIN PIN4     
#define HAL_SPI_SOMI_PORT GPIOF
#define HAL_SPI_SOMI_PIN PIN5
#define HAL_SPI_SIMO_PIN PIN6
#define HAL_SPI_SCLK_PIN PIN4
#define HAL_SPI_CS_DEASSERT   GPIO_Set(GPIOD,HAL_SPI_CS_PIN)
#define HAL_SPI_CS_ASSERT     GPIO_Clr(GPIOD,HAL_SPI_CS_PIN)
#define HAL_SPI_RF_OFF()        GPIO_Set(GPIOF,PIN5)
#define HAL_SPI_RF_ON()        GPIO_Clr(GPIOF,PIN5)
#define HAL_SPI_SOMI_VAL()      (GPIO_Get(GPIOF) & HAL_SPI_SOMI_PIN)
//#define HAL_SPI_SOMI_VAL()      (0)
#define HAL_SPI_END()           HAL_SPI_CS_DEASSERT

#define HAL_SPI_WAIT_RXFIN()    while (!SPI_RxFull(SPI1))
#define HAL_SPI_WAIT_TXFIN()    while (!SPI_TxCompl(SPI1))

#define HAL_SPI_RXBUF()         SPI_GetData(SPI1)
#define HAL_SPI_TXBUF_SET(x)    SPI_PutData(SPI1,x)
#define HAL_SPI_BEGIN()         halSpiBegin()
void spi_init(uint8_t clockrate);
void spi_reset_device(void);
uint8_t spi_send(uint8_t addr, const uint8_t *data, uint16_t len);
uint8_t spi_send_byte(uint8_t cmd);
uint8_t spi_receive(uint8_t addr, uint8_t *data, uint16_t len);
uint8_t halSpiBegin(void);


#endif
