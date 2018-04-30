/*
 * device_info.c
 *
 *  Created on: Oct 19, 2016
 *      Author: trunghv3
 */
#include "uart.h"
#include "auth.h"
#include "device_info.h"
#include "global.h"
#include "ctype.h"
#define DEV_INFO_DBG(...) //mprintf(__VA_ARGS__)
#define WHITE_SPACE 0x20

extern FLASH_INFO_T G_FLASH_INFO;


bool is_valid_number(char *number){
	remove_country_code(&number);
	int i = 0;
#ifdef BOARD_SUPPORT_SMS_BURNER
	//check if is sms burner
	DEV_INFO_DBG("\n\rnumber: %s", number);
	if(G_FLASH_INFO.sms_burner[0] != 0)
	{
		DEV_INFO_DBG("\n\rsms_burner: %s", G_FLASH_INFO.sms_burner);
    	if(strncmp_ignore_case(number, G_FLASH_INFO.sms_burner, strlen(number))) {
    		DEV_INFO_DBG("\n\ris sms burner");
    		return true;
    	}
	}
#endif
	if(strlen(number) < MIN_NUM_LEN || strlen(number) > MAX_NUMBER_LEN) {
		DEV_INFO_DBG("\n\r%s, not valid 1", number);
		return false;
	}
	for(i = strlen(number) - 1; i >= 0;i--){
		if(number[i] < '0' || number[i] > '9') {
			DEV_INFO_DBG("\n\r%s, not valid 2", number);
			return false;
		}
	}
	if(!is_viettel_num(number)){
		DEV_INFO_DBG("\n\r%s, not valid 3", number);
		return false;
	}
	DEV_INFO_DBG("\n\r%s, valid", number);
	return true;
}
bool is_control_number(char *num) {
	if(is_valid_number(num)){
		return (is_owner(num) || is_big_boss(num));
	}else {
		return false;
	}
}
void remove_ending_white_spaces(char **number) {
	char *temp = *number;
	int i;
	//remove ending white space if exist
	for(i =  strlen(temp) - 1;i >= 0;i--) {
		if(temp[i] == WHITE_SPACE){
			temp[i] = 0;
		}
	}
}

bool update_my_number(char* my_number){
	if(strlen(my_number) > MAX_NUMBER_LEN) {
		DEV_INFO_DBG("\n\rmy number is too long");
		return false;
	}
	if(strlen(G_FLASH_INFO.my_number) == 0){
		DEV_INFO_DBG("\n\rFirst time update device number");
		remove_country_code(&my_number);
		memcpy(G_FLASH_INFO.my_number, my_number,strlen(my_number));
		update_flash_info();
		return true;
	}else {
		return true;
	}

}

bool update_password(char *old_pass, char *new_pass) {
	if(auth_password(old_pass)) {
		//DEV_INFO_DBG("\n\rold pass: %s", G_FLASH_INFO.password);
		memcpy((void*)G_FLASH_INFO.password, new_pass,strlen(new_pass));
		//DEV_INFO_DBG("\n\rnew pass: %s", G_FLASH_INFO.password);
		G_FLASH_INFO.password[strlen(new_pass)] = 0;
		update_flash_info();
		return true;
	}
	//DEV_INFO_DBG("\n\rUpdate pass fail!!!");
	return false;
}

void rm_owner(char *owner) {
	int i,j;
	remove_country_code(&owner);
	for(i = 0; i < MAX_OWNER;i++) {
		if(memcmp(owner, G_FLASH_INFO.owner[i].owner_number, strlen(owner)) == 0) {
			//G_FLASH_INFO.owner[i].is_valid = false;
			//memset(G_FLASH_INFO.owner[i].owner_number, 0, MAX_NUMBER_LEN);
			//DEV_INFO_DBG("\n\rremove owner: %s successfully", owner);
			for(j = i; j < MAX_OWNER - 1; j++)//fill other number to this positon
			{
			    G_FLASH_INFO.owner[j] = G_FLASH_INFO.owner[j+1];
			}
			G_FLASH_INFO.owner[MAX_OWNER - 1].is_valid = false;
			memset(G_FLASH_INFO.owner[MAX_OWNER - 1].owner_number, 0, MAX_NUMBER_LEN);
			break;
		}
	}

	update_flash_info();
}
/*
 * burner is #HALICO#
 */
