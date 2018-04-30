
#ifndef __POWER_SENSOR_H__
#define __POWER_SENSOR_H__
#include <stdint.h>
typedef struct{
	float Ir;
	float P;
	float Q;
	float U;
	float IL;
	float IN;
	float PF;
	long wh;
}POWER_SENSOR_OBJ_TYPE;	
void PowerSensor_Task(void const *param);
void PowerSensor_Init(void);
void PowerSensorPause(uint8_t sec);
extern POWER_SENSOR_OBJ_TYPE powerLine1;
extern POWER_SENSOR_OBJ_TYPE powerLine2;
extern POWER_SENSOR_OBJ_TYPE powerLine3;
#endif

