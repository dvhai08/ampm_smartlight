/*
 * auth.c
 *
 *  Created on: Oct 19, 2016
 *      Author: trunghv3
 */

#include "auth.h"
#include "device_info.h"
#include "stdbool.h"
#include "string.h"
#include "global.h"

#define AUTH_DBG(...) //mprintf(__VA_ARGS__)
extern FLASH_INFO_T G_FLASH_INFO;

bool auth_password(void *pass) {
	//dprintf("input pass: %s", pass);
	//dprintf("save pass: %s", G_FLASH_INFO.password);
	if(strcmp(G_FLASH_INFO.password, pass) == 0) {
		AUTH_DBG("\n\rAuth success!!!");
		return true;
	}
	AUTH_DBG("\n\rAuth fail!!!");
	return false;
}
bool is_big_boss(char *number) {
	if(strlen(G_FLASH_INFO.big_boss) == 0) {
		AUTH_DBG("\n\rDevice is not auth!!!");
		return false;
	}

	remove_country_code(&number);
	if(0 == memcmp(number, G_FLASH_INFO.big_boss, strlen(number))) {
		AUTH_DBG("\n\ris big boss");
		return true;
	}
	else
	{
		AUTH_DBG("\n\rBIG_BOSS:%s not mask with %s", G_FLASH_INFO.big_boss,number);
	}
	return false;
}
bool is_sms_burner(char *sender){
	//check if is sms burner
	if(strncmp_ignore_case(sender, G_FLASH_INFO.sms_burner, strlen(sender))) {
		AUTH_DBG("\n\ris sms burner");
		return true;
	}
	AUTH_DBG("\n\ris not sms burner");
	return false;
}
bool is_owner(char *new_owner) {
	remove_country_code(&new_owner);
	int i;
	for(i = 0; i < MAX_OWNER;i++){
		if(G_FLASH_INFO.owner[i].is_valid){
			if(0 == memcmp(new_owner, G_FLASH_INFO.owner[i].owner_number, strlen(new_owner))) {
				AUTH_DBG("\n\rexisting owner");
				return true;
			}
		}
	}
	AUTH_DBG("\n\rdevice owner not match!!!");
	return false;
}

