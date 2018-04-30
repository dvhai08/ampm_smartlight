/*
 * device_info.h
 *
 *  Created on: Oct 19, 2016
 *      Author: trunghv3
 */

#ifndef SRC_LIB_DEVICE_INFO_H_
#define SRC_LIB_DEVICE_INFO_H_

#include "flash.h"
#include "stdint.h"
#include "stdbool.h"

void update_owner(char *owner_number);
bool update_my_number(char* my_number);
bool update_password(char *old_pass, char *new_pass);
pro_sms_event_t handle_add_auth_msg(char *owner, char *auth_msg);
pro_sms_event_t handle_add_owner(char *sender, char *add_msg, char **new_owner_p);
pro_sms_event_t handle_rm_owner(char *sender, char *rm_msg, char **new_owner_p);
pro_sms_event_t handle_device_type_msg(char *owner, char *msg) ;
pro_sms_event_t handle_add_head_num(char *sender, char *msg, char **head);
bool is_control_number(char *num);
bool is_valid_number(char *number);
#endif /* SRC_LIB_DEVICE_INFO_H_ */
