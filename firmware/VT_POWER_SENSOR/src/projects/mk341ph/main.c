#include "main.h"


#define PULSE_OUT_TIME               24


uint32_t romCnt[NUM_CHAN];
uint32_t lastROMCnt[NUM_CHAN];
uint16_t energyDaily0,energyDaily1,energyDaily2;

Frac32 daily_flash_data[DAILY_BUFFER] = {123};

/* Public variable */
static Frac24 sample[NUM_CHAN];
volatile int32_t u_sample=0;
double umax, imax;
tSMARTHOME_DATA mlib=SMARTHOME_CFG;
tSMARTHOME_RESULT result;

uint8_t relay_state = 0;
uint8_t newDayFlag = 0;
uint32_t dayFail = 0;
energy_t energy={10000,20000,30000,400000};
uint8_t tempBuff[16];
uint8_t calibChannel = 0;
uint8_t calibFlag = 0;
uint32_t sysTickCnt = 0;
const uint8_t month[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
uint8_t flag_update_status_to_host = 0;
uint8_t update_status_timeout = 0;
// Daily channel cnt

volatile int zero_cnt=20;
volatile uint8_t eeprom_enable =0;
#define ENABLE_MASK 0x80
uint8_t device_id[64];

void pulseOut(void);
/** @brief Init base peripheral */ 
void SysInit(void)
{
	/* reset pin filtered in all modes and filter clocked by LPO                */
  RCM_Init (RCM_MODULE_FILT_CLOCKED_BY_LPO_EN_IN_ALL_MODES_CONFIG);
  SIM_Init (BOARD_PERIPH_ON_CONFIG);
  /* enable PMC buffer operation also in VLPx modes                           */      
  PMC_Init (PMC_MODULE_LVDRE_ON_LVDINT_OFF_LVWINT_OFF_CONFIG(PMC_LVDL,PMC_LVW1),
            PMC_INTREG_BGEN_ON_BGBE_LDON_CONFIG, PRI_LVL0, (PMC_CALLBACK)NULL);
  /* route OSC32KCLK to LCD's default clock                                    */
  SIM_SelOsc32kClkSrc (SIM_OSC32K_SRC1);
}

/** @brief main function */
void main (void)
{
	
	SysInit();
	
	/* check validity of non-initialized variables in system ram and if data    */
	/* are not valid use default data stored in flash                           */
	if (ramcnt.flag != 0x5555){
		memcpy ((void*)&ramcnt, (void*)&nvmcnt, sizeof(tCONFIG_NOINIT_DATA));
		AppRTC_Init();
	}
	//CONFIG_SaveFlash((void*)__CONFIG_DAILY_FLASH_BLOCK__,(void*)daily_flash_data, DAILY_BUFFER);
	SystemConfigInit();			
	//while(1);
	AppRTC_Get(&rTime);
	
	hw_norm_init();
	COMM_Puts((uint8 *)"ampm smart home!\r");
	while (1)
	{
		auto_calib_task();
	}
}


void ReStartAFE(void)
{
	//restart AFE
		DisableInterrupts();
		VREF_VREFH_SC = 0; //disable VREF module
		AFE_Disable();
				
				/* VREF module must be initialized after SIM module */
		VREF_Init(VREF_MODULE_CHOP_ON_1V75_0V4_HPWR_CONFIG,
				VREF_SWITCH_S1_H_S2_L_S3_L);
		VREFH_Trim(46);/* VREFH trimming value - trimmed to 1.200 V on 2.0 */
		VREFL_Trim(3); /* VREFL trimming value - trimmed to 0.400 V on 2.0 */
										 
		/* RESET pin set as GPIO and driven low to improve EMC susceptibility     */

		/* AFE module */
		SIM_SelAfePllClkSrc(SIM_MCG_PLL_CLK);
					 
		AFE_ChanInit(CH0, AFE_CH_SWTRG_CCM_PGAON_CONFIG(DEC_OSR1024,PGA_GAIN),
		       -ramcfg.delay[0], PRI_LVL0, (AFE_CH_CALLBACK)NULL);
		AFE_ChanInit(CH1, AFE_CH_SWTRG_CCM_PGAON_CONFIG(DEC_OSR1024,PGA_GAIN),
						 -ramcfg.delay[0], PRI_LVL0, (AFE_CH_CALLBACK)NULL);
		AFE_ChanInit(CH2, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR1024),
				ramcfg.delay[0], PRI_LVL0, (AFE_CH_CALLBACK)NULL);
		AFE_ChanInit(CH3, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR1024),
				-ramcfg.delay[0], PRI_LVL0, (AFE_CH_CALLBACK)afeCallback);
					 
		AFE_Init(AFE_MODULE_RJFORMAT_CONFIG(AFE_PLL_CLK,AFE_DIV2,12.288e6));
		
		AFE_SwTrigger(CH0|CH1|CH2|CH3);
		EnableInterrupts();
}

