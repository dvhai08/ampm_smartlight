#include "io_control.h"
#ifdef CMSIS_OS_RTX 
#include "cmsis_os.h"
osSemaphoreId io_control_sem_id;
osSemaphoreDef(IO_CONTROL_SEM);
#endif

void IO_ControlInit(void)
{
	io_control_sem_id = osSemaphoreCreate(osSemaphore(IO_CONTROL_SEM), 1);
}

void IO_ToggleSetStatus(IO_TOGGLE_TYPE *ioCtrl,uint32_t onTime,uint32_t offTime,uint32_t enable,uint32_t times)		
{
	if(osSemaphoreWait(io_control_sem_id, 100) == osOK)
	{
		if(ioCtrl->timesSave != times 
		|| ioCtrl->times == 0
		|| ioCtrl->enable != enable
		|| ioCtrl->onTime != onTime
		|| ioCtrl->offTime != offTime
		)
		{
			ioCtrl->onTime = onTime;
			ioCtrl->offTime = offTime;
			ioCtrl->counter = 0;
			ioCtrl->enable = enable;
			ioCtrl->times = times;
			ioCtrl->timesSave = times;
		}
	}
	osSemaphoreRelease(io_control_sem_id);
}													
											
uint8_t IO_ToggleProcess(IO_TOGGLE_TYPE *ioCtrl, uint32_t preodic)	
{
	if(ioCtrl->enable == IO_TOGGLE_ENABLE) 
	{
			if(ioCtrl->counter > preodic)
				ioCtrl->counter -= preodic;
			else ioCtrl->counter = 0;
				
			if(ioCtrl->counter == 0) 
			{
				if(ioCtrl->times) 
				{
					ioCtrl->times--;
					ioCtrl->counter = ioCtrl->offTime + ioCtrl->onTime;
					ioCtrl->status = IO_STATUS_ON;
				}
				else
				{
					ioCtrl->enable = IO_TOGGLE_DISABLE;
					ioCtrl->status = IO_STATUS_OFF;
				}
			}
			
			if(ioCtrl->counter <= ioCtrl->offTime) 
				ioCtrl->status = IO_STATUS_OFF;
	}else{
		ioCtrl->status = IO_STATUS_OFF;
	}
	return ioCtrl->status;
}

