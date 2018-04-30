/**
 * @file timer.c
 * @brief mbed-os implementation of the timer interface needed for AWS.
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "timer_interface.h"
#include "lib/sys_tick.h"
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

bool has_timer_expired(Timer* timer) {
	return (timer->read_ms() > (timer->time_now_ms + timer->timeout_ms));
}

void countdown_ms(Timer *timer, uint32_t timeout) {
	timer->timeout_ms = timeout;
	timer->time_now_ms = timer->read_ms();
}

void countdown_sec(Timer *timer, uint32_t timeout) {
	timer->timeout_ms = (timeout * 1000);
	timer->time_now_ms = timer->read_ms();
}

uint32_t left_ms(Timer* timer) {
	if (timer->read_ms() < (timer->time_now_ms + timer->timeout_ms))
	    return ((timer->time_now_ms + timer->timeout_ms) - timer->read_ms());
	return 0;
}

void init_timer(Timer * timer){
	timer->read_ms = xTaskGetTickCount;
	timer->timeout_ms = 0;
	timer->time_now_ms = timer->read_ms();
}
#ifdef __cplusplus
}
#endif
