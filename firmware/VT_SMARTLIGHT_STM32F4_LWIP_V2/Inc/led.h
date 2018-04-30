#ifndef __LED__H__
#define __LED__H__


#include <stdint.h>
#include "hw_config.h"
#include "io_control.h"

extern IO_TOGGLE_TYPE	io_network_led;
extern IO_TOGGLE_TYPE	io_debug_led;


void LedInit(void);
void CtrLed(uint32_t time);

#endif

