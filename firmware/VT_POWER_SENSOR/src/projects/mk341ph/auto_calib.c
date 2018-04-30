#include "main.h"
#include "comm.h"

#define CALIB_TEST 1

#define TIMEOUT_IN_SEC(x) x*4
uint32 timeout = 0;
static char cmd_buf[64];
static uint8_t cmd_len = 0;
COMPARE_TYPE meterCmd;
extern tSMARTHOME_RESULT result;
CALIB_STATES calib_state = CALIB_NO;             /* calibration status */

uint8_t calib_done = 0;
uint8_t calib_type = AUTO_CALIB_TYPE;
static uint8 in_range(double value, double nominal, double tolerance)
{
  if ((value >= (nominal-tolerance)) && (value <= (nominal+tolerance))) return(TRUE);
  else return (FALSE);     
}

void set_calib_state(CALIB_STATES new_state)
{
	calib_state =  new_state;
}

CALIB_STATES get_calib_state( void)
{
	return calib_state;;
}

void auto_calib_init(void)
{
	InitFindData(&meterCmd,"AT+"); 
}

void start_calib(uint8_t type)
{
	calib_state = CALIB_PREPARE;
	calib_type = type;
	COMM_Puts("METER IS CALIBRATING!\r\n");
}


void auto_calib_task(void)
{
	uint8_t c,index;
	char buf[128];
	double lf;
	float f,f1,f2;
	uint32_t t0,t1,t2,t3,t4,t5;
	int32_t i32Temp1;
	tTIME wTime;
	uint32_t u32_temp;
	if(calib_done == 1 && calib_state == CALIB_NO)
	{
		calib_done = 0;
		CONFIG_SaveFlash((void *)__CONFIG_FLASH_BLOCK__,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA));
		COMM_Puts("METER READY!\r\n");
	}
		
	if(RINGBUF_Get(commRxRingBuf,&c) == 0)
	{
		if(cmd_len < sizeof(cmd_buf))
			cmd_buf[cmd_len++]= c;
		if(FindData(&meterCmd,c) == 0)
		{
			InitFindData(&meterCmd,"AT+");
			cmd_buf[0] = '\0'; 
			strcpy(cmd_buf,"AT+");
			cmd_len = strlen(cmd_buf);
		}
		if((c == '\n' || c == '\r'))
		{
			if(cmd_len > 3 && cmd_buf[0] == 'A' && cmd_buf[1] == 'T'&& cmd_buf[2] == '+')
			{
				if(strstr(cmd_buf,"AT+READALL0=?")){
					
					sprintf(buf,"AT+READALL0=%0.2fV,%0.3fA,%0.2fW,%0.2fVAR,%0.5fPF,%04X\r\n",
					result.urms,result.irms[0],result.p[0],result.q[0],result.p[0]/result.urms/result.irms[0]-0.002,calib_state);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READALL=?")){
					sprintf(buf,"AT+READALL=%0.2fV,%0.4fA,%0.4fA,%0.3fA,%0.2fW,%ldwh,%0.6fPF\r\n",
					result.urms,result.irms[0],result.irms[1],result.irms[0]-result.irms[1],result.p[0],ramcnt.wh_cnt[0],result.p[0]/result.urms/result.irms[0]-0.002);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READALL1=?")){
					sprintf(buf,"AT+READALL1=%0.2fV,%0.3fA,%0.2fW,%0.2fVAR,%0.5fPF,%04X\r\n",
					result.urms,result.irms[1],result.p[1],result.q[1],result.p[1]/result.urms/result.irms[1]-0.002,calib_state);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READALL2=?")){
					sprintf(buf,"AT+READALL2=%0.2fV,%0.3fA,%0.2fW,%0.2fVAR,%0.5fPF,%04X\r\n",
					result.urms,result.irms[2],result.p[2],result.q[2],result.p[2]/result.urms/result.irms[2]-0.002,calib_state);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+FORMAT=CALIB")){
					
					sprintf(buf,"AT+FORMAT=CALIB\r\n");
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+FORMAT=SYSTEM")){
					
					sprintf(buf,"AT+FORMAT=SYSTEM\r\n");
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+FORMAT=CONFIG")){
					
					sprintf(buf,"AT+FORMAT=CONFIG\r\n");
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+FORMAT=ALL")){
					
					sprintf(buf,"AT+FORMAT=ALL\r\n");
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+SAVE=CALIB")){
					CONFIG_SaveFlash((void *)__CONFIG_FLASH_BLOCK__,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA));
					sprintf(buf,"AT+SAVE=CALIB\r\n");
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+SAVE=SYSTEM")){

					sprintf(buf,"AT+SAVE=SYSTEM\r\n");
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+SAVE=CONFIG")){
					CONFIG_SaveFlash((void *)__CONFIG_FLASH_BLOCK__,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA));
					sprintf(buf,"AT+SAVE=CONFIG\r\n");
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+SAVE=ALL")){
					CONFIG_SaveFlash((void *)__CONFIG_FLASH_BLOCK__,(void *)&ramcfg,sizeof(tCONFIG_FLASH_DATA));
					sprintf(buf,"AT+SAVE=ALL\r\n");
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=U")){
					sprintf(buf,"AT+READ=U:%0.2fV\r\n",result.urms);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=I0")){
					sprintf(buf,"AT+READ=I0:%0.3fA\r\n",result.irms[0]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=I1")){
					sprintf(buf,"AT+READ=I1:%0.3fA\r\n",result.irms[1]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=I2")){
					sprintf(buf,"AT+READ=I2:%0.3fA\r\n",result.irms[2]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=I3")){
					sprintf(buf,"AT+READ=I3:%0.3fA\r\n",result.irms[3]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=PF0")){
					lf = (result.p[0]/result.urms/result.irms[0]);
					sprintf(buf,"AT+READ=PF0:%0.5frRAD\r\n",lf);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=PF1")){
					lf = (result.p[1]/result.urms/result.irms[1]);
					sprintf(buf,"AT+READ=PF1:%0.5frRAD\r\n",lf);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=PF2")){
					lf = (result.p[2]/result.urms/result.irms[2]);
					sprintf(buf,"AT+READ=PF2:%0.5frRAD\r\n",lf);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=PF3")){
					lf = (result.p[3]/result.urms/result.irms[3]);
					sprintf(buf,"AT+READ=PF3:%0.5frRAD\r\n",lf);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=P0")){
					sprintf(buf,"AT+READ=P0:%0.2fW\r\n",result.p[0]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=P1")){
					sprintf(buf,"AT+READ=P1:%0.2fW\r\n",result.p[1]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=P2")){
					sprintf(buf,"AT+READ=P2:%0.2fW\r\n",result.p[2]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=P3")){
					sprintf(buf,"AT+READ=P3:%0.2fW\r\n",result.p[3]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=Q0")){
					sprintf(buf,"AT+READ=Q0:%0.2fVar\r\n",result.q[0]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=Q1")){
					sprintf(buf,"AT+READ=Q1:%0.2fVar\r\n",result.q[1]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=Q2")){
					sprintf(buf,"AT+READ=Q2:%0.2fVar\r\n",result.q[2]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+READ=Q3")){
					sprintf(buf,"AT+READ=Q3:%0.2fVar\r\n",result.q[3]);
					COMM_Puts((uint8_t *)buf);
				}else if(strstr(cmd_buf,"AT+USCALE=")){
					sscanf(cmd_buf,"AT+USCALE=%s",buf);
					if(buf[0] == '?')
					{
						sprintf(buf,"AT+USCALE=%0.5f\r\n",F32TODBL(ramcfg.correct.u_gain));
						COMM_Puts((uint8_t *)buf);
					}
					else
					{
						sscanf(cmd_buf,"AT+USCALE=%f",&f); 
						ramcfg.correct.u_gain = (uint32_t)(((double)ramcfg.correct.u_gain) * (1 + (f/100)));
						if(ramcfg.correct.u_gain < 0) ramcfg.correct.u_gain = FRAC32(1.0);
						sprintf(buf,"AT+USCALE=%0.5f\r\n",F32TODBL(ramcfg.correct.u_gain));
						COMM_Puts((uint8_t *)buf);
						sprintf(buf,"AT+READALL=%0.2fV,%0.3fA,%0.2fW,%0.2fVAR,%0.5fPF,%04X\r\n",
							result.urms,result.irms[0],result.p[0],result.q[0],result.p[0]/result.urms/result.irms[0]-0.002,calib_state);
						COMM_Puts((uint8_t *)buf);
					}
				}else if(strstr(cmd_buf,"AT+I0SCALE=")){
					sscanf(cmd_buf,"AT+I0SCALE=%s",buf);
					if(buf[0] == '?')
					{
						sprintf(buf,"AT+I0SCALE=%0.5f\r\n",F32TODBL(ramcfg.correct.i_gain[0]));
						COMM_Puts((uint8_t *)buf);
					}
					else
					{
						sscanf(cmd_buf,"AT+I0SCALE=%f",&f); 
						i32Temp1 = ramcfg.correct.i_gain[0];
						i32Temp1 = (uint32_t)(((double)i32Temp1) * (1 + (f/100)));
						if(i32Temp1 < 0) i32Temp1 = I_NOMINAL;
						ramcfg.correct.i_gain[0] = i32Temp1;
						
						sprintf(buf,"AT+I0SCALE=%0.5f\r\n",F32TODBL(ramcfg.correct.i_gain[0]));
						COMM_Puts((uint8_t *)buf);
						sprintf(buf,"AT+READALL=%0.2fV,%0.3fA,%0.2fW,%0.2fVAR,%0.5fPF,%04X\r\n",
							result.urms,result.irms[0],result.p[0],result.q[0],result.p[0]/result.urms/result.irms[0]-0.002,calib_state);
						COMM_Puts((uint8_t *)buf);
					}
				}else if(strstr(cmd_buf,"AT+I1SCALE=")){
					sscanf(cmd_buf,"AT+I1SCALE=%s",buf);
					if(buf[0] == '?')
					{
						sprintf(buf,"AT+I1SCALE=%0.5f\r\n",F32TODBL(ramcfg.correct.i_gain[1]));
						COMM_Puts((uint8_t *)buf);
					}
					else
					{
						sscanf(cmd_buf,"AT+I1SCALE=%f",&f); 
						i32Temp1 = ramcfg.correct.i_gain[1];
						i32Temp1 = (uint32_t)(((double)i32Temp1) * (1 + (f/100)));
						if(i32Temp1 < 0) i32Temp1 = I_NOMINAL;
						ramcfg.correct.i_gain[1] = i32Temp1;
						
						sprintf(buf,"AT+I1SCALE=%0.5f\r\n",F32TODBL(ramcfg.correct.i_gain[1]));
						COMM_Puts((uint8_t *)buf);
						sprintf(buf,"AT+READALL=%0.2fV,%0.3fA,%0.2fW,%0.2fVAR,%0.5fPF,%04X\r\n",
							result.urms,result.irms[1],result.p[1],result.q[1],result.p[1]/result.urms/result.irms[1]-0.002,calib_state);
						COMM_Puts((uint8_t *)buf);
					}
				}else if(strstr(cmd_buf,"AT+I2SCALE=")){
					sscanf(cmd_buf,"AT+I2SCALE=%s",buf);
					if(buf[0] == '?')
					{
						sprintf(buf,"AT+I2SCALE=%0.5f\r\n",F32TODBL(ramcfg.correct.i_gain[2]));
						COMM_Puts((uint8_t *)buf);
					}
					else
					{
						sscanf(cmd_buf,"AT+I2SCALE=%f",&f); 
						i32Temp1 = ramcfg.correct.i_gain[2];
						i32Temp1 = (uint32_t)(((double)i32Temp1) * (1 + (f/100)));
						if(i32Temp1 < 0) i32Temp1 = I_NOMINAL;
						ramcfg.correct.i_gain[2] = i32Temp1;
						
						sprintf(buf,"AT+I2SCALE=%0.5f\r\n",F32TODBL(ramcfg.correct.i_gain[2]));
						COMM_Puts((uint8_t *)buf);
						sprintf(buf,"AT+READALL=%0.2fV,%0.3fA,%0.2fW,%0.2fVAR,%0.5fPF,%04X\r\n",
							result.urms,result.irms[2],result.p[2],result.q[2],result.p[2]/result.urms/result.irms[2]-0.002,calib_state);
						COMM_Puts((uint8_t *)buf);
					}
				}else if(strstr(cmd_buf,"AT+PF0SCALE=")){
					sscanf(cmd_buf,"AT+PF0SCALE=%s",buf);
					if(buf[0] == '?')
					{
						sprintf(buf,"AT+PF0SCALE=%d\r\n",ramcfg.delay[0]);
						COMM_Puts((uint8_t *)buf);
					}
					else
					{
						sscanf(cmd_buf,"AT+PF0SCALE=%f",&f); 
						//correction phase
						//meterCfg.phase_correction += METERLIB_RAD2SH(f,60.0);
						ramcfg.delay[0] += f;
						AFE_DlyUpdate(-f,0,f,0);
						sprintf(buf,"AT+PF0SCALE=%d\r\n",ramcfg.delay[0]);
						COMM_Puts((uint8_t *)buf);
						sprintf(buf,"AT+READALL=%0.2fV,%0.3fA,%0.2fW,%0.2fVAR,%0.5fPF,%04X\r\n",
							result.urms,result.irms[0],result.p[0],result.q[0],result.p[0]/result.urms/result.irms[0]-0.002,calib_state);
						COMM_Puts((uint8_t *)buf);
					}
				}else if(strstr(cmd_buf,"AT+PF1SCALE=")){
					sscanf(cmd_buf,"AT+PF1SCALE=%s",buf);
					if(buf[0] == '?')
					{
						sprintf(buf,"AT+PF1SCALE=%d\r\n",ramcfg.delay[1]);
						COMM_Puts((uint8_t *)buf);
					}
					else
					{
						sscanf(cmd_buf,"AT+PF1SCALE=%f",&f); 
						//correction phase
						//meterCfg.phase_correction += METERLIB_RAD2SH(f,60.0);
						ramcfg.delay[1] += f;
						AFE_DlyUpdate(-f,0,f,0);
						sprintf(buf,"AT+PF1SCALE=%d\r\n",ramcfg.delay[1]);
						COMM_Puts((uint8_t *)buf);
						sprintf(buf,"AT+READALL=%0.2fV,%0.3fA,%0.2fW,%0.2fVAR,%0.5fPF,%04X\r\n",
							result.urms,result.irms[1],result.p[1],result.q[1],result.p[1]/result.urms/result.irms[1]-0.002,calib_state);
						COMM_Puts((uint8_t *)buf);
					}
				}else if(strstr(cmd_buf,"AT+RTC=")){
					sscanf(cmd_buf,"AT+RTC=%s",buf);
					if(buf[0] == '?')
					{
						sprintf(buf,"AT+RTC=%04d/%02d/%02d %02d:%02d:%02d\r\n",rTime.year,rTime.mon,rTime.day,rTime.hour,rTime.min,rTime.sec);
						COMM_Puts((uint8_t *)buf);
					}
					else
					{
						sscanf(cmd_buf,"AT+RTC=%04d/%02d/%02d %02d:%02d:%02d\r\n",&t0,&t1,&t2,&t3,&t4,&t5); 
						if(t0 < 2099 && t0 >= 2017 && t1 <= 12 && t2 <= 31 && t3 < 24 && t4 < 60 && t5 < 60)
						{
							wTime.year = t0;
							wTime.mon = t1;
							wTime.day = t2;
							wTime.hour = t3;
							wTime.min = t4;
							wTime.sec = t5;
							IRTC_SetClock   (&wTime);
						}
						sprintf(buf,"AT+RTC=%04d/%02d/%02d %02d:%02d:%02d\r\n",wTime.year,wTime.mon,wTime.day,wTime.hour,wTime.min,wTime.sec);
						COMM_Puts((uint8_t *)buf);
					}
				}else if(strstr(cmd_buf,"AT+UCAL=")){
					sscanf(cmd_buf,"AT+UCAL=%s",buf);
					if(buf[0] == '?')
					{
						sprintf(buf,"AT+UCAL=%0.2f\r\n",(double)ramcfg.urms_cal);
						COMM_Puts((uint8_t *)buf);
					}
					else
					{
						sscanf(cmd_buf,"AT+UCAL=%f",&f); 
						ramcfg.urms_cal = f;
						sprintf(buf,"AT+UCAL=%0.2f\r\n",(double)ramcfg.urms_cal);
						COMM_Puts((uint8_t *)buf);
					}
				}else if(strstr(cmd_buf,"AT+ICAL=")){
					sscanf(cmd_buf,"AT+ICAL=%s",buf);
					if(buf[0] == '?')
					{
						sprintf(buf,"AT+ICAL=%0.3f\r\n",(double)ramcfg.irms_cal[0]);
						COMM_Puts((uint8_t *)buf);
					}
					else
					{
						sscanf(cmd_buf,"AT+ICAL=%f",&f); 
						ramcfg.irms_cal[0] = f;
						sprintf(buf,"AT+ICAL=%0.3f\r\n",(double)ramcfg.irms_cal[0]);
						COMM_Puts((uint8_t *)buf);
					}
				}else if(strstr(cmd_buf,"AT+AUTOCALIB=ON")){
					start_calib(AUTO_CALIB_TYPE);
				}else if(strstr(cmd_buf,"AT+CALIB=ON")){
					ramcfg.u_msrmax =  FRAC32(-1.0);
					ramcfg.u_msrmin =  FRAC32(1.0);
					ramcfg.i_msrmax[0] =  FRAC32(-1.0);
					ramcfg.i_msrmin[0] =  FRAC32(1.0);
					ramcfg.i_msrmax[1] =  FRAC32(-1.0);
					ramcfg.i_msrmin[1] =  FRAC32(1.0);
					ramcfg.i_msrmax[2] =  FRAC32(-1.0);
					ramcfg.i_msrmin[2] =  FRAC32(1.0);
					ramcfg.i_msrmax[3] =  FRAC32(-1.0);
					ramcfg.i_msrmin[3] =  FRAC32(1.0);
					ramcfg.flag = 0xfff5;
					start_calib(MANUAL_CALIB_TYPE);
					
				}else if(strstr(cmd_buf,"AT+REBOOT=1")){
					SystemReset();
				}
				else
				{
					sprintf(buf,"\r\nERROR\r\n");
					COMM_Puts((uint8_t *)buf);
					return;
				}
				sprintf(buf,"\r\nOK\r\n");
				COMM_Puts((uint8_t *)buf);
			}
			else
			{
				cmd_len = 0;
			}
		}
	}
}


void auto_calib(tCONFIG_FLASH_DATA *ptr,tSMARTHOME_RESULT val)
{
	int16 dly;
	static double pf,f,cal_sum = 0;
	static uint8_t sum_cnt = 0;
	switch(calib_state)
	{
		case CALIB_NO:

		break;
		case CALIB_PREPARE:
			ptr->flag = 0xffff;
			ptr->u_msrmax =  FRAC32(-1.0);
			ptr->u_msrmin =  FRAC32(1.0);
			ptr->i_msrmax[0] =  FRAC32(-1.0);
			ptr->i_msrmin[0] =  FRAC32(1.0);
			ptr->i_msrmax[1] =  FRAC32(-1.0);
			ptr->i_msrmin[1] =  FRAC32(1.0);
			ptr->i_msrmax[2] =  FRAC32(-1.0);
			ptr->i_msrmin[2] =  FRAC32(1.0);
			ptr->i_msrmax[3] =  FRAC32(-1.0);
			ptr->i_msrmin[3] =  FRAC32(1.0);
			ptr->flag = 0xfff5;
			calib_state = CALIB_OFFS_U;
			timeout = OFFS_AVRG;
		break;
		case 	CALIB_OFFS_U:
			if (--timeout == 0) 
			{
				ptr->u_offset = (ptr->u_msrmax + ptr->u_msrmin)>>1;
				ptr->i_offset[0] = (ptr->i_msrmax[0] + ptr->i_msrmin[0])>>1;
				ptr->i_offset[1] = (ptr->i_msrmax[1] + ptr->i_msrmin[1])>>1;
				ptr->i_offset[2] = (ptr->i_msrmax[1] + ptr->i_msrmin[2])>>1;
				ptr->i_offset[3] = (ptr->i_msrmax[3] + ptr->i_msrmin[3])>>1;
				calib_state =  CALIB_U;
				timeout = TIMEOUT_CALIB_U;//1 SEC
				sum_cnt = 0;
				ptr->flag = 0xffa4;
				if(calib_type == MANUAL_CALIB_TYPE)
				{
					calib_done = 1;
					calib_state = CALIB_NO;
				}
			}
		break;
		case 	CALIB_U:
			#if CALIB_TEST
			calib_done = 1;
			#endif
			if(!calib_done)
			{
			 cal_sum += val.urms;
			 sum_cnt++;
			 if(sum_cnt >= 4)
			 {
				 sum_cnt = 0;
				 val.urms = cal_sum/4;
				 cal_sum = 0;
			 }
			 if((sum_cnt == 0) 
				 && (timeout < (TIMEOUT_CALIB_U - 4)) 
				 && in_range(val.urms,ptr->urms_cal + 0.05,U_END_DIFF)) 
			 {
				 calib_state = CALIB_I0; 
				 timeout = TIMEOUT_CALIB_I;
				 cal_sum = 0;
				 sum_cnt = 0;
			 }
			 else if(--timeout == 0)
			 {
				 cal_sum = 0;
				 sum_cnt = 0;
				 timeout = TIMEOUT_CALIB_U;
				 if(val.urms > 0)
				 {
					ptr->correct.u_gain = ( (ptr->urms_cal + 0.05) * ptr->correct.u_gain) / val.urms;
				 }
				 if(ptr->correct.u_gain < 0) ptr->correct.u_gain = -ptr->correct.u_gain;
			 }
		  }
		break;
		case 	CALIB_I0:
			#if CALIB_TEST
			calib_state = CALIB_PH_I0;
			#endif
			cal_sum += val.irms[0];
			sum_cnt++;
			if(sum_cnt >= 4)
			{
			 sum_cnt = 0;
			 val.irms[0] = cal_sum/4;
			 cal_sum = 0;
			}
			if ((sum_cnt == 0) 
				&& (timeout < (TIMEOUT_CALIB_I - 4))
				&& in_range(val.irms[0],ptr->irms_cal[0],I_END_DIFF)) 
			{
				calib_state = CALIB_PH_I0;
				timeout = TIMEOUT_CALIB_PF;
				cal_sum = 0;
				sum_cnt = 0;
			}
			else if(--timeout == 0)
			{
				cal_sum = 0;
				sum_cnt = 0;
				timeout = TIMEOUT_CALIB_I;
				if(val.irms[0] > 0)
				{
					ptr->correct.i_gain[0] = (double)(ptr->correct.i_gain[0])*ptr->irms_cal[0]/val.irms[0]; 
				}
				if(ptr->correct.i_gain[0] < 0)
					ptr->correct.i_gain[0] = -ptr->correct.i_gain[0];
				else if(ptr->correct.i_gain[0] == 0) 
					ptr->correct.i_gain[0] = FRAC32(1.0);
			}
		break;
		case 	CALIB_PH_I0:
			#if CALIB_TEST
			calib_done = 1;
			#endif
			if(!calib_done)
			{
				pf = (val.p[0]/val.urms/val.irms[0])-0.002;
				cal_sum += pf;
				if (in_range(cal_sum/(TIMEOUT_CALIB_PF-TIME_CALIB_SUM_STABLE-timeout+1),PF_NOMINAL,PF_END_DIFF) && (timeout < (TIMEOUT_CALIB_PF - TIME_CALIB_STABLE))) 
				{
					calib_done = 1;
					ptr->flag = 0xa5a5;
					calib_state = CALIB_NO;
//					f = -0.0025;
//					f = f*6000.00*1024.00/(2.0*3.14159265358979323846*50.00);
//					meterCfg.delay += f;
//					AFE_DlyUpdate(-f,0,f,0);
					//meterCfg.phase_correction += METERLIB_RAD2SH(-0.003,60.0);
					timeout = TIMEOUT_CALIB_PF;    /* set the number of right PF values again */ 
				}
				else if(--timeout == 0)
				{
					cal_sum = 0;
					timeout = TIMEOUT_CALIB_PF;
					f = acos(PF_NOMINAL) - acos(pf); 
					f = f*6000.00*1024.00/(2.0*3.14159265358979323846*50.00);
					ptr->delay[0] += f;
					AFE_DlyUpdate(-f,0,f,0);
					//meterCfg.phase_correction += METERLIB_RAD2SH(f,60.0);
				}
				if((timeout == (TIMEOUT_CALIB_PF - TIME_CALIB_SUM_STABLE)))
					cal_sum = 0;
			}
		break;
		default:
			break;
	}
}



/***************************************************************************//*!
 * @brief   Updates offset of the phase voltage and current measurements 
 *          conditionally.
 * @param   ptr   - pointer to tCONFIG_FLASH_DATA
 * @param   u     - phase voltage sample
 * @param   i     - phase current sample
 * @note    Implemented as a function call.
 ******************************************************************************/
void CONFIG_UpdateOffsets (tCONFIG_FLASH_DATA *ptr, Frac32 u, Frac32* chan, int num)
{
	if (ptr->flag == 0xfff5) /* update offsets if pre-processing active         */
	{
		if (ptr->u_msrmax < u) { ptr->u_msrmax = u; } /* find voltage max. value  */
		if (ptr->u_msrmin > u) { ptr->u_msrmin = u; } /* find voltage min. value  */

		if (ptr->i_msrmax[0] < chan[0]) { ptr->i_msrmax[0] = chan[0]; } /* find current max. value  */
		if (ptr->i_msrmin[0] > chan[0]) { ptr->i_msrmin[0] = chan[0]; } /* find current min. value  */

		if (ptr->i_msrmax[1] < chan[1]) { ptr->i_msrmax[1] = chan[1]; } /* find current max. value  */
		if (ptr->i_msrmin[1] > chan[1]) { ptr->i_msrmin[1] = chan[1]; } /* find current min. value  */

		if (ptr->i_msrmax[2] < chan[2]) { ptr->i_msrmax[2] = chan[2]; } /* find current max. value  */
		if (ptr->i_msrmin[2] > chan[2]) { ptr->i_msrmin[2] = chan[2]; } /* find current min. value  */

		if (ptr->i_msrmax[3] < chan[3]) { ptr->i_msrmax[3] = chan[3]; } /* find current max. value  */
		if (ptr->i_msrmin[3] > chan[3]) { ptr->i_msrmin[3] = chan[3]; } /* find current min. value  */
	}
}


