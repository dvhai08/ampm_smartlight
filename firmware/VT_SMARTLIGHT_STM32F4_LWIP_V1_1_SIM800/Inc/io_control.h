#ifndef __IO_CONTROL__H__
#define __IO_CONTROL__H__


#include <stdint.h>





typedef struct
{
    uint32_t onTime;
		uint32_t offTime;
		int32_t counter;
		uint8_t status;
		uint8_t enable;
		uint32_t times;
		uint32_t timesSave;
} IO_TOGGLE_TYPE;


#define TIMER_PERIOD	1	//ms

#define IO_STATUS_ON	1
#define IO_STATUS_OFF 0

#define IO_STATUS_ON_TIME_DFG	(500 / TIMER_PERIOD) /*1s */
#define IO_STATUS_OFF_TIME_DFG	(500 / TIMER_PERIOD) /*1s */

#define IO_TOGGLE_ENABLE	1
#define IO_TOGGLE_DISABLE 0

#define IO_MAX_TIMES 0xffffffff
#define IO_MAX_VALUE 0xffffffff


uint8_t IO_ToggleProcess(IO_TOGGLE_TYPE *ioCtrl, uint32_t preodic);
void IO_ToggleSetStatus(IO_TOGGLE_TYPE *ledCtr,uint32_t onTime,uint32_t offTime,uint32_t enable,uint32_t times);
void IO_ControlInit(void);

#endif

