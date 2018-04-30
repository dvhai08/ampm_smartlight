/**
 *
 *
 *
 */
#ifdef _ENABLE_THREAD_SUPPORT_
#include "threads_platform.h"

IoT_Error_t aws_iot_thread_mutex_init(IoT_Mutex_t *pMutex)
{
	pMutex->lock = xSemaphoreCreateMutex();
	if(pMutex->lock == NULL)
	{
		return MUTEX_INIT_ERROR;
	}
	return AWS_SUCCESS;
}
IoT_Error_t aws_iot_thread_mutex_lock(IoT_Mutex_t *pMutex)
{
	xSemaphoreTake(pMutex->lock, ( TickType_t ) portMAX_DELAY);
	return AWS_SUCCESS;
}
IoT_Error_t aws_iot_thread_mutex_unlock(IoT_Mutex_t *pMutex)
{
	xSemaphoreGive(pMutex->lock);
	return AWS_SUCCESS;
}
IoT_Error_t aws_iot_thread_mutex_destroy(IoT_Mutex_t *pMutex)
{
	vSemaphoreDelete(pMutex->lock);
	return AWS_SUCCESS;
}
IoT_Error_t aws_iot_thread_mutex_trylock(IoT_Mutex_t *pMutex)
{
	return AWS_SUCCESS;
}
#endif
