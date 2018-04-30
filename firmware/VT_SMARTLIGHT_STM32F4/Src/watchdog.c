


#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
/* USER CODE BEGIN Includes */
#include "system_config.h"
#include "hw_config.h"
#include "watchdog.h"
#include "uart5.h"
uint32_t watchdogSecCnt = 0;
uint32_t watchdogEnable[WATCHDOG_NUM] = {0};
volatile uint32_t watchdogCnt[WATCHDOG_NUM] = {0};
volatile uint32_t watchdogFeed[WATCHDOG_NUM] = {0};
const uint32_t watchdogCntValueMax[WATCHDOG_NUM] = {MAIN_LOOP_TIME_MAX,SHADOW_LOOP_TIME_MAX,GSM_LOOP_TIME_MAX};
osSemaphoreId watchdog_sem_id;
osSemaphoreDef(WATCHDOG_SEM);
osMessageQId watchdogQueue;
osThreadId watchdog_thread_id;
static void vWatchdog_Task(void const *param);
#define WATCHDOG_QUEUE_SIZE        (uint32_t) WATCHDOG_TASK_NUM

volatile uint32_t osWatchdogCnt[WATCHDOG_TASK_NUM] = {0};
volatile uint32_t osWatchdogFeed[WATCHDOG_TASK_NUM] = {0};
const uint32_t osWatchdogCntValueMax[WATCHDOG_TASK_NUM];
uint32_t osWatchdogSecCnt = 0;
uint32_t osWatchdogEnable[WATCHDOG_TASK_NUM] = {MAIN_LOOP_TIME_MAX,SHADOW_LOOP_TIME_MAX};

void SysWatchdogInit(void)
{
	uint8_t i;
	watchdog_sem_id = osSemaphoreCreate(osSemaphore(WATCHDOG_SEM), 1);
	
//  osMessageQDef(watchdog_queue, WATCHDOG_QUEUE_SIZE, uint16_t);
//  watchdogQueue = osMessageCreate (osMessageQ(watchdog_queue), NULL);
	
//	osThreadDef(watchdog_task, vWatchdog_Task, osPriorityNormal, 0, 512);
//	watchdog_thread_id = osThreadCreate(osThread(watchdog_task), NULL);
//	
	for(i = 0; i < WATCHDOG_NUM;i++)
	{
		watchdogEnable[i] = 1;
	}
}


void OsWatchdogFeed(uint32_t task_id)
{
	if(osMessagePut (watchdogQueue, task_id, 1000) != osOK)
	{
		printf("osWatchdogFeed from task_id:%d):ERROR\r\n",task_id);
	}
	else
	{
		printf("osWatchdogFeed from task_id:%d):OK\r\n",task_id);
	}
}

void OsWatchdogEnable(uint32_t task_id,uint32_t timeout)
{
	if(osSemaphoreWait(watchdog_sem_id, 100) == osOK)
	{
		task_id %= WATCHDOG_TASK_NUM;
		osWatchdogEnable[task_id] = 1;
		osSemaphoreRelease(watchdog_sem_id);
	}
}

void OsWatchdogDisable(uint32_t task_id)
{
	if(osSemaphoreWait(watchdog_sem_id, 100) == osOK)
	{
		task_id %= WATCHDOG_TASK_NUM;
		osWatchdogEnable[task_id] = 0;
		osSemaphoreRelease(watchdog_sem_id);
	}
}

static void vWatchdog_Task(void const *param)
{
	osEvent event;
	for(;;)
  {
	/* Get the message from the queue */
    event = osMessageGet(watchdogQueue, 1000);
		if(event.status == osEventMessage)
    {
			
		}
		
	}
}

	
void SysWatchdogEnable(uint32_t id)
{
	if(osSemaphoreWait(watchdog_sem_id, 100) == osOK)
	{
		id %= WATCHDOG_NUM;
		watchdogEnable[id] = 1;
		osSemaphoreRelease(watchdog_sem_id);
	}
}

void SysWatchdogDisable(uint32_t id)
{
	if(osSemaphoreWait(watchdog_sem_id, 100) == osOK)
	{
		id %= WATCHDOG_NUM;
		watchdogEnable[id] = 1;
		osSemaphoreRelease(watchdog_sem_id);
	}
}

void SysWatchdogFeed(uint32_t id)
{
	if(osSemaphoreWait(watchdog_sem_id, 100) == osOK)
	{
		id %= WATCHDOG_NUM;
		watchdogFeed[id] = 0;
		osSemaphoreRelease(watchdog_sem_id);
	}
}

void SysWatchdog(void)
{
	uint32_t temp;
	watchdogSecCnt++;
	temp = watchdogSecCnt % WATCHDOG_NUM;
	if(watchdogEnable[temp])
	{
		//check watchdog value for funtion
		watchdogCnt[temp] += WATCHDOG_NUM;
		if(watchdogCnt[temp] >= watchdogCntValueMax[temp]) 
		{
//			char buf[64];
//			sprintf(buf,"Watchdog Reset:%d\r\n",temp);
//			mUART5_PutString(buf);
//			while(1)
//			{
//				IWDG->KR = ((uint16_t)0xAAAA);
//			}
//			temp = 1000000;
//			while(temp--);
			//NVIC_SystemReset();
		}
		if(watchdogFeed[temp] == 0)
		{
			watchdogCnt[temp] = 0;
			watchdogFeed[temp] = 1;
		}
	}
	else	
	{
		watchdogCnt[temp] = 0;
	}
}


