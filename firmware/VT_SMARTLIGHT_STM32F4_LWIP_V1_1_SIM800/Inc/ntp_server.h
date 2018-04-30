

#ifndef __NTP_SERVER_H__
#define __NTP_SERVER_H__

#include "xtea.h"
#include "lib/sys_tick.h"
#include "lib/sys_time.h"
void ntp_start(void);
void ntp_stop(void);
int is_ntp_server_running(void);
void ntp_get_time(void);
#endif