bool add_sms_burner(char *new_owner){
	DEV_INFO_DBG("\n\radd_sms_burner");
	char *buf = new_owner+1;
	buf[strlen(buf) - 1] = 0;
	DEV_INFO_DBG("\n\rbuf: %s", buf);
	if(strlen(buf) < MAX_NUMBER_LEN && _isascii(buf[0])) {
		if(buf[0] < '9' && buf[0] > '0'){
			DEV_INFO_DBG("\n\radd_sms_burner starts with num err");
			return false;
		}
		memcpy(G_FLASH_INFO.sms_burner, buf, strlen(buf) + 1);
		update_flash_info();
		DEV_INFO_DBG("\n\radd_sms_burner ok");
		return true;
	}else {
		DEV_INFO_DBG("\n\radd_sms_burner fail");
		return false;
	}

}
void add_new_owner(char *new_owner) {
	owner_number_t *owner_ptr = NULL;
	int i;
	//owner_ptr = &G_FLASH_INFO.owner[G_FLASH_INFO.owner_next_free];
	remove_country_code(&new_owner);

	for(i = 0; i < MAX_OWNER;i++) 
	{
		if(G_FLASH_INFO.owner[i].is_valid == false)
		{
    		owner_ptr = &G_FLASH_INFO.owner[i];
            break;
		}

	}
	if(owner_ptr == NULL) 
	{
		//rearrange list, to delete list[0]  oldest number
		for(i = 0; i < MAX_OWNER - 1; i++) 
		{
			G_FLASH_INFO.owner[i] = G_FLASH_INFO.owner[i+1];
		}
		owner_ptr = &G_FLASH_INFO.owner[MAX_OWNER - 1];
	}
	memcpy(owner_ptr->owner_number, new_owner, strlen(new_owner));
	owner_ptr->is_valid = true;
	//DEV_INFO_DBG("add owner: %s successfully", new_owner);
	update_flash_info();
}
void add_big_boss(char *boss) {
	remove_country_code(&boss);
	memcpy(G_FLASH_INFO.big_boss, boss, strlen(boss));
	G_FLASH_INFO.big_boss[strlen(boss)] = 0; //end string
	update_flash_info();
}
pro_sms_event_t handle_rm_owner(char *sender, char *rm_msg, char **new_owner_p) {
	DEV_INFO_DBG("\n\rhandle_rm_owner");
	if(is_big_boss(sender)) {
		char *ptr = rm_msg + strlen(RM_OWNER_HEADER);
		char pass[7];
		int i = 0;
		while(ptr[i++] != ',');
		if((i - 1) >= MAX_PASS_LEN) {
			DEV_INFO_DBG("\n\rinvalid pass len");
			return PR_SMS_ERR_PASS_EVT;
		}
		memcpy(pass, ptr, i - 1);
		pass[i-1] = 0;//end string
		//DEV_INFO_DBG("\n\rpass: ", pass);
		if(!auth_password(pass)) return PR_SMS_ERR_PASS_EVT;
		ptr += i;

		if(strlen(ptr) >= MAX_NUMBER_LEN)
			return PR_SMS_ERR_NUM_EVT;

		char *new_owner = ptr;
		remove_ending_white_spaces(&new_owner);

		memcpy(*new_owner_p, new_owner, strlen(new_owner));

		if(!is_valid_number(new_owner)) {
			return PR_SMS_ERR_NUM_EVT;
		}
		if(is_owner(new_owner)) {
			//DEV_INFO_DBG("\n\rowner existing");
			rm_owner(new_owner);
			//DEV_INFO_DBG("\n\rremove owner ok");
			return PR_SMS_RM_OWNER_OK_EVT;
			//return false;
		}else {
			DEV_INFO_DBG("\n\rOwner not exist");
			return PR_SMS_NUM_IS_NOT_EXIST_EVT;
		}
	}else {
		DEV_INFO_DBG("\n\rOnly big boss can do this");
		return PR_SMS_NOT_BIG_BOSS_EVT;
	}

}
#ifdef BOARD_SUPPORT_SMS_BURNER
bool is_sms_burner_string(char *new_owner){
	//detect if start with . and end with . as .HAILICO.
	int len = strlen(new_owner);
	if(new_owner[0] == '#' && new_owner[len-1] == '#'){
		DEV_INFO_DBG("\n\ris_sms_burner_string true");
		return true;
	}
	DEV_INFO_DBG("\n\ris_sms_burner_string false");
	return false;
}
#endif
pro_sms_event_t handle_add_owner(char *sender, char *add_msg, char **new_owner_p) {
	DEV_INFO_DBG("\n\rhandle_add_owner");
	if(is_big_boss(sender)) {
		char *ptr = add_msg + strlen(ADD_OWNER_HEADER);
		char pass[7];
		int i = 0;
		while(ptr[i++] != ',');
		if((i - 1) >= MAX_PASS_LEN) {
			DEV_INFO_DBG("\n\rinvalid pass len");
			return PR_SMS_ERR_PASS_EVT;
		}
		memcpy(pass, ptr, i - 1);
		pass[i-1] = 0;//end string
		//DEV_INFO_DBG("\n\rpass: ", pass);
		ptr += i;

		if(strlen(ptr) >= MAX_NUMBER_LEN)
			return PR_SMS_ERR_NUM_EVT;

		char *new_owner = ptr;
		if(!auth_password(pass)) return PR_SMS_ERR_PASS_EVT;
		remove_ending_white_spaces(&new_owner);

		memcpy(*new_owner_p, new_owner, strlen(new_owner));
#ifdef BOARD_SUPPORT_SMS_BURNER
		if(is_sms_burner_string(new_owner)){
			if(add_sms_burner(new_owner)){
				DEV_INFO_DBG("\n\rAdd sms burner ok");
				return PR_SMS_ADD_OWNER_OK_EVT;
			}else {
				DEV_INFO_DBG("\n\rAdd sms burner err");
				return PR_SMS_ERR_NUM_EVT;
			}
		}
#endif
		if(!is_valid_number(new_owner)){
			return PR_SMS_ERR_NUM_EVT;
		}
		if(is_owner(new_owner)||is_big_boss(new_owner)) {
			DEV_INFO_DBG("\n\rowner existing");
			return PR_SMS_NUM_IS_EXIST_EVT;
		}
		//DEV_INFO_DBG("\n\rnew_owner: ", new_owner);
		add_new_owner(new_owner);
		DEV_INFO_DBG("\n\rAdd new owner ok");
		return PR_SMS_ADD_OWNER_OK_EVT;

	}else {
		DEV_INFO_DBG("\n\rOnly big boss can do this");
		return PR_SMS_NOT_BIG_BOSS_EVT;
	}

}

