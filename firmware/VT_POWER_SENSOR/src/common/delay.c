
#include "delay.h"

void DelayUs(uint32_t us)//1us
{
    us *= core_clk_mhz;
    arch_delay(us);
}




