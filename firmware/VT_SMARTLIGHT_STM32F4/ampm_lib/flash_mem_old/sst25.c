#include "sst25.h"
#include <string.h>
#include "spi.h"


#define AMPM_FLAH_SPI_DBG(...)		//NRF_LOG_INFO(__VA_ARGS__)

#define SST_CS_Assert()			{SPI_SD_CS_PIN_CLR;}
#define SST_CS_DeAssert()		{SPI_SD_CS_PIN_SET;}	

#define SST25_READ_BURST_SIZE 32

#define SST25_TIMEOUT	100000

#define SPI_Write(x)		halSpiWriteByte(SPI2,x)

static uint8_t SST25__Status(void);
static uint8_t SST25__WriteEnable(void);
static uint8_t SST25__WriteDisable(void);
static uint8_t SST25__GlobalProtect(uint8_t enable);
uint8_t _SST25_Read(uint32_t addr, uint8_t *buf, uint32_t count);
uint32_t countErrTemp = 0;
uint32_t writeErrCnt = 0;
uint8_t sst25buf[256];

static uint8_t SST25__WriteEAR(uint32_t addr);

static uint8_t SST25__Status(void)
{
	uint8_t res;
	SST_CS_Assert(); // assert CS
	SPI_Write(0x05); // READ status command
	res = SPI_Write(0xFF); // Read back status register content
	SST_CS_DeAssert(); // de-assert cs
	return res;
}


static uint8_t SST25__CmdRead(uint8_t cmd)
{
	uint8_t res;
	SST_CS_Assert(); // assert CS
	SPI_Write(cmd); // Read confguration register
	res = SPI_Write(0xFF); // Read back status register content
	SST_CS_DeAssert(); // de-assert cs
	return res;
}

static uint8_t SST25__CmdWrite(uint8_t cmd,uint8_t data)
{
	uint8_t res;
	SST_CS_Assert(); // assert CS
	SPI_Write(cmd); // Read confguration register
	SPI_Write(data); // Read back status register content
	SST_CS_DeAssert(); // de-assert cs
	return res;
}

static uint8_t SST25__CmdReadBuf(uint8_t cmd,uint8_t *buf,uint8_t len)
{
	uint8_t res;
	SST_CS_Assert(); // assert CS
	SPI_Write(cmd); // Read confguration register
	while(len--)
	{
		*buf++ = SPI_Write(0xFF); // Read back status register content
	}
	SST_CS_DeAssert(); // de-assert cs
	return res;
}

uint8_t SST25_Init(void)
{
uint8_t buf[16];
	uint32_t timeout = SST25_TIMEOUT;
	
	SPI_InitHighFreq(2);
	while((SST25__Status() & 0x01) && timeout)
	{
		timeout--;
	}
	if(SST25__Status() != 0x00)
	{
		SST25__WriteEnable();
		SST25__CmdWrite(CMD_WRITE_STATUS,0x00);//writeStatus
		timeout = SST25_TIMEOUT;
		while((SST25__Status() & 0x01) && timeout)
		{
			timeout--;
		}
	}

	AMPM_FLAH_SPI_DBG("SST25__ReadStatus()->0x%02X\r\n",SST25__Status());
	SST25__CmdReadBuf(0x9F,buf,3);
	AMPM_FLAH_SPI_DBG("ManufactureID:0x%02X\r\n",buf[0]);
	AMPM_FLAH_SPI_DBG("Memory Type:0x%02X\r\n",buf[1]);
	AMPM_FLAH_SPI_DBG("Memory Density:0x%02X\r\n",buf[2]);
	return SST25_OK;
}

uint8_t SST25_Erase(uint32_t addr, enum SST25_ERASE_MODE mode)
{
	uint32_t  status,i,timeout = SST25_TIMEOUT;
	AMPM_FLAH_SPI_DBG("SST25_Erase(sector:0x%08X)->",addr);
	
	if(SST25__WriteEAR(addr) == SST25_FAIL)
		return SST25_FAIL;
	while(timeout)
	{
		timeout--;
		status = SST25__Status();
		if((status & 0x01) == 0) break; //BUSY
	}
	if(timeout == 0)
	{
		countErrTemp++;
		return SST25_FAIL;
	}
	if((status & 0x02) == 0)
		SST25__WriteEnable();

	SST_CS_Assert();
	switch(mode){
	case all:
		SPI_Write(CMD_ERASE_ALL);
		SST_CS_DeAssert();
		return 0;

	case block4k:
		SPI_Write(CMD_ERASE_4K);
		break;

	case block32k:
		SPI_Write(CMD_ERASE_32K);
		break;

	case block64k:
		SPI_Write(CMD_ERASE_64K);
		break;
	}

	SPI_Write(addr >> 16);
	SPI_Write(addr >> 8);
	SPI_Write(addr);

	SST_CS_DeAssert();

	timeout = SST25_TIMEOUT;
	while(timeout)
	{
		timeout--;
		i = SST25__Status();
		if((i & 0x03) == 0) break; //BUSY and WEL bit
	}
	if(timeout == 0)
	{
		countErrTemp++;
	}
	if(timeout)
	{
		AMPM_FLAH_SPI_DBG("SST25_OK\r\n");
		return SST25_OK;
	}
	AMPM_FLAH_SPI_DBG("SST25_FAIL\r\n");
	return SST25_FAIL;
}




