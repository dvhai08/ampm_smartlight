/*
 * relay.h
 *
 *  Created on: Oct 16, 2016
 *      Author: trunghv3
 */


#ifndef SRC_COMMON_RELAY_H_
#define SRC_COMMON_RELAY_H_
#include <stdbool.h>
#include <stdint.h>
void relay_init(void) ;
void relay_on(uint8_t num);
void relay_off(uint8_t num);
#endif /* SRC_COMMON_RELAY_H_ */

