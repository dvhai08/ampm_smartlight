
/*---------------------------------------------------------------------------------------------------------*/
/* Includes of system headers                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "flashprog.h"
//#include "aes128.h"
#include "aes.h"
#include "string.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/


static uint32_t GetSector(uint32_t Address);
static uint32_t GetSectorSize(uint32_t Sector);



HAL_StatusTypeDef FLASH_ErasePage(uint32_t Page_Address)
{
	uint32_t SectorError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = GetSector(Page_Address);
  EraseInitStruct.NbSectors = 1;
	//HAL_Delay(10);
	return HAL_FLASHEx_Erase(&EraseInitStruct,&SectorError);
}

void FMC_DataDecryptInit(void)
{
	
}



uint16_t FMC_ProgramData(uint32_t u32startAddr, uint32_t *u32buff,uint32_t size)
{
	uint16_t  FlashStatus;
	uint32_t i;
	if(size % 4) return HAL_ERROR;

	HAL_FLASH_Unlock();
	//i = 10000;
	//while(i--);
	for (i = 0; i < size/4; i++)
	{
		
		FlashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,u32startAddr + i*4,u32buff[i]);
		
		if(HAL_OK != FlashStatus) 
		{
			HAL_FLASH_Lock();
			return FlashStatus;
			//NVIC_SystemReset();
		}
		if(u32buff[i] != *(uint32_t*)(u32startAddr + i*4)) //check wrote data
		{
			HAL_FLASH_Lock();
			return HAL_ERROR;
			//NVIC_SystemReset();
		}
	}
	HAL_FLASH_Lock();
	return FlashStatus;
}



/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
  {
    sector = FLASH_SECTOR_11;  
  }

  return sector;
}

/**
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  */
static uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;
  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) ||\
     (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
  {
    sectorsize = 16 * 1024;
  }
  else if((Sector == FLASH_SECTOR_4))
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }  
  return sectorsize;
}