uint8_t __SST25_Write(uint32_t addr, const uint8_t *buf, uint32_t count)
{
	uint32_t  status,i,timeout = SST25_TIMEOUT;
	AMPM_FLAH_SPI_DBG("__SST25_Write(sector:0x%08X)->",addr);
	if(SST25__WriteEAR(addr) == SST25_FAIL)
		return SST25_FAIL;
	while(timeout)
	{
		timeout--;
		status = SST25__Status();
		if((status & 0x01) == 0) break; //BUSY
	}
	if(timeout == 0)
	{
		countErrTemp++;
		AMPM_FLAH_SPI_DBG("SST25_FAIL\r\n");
		return SST25_FAIL;
	}
	if((status & 0x02) == 0)
		SST25__WriteEnable();
	SST_CS_Assert();
	SPI_Write(CMD_WRITE_BYTE);
	SPI_Write(addr >> 16);
	SPI_Write(addr >> 8);
	SPI_Write(addr);
	while(count)
	{
		SPI_Write(*buf++);
		count--;
	}
	AMPM_FLAH_SPI_DBG("->",addr);
	SST_CS_DeAssert();
	timeout = SST25_TIMEOUT;
	while(timeout)
	{
		timeout--;
		i = SST25__Status();
		if((i & 0x03) == 0) break; //BUSY and WEL bit
	}
	if(timeout == 0)
	{
		countErrTemp++;
	}
	if(timeout)
	{
		AMPM_FLAH_SPI_DBG("SST25_OK\r\n");
		return SST25_OK;
	}
	AMPM_FLAH_SPI_DBG("SST25_FAIL\r\n");
	return SST25_FAIL;
}

uint8_t _SST25_Write(uint32_t addr, const uint8_t *buf, uint32_t count)
{
	uint32_t count1 = 0;
	if((addr & 0x00000FFF) == 0)
		SST25_Erase(addr,block4k);
	if(((addr & 0x00000FFF) + count) > 4096)
	{
		count1 = (addr  + count) & 0x00000FFF;
		count -= count1;
	}
	if(__SST25_Write(addr,buf,count) != SST25_OK)
	{
		return SST25_FAIL;
	}
	if(count1)
	{
		SST25_Erase(addr + count,block4k);
		if(__SST25_Write(addr + count,buf + count,count1) != SST25_OK)
			return SST25_FAIL;
	}
	return SST25_OK;
}

uint8_t SST25_Write(uint32_t addr, const uint8_t *buf, uint32_t count)
{
	uint32_t offset = 0;
	uint16_t cnt = count;
	uint8_t *pt = (uint8_t *)buf;
	
	if(cnt <= 256)
	{
		if(_SST25_Write( addr, buf, cnt) != SST25_OK)
		{
			return SST25_FAIL;
		}
		else if(_SST25_Read( addr, sst25buf, cnt) != SST25_OK)// Check data
		{
			return SST25_FAIL;
		}
		if(memcmp(sst25buf,buf,cnt) != NULL)
		{
			writeErrCnt++;
			AMPM_FLAH_SPI_DBG("SST25_FAIL TIMES=%d\r\n",writeErrCnt);
			return SST25_FAIL;
		}
	}
	else
	{
		while(cnt)
		{
			if(cnt >= 256)
			{
				if(_SST25_Write( addr + offset, pt, 256) != SST25_OK)
				{
					return SST25_FAIL;
				}
				else if(_SST25_Read( addr + offset, sst25buf, 256) != SST25_OK)
				{
					return SST25_FAIL;
				}
				if(memcmp(sst25buf,pt,256) != NULL)
				{
					writeErrCnt++;
					AMPM_FLAH_SPI_DBG("SST25_FAIL TIMES=%d\r\n",writeErrCnt);
					return SST25_FAIL;
				}
				pt += 256;
				cnt -= 256;
				offset += 256;
			}
			else if(cnt)
			{
				if(_SST25_Write( addr + offset, pt, cnt) != SST25_OK)
				{
					return SST25_FAIL;
				}
				else if(_SST25_Read( addr + offset, sst25buf, cnt) != SST25_OK)
				{
					return SST25_FAIL;
				}
				if(memcmp(sst25buf,pt,cnt) != NULL)
				{
					writeErrCnt++;
					AMPM_FLAH_SPI_DBG("SST25_FAIL TIMES=%d\r\n",writeErrCnt);
					return SST25_FAIL;
				}
				return SST25_OK;
			}
		}
	}
	return SST25_OK;
}


