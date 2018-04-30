
#ifndef __RELAY_H__
#define __RELAY_H__
#include <stdint.h>
#include "drivers.h"
#include "system_config.h"
#include "i2ceeprom.h"


#define RELAY3_PDOR GPIOE_PDOR
#define RELAY2_PDOR GPIOA_PDOR
#define RELAY1_PDOR GPIOA_PDOR

#define RELAY1_ON 0x01
#define RELAY2_ON 0x02
#define RELAY3_ON 0x04
#define RELAY_ON  0xAA
#define RELAY_OFF 0

#define RELAY3_PIN PIN2
#define RELAY2_PIN PIN6
#define RELAY1_PIN PIN7

extern uint8_t relay3,relay1,relay2;
extern volatile uint8_t relay_update_flag;
void relay_control(uint8_t pinmask, uint8_t pinval);
void tmr0Callback(TMR_CH_CALLBACK_TYPE type, int32 result);


#endif

