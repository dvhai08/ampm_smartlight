/* @file i2ceeprom.c
 * @brief i2c eeprom driver
 * @author thienhaiblue (thienhaiblue@gmail.com)
 */

#include <stdint.h>
#include "i2ceeprom.h"
#include "delay.h"

#define PACKET_LENGTH 16
#define CHANNEL_PACKET_LENGTH 5
#define ADDR_OFFSET_CHANNEL0 (0)
#define ADDR_OFFSET_CHANNEL1 (CHANNEL_PACKET_LENGTH)
#define ADDR_OFFSET_CHANNEL2 (CHANNEL_PACKET_LENGTH*2)
#define ADDR_OFFSET_CHANNEL3 (ADDR_PAGE_OFFSET+0)
#define ADDR_WRAP_SPACE 0x10

#define ERROR_BASE1_DATA1 1
#define ERROR_BASE1_DATA2 2
#define ERROR_BASE2_DATA1 3
#define ERROR_BASE2_DATA2 4

#define MAX_IIC_TRIES 200
#define IIC_TIMEOUT     10000

#define I2C_SDA_DD_SET GPIOE_PDDR &= ~PIN2
#define I2C_SDA_DD_CLR GPIOE_PDDR |= PIN2;GPIOE_PDOR &= ~PIN2
#define I2C_SDA_PIN (GPIOE_PDIR & PIN2)

#define I2C_SCL_DD_SET GPIOE_PDDR &= ~PIN3
#define I2C_SCL_DD_CLR GPIOE_PDDR |= PIN3;GPIOE_PDOR &= ~PIN3

#if defined (__ICCARM__) /* IAR   */
#pragma pack(1)
typedef union
#elif defined (__CC_ARM)
typedef union __attribute__((packed))
#endif
/* TypeDefs */
{
    uint8_t  bytes[4];
    uint32_t ulword;	
    uint16_t uword[2];	
}T32_16_8;

#if defined (__ICCARM__) /* IAR   */
#pragma pack(1)
typedef struct
#elif defined (__CC_ARM)
typedef struct __attribute__((packed))
#endif
{ 
    T32_16_8 data1;
	  uint8_t  crc1;
    T32_16_8 data2;
	  uint8_t  crc2;
    T32_16_8 data3;
    uint8_t  crc3;
	  T32_16_8 data4;
    uint8_t  crc4;
} EEPROM_STRUCT_TYPE;

#if defined (__ICCARM__) /* IAR   */
#pragma pack(1)
typedef struct
#elif defined (__CC_ARM)
typedef struct __attribute__((packed))
#endif
{ 
    T32_16_8 data;
    uint8_t  crc;
} CHANNEL_STRUCT_TYPE;

#if defined (__ICCARM__) /* IAR   */
#pragma pack()
#endif
/* TypeDefs */
typedef union
{
    uint8_t  buff[PACKET_LENGTH];
    EEPROM_STRUCT_TYPE frame;		
}EEPROM_PACKET_TYPE;

typedef union
{
    uint8_t buff[CHANNEL_PACKET_LENGTH];
    CHANNEL_STRUCT_TYPE frame;
}CHANNEL_PACKET_TYPE;

