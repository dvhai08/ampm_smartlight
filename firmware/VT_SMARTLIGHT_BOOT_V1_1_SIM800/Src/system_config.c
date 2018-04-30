#include "system_config.h"
#include <stdlib.h>
#include "flashprog.h"

CONFIG_POOL sysCfg;
NOINIT_VARIABLE_TYPE variableNoInit  __attribute__((at(0x1000FF80)));
void CFG_ReLoad(void)
{
	memcpy(&sysCfg, (void*)CONFIG_AREA_START, sizeof(CONFIG_POOL));
}

uint8_t CFG_CheckSum(CONFIG_POOL *sysCfg)
{
	uint32_t u32Temp = 0;
	int16_t i;
	uint8_t *u8Pt;
	u8Pt = (uint8_t *)sysCfg;
	u32Temp = 0;
	for(i = 0;i < sizeof(CONFIG_POOL)- sizeof(sysCfg->crc);i++)
	{
		u32Temp += u8Pt[i];
	}
	if(u32Temp != sysCfg->crc)
	{
		return 0;
	}
	return 1;
}


void CFG_Save(void)
{
	int16_t i;
	uint32_t u32Temp;
	uint8_t *u8Pt;
	uint32_t *cfgdest;
	uint16_t  FlashStatus;
	uint32_t pageError = 0;
	
	if(memcmp((void *)CONFIG_AREA_START,&sysCfg,sizeof(CONFIG_POOL)) == NULL)
		return;
//	__disable_irq();	
	
	u8Pt = (uint8_t *)&sysCfg;
	u32Temp = 0;
	for(i = 0;i < sizeof(CONFIG_POOL)-sizeof(sysCfg.crc);i++)
	{
		u32Temp += u8Pt[i];
	}
	sysCfg.crc = u32Temp;
	
	HAL_FLASH_Unlock();
	if(FLASH_ErasePage(CONFIG_AREA_START) != HAL_OK)
	{
		HAL_FLASH_Lock();
		//__enable_irq();
		return;
	}
	
	cfgdest = (uint32_t*)&sysCfg;
	for(i=0; i < sizeof(CONFIG_POOL); i+=4) 
	{
		FlashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,CONFIG_AREA_START + i, *(uint32_t*)(cfgdest + i/4));
		if(*(uint32_t*)(cfgdest + i/4) != *(uint32_t*)(CONFIG_AREA_START + i)) //check wrote data
		{
			HAL_FLASH_Lock();
			return;
		}
	}
	HAL_FLASH_Lock();

}