void hw_norm_init(void)
{
	uint8_t init=0;
	uint8_t i=0;
	/* SIM initialization */
	SIM_Init (BOARD_PERIPH_ON_CONFIG);   /* full configuration */
	/* Init Clock */
	core_clk_mhz = 24;
	PLL_Init (PLL_MODULE_PEE_12_288MHZ_CONFIG); 
	FLL_Init (FLL_MODULE_FEE_24MHZ_CONFIG);
	//MCG_C4 = (MCG_C4 & (MCG_C4_FCTRIM_MASK|MCG_C4_SCFTRIM_MASK)) | SET(MCG_C4_DRST_DRS(0x01));
	/* VREF module must be initialized after SIM module */
	VREF_Init(VREF_MODULE_CHOP_ON_1V75_0V4_HPWR_CONFIG,
		VREF_SWITCH_S1_H_S2_L_S3_L);
	VREFH_Trim(46);/* VREFH trimming value - trimmed to 1.200 V on 2.0 */
	VREFL_Trim(3); /* VREFL trimming value - trimmed to 0.400 V on 2.0 */
	/* I2C init */
	PORT_Init (PORTE, PORT_MODULE_ALT4_MODE, PIN2|PIN3);
	/* I2C module initialize */
	I2C_Init (I2C1, I2C_MODULE_100K_MASTER_IRQ_DI_CONFIG);
	
	EepromLoadEnerry(&energy);
	ramcnt.wh_cnt[0] = energy.e0;
	ramcnt.wh_cnt[1] = energy.e1;
	ramcnt.wh_cnt[2] = energy.e2;
	ramcnt.wh_cnt[3] = energy.e3;
	for (i=0;i<NUM_CHAN;i++)
	{
		lastROMCnt[i] = ramcnt.wh_cnt[i]/PHASE_CONVERT_PULSE_TO_ENERGY;
	}
  
	iicEEPROM_read(0x0F,&init,1);
	if (init != 0xA5)
	{	 
		// Init rom
		init = 0xA5;
		iicEEPROM_write(0x0F,&init,1);
		DelayUs(50000);
		EepromInit();
	}
	else
	{
		DelayUs(50000);
		// TODO: Restore daily cnt
		CONFIG_ReadFlash((void*)__CONFIG_DAILY_FLASH_BLOCK__,(void*)daily_flash_data, DAILY_BUFFER*4);

		if (daily_flash_data[DAILY_BUFFER-1]!=0xA5)
		{
			reset_daily();
			daily_flash_data[DAILY_BUFFER-1]=0xA5;
			CONFIG_SaveFlash((void*)__CONFIG_DAILY_FLASH_BLOCK__,(void*)daily_flash_data, DAILY_BUFFER*4);
		}
	}

	/* AFE module */
	SIM_SelAfePllClkSrc(SIM_MCG_PLL_CLK);

	AFE_ChanInit(CH0, AFE_CH_SWTRG_CCM_PGAON_CONFIG(DEC_OSR1024,PGA_GAIN),
		       ramcfg.delay[0], PRI_LVL0, (AFE_CH_CALLBACK)NULL);
	AFE_ChanInit(CH1, AFE_CH_SWTRG_CCM_PGAON_CONFIG(DEC_OSR1024,PGA_GAIN),
		       ramcfg.delay[0], PRI_LVL0, (AFE_CH_CALLBACK)NULL);
	AFE_ChanInit(CH2, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR1024),
				-ramcfg.delay[0], PRI_LVL0, (AFE_CH_CALLBACK)NULL);
	AFE_ChanInit(CH3, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR1024),
			ramcfg.delay[0], PRI_LVL0, (AFE_CH_CALLBACK)afeCallback);

	AFE_Init(AFE_MODULE_RJFORMAT_CONFIG(AFE_PLL_CLK,AFE_DIV2,12.288e6));
	//while(1);
	
	SWISR_HandlerInit(0, PRI_LVL1, calcCallback);
	SWISR_HandlerInit(1, PRI_LVL2, dataSaveCallback);
	SWISR_HandlerInit(2, PRI_LVL2, displayCallback);
	
	// RS485_DIR
	PORT_Init (PORTH, PORT_MODULE_LED_MODE, PIN7);
	GPIO_Init(GPIOH,GPIO_OUT_LOGIC0_MODE,PIN7);
	
  
	LPTMR_Disable();
	LPTMR_Init(LPTMR_MODULE_TIMER_CNT_RST_ONTCF_MODE_CONFIG(0,LPTMR_LPOCLK),500);
	LPTMR_InstallCallback (PRI_LVL1, lptmr_callback);
	LPTMR_Enable(); /* enable LPTMR (1000ms interrupt) */

	/* Init UART port */
	PORT_Init (PORTI, PORT_MODULE_ALT2_PULLUP_ON_MODE, PIN0 | PIN1);
	//UART_Init (UART1, UART_MODULE_INTRMODE_CONFIG(9600,12.288e6));
	UART_Init (UART1, UART_MODULE_INTRMODE_CONFIG(9600,24e6));

	//UART_PutStr(UART1,"\n\rSystem Starting...\r\n");
	UART_InstallCallback (UART0_UART1, PRI_LVL0, (UART_CALLBACK)uart_callback);
	COMM_Init();
	zero_cnt = 50;
	EnableInterrupts();
	AFE_SwTrigger(CH0|CH1|CH2|CH3);
}