void update_device_type(char type){
	DEV_INFO_DBG("\n\rdevice type: %i", type);
	G_FLASH_INFO.type = (device_type_t)type;
	update_flash_info();
}

pro_sms_event_t handle_device_type_msg(char *sender, char *msg) {
	if(is_big_boss(sender)) {
		char *ptr = msg + strlen(ADD_OWNER_HEADER);
		char pass[7];
		int i = 0;
		while(ptr[i++] != ',');
		if((i - 1) >= MAX_PASS_LEN) {
			DEV_INFO_DBG("\n\rinvalid pass len");
			return PR_SMS_ERR_PASS_EVT;
		}
		memcpy(pass, ptr, i - 1);
		pass[i-1] = 0;//end string
		//DEV_INFO_DBG("\n\rpass: ", pass);
		if(!auth_password(pass)) return PR_SMS_ERR_PASS_EVT;
		ptr += i;

		char type = *ptr;
		type = type - 0x30; //convert to number.
		if(type > 1) {
			return PR_SMS_SET_TYPE_ERR_EVT;
		}
		update_device_type(type);
		return PR_SMS_SET_TYPE_OK_EVT;

	}else {
		DEV_INFO_DBG("\n\rOnly big boss can do this");
		return PR_SMS_NOT_BIG_BOSS_EVT;
	}
}
void clear_owner_list() {
	memset(G_FLASH_INFO.owner, 0, sizeof(owner_number_t)*MAX_OWNER);
	update_flash_info();
}
pro_sms_event_t handle_add_auth_msg(char *owner, char *auth_msg) {
	//AUTH_HEADER_MSG
	//get password:
	char old_pass[7];
	char new_pass[7];
	char *my_number;
	char *ptr = auth_msg + strlen(AUTH_HEADER_MSG);
	int c = 0;
	while(ptr[c++] != ',');

	if((c - 1) >= MAX_PASS_LEN) {
		DEV_INFO_DBG("\n\rold pass len too long");
		return PR_SMS_ERR_PASS_EVT;
	}
	//old pass
	memcpy(old_pass, ptr, c-1);
	old_pass[c-1] = 0;
	//DEV_INFO_DBG("\n\rold pass: %s", old_pass);
	ptr += c;
	c = 0;
	while(ptr[c++] != ',');
	if((c - 1) >= MAX_PASS_LEN) {
		DEV_INFO_DBG("\n\rnew pass len too long");
		return PR_SMS_ERR_PASS_EVT;
	}
	//new pass
	memcpy(new_pass, ptr, c-1);
	new_pass[c-1] = 0;
	//DEV_INFO_DBG("\n\rnew_pass: %s", new_pass);
	ptr += c;

	my_number = ptr;
	remove_ending_white_spaces(&my_number);
	if(!is_valid_number(my_number)) {
		return PR_SMS_ERR_NUM_EVT;
	}
	if(strlen(owner) > MAX_NUMBER_LEN){
		DEV_INFO_DBG("\n\rBoss num len is too long");
		return PR_SMS_ERR_PASS_EVT;
	}
	if(is_big_boss(owner)) {
		if(update_password(old_pass, new_pass)){
			update_my_number(my_number);
			clear_owner_list();
			return PR_SMS_AUTH_OK_EVT;
		}else {
			return PR_SMS_ERR_PASS_EVT;
		}

	}else {
		//check pass first
		if(auth_password(old_pass)){
			add_big_boss(owner);
			update_password(old_pass, new_pass);
			update_my_number(my_number);
			clear_owner_list();
			return PR_SMS_AUTH_OK_EVT;
		}else {
			return PR_SMS_ERR_PASS_EVT;
		}
	}

}

