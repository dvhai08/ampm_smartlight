

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__
#include <stdint.h>

#define WTD_MAIN_LOOP	0
#define WTD_SHADOW_LOOP	1
#define WTD_GSM_LOOP	2

#define SHADOW_LOOP_TIME_MAX 60000 //120 sec
#define MAIN_LOOP_TIME_MAX 30000 //10 sec
#define GSM_LOOP_TIME_MAX	30000

#define WATCHDOG_NUM	3 /*MAIN*/ 

#define WATCHDOG_TASK_NUM	2

#define OS_LOOP_TIME_MAX 10 //10 sec

void SysWatchdog(void);
void SysWatchdogFeed(uint32_t id);
void SysWatchdogDisable(uint32_t id);
void SysWatchdogEnable(uint32_t id);
void SysWatchdogInit(void);
#endif
