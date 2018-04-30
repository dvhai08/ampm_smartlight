/*
 * auth.h
 *
 *  Created on: Oct 19, 2016
 *      Author: trunghv3
 */

#ifndef SRC_LIB_AUTH_H_
#define SRC_LIB_AUTH_H_
#include "stdint.h"
#include "stdbool.h"
bool auth_password(void *pass);
bool is_owner(char *number);
bool is_big_boss(char *new_num);
bool is_sms_burner(char *sender);
#endif /* SRC_LIB_AUTH_H_ */
