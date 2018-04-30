/**
 * @file timer.c
 * @brief mbed-os implementation of the timer interface needed for AWS.
 */
//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stddef.h>
#include "timer_interface.h"

TimerExt::TimerExt() : mbed::Timer(){

}

bool has_timer_expired(TimerAWS* timer) {
	return (timer->read_ms() > timer->timeout_ms);
}

void countdown_ms(TimerAWS *timer, uint32_t timeout) {
	timer->timeout_ms = timeout;
	timer->reset();
	timer->start();
}

void countdown_sec(TimerAWS *timer, uint32_t timeout) {
	timer->timeout_ms = (timeout * 1000);
	timer->reset();
	timer->start();
}

uint32_t left_ms(TimerAWS* timer) {
	if (timer->read_ms() < timer->timeout_ms)
	    return (timer->timeout_ms - timer->read_ms());
	return 0;
}

void init_timer(TimerAWS * timer){
	timer->stop();
	timer->reset();
	timer->timeout_ms = 0;
}
//#ifdef __cplusplus
//}
//#endif