void afeCallback(AFE_CH_CALLBACK_TYPE type, int32 result)
{
	if (type==COC_CALLBACK)
	{
		sample[0] = -AFE_ChanRead(CH0);
		sample[1] = -AFE_ChanRead(CH1);
		sample[2] = -AFE_ChanRead(CH2);
		sample[3] = -AFE_ChanRead(CH3);
		u_sample = sample[2]<<U_SCALE;
	//	sample[2] = sample[0];
		CONFIG_UpdateOffsets((tCONFIG_FLASH_DATA *)&ramcfg ,u_sample
                         ,sample, NUM_CHAN);

		SWISR_HandlerFreqCall(0,6000,1200);
  }
}

const Frac64 outResPuls[NUM_CHAN] = {KWH_PULS_NUM0(1600),KWH_PULS_NUM1(1600),KWH_PULS_NUM2(1600),KWH_PULS_NUM3(1600)};

void calcCallback(void)
{
	int i;
	
	for (i=0;i<NUM_CHAN;i++)
	{
		sample[i] = L_mul((sample[i] - ramcfg.i_offset[i]),ramcfg.correct.i_gain[i]);
	}
  SMARTHOME_RemoveDcBias((tSMARTHOME_DATA*)&mlib
                         ,L_mul(u_sample-ramcfg.u_offset,ramcfg.correct.u_gain)
                         ,sample);
  SMARTHOME_CalcWattHours((tSMARTHOME_DATA*)&mlib
                         ,(long*)&ramcnt.wh_cnt
                         ,(Frac64 *)outResPuls);
//	SMARTHOME_CalcVarHours((tSMARTHOME_DATA*)&mlib
//                         ,(long*)&ramcnt.varh_cnt
//                         ,(Frac64 *)outResPuls);
  SMARTHOME_CalcAuxiliary((tSMARTHOME_DATA*)&mlib);
												 
	if(calibFlag)
		pulseOut();											 
												 
  SWISR_HandlerFreqCall(1,1200,100);
}



