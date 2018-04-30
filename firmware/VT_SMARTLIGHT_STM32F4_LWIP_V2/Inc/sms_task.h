#ifndef __SMS_TASK_H__
#define __SMS_TASK_H__

#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "lib/ringbuf.h"
#include "ampm_gsm_sms.h"

#define ALARM_INFO		"Thong tin hen gio: \n"
#define NO_ALARM_INFO	"Thiet bi chua co thong tin hen gio"

#define DEVICE_OUT_OF_MONEY_ALERT		"Tai khoan goc cua Quy khach hien con duoi 1000d\n"
#define DEVICE_QUERY_101_RESPOND		"Tai khoan goc cua quy khach la\n"

#define DEVICE_USSD_RESPONE_TO_SMS      "He thong se gui tin nhan cho quy khach\n"
#define DEV_AUTH_OK_REPORT				__MY_VERSION_FULL__ ": " "Xac thuc thiet bi thanh cong\n"
#define DEV_AUTH_ERR_REPORT				"Loi cu phap/mat khau/so dien thoai\n"


#define DEV_ADD_OWNER_REPORT			"Them so dieu khien (%s) thanh cong\n"
#define DEV_ADD_OWNER_EXIST_REPORT		"So dieu khien (%s) da co trong danh sach\n"
#define DEV_RM_OWNER_REPORT				"Xoa so dieu khien (%s) thanh cong\n"
#define DEV_RM_OWNER_NOT_EXIST_REPORT	"So dieu khien (%s) khong co trong danh sach\n"
#define DEV_DEVICE_ON_OK_REPORT			"So %s da bat thiet bi thanh cong\n"
#define DEV_BUMP_OFF_REPORT				"So %s da tat thiet bi\n"
#define DEV_BUMP_ALREADY_OFF_REPORT		"Thiet bi dang tat\n"
#define DEV_BUMP_ALREADY_ON_REPORT		"Thiet bi dang bat\n"
#define SIM_OUT_OF_MONEY				"Tai khoan goc cua thiet bi hien con duoi 1000d, Quy khach vui long nap them de khong bi gian doan dich vu\n"
#define DEV_SET_ALARM_OK_REPORT			"Hen gio thanh cong\n"
#define DEV_SET_ALARM_ERR_REPORT		"Hen gio that bai! xin kiem tra cu phap\n"
#define DEV_DEL_ALARM_OK_REPORT			"Xoa hen gio thanh cong\n"
#define DEV_DEL_ALARM_ERR_REPORT		"Xoa gio that bai! xin kiem tra cu phap\n"
#define DEV_SET_TIME_OK_REPORT			"Cai dat thoi gian thanh cong\n"
#define DEV_SET_TIME_ERR_REPORT			"Cai dat thoi gian that bai! xin kiem tra cu phap\n"
#define DEV_SET_DATE_OK_REPORT			"Cai dat ngay thanh cong\n"
#define DEV_SET_DATE_ERR_REPORT			"Cai dat ngay that bai! xin kiem tra cu phap\n"
#define DEV_ALARM_BUMP_ON_OK			"Thiet bi da tu dong bat thanh cong\n"
#define DEV_ALARM_BUMP_OFF_OK			"Thiet bi da tu dong tat\n"

#define DEVICE_AC_LINE_OUT_RES_ON_HP		"Thiet bi dang BAT. Mat dien luoi luc \n"
#define DEVICE_AC_LINE_OUT_RES_OFF_HP		"Thiet bi dang TAT. Mat dien luoi luc \n"
#define DEVICE_AC_LINE_OK_RES_ON_HP		"Thiet bi dang BAT. Da co dien luoi luc \n"
#define DEVICE_AC_LINE_OK_RES_OFF_HP		"Thiet bi dang TAT. Da co dien luoi luc \n"

#define DEV_RESET_REPORT "Thiet bi se khoi dong lai trong 30 giay\n\r"
#define DEV_INTERVAL_REPORT "Chu ky gui goi tin la %d giay\n\r"
#define DEV_HIGH_VOLTAGE_LEVEL_REPORT "Muc canh bao ap cao la %f V\n\r"
#define DEV_LOW_VOLTAGE_LEVEL_REPORT "Muc canh bao ap thap la %f V\n\r"
#define DEV_HIGH_CURRENT_LEVEL_REPORT "Muc canh bao dong cao la %f A\n\r"
#define DEV_LOW_CURRENT_LEVEL_REPORT "Muc canh bao dong thap la %f A\n\r"

#define DEV_CURRENT_LEAKAGE_LEVEL_REPORT "Muc canh bao dong ro la %d mA\n\r"

#define DEV_GET_RELAY_STATUS_RES		"Trang thai hien tai: %s"

char * FindBossPhone(char *phone);
char * FindUserPhone(char *phone);
char * FindAllPhone(char *phone);

void io_process_action_task(void);
void all_sms_init(void);


#endif