void EepromInit(void)
{
  uint8_t write[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  uint8_t index = 0;
  for (index=0x11; index<0x20; index++)
  {
    DelayUs(5000);//5ms
    iicEEPROM_write(index<<4,write,16);
  }
}

uint16_t EepromLoadEnerry(energy_t *energy)
{
  uint32_t exit = 0,tryCnt = 0,i;
  energy_t total_energy,temp;
  while(!exit)
  {
    /*Init iic*/
    
    iic_bus_Reset();
    for(i = 0;i < 10;i++)
    {
      DelayUs(5000);//5ms
      if(EepromReadEnergy(&total_energy))
      {
        if(i == 0)
        {
          temp.e0 = total_energy.e0;
          temp.e1 = total_energy.e1;
          temp.e2 = total_energy.e2;
					temp.e3 = total_energy.e3;
        }
        else if ((temp.e0 != total_energy.e0)
					|| (temp.e1 != total_energy.e1)
				  || (temp.e2 != total_energy.e2)
				  || (temp.e3 != total_energy.e3))
//        else if(u32Temp != total_consumed_energy)
        {
          break;
        }
      }
      else
      {
        break;
      }
    }
    if(i >= 10)
    {
      exit = 1;
    }
    else
    {
      DelayUs(10000);//10ms
    }
    
    if(tryCnt++ > 10)
    {
      exit = 1;
      total_energy.e0 = 0;
      total_energy.e1 = 0;
      total_energy.e2 = 0;
			total_energy.e3 = 0;
    }
  }
  
  *energy = total_energy;
  
  if(tryCnt > 10)
    return 0; //fail
  return 1; //sucess
}

uint16_t EepromSaveEnergy(energy_t energy)
{  
  uint32_t energy_addr,energy_long;
  uint32_t kWh_wrap=0;
  uint32_t *p = (uint32_t*)&energy;
  int i;
  for (i=0;i<4;i++)  /* 4 channel */
  {
    if (*p > ROLL_BACK) /* check energy overflow */
    {
      *p -= ROLL_BACK;
    }
    energy_long = *p;
    energy_addr = *p/PHASE_CONVERT_PULSE_TO_ENERGY;
    EepromReadKWhWrapData(ADDR_kWh_WRAP+i*ADDR_WRAP_SPACE,&kWh_wrap);
	if (i!=3)
	{
		if(EepromSaveData((energy_addr>>12)&0xFFF0+i*CHANNEL_PACKET_LENGTH,energy_long) == FALSE)
			return FALSE;
	}
	else
	{
		if(EepromSaveData((energy_addr>>12)&0xFFF0+ADDR_OFFSET_CHANNEL3,energy_long) == FALSE)
			return FALSE;
	}
    if(kWh_wrap != (energy_addr>>16))
    {
      EepromSaveData(ADDR_kWh_WRAP+i*ADDR_WRAP_SPACE,energy_addr>>16);
    }
    p++;
  }
  return TRUE;
}

uint16_t EepromSaveChannel(int channel, uint32_t energy)
{
  uint32_t energy_addr,energy_long;
  uint32_t kWh_wrap;
  uint32_t *p = (uint32_t*)&energy;
  if (channel > 2) return FALSE;
  if (*p > ROLL_BACK)
  {
    *p -= ROLL_BACK;
  }
  energy_long = *p;
  energy_addr = *p/PHASE_CONVERT_PULSE_TO_ENERGY;
  EepromReadKWhWrapData(ADDR_kWh_WRAP+channel*CHANNEL_PACKET_LENGTH,&kWh_wrap);
  if(EepromSaveData(((energy_addr>>16)*16)+channel*CHANNEL_PACKET_LENGTH,energy_long) == FALSE)  return FALSE;
  if(kWh_wrap != (energy_addr>>16))
  {
    EepromSaveData(ADDR_kWh_WRAP+channel*CHANNEL_PACKET_LENGTH,energy_addr>>16);
  }
  return TRUE;
}

uint16_t EepromReadEnergy(energy_t *energy)
{   
    uint32_t kWh_wrap,energy_long;
  
    if(EepromReadData(ADDR_kWh_WRAP,&kWh_wrap) == FALSE)
			return FALSE;
    if(EepromReadData(kWh_wrap + ADDR_OFFSET_CHANNEL0,&energy_long) == FALSE)
			return FALSE;
    energy->e0 = energy_long;
		
    if(EepromReadData(ADDR_kWh_WRAP1,&kWh_wrap) == FALSE)
			return FALSE;
    if(EepromReadData(kWh_wrap + ADDR_OFFSET_CHANNEL1,&energy_long) == FALSE)
			return FALSE;
    energy->e1 = energy_long;
    
    if(EepromReadData(ADDR_kWh_WRAP2,&kWh_wrap) == FALSE)
			return FALSE;
    if(EepromReadData(kWh_wrap + ADDR_OFFSET_CHANNEL2,&energy_long) == FALSE)
			return FALSE;
    energy->e2 = energy_long;
		
		if(EepromReadData(ADDR_kWh_WRAP3,&kWh_wrap) == FALSE)
			return FALSE;
    if(EepromReadData(kWh_wrap + ADDR_OFFSET_CHANNEL3,&energy_long) == FALSE) 
			return FALSE;
    energy->e3 = energy_long;
    return TRUE;
}

uint16_t EepromSaveData(uint16_t addr,uint32_t energy)
{   
    uint16_t res;
    uint8_t tryCnt = 3;
    //EEPROM_PACKET_TYPE  eeprom_packet_data;
    CHANNEL_PACKET_TYPE channel_packet_data;

//    IICEEPROM_WP_ENABLE;
    while(tryCnt)
    {
      tryCnt--;
      res = FALSE;
      DelayUs(5000);//5ms
      channel_packet_data.frame.data.ulword = energy;
      channel_packet_data.frame.crc = CheckSumCalculation((uint8_t *)&channel_packet_data.frame.data.bytes[0], 4);
      res = iicEEPROM_write(addr, (uint8_t *)&channel_packet_data.buff[0], CHANNEL_PACKET_LENGTH);
      if(res == TRUE)
      {
         DelayUs(5000);//5ms
         if(iicEEPROM_read(addr , (uint8_t *)&channel_packet_data.buff[0], CHANNEL_PACKET_LENGTH) == TRUE)
         {
            if(channel_packet_data.frame.crc == CheckSumCalculation((uint8_t *)&channel_packet_data.buff[0], CHANNEL_PACKET_LENGTH - 1))
            {
              tryCnt = 0; //exit
            }
            else
            {
               res = FALSE;
            }
         }
      }
    }
    //IICEEPROM_WP_DISABLE;
    return  res;
}


uint16_t EepromReadData(uint16_t addr,uint32_t *data)
{   
    uint8_t readTimes = 3;
    uint16_t res = FALSE;
    //EEPROM_PACKET_TYPE  eeprom_packet_data;
    CHANNEL_PACKET_TYPE channel_packet_data;
    
    //IICEEPROM_WP_ENABLE;
    while(readTimes)
    {
      readTimes--;
      res = FALSE;
      if(iicEEPROM_read(addr, (uint8_t *)&channel_packet_data.buff[0], CHANNEL_PACKET_LENGTH) == TRUE)
      {
        if(channel_packet_data.frame.crc == CheckSumCalculation((uint8_t *)&channel_packet_data.frame.data.bytes[0], 4))
        {
            *data = channel_packet_data.frame.data.ulword;
            res = TRUE;
            readTimes = 0; //exit
        }
      }

    }
    //IICEEPROM_WP_DISABLE;
    return res;
}

uint16_t EepromReadKWhWrapData(uint16_t addr,uint32_t *data)
{   
    uint8_t readTimes = 3;
    uint16_t res;
    //EEPROM_PACKET_TYPE  eeprom_packet_data;
    CHANNEL_PACKET_TYPE channel_packet_data;

    //IICEEPROM_WP_ENABLE;
    while(readTimes)
    {
      readTimes--;
      res = FALSE;
      if(iicEEPROM_read(addr, (uint8_t *)&channel_packet_data.buff[0], CHANNEL_PACKET_LENGTH) == TRUE)
      {
        if(channel_packet_data.frame.crc == CheckSumCalculation((uint8_t *)&channel_packet_data.frame.data.bytes[0], 4))
        {
            *data = channel_packet_data.frame.data.ulword;
            res = TRUE;
        }
      }
    }
    //IICEEPROM_WP_DISABLE;
    return (res);
}

uint8_t CheckSumCalculation(uint8_t *buff_in,uint8_t number_byte)
{   uint8_t i,check_sum = 0;
    for(i = 0; i < number_byte; i++)
    {
      check_sum += buff_in[i];
    }
    return check_sum;
}

uint16_t EepromClearAll(void)
{   
    uint16_t i,res;
    EEPROM_PACKET_TYPE  eeprom_packet_data;
    //IICEEPROM_WP_ENABLE;
    for(i = 0; i < PACKET_LENGTH ;i++)
      eeprom_packet_data.buff[i] = 0x00;
    for(i = 0;i < 512; i += 16)
    {
      res = iicEEPROM_write(i, (uint8_t *)&eeprom_packet_data.buff[0], PACKET_LENGTH);
      DelayUs(5000); //5ms
    }
    // IICEEPROM_WP_DISABLE;
    return res;
}

uint16_t EepromReadAll(void)
{   
    uint16_t i,res;
    EEPROM_PACKET_TYPE  eeprom_packet_data;
    //IICEEPROM_WP_ENABLE;
    for(i = 0;i < 512; i += 16)
    {
      for(i = 0; i < PACKET_LENGTH ;i++)
        eeprom_packet_data.buff[i] = 0x00;
      res = iicEEPROM_read(i, (uint8_t *)&eeprom_packet_data.buff[0], PACKET_LENGTH);
    }
    //IICEEPROM_WP_DISABLE;
    return res;
}


uint8_t iic_CheckBusBusy(void)
{
  volatile uint32_t timeout = core_clk_mhz *  IIC_TIMEOUT;
  I2C_StopSignal(I2C1);
  volatile int testTimeout=10000;
  while(timeout--)
  {
    if(I2C_GetBusyFlag(I2C1) == 0)
      return 0;
  }
  return 1;
}

/*********************************************************\
* Initiate IIC Start Condition
\*********************************************************/
void iic_start(void)
{
 uint32_t timeout = core_clk_mhz *  IIC_TIMEOUT;
 I2C_StartSignal(I2C1);
 while ((!I2C_GetBusyFlag(I2C1)) && timeout--);
} //*** Wait until BUSY=1


/*********************************************************\
* Initiate IIC Stop Condition
\*********************************************************/
void iic_stop(void)
{
 uint32_t timeout = core_clk_mhz *  IIC_TIMEOUT;
 I2C_StopSignal(I2C1);
 while ((!I2C_GetBusyFlag(I2C1)) && timeout--);
} //*** Wait until BUSY=1


uint8_t iic_send(uint8_t data)
{  
  volatile uint32 i=0,timeout = core_clk_mhz *  IIC_TIMEOUT,err;
  while ((!I2C_GetTcfFlag(I2C1)) && timeout--);
  /* Send data to Slave, Wait on action complete, Clear Irq. flag */
  I2C_WrData(I2C1, data);
  /*Wait*/
  timeout = core_clk_mhz *  IIC_TIMEOUT;
  while((!I2C_GetIsrFlag(I2C1)) && timeout--);
  I2C_ClrIsrFlag(I2C1);
  /*Get a ack*/
  return I2C_GetRxakFlag(I2C1);
}

uint8_t iic_receive(uint8_t ok)
{
  volatile uint32_t timeout = core_clk_mhz *  IIC_TIMEOUT;
  uint8_t data = 0;
  while ((!I2C_GetTcfFlag(I2C1)) && timeout--);
  if(ok == TRUE){
    I2C_SetAckMode(I2C1);
  }else{
    I2C_SetNackMode(I2C1);
  }
  /*get message*/
  data = I2C_RdData(I2C1);
  timeout = core_clk_mhz *  IIC_TIMEOUT;
  while((!I2C_GetIsrFlag(I2C1)) && timeout--);
  I2C_ClrIsrFlag(I2C1);
  return data;
}

uint16_t iicEEPROM_write(uint16_t addr, void *dat, uint16_t len)
{
  int i;
  int j;
  uint16_t section_len;
  uint8_t *p;
  uint8_t *q;
  /* If the write spreads across pages in the EEPROM, we need to split the write
     into sections. */
  q = (uint8_t *) dat;
  while (len > 0)
  {
    section_len = len;
    for (i = 0;  i < MAX_IIC_TRIES;  ++i)
    {
            /*test device already*/
      if(i)
      {
        /* Read FALSE, retry */
        if(iic_CheckBusBusy())
          continue;
      }
      /*Send a start*/
      I2C_SetTxMode(I2C1);
      iic_start();
#if EEPROM_PAGE_SIZE == 32
      if (iic_send(0xA0)  ||  iic_send(addr/0x100)  ||  iic_send(addr))
          continue;
#else
      if (iic_send(0xA0 | ((uint8_t)(addr/0x100)*2))  ||  iic_send(addr))
          continue;
#endif
      p = q;
      for (j = section_len;  j > 0;  j--)
      {
          if (iic_send(*p++))
              break;
      }
      if (j == 0)
          break;
     iic_stop();
    }
    iic_stop();
    if (i >= MAX_IIC_TRIES)
    {
        iic_bus_Reset(); 
        return FALSE;
    }
    len -= section_len;
    addr += section_len;
    q += section_len;
  }
  return TRUE;
}



int16 iicEEPROM_read(uint16_t addr, void *dat, int len)
{
  uint8 i,j;
  uint8_t *p;
  for (i = 0;  i < MAX_IIC_TRIES;  ++i)
  {
    /*test device already*/
    if(i)
    {
      /* Read FALSE, retry */
      if(iic_CheckBusBusy())
        continue;
    }
    /*Send a start*/
    I2C_SetTxMode(I2C1);
    iic_start();
#if EEPROM_PAGE_SIZE == 32
    if (iic_send(0xA0)  ||  iic_send(addr/0x100)  ||  iic_send(addr))
      continue;
#else
    if (iic_send(0xA0 | ((uint8_t)(addr/0x100)*2))  ||  iic_send(addr))
      continue;
#endif
    p = (uint8_t *) dat;
    /*Send start again*/
    I2C_RepeatStartSignal(I2C1);
#if EEPROM_PAGE_SIZE == 32
    if (iic_send(0xA1))
        continue;
#else
    if (iic_send(0xA1 | ((uint8_t)(addr/0x100)*2)))
        continue;
#endif
    I2C_SetRxMode(I2C1);
    iic_receive(TRUE);
    for (j = len;  j > 0;  j--)
        *p++ = iic_receive(TRUE);
    iic_receive(FALSE);
    iic_stop();
    return TRUE;
  }
  iic_stop();
  iic_bus_Reset(); 
  return FALSE;
}

void iic_init_lowpower(void)
{
  I2C1_F = SET(I2C_F_MULT(0))|SET(I2C_F_ICR(0x00));
}

void iic_init_highpower(void)
{
  iic_bus_Reset();
}


void iic_bus_Reset(void)
{
 int loop;
 // Disable the I2C block on the Host Controller
 I2C1_C1 &= ~(I2C_C1_IICEN_MASK);
 PORT_Init (PORTE, PORT_MODULE_LED_MODE, PIN2|PIN3);
 I2C_SDA_DD_SET;
 I2C_SCL_DD_SET;
 arch_delay(5);
 /* Create START condition (SDA goes low while SCL is high) */
 I2C_SDA_DD_CLR; // SDA = 0
 arch_delay(5);
 I2C_SCL_DD_SET; // SCL = 1
 arch_delay(5);
 /* Release SDA back high */
 I2C_SDA_DD_SET; // SDA = 1
 arch_delay(5);
 /* Clock SCL for at least 9 clocks until SDA goes high */
 /* This loop is significantly greater than 9 clocks to */
 /* make sure that this condition is met. */
 loop = 0;
 while (loop < 100)
 {
   loop++;
   /* Apply one SCL clock pulse */
   I2C_SCL_DD_CLR; // SCL = 1
   arch_delay(5);
   I2C_SCL_DD_SET; // SCL = 0
   arch_delay(5);
   /* If SDA is high and a complete byte was sent then exit the loop */
   if ((I2C_SDA_PIN) && ((loop % 9) == 0))
    break;
 }

 /* Create STOP condition (SDA goes high while SCL is high) */
 I2C_SDA_DD_CLR; // SDA = 0
 arch_delay(5);
 I2C_SCL_DD_SET; // SCL = 1
 arch_delay(5);
 I2C_SDA_DD_SET; // SDA = 1
 arch_delay(5);
 //Set operation back to default for all pins on PTBDD and Enable I2C
 PORT_Init (PORTE, PORT_MODULE_ALT4_PULLUP_ON_MODE, PIN2|PIN3);
 I2C_Init (I2C1, I2C_MODULE_100K_MASTER_IRQ_DI_CONFIG);
 /*Lowpwer configutation*/
 if(core_clk_mhz == 2)
   I2C1_F = SET(I2C_F_MULT(0))|SET(I2C_F_ICR(0x00));
}