uint8_t tempSw;
void dataSaveCallback(void)
{
	int i;
  // Periodic save Cnt
	if (eeprom_enable&ENABLE_MASK)
	{
		for (i=0;i<NUM_CHAN;i++)
		{
			romCnt[i] = ramcnt.wh_cnt[i] / 100;
			if (lastROMCnt[i] != romCnt[i])
			{
				EepromSaveChannel(i,ramcnt.wh_cnt[i]);
				lastROMCnt[i] = romCnt[i];
			}
		}
	}
	// Reset Cnt  
	for (i=0;i<NUM_CHAN;i++)
	{
		if (ramcnt.wh_cnt[i] > ROLL_BACK)
		{
			ramcnt.wh_cnt[i] -= ROLL_BACK;
		}
	}
  
  
    //Daily Saved Kwh
	if(newDayFlag){
		newDayFlag = 0;
		CONFIG_ReadFlash((void*)__CONFIG_DAILY_FLASH_BLOCK__,(void*)daily_flash_data,DAILY_BUFFER*4);
		for (i=0;i<NUM_CHAN;i++)
		{
			daily_flash_data[(rTime.day-1)*(i+1)*DAILY_SAVE]=ramcnt.wh_cnt[i];
		}
	}
	
  SWISR_HandlerFreqCall(2,100,4);
}


void displayCallback(void)
{
	int i;
	SMARTHOME_ReadResults((tSMARTHOME_DATA*)&mlib,&result,&umax,&imax);

	auto_calib((tCONFIG_FLASH_DATA *)&ramcfg,result);
	
	if(update_status_timeout) update_status_timeout--;
	IRTC_GetClock(&rTime);
	
	/* Power-off detect */
	if (zero_cnt<10)
	{
		eeprom_enable = 0x03;
	}
	else
	{
		if (eeprom_enable==0)
		{
			eeprom_enable = 0x80;
		}
	}
	zero_cnt = 0;
}

/* LPTMR callback */
void lptmr_callback(void)
{
  if(RTC_ISR & RTC_ISR_DAY_IS_MASK) //new min incomming
  {
    IRTC_LockUnlock(); 
    RTC_ISR = RTC_ISR_DAY_IS_MASK;
    newDayFlag = 1;
  }
  
  /* Remove EEPROM block */
  if ((eeprom_enable&ENABLE_MASK) ==0)
  {
		if (eeprom_enable!=0)
		{
			eeprom_enable--;
		}
  }
	
	sysTickCnt++;
	if(sysTickCnt == 5)//5s
	{
		//PORT_Init(PORTE,PORT_MODULE_ALT0_MODE,PIN6);
	}
}


void pulseOut(void)
{
  static uint16 cntWh=0;
  /* generate pulses on output LEDs                                           */
  register Frac16 dly;
  //dly = mlib.Wh[calibChannel].pulsDly;
  // Wh pulse
  //if (dly > 0)
  {
    if (mlib.Wh[calibChannel].pulsOut == 1) 
    {
      //GPIO_Clr(GPIOH,PIN7);
      cntWh = 0;
    }
  }
  
  if (cntWh > PULSE_OUT_TIME)
  {
    cntWh=0;
    //GPIO_Set(GPIOH,PIN7);
  }
  cntWh++;
}

void reset_daily(void)
{
	int i;
	for (i=0; i<DAILY_BUFFER;i++)
	{
		daily_flash_data[i]=0;
	}
}
/******************************************************************************/
/* Hardfault isr */
void hardfault_isr   (void)
{
  SystemReset();
  for (;;);
}
