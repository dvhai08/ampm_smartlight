/* @file i2ceeprom.h
 * @brief function prototype i2c eeprom driver 
 * @author thienhaiblue (thienhaiblue@gmail.com)
 */

#ifndef _I2CEEPROM_H_
#define _I2CEEPROM_H_
#include "drivers.h"
#define PHASE_CONVERT_PULSE_TO_ENERGY 100
#define ADDR_kWh_WRAP 0x010A
#define ADDR_kWh_WRAP1 0x011A
#define ADDR_kWh_WRAP2 0x012A
#define ADDR_kWh_WRAP3 0x013A
#define ADDR_DAY_FAIL  0x0120
#define ADDR_BASE1 0x0000
#define ADDR_RELAY_BASE 0x150
#define ADDR_PAGE_OFFSET 0x100
#define EEPROM_ADDRESS 0xA0
#define ROLL_BACK 999999999
#define WRITE_ENABLE() GPIO_Init  (GPIOF, GPIO_OUT_LOGIC0_MODE, PIN1)
#define WRITE_DISABLE() GPIO_Init  (GPIOF, GPIO_OUT_LOGIC1_MODE, PIN1)
//#define IICEEPROM_WP_DISABLE GPIO_Set (GPIOG,PIN2)
//#define IICEEPROM_WP_ENABLE GPIO_Clr (GPIOG,PIN2)

/** @brief energy data struct
 */
typedef struct
{
	uint32_t e0;
	uint32_t e1;
	uint32_t e2;
	uint32_t e3;
}energy_t;

typedef struct
{
	uint16_t a0;
	uint16_t a1;
	uint16_t a2;
}addr_t;

extern uint32_t i2cClock;

/** \addtogroup eeprom driver
 * @{
 */

/** @brief calculate checksum
 *  @param buff_in pointer to buffer
 *  @param len length of buffer
 *  @return checksum of buffer
 */
extern uint8_t CheckSumCalculation(uint8_t *buff_in,uint8_t len);

/** @brief read data from eeprom
 *  @param addr memory address
 *  @param data address to which energy written (24bit)
 *  @return read status
 */
extern uint16_t EepromReadData(uint16_t addr,uint32_t *data);

/** @brief save data to eeprom
 *  @param addr memory address in eeprom which energy written
 *  @param data energy data
 */
extern uint16_t EepromSaveData(uint16_t addr,uint32_t data);

/** @brief read wrap data from eeprom
 *  @param addr address of wrap
 *  @param data address to which wrap written
 */
extern uint16_t EepromReadKWhWrapData(uint16_t addr,uint32_t *data);
extern uint16_t EepromReadEnergy(energy_t *energy);

/** @brief save energy to eeprom
 *  @param energy energy struct
 */
extern uint16_t EepromSaveEnergy(energy_t energy);

/** @brief delay micro second 
 */
extern void DelayUs(uint32_t i);

/** @brief delete entire eeprom data 
 */
extern uint16_t EepromClearAll(void);

/** @brief read entire eeprom data
 */
extern uint16_t EepromReadAll(void);

/** @brief write buffer to i2c eeprom
 *  @param addr address in eeprom which buffer written
 *  @param data address of buffer
 *  @param len length of buffer
 */
 
 /** @} */
 
 /** \addtogroup i2c driver
  * @{
  */
extern uint16_t iicEEPROM_write(uint16_t addr, void *dat, uint16_t len);
extern int16 iicEEPROM_read(uint16_t addr, void *dat, int len);
extern uint8_t iic_send(uint8_t data);
extern uint8_t iic_CheckBusBusy(void);
extern uint8_t iic_receive(uint8_t ok);
extern void iic_start(void);
extern void iic_stop(void);
extern void iic_bus_Reset(void);

/** @brief init i2c in lowpower mode
 */
extern void iic_init_lowpower(void);
extern void iic_init_highpower(void);
extern uint16_t EepromLoadEnerry(energy_t *energy);
extern uint16_t EepromSaveChannel(int channel, uint32_t energy);
extern void EepromInit(void);
/** @} */
#endif //emeter-iiceeprom
