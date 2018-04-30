#include <stdint.h>

#include "hal_spi.h"
#include "cc1100.h"
#include "common.h"

extern uint8_t rf_spi_ok;

#define SPI_MODULE_DIV24_8B_POLL_CONFIG_BOARD                                        \
(tSPI){                                                                        \
/* C1   */ CLR(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|SET(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|CLR(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(2))|SET(SPI_BR_SPR(2)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}
//----------------------------------------------------------------------------------
//   Generic SPI plugin module. Supports all known MSP430 SPI interfaces.
//----------------------------------------------------------------------------------

void spi_init(uint8_t clockrate)
{
   PORT_Init (PORTF, PORT_MODULE_ALT2_MODE, HAL_SPI_SOMI_PIN | HAL_SPI_SIMO_PIN | HAL_SPI_SCLK_PIN);
   if(core_clk_mhz != 2/*Mhz*/) //BUSCLOCK = 48Mhz
    SPI_Init (SPI1, SPI_MODULE_DIV96_8B_POLL_CONFIG,NULL,PRI_LVL2,NULL);
   else
    SPI_Init (SPI1, SPI_MODULE_DIV2_8B_POLL_CONFIG,NULL,PRI_LVL2,NULL);
   /* Init CSn */
   PORT_Init (PORTD, PORT_MODULE_ALT1_MODE, HAL_SPI_CS_PIN);
   GPIO_Init (GPIOD, GPIO_OUT_LOGIC1_MODE, HAL_SPI_CS_PIN);
}


//----------------------------------------------------------------------------------
//  void spi_send(uint8_t addr, const uint8_t *buffer, uint16 length)
//
//  DESCRIPTION:
//    Write data to device, starting at internal device address "addr".
//    The device will increment the address internally for every new byte
//    that is written. For single byte write, set length to 1.
//----------------------------------------------------------------------------------
uint8_t spi_send(uint8_t addr, const uint8_t *data, uint16_t length)
{
    uint8_t rc;
    if(HAL_SPI_BEGIN() == 0) return 0xff;
    register uint16 i;
    rc = SPI_TxRxByte(SPI1,addr);
    for (i = 0; i<length; i++) {     /* writing cycle */
      SPI_TxRxByte(SPI1,data[i]);     /* Write an 8-bit data */
    }

    HAL_SPI_END();
    return rc;
}

//----------------------------------------------------------------------------------
//  uint8_t spi_send_byte(uint8_t cmd)
//
//  DESCRIPTION:
//    Special write function, writing only one byte (cmd) to the device.
//----------------------------------------------------------------------------------
uint8_t spi_send_byte(uint8_t cmd)
{
    uint8_t rc;
    if(HAL_SPI_BEGIN() == 0) return 0xff;
    rc=SPI_TxRxByte(SPI1,cmd);
    HAL_SPI_END();
    return rc;
}

//----------------------------------------------------------------------------------
//  uint8_t spi_receive(uint8_t addr, uint8_t *data, uint16_t length)
//
//  DESCRIPTION:
//    Read data from device, starting at internal device address "addr".
//    The device will increment the address internally for every new byte
//    that is read. Note that the master device needs to write a dummy byte
//    (in this case 0) for every new byte in order to generate the clock to
//    clock out the data. For single byte read, set length to 1.
//----------------------------------------------------------------------------------
uint8_t spi_receive(uint8_t addr, uint8_t *data, uint16_t length)
{

    if(HAL_SPI_BEGIN() == 0) return 0xff;
    SPI_CommByte(SPI1,&addr,1,data,length);
    HAL_SPI_END();
    return data[0];
}

uint8_t halSpiBegin(void)
{
	static uint8_t errorCnt = 0;
  uint32_t timeout = core_clk_mhz * 100;
  HAL_SPI_CS_ASSERT;
  while(timeout--)
  {
    if(!HAL_SPI_SOMI_VAL())
    {
			rf_spi_ok = 1;
			errorCnt  = 0;
      return 1;
    }
  }
	if(errorCnt++ >= 10)
	{
		rf_spi_ok = 0;
	}
  return 0;
}