uint8_t _SST25_Read(uint32_t addr, uint8_t *buf, uint32_t count)
{
	uint32_t timeout = SST25_TIMEOUT;
	AMPM_FLAH_SPI_DBG("_SST25_Read(sector:0x%08X)->",addr);
	if(SST25__WriteEAR(addr) == SST25_FAIL)
		return SST25_FAIL;
	while((SST25__Status() & 0x01) && timeout)
	{
		timeout--;
	}
	SST_CS_Assert();
	SPI_Write(CMD_READ80);
	SPI_Write(addr >> 16);
	SPI_Write(addr >> 8);
	SPI_Write(addr);
	SPI_Write(0); // write a dummy byte
	while(count--)
	{
		
		*buf = SPI_Write(0);
		//AMPM_FLAH_SPI_DBG("%02X",(uint32_t)(*buf));
		buf++;
	}
	AMPM_FLAH_SPI_DBG("->",(uint32_t)(*buf));
	SST_CS_DeAssert();
	if(timeout)
	{
		AMPM_FLAH_SPI_DBG("SST25_OK\r\n");
		return SST25_OK;
	}
	AMPM_FLAH_SPI_DBG("SST25_FAIL\r\n");
	return SST25_FAIL;
}

uint8_t SST25_Read(uint32_t addr, uint8_t *buf, uint32_t count)
{
	uint32_t offset = 0;
	uint16_t cnt = count;
	uint8_t *pt = buf;
	if(cnt <= SST25_READ_BURST_SIZE)
	{
			if(_SST25_Read( addr, buf, cnt) != SST25_OK)
				return SST25_FAIL;
	}
	else
	{
		while(cnt)
		{
			 if(cnt >= SST25_READ_BURST_SIZE)
			{
				if(_SST25_Read( addr + offset, pt, SST25_READ_BURST_SIZE) != SST25_OK)
					return SST25_FAIL;
				pt += SST25_READ_BURST_SIZE;
				cnt -= SST25_READ_BURST_SIZE;
				offset += SST25_READ_BURST_SIZE;
			}
			else if(cnt)
			{
				if(_SST25_Read( addr + offset, pt, cnt) != SST25_OK)
					return SST25_FAIL;
				return SST25_OK;
			}
		}
	}
	return SST25_OK;
}


static uint8_t SST25__WriteEAR(uint32_t addr)
{
	uint8_t status,earValue;
	uint32_t timeout = SST25_TIMEOUT;
	do
	{
		status = SST25__Status();
		timeout--;
	}while((status & 0x01) && timeout);

	if(timeout == 0) 
		return SST25_FAIL;
	earValue = SST25__CmdRead(CMD_READ_EAR);
	if(earValue != (addr >> 24))
	{
		if((status & 0x02) == 0)
			SST25__WriteEnable();
		SST25__CmdWrite(CMD_WRITE_EAR,(addr >> 24));
		timeout = SST25_TIMEOUT;
		// loop until BUSY is cleared and WEL is set
		do
		{
			status = SST25__Status();
			timeout--;
		}while((status & 0x01) && timeout);
		if(timeout == 0) 
			return SST25_FAIL;
		if( SST25__CmdRead(CMD_READ_EAR) != (addr >> 24))
			return SST25_FAIL;
	}
	return SST25_OK;
}


static uint8_t SST25__WriteEnable(void)
{
	uint8_t status;
	uint32_t timeout = SST25_TIMEOUT;
	while((SST25__Status() & 0x01) && timeout)
	{
		timeout--;
	}

	SST_CS_Assert();
	SPI_Write(CMD_WRITE_ENABLE);
	SST_CS_DeAssert(); // deassert CS to excute command
	timeout = SST25_TIMEOUT;
	// loop until BUSY is cleared and WEL is set
	do{
		status = SST25__Status();
		timeout--;
	}while(((status & 0x01) || !(status & 0x02)) && timeout);

	return 0;
}

static uint8_t SST25__WriteDisable(void)
{
	uint8_t status;

 	while(SST25__Status()&0x01);

 	SST_CS_Assert();
 	SPI_Write(CMD_WRITE_DISABLE);
 	SST_CS_DeAssert(); // deassert CS to excute command

 	// loop until BUSY is cleared and WEL is set
 	do{
 		status = SST25__Status();
 	}while((status & 0x01) || (status & 0x02));

	return 0;
}

static uint8_t SST25__GlobalProtect(uint8_t enable)
{
	uint8_t status = enable ? 0x3C : 0x00;
	uint32_t timeout = SST25_TIMEOUT;
	while((SST25__Status() & 0x01) && timeout)
	{
		timeout--;
	}

	SST_CS_Assert();
	SPI_Write(CMD_WRITE_STATUS_ENABLE);
	SST_CS_DeAssert();

	SST_CS_Assert(); // assert CS

	SPI_Write(CMD_WRITE_STATUS); // WRITE STATUS command
	SPI_Write(status); //

	SST_CS_DeAssert(); // de-assert cs

	return 0;
}





