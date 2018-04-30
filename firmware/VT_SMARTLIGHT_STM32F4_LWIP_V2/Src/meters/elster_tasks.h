

#ifndef __ELSTER_TASKS_H__
#define __ELSTER_TASKS_H__
#include <stdint.h>

#define SAVE_TO_BLE 2
#define SAVE_TO_FLASH 1

typedef enum{
	METER_READ_0 = 0,
	METER_READ_1,
	METER_READ_2,
	METER_READ_ALL
}METER_ELSTER_READ_TYPE;


typedef struct{
	uint64_t Import_Wh;
	uint64_t Export_Wh;
	uint64_t Q1_varh;
	uint64_t Q2_varh;
	uint64_t Q3_varh;
	uint64_t Q4_varh;
	uint64_t VAh_vah;
	uint64_t CD1_varh;
	uint64_t CD2_varh;
	uint64_t CD3_varh;
	uint64_t Rates1;
	uint64_t Rates2;
	uint64_t Rates3;
	uint64_t Rates4;
	uint64_t Rates5;
	uint64_t Rates6;
	uint64_t Rates7;
	uint64_t Rates8;
	
}METER_CURRENT_VALUE_TYPE;

typedef struct{
	float voltage_phase_A;
	float voltage_phase_B;
	float voltage_phase_C;
	float current_phase_A;
	float current_phase_B;
	float current_phase_C;
	float angle_phase_A;
	float angle_phase_B;
	float angle_phase_C;
	float factor_phase_A;
	float factor_phase_B;
	float factor_phase_C;
	float frequency_phase_A;
	float frequency_phase_B;
	float frequency_phase_C;
}METER_INSTANTANEOUS_VALUE_TYPE;
extern METER_CURRENT_VALUE_TYPE currentValue;
extern METER_INSTANTANEOUS_VALUE_TYPE instantaneousValue;
extern uint8_t meterElsterReadOk;
extern uint8_t readFromFlash;
extern uint8_t dataSaveTo;
uint32_t MeterElterTasks_IsIdle(void);
void MeterElterRead(METER_ELSTER_READ_TYPE cmd);
void MeterElterTasks(void);
void MeterElterTasksInit(void);
void MeterElsterTasksClose(void);
void MeterElterTasksStart(void);
#endif


