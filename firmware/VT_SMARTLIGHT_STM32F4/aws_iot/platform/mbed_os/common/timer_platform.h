#ifndef SRC_PROTOCOL_MQTT_AWS_IOT_EMBEDDED_CLIENT_WRAPPER_PLATFORM_LINUX_COMMON_TIMER_PLATFORM_H_
#define SRC_PROTOCOL_MQTT_AWS_IOT_EMBEDDED_CLIENT_WRAPPER_PLATFORM_LINUX_COMMON_TIMER_PLATFORM_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "Timer.h"
//using namespace mbed;
/**
 * Extension of the Platform specific Timer class
 */
class TimerExt: public mbed::Timer {
public:
	TimerExt();
	int timeout_ms;
};

//#ifdef __cplusplus
//}
//#endif

#endif /* SRC_PROTOCOL_MQTT_AWS_IOT_EMBEDDED_CLIENT_WRAPPER_PLATFORM_LINUX_COMMON_TIMER_PLATFORM_H_ */

