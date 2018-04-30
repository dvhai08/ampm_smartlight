
#ifndef __POWER_SENSOR_H__
#define __POWER_SENSOR_H__

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

void PowerSensor_Init(void);

extern POWER_SENSOR_OBJ_TYPE powerLine1;
#endif

