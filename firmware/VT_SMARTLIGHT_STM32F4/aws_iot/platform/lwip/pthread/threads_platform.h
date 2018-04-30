/**
 *
 *
 */
#ifndef THREADS_PLATFORM_H
#define THREADS_PLATFORM_H
#ifdef _ENABLE_THREAD_SUPPORT_
#include "threads_interface.h"
#include "FreeRTOS.h"
#include "semphr.h"
/**
 * @brief Mutex Type
 *
 * definition of the Mutex	 struct. Platform specific
 *
 */
struct _IoT_Mutex_t {
	xSemaphoreHandle lock;
};
#endif
#endif
