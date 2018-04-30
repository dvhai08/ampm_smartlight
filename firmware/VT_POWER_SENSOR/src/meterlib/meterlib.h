/******************************************************************************
 *
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 *
 * ALL RIGHTS RESERVED.
 *
 ***************************************************************************//*!
 *
 * @file      METERLIB.h
 *
 * @author    R55013
 *
 * @version   1.0.7.0
 *
 * @date      Feb-07-2014
 *
 * @brief     Header file containing common data types, macros and list of 
 *            exported functions supporting metering specific calculations.
 *
 ******************************************************************************/
#ifndef __METERLIB_H
#define __METERLIB_H
#include "fraclib.h"
/******************************************************************************
 * user data type & macro definitions                                         *
 ******************************************************************************/
#define PWR_SENS_THR(x)   FRAC32(((x)/(U_MAX*I0_MAX/2.0)))
#define KWH_DISP_RES0(x)   FRAC48(((1e3/(x))/(U_MAX*I0_MAX/3600/CALCFREQ)))
#define KWH_DISP_RES1(x)   FRAC48(((1e3/(x))/(U_MAX*I1_MAX/3600/CALCFREQ)))
#define KWH_DISP_RES2(x)   FRAC48(((1e3/(x))/(U_MAX*I2_MAX/3600/CALCFREQ)))
#define KWH_DISP_RES3(x)   FRAC48(((1e3/(x))/(U_MAX*I2_MAX/3600/CALCFREQ)))
#define KWH_PULS_NUM0(x)   FRAC48(((5e2/(x))/(U_MAX*I0_MAX/3600/CALCFREQ)))
#define KWH_PULS_NUM1(x)   FRAC48(((5e2/(x))/(U_MAX*I1_MAX/3600/CALCFREQ)))
#define KWH_PULS_NUM2(x)   FRAC48(((5e2/(x))/(U_MAX*I2_MAX/3600/CALCFREQ)))
#define KWH_PULS_NUM3(x)   FRAC48(((5e2/(x))/(U_MAX*I3_MAX/3600/CALCFREQ)))
#define KVARH_DISP_RES(x) FRAC48(((1e3/(x))/(U_MAX*I0_MAX/3600/CALCFREQ))) 
#define KVARH_PULS_NUM(x) FRAC48(((5e2/(x))/(U_MAX*I0_MAX/3600/CALCFREQ)))
#define MAXCOF            101     /*!< max. number of Hilbert FIR filter coef.*/
 
extern Frac32 PCorrect;

/*! DC bias supressor structure definition                                    */
typedef struct
{
  Frac32    cofIIR[3];            /*!< filter coefficients                    */
} tBIASFILTER_COEF;

typedef struct
{
  Frac32    xIIR;                 /*!< previous values of filter inputs       */  
  Frac64    yIIR;                 /*!< filtered sample - dc bias removed      */
} tBIASFILTER_DATA;

/*! Phase shifter filter structure definition                                 */
typedef struct
{
  Word16    lenFIR1;              /*!< length of Hilbert FIR filter           */ 
  Frac32    cofFIR1[MAXCOF];      /*!< coefficients of the Hilbert FIR filter */
  Word16    lenFIR2;              /*!< length of Group delay FIR filter       */
  Frac16    cofFIR2[(MAXCOF+1)/2];/*!< Group delay filter coefficients        */
} tPHSHFILTER_COEF;

typedef struct
{
  Frac32    xFIR1[MAXCOF];        /*!< previous values of the filter inputs   */ 
  Frac64    yFIR1;                /*!< FIR1 filter output value               */
  Frac32    xFIR2[(MAXCOF+1)/2];  /*!< previous values of the filter inputs   */  
  Frac32    yFIR2;                /*!< FIR2 filter output value               */
} tPHSHFILTER_DATA;

/*! Voltage RMS, current RMS, active power and reactive power averager        */
/* structure definition                                                       */
typedef struct
{
  Frac32    cofIIR1[3];           /*!< prefilter coefficients                 */
  Frac32    cofIIR2[3];           /*!< postfilter coefficients                */
} tAVERFILTER_COEF;

typedef struct
{
  Frac64    xIIR1;                /*!< previous values of the prefilter input */  
  Frac64    yIIR1;                /*!< output value of the prefilter          */    
  Frac32    xIIR2;                /*!< previous value of the postfilter input */  
  Frac64    yIIR2;                /*!< output value of the postfilter         */   
} tAVERFILTER_DATA;

/*! Accumulative energy structure definition                                  */ 
typedef struct
{ 
  Frac64    offset;               /*!< power offset due to channel crosstalk  */
  Frac64    dispRes;              /*!< resolution of energy display           */
  Frac64    dispRem;              /*!< reminder of energy display             */
  Frac64    pulsRem;              /*!< reminder of energy pulse               */
  int       pulsOut;              /*!< current state of the rough pulse output*/ 
  Frac16    pulsDly;              /*!< fine pulse delay generated by timer    */ 
  Frac32    cofIIR[3];            /*!< pulse output smoothing filter coef.    */
  Frac64    xIIR;                 /*!< smoothing filter previous input value  */  
  Frac64    yIIR;                 /*!< smoothing filter previous input value  */
} tENERGY_DATA;

/*! Metering library data structure definition - 1PH METER                    */
typedef struct
{ 
  double            umax;         /*!< maximal voltage measurement range      */
  double            imax;         /*!< maximal current measurement range      */
  Frac32            thrd;         /*!< sensitivity threshold                  */
  int               sens;         /*!< sensor: 1-proportional, 2-derivative   */
  tBIASFILTER_COEF  bias;         /*!< dc bias supressor filter coefficients  */     
  tBIASFILTER_COEF  intg;         /*!< integrator filter coefficients         */
  tBIASFILTER_DATA  uDcb[2];      /*!< ph1-voltage dc bias supressor data     */
  tBIASFILTER_DATA  intd;         /*!< ph1-current integrator filter data     */       
  tBIASFILTER_DATA  iDcb[2];      /*!< ph1-current dc bias supressor data     */     
  tPHSHFILTER_COEF  hilb;         /*!< Hilbert & delay filter coefficients    */
  tPHSHFILTER_DATA  phSh;         /*!< ph1-Hilbert & delay filter data        */
  tAVERFILTER_COEF  aver;         /*!< averager filter coefficients           */ 
  tAVERFILTER_DATA  uRms;         /*!< ph1-voltage RMS averager data          */ 
  tAVERFILTER_DATA  iRms;         /*!< ph1-current RMS averager data          */
  tAVERFILTER_DATA  PAve;         /*!< ph1-active power averager data         */ 
  tAVERFILTER_DATA  QAve;         /*!< ph1-reactive power averager data       */    
  tENERGY_DATA      Wh;           /*!< active energy                          */          
  tENERGY_DATA      VARh;         /*!< reactive energy                        */
} tMETERLIB1PH_DATA;


/*! Metering library data structure definition - 2PH METER                    */
typedef struct
{ 
  double            umax;         /*!< maximal voltage measurement range      */
  double            imax;         /*!< maximal current measurement range      */
  Frac32            thrd;         /*!< sensitivity threshold                  */
  int               sens;         /*!< sensor: 1-proportional, 2-derivative   */
  tBIASFILTER_COEF  bias;         /*!< dc bias supressor filter coefficients  */     
  tBIASFILTER_COEF  intg;         /*!< integrator filter coefficients         */
  tBIASFILTER_DATA  uDcb1[2];     /*!< ph1-voltage dc bias supressor data     */
  tBIASFILTER_DATA  intd1;        /*!< ph1-current integrator filter data     */
  tBIASFILTER_DATA  iDcb1[2];     /*!< ph1-current dc bias supressor data     */
  tBIASFILTER_DATA  uDcb2[2];     /*!< ph2-voltage dc bias supressor data     */
  tBIASFILTER_DATA  intd2;        /*!< ph2-current integrator filter data     */       
  tBIASFILTER_DATA  iDcb2[2];     /*!< ph2-current dc bias supressor data     */
  tPHSHFILTER_COEF  hilb;         /*!< Hilbert & delay filter coefficients    */
  tPHSHFILTER_DATA  phSh1;        /*!< ph1-Hilbert & delay filter data        */
  tPHSHFILTER_DATA  phSh2;        /*!< ph2-Hilbert & delay filter data        */
  tAVERFILTER_COEF  aver;         /*!< averager filter coefficients           */ 
  tAVERFILTER_DATA  uRms1;        /*!< ph1-voltage RMS averager data          */
  tAVERFILTER_DATA  uRms2;        /*!< ph2-voltage RMS averager data          */
  tAVERFILTER_DATA  iRms1;        /*!< ph1-current RMS averager data          */
  tAVERFILTER_DATA  iRms2;        /*!< ph2-current RMS averager data          */
  tAVERFILTER_DATA  PAve1;        /*!< ph1-active power averager data         */
  tAVERFILTER_DATA  PAve2;        /*!< ph2-active power averager data         */
  tAVERFILTER_DATA  QAve1;        /*!< ph1-reactive power averager data       */
  tAVERFILTER_DATA  QAve2;        /*!< ph2-reactive power averager data       */
  tENERGY_DATA      Wh;           /*!< active energy                          */          
  tENERGY_DATA      VARh;         /*!< reactive energy                        */
} tMETERLIB2PH_DATA;

/*! Metering library data structure definition - 3PH METER                    */
typedef struct
{ 
  double            umax;         /*!< maximal voltage measurement range      */
  double            imax;         /*!< maximal current measurement range      */
  Frac32            thrd;         /*!< sensitivity threshold                  */
  int               sens;         /*!< sensor: 1-proportional, 2-derivative   */
  tBIASFILTER_COEF  bias;         /*!< dc bias supressor filter coefficients  */     
  tBIASFILTER_COEF  intg;         /*!< integrator filter coefficients         */
  tBIASFILTER_DATA  uDcb1[2];     /*!< ph1-voltage dc bias supressor data     */
  tBIASFILTER_DATA  intd1;        /*!< ph1-current integrator filter data     */
  tBIASFILTER_DATA  iDcb1[2];     /*!< ph1-current dc bias supressor data     */
  tBIASFILTER_DATA  uDcb2[2];     /*!< ph2-voltage dc bias supressor data     */
  tBIASFILTER_DATA  intd2;        /*!< ph2-current integrator filter data     */
  tBIASFILTER_DATA  iDcb2[2];     /*!< ph2-current dc bias supressor data     */
  tBIASFILTER_DATA  uDcb3[2];     /*!< ph3-voltage dc bias supressor data     */
  tBIASFILTER_DATA  intd3;        /*!< ph3-current integrator filter data     */
  tBIASFILTER_DATA  iDcb3[2];     /*!< ph3-current dc bias supressor data     */
  tPHSHFILTER_COEF  hilb;         /*!< Hilbert & delay filter coefficients    */
  tPHSHFILTER_DATA  phSh1;        /*!< ph1-Hilbert & delay filter data        */
  tPHSHFILTER_DATA  phSh2;        /*!< ph2-Hilbert & delay filter data        */
  tPHSHFILTER_DATA  phSh3;        /*!< ph3-Hilbert & delay filter data        */
  tAVERFILTER_COEF  aver;         /*!< averager filter coefficients           */ 
  tAVERFILTER_DATA  uRms1;        /*!< ph1-voltage RMS averager data          */
  tAVERFILTER_DATA  uRms2;        /*!< ph2-voltage RMS averager data          */
  tAVERFILTER_DATA  uRms3;        /*!< ph3-voltage RMS averager data          */
  tAVERFILTER_DATA  iRms1;        /*!< ph1-current RMS averager data          */
  tAVERFILTER_DATA  iRms2;        /*!< ph2-current RMS averager data          */
  tAVERFILTER_DATA  iRms3;        /*!< ph3-current RMS averager data          */
  tAVERFILTER_DATA  PAve1;        /*!< ph1-active power averager data         */
  tAVERFILTER_DATA  PAve2;        /*!< ph2-active power averager data         */
  tAVERFILTER_DATA  PAve3;        /*!< ph3-active power averager data         */
  tAVERFILTER_DATA  QAve1;        /*!< ph1-reactive power averager data       */
  tAVERFILTER_DATA  QAve2;        /*!< ph2-reactive power averager data       */
  tAVERFILTER_DATA  QAve3;        /*!< ph3-reactive power averager data       */
  tENERGY_DATA      Wh;           /*!< active energy                          */          
  tENERGY_DATA      VARh;         /*!< reactive energy                        */
} tMETERLIB3PH_DATA;

/******************************************************************************
 * exported function prototypes                                               *
 ******************************************************************************/
/******************************************************************************
 * 1-PH METER API                                                             *
 ******************************************************************************/
/*************************************************************************//*!
 *
 * @brief   Remove DC bias from phase voltage and phase current samples.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB1PH_DATA).
 * @param   u1Q     Phase 1 instantaneous voltage sample in 24-bit data format.
 * @param   i1Q     Phase 1 instantaneous current sample in 24-bit data format.
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. 
 *
 ****************************************************************************/  
extern void METERLIB1PH_RemoveDcBias  (tMETERLIB1PH_DATA *p, Frac24 u1Q, Frac24 i1Q);

/*************************************************************************//*!
 *
 * @brief   Recalculate active energy using new voltage and current samples.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB1PH_DATA).
 * @param   pCnt    Pointer to LCD counter representing active energy.
 * @param   outRes  Resolution of pulse output (calculated by KWH_PULS_NUM())
 * @return  Function returns rough pulse output state.
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. Typical calling frequency depends on the
 *          possibilities of the analogue front-end. This frequency must be 
 *          used for calculation IIR high-pass and FIR filter parameters.
 *          This function must be called prior METERLIB1PH_CalcVarHours and 
 *          METERLIB1PH_CalcAuxiliary functions.
 *
 ****************************************************************************/
extern int  METERLIB1PH_CalcWattHours (tMETERLIB1PH_DATA *p, Frac32 *pCnt, 
                                        Frac64 outRes, Frac32);

/*************************************************************************//*!
 *
 * @brief   Recalculate reactive energy.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB1PH_DATA).
 * @param   pCnt    Pointer to LCD counter representing reactive energy.
 * @param   outRes  Resolution of pulse output (calculated by KVARH_PULS_NUM())
 * @return  Function returns rough pulse output state.
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. Typical calling frequency is several times 
 *          lower than calling frequency of the METERLIB1PH_CalcWattHours function. 
 *          The frequency of this function calls must be used for calculation FIR 
 *          filters parameters. Function disables and enables interrupts during
 *          execution.
 *
 ****************************************************************************/
extern int  METERLIB1PH_CalcVarHours  (tMETERLIB1PH_DATA *p, Frac32 *pCnt, 
                                        Frac64 outRes, Frac32 Correct);

/*************************************************************************//*!
 *
 * @brief   Recalculate auxiliary variables; IRMS, URMS, P, Q and S.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB1PH_DATA).
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. Typical calling frequency is several times 
 *          lower than calling frequency of the METERLIB1PH_CalcWattHours function. 
 *          The frequency of this function calls must be used for calculation all 
 *          IIR low pass filter parameters. Function disables and enables 
 *          interrupts during execution.
 *
 ****************************************************************************/
extern void METERLIB1PH_CalcAuxiliary (tMETERLIB1PH_DATA *p);

/*************************************************************************//*!
 * @brief   Returns auxiliary variables: IRMS, URMS, P, Q and S.
 *
 * @param   p         Pointer to metering library data structure (tMETERLIB1PH_DATA).
 * @param   urms      Pointer to RMS value of phase voltage [U]
 * @param   irms      Pointer to RMS value of phase current calculated using Explicit 
 *                    RMS Estimator algorithm [A]
 * @param   w         Pointer to active power [W]
 * @param   var       Pointer to reactive power [VAr]
 * @param   va        Pointer to apparent power [VA]
 * @param   umax      Pointer to phase voltage circuit scaling (maximal peak voltage) [V]
 * @param   imax      Pointer to phase current circuit scaling (maximal peak current) [I]
 *
 * @remarks Implemented in C-language. Function disables interrupts prior 
 *          execution. Interrupts are re-enabled after function execution.
 *
 ****************************************************************************/
void METERLIB1PH_ReadResults (tMETERLIB1PH_DATA *p, double *urms, double *irms, 
                           double *w, double *var, double *va, double *umax, 
                           double *imax, Frac32 u_gain, Frac32 i_gain, Frac32 pGain);
                                        
/******************************************************************************
 * 2-PH METER API                                                             *
 ******************************************************************************/
/*************************************************************************//*!
 *
 * @brief   Remove DC bias from phase voltage and phase current samples.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB2PH_DATA).
 * @param   u1Q     Phase 1 instantaneous voltage sample in 24-bit data format.
 * @param   i1Q     Phase 1 instantaneous current sample in 24-bit data format.
 * @param   u2Q     Phase 2 instantaneous voltage sample in 24-bit data format.
 * @param   i2Q     Phase 2 instantaneous current sample in 24-bit data format.
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. 
 *
 ****************************************************************************/
extern void METERLIB2PH_RemoveDcBias  (tMETERLIB2PH_DATA *p, Frac24 u1Q, Frac24 i1Q, 
                                        Frac24 u2Q, Frac24 i2Q);

/*************************************************************************//*!
 *
 * @brief   Recalculate active energy using new voltage and current samples.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB2PH_DATA).
 * @param   pCnt    Pointer to LCD counter representing active energy.
 * @param   outRes  Resolution of pulse output (calculated by KWH_PULS_NUM())
 * @return  Function returns rough pulse output state.
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. Typical calling frequency depends on the
 *          possibilities of the analogue front-end. This frequency must be 
 *          used for calculation IIR high-pass and FIR filter parameters.
 *          This function must be called prior METERLIB2PH_CalcWattHours and 
 *          METERLIB2PH_CalcAuxiliary functions.
 *
 ****************************************************************************/
extern int  METERLIB2PH_CalcWattHours (tMETERLIB2PH_DATA *p, Frac32 *pCnt, 
                                        Frac64 outRes);

/*************************************************************************//*!
 *
 * @brief   Recalculate reactive energy.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB2PH_DATA).
 * @param   pCnt    Pointer to LCD counter representing reactive energy.
 * @param   outRes  Resolution of pulse output (calculated by KVARH_PULS_NUM())
 * @return  Function returns rough pulse output state.
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. Typical calling frequency is several times 
 *          lower than calling frequency of the METERLIB2PH_CalcWattHours function. 
 *          The frequency of this function calls must be used for calculation FIR 
 *          filters parameters. Function disables and enables interrupts during
 *          execution.
 *
 ****************************************************************************/
extern int  METERLIB2PH_CalcVarHours  (tMETERLIB2PH_DATA *p, Frac32 *pCnt, 
                                        Frac64 outRes);

/*************************************************************************//*!
 *
 * @brief   Recalculate auxiliary variables; IRMS, URMS, P, Q and S.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB2PH_DATA).
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. Typical calling frequency is several times 
 *          lower than calling frequency of the METERLIB2PH_CalcWattHours function. 
 *          The frequency of this function calls must be used for calculation all 
 *          IIR low pass filter parameters. Function disables and enables 
 *          interrupts during execution.
 *
 ****************************************************************************/
extern void METERLIB2PH_CalcAuxiliary (tMETERLIB2PH_DATA *p);

/*************************************************************************//*!
 * @brief   Returns phase 1 auxiliary variables: IRMS, URMS, P, Q and S.
 *
 * @param   p         Pointer to metering library data structure (tMETERLIB2PH_DATA).
 * @param   urms      Pointer to RMS value of phase voltage [U]
 * @param   irms      Pointer to RMS value of phase current calculated using Explicit 
 *                    RMS Estimator algorithm [A]
 * @param   w         Pointer to active power [W]
 * @param   var       Pointer to reactive power [VAr]
 * @param   va        Pointer to apparent power [VA]
 * @param   umax      Pointer to phase voltage circuit scaling (maximal peak voltage) [V]
 * @param   imax      Pointer to phase current circuit scaling (maximal peak current) [I]
 *
 * @remarks Implemented in C-language. Function disables interrupts prior 
 *          execution. Interrupts are re-enabled after function execution.
 *
 ****************************************************************************/
extern void METERLIB2PH_ReadResultsPh1(tMETERLIB2PH_DATA *p, double *urms, 
                                        double *irms, double *w, double *var, 
                                        double *va, double *umax, double *imax);
                                        
/*************************************************************************//*!
 * @brief   Returns phase 2 auxiliary variables: IRMS, URMS, P, Q and S.
 *
 * @param   p         Pointer to metering library data structure (tMETERLIB2PH_DATA).
 * @param   urms      Pointer to RMS value of phase voltage [U]
 * @param   irms      Pointer to RMS value of phase current calculated using Explicit 
 *                    RMS Estimator algorithm [A]
 * @param   w         Pointer to active power [W]
 * @param   var       Pointer to reactive power [VAr]
 * @param   va        Pointer to apparent power [VA]
 * @param   umax      Pointer to phase voltage circuit scaling (maximal peak voltage) [V]
 * @param   imax      Pointer to phase current circuit scaling (maximal peak current) [I]
 *
 * @remarks Implemented in C-language. Function disables interrupts prior 
 *          execution. Interrupts are re-enabled after function execution.
 *
 ****************************************************************************/
extern void METERLIB2PH_ReadResultsPh2(tMETERLIB2PH_DATA *p, double *urms, 
                                        double *irms, double *w, double *var, 
                                        double *va, double *umax, 
                                        double *imax);

/******************************************************************************
 * 3-PH METER API                                                             *
 ******************************************************************************/
/*************************************************************************//*!
 *
 * @brief   Remove DC bias from phase voltage and phase current samples.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB3PH_DATA).
 * @param   u1Q     Phase 1 instantaneous voltage sample in 24-bit data format.
 * @param   i1Q     Phase 1 instantaneous current sample in 24-bit data format.
 * @param   u2Q     Phase 2 instantaneous voltage sample in 24-bit data format.
 * @param   i2Q     Phase 2 instantaneous current sample in 24-bit data format.
 * @param   u3Q     Phase 3 instantaneous voltage sample in 24-bit data format.
 * @param   i3Q     Phase 3 instantaneous current sample in 24-bit data format.
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. 
 *
 ****************************************************************************/
extern void METERLIB3PH_RemoveDcBias  (tMETERLIB3PH_DATA *p, Frac24 u1Q, Frac24 i1Q, 
                                        Frac24 u2Q, Frac24 i2Q, Frac24 u3Q, Frac24 i3Q);                                      

/*************************************************************************//*!
 *
 * @brief   Recalculate active energy using new voltage and current samples.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB3PH_DATA).
 * @param   pCnt    Pointer to LCD counter representing active energy.
 * @param   outRes  Resolution of pulse output (calculated by KWH_PULS_NUM())
 * @return  Function returns rough pulse output state.
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. Typical calling frequency depends on the
 *          possibilities of the analogue front-end. This frequency must be 
 *          used for calculation IIR high-pass and FIR filter parameters.
 *          This function must be called prior METERLIB3PH_CalcWattHours and 
 *          METERLIB3PH_CalcAuxiliary functions.
 *
 ****************************************************************************/
extern int METERLIB3PH_CalcWattHours (tMETERLIB3PH_DATA *p, Frac32 *pCnt, 
                                        Frac64 outRes, Frac32 Correct);

/*************************************************************************//*!
 *
 * @brief   Recalculate reactive energy.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB3PH_DATA).
 * @param   pCnt    Pointer to LCD counter representing reactive energy.
 * @param   outRes  Resolution of pulse output (calculated by KVARH_PULS_NUM())
 * @return  Function returns rough pulse output state.
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. Typical calling frequency is several times 
 *          lower than calling frequency of the METERLIB3PH_CalcWattHours function. 
 *          The frequency of this function calls must be used for calculation FIR 
 *          filters parameters. Function disables and enables interrupts during
 *          execution.
 *
 ****************************************************************************/ 
extern int METERLIB3PH_CalcVarHours  (tMETERLIB3PH_DATA *p, Frac32 *pCnt, 
                                        Frac64 outRes, Frac32 Correct);

/*************************************************************************//*!
 *
 * @brief   Recalculate auxiliary variables; IRMS, URMS, P, Q and S.
 *
 * @param   p       Pointer to metering library data structure (tMETERLIB3PH_DATA).
 *
 * @remarks Implemented in C-language. Note that this function must be called
 *          at constant frequency. Typical calling frequency is several times 
 *          lower than calling frequency of the METERLIB3PH_CalcWattHours function. 
 *          The frequency of this function calls must be used for calculation all 
 *          IIR low pass filter parameters. Function disables and enables 
 *          interrupts during execution.
 *
 ****************************************************************************/
extern void METERLIB3PH_CalcAuxiliary (tMETERLIB3PH_DATA *p);

/*************************************************************************//*!
 * @brief   Returns phase 1 auxiliary variables: IRMS, URMS, P, Q and S.
 *
 * @param   p         Pointer to metering library data structure (tMETERLIB3PH_DATA).
 * @param   urms      Pointer to RMS value of phase voltage [U]
 * @param   irms      Pointer to RMS value of phase current calculated using Explicit 
 *                    RMS Estimator algorithm [A]
 * @param   w         Pointer to active power [W]
 * @param   var       Pointer to reactive power [VAr]
 * @param   va        Pointer to apparent power [VA]
 * @param   umax      Pointer to phase voltage circuit scaling (maximal peak voltage) [V]
 * @param   imax      Pointer to phase current circuit scaling (maximal peak current) [I]
 *
 * @remarks Implemented in C-language. Function disables interrupts prior 
 *          execution. Interrupts are re-enabled after function execution.
 *
 ****************************************************************************/
extern void METERLIB3PH_ReadResultsPh1(tMETERLIB3PH_DATA *p, double *urms, 
                                    double *irms, double *w, double *var, 
                                    double *va, double *umax, double *imax);
                                    
/*************************************************************************//*!
 * @brief   Returns phase 2 auxiliary variables: IRMS, URMS, P, Q and S.
 *
 * @param   p         Pointer to metering library data structure (tMETERLIB3PH_DATA).
 * @param   urms      Pointer to RMS value of phase voltage [U]
 * @param   irms      Pointer to RMS value of phase current calculated using Explicit 
 *                    RMS Estimator algorithm [A]
 * @param   w         Pointer to active power [W]
 * @param   var       Pointer to reactive power [VAr]
 * @param   va        Pointer to apparent power [VA]
 * @param   umax      Pointer to phase voltage circuit scaling (maximal peak voltage) [V]
 * @param   imax      Pointer to phase current circuit scaling (maximal peak current) [I]
 *
 * @remarks Implemented in C-language. Function disables interrupts prior 
 *          execution. Interrupts are re-enabled after function execution.
 *
 ****************************************************************************/
extern void METERLIB3PH_ReadResultsPh2(tMETERLIB3PH_DATA *p, double *urms, 
                                    double *irms, double *w, double *var, 
                                    double *va, double *umax, double *imax);
                                    
/*************************************************************************//*!
 * @brief   Returns phase 3 auxiliary variables: IRMS, URMS, P, Q and S.
 *
 * @param   p         Pointer to metering library data structure (tMETERLIB3PH_DATA).
 * @param   urms      Pointer to RMS value of phase voltage [U]
 * @param   irms      Pointer to RMS value of phase current calculated using Explicit 
 *                    RMS Estimator algorithm [A]
 * @param   w         Pointer to active power [W]
 * @param   var       Pointer to reactive power [VAr]
 * @param   va        Pointer to apparent power [VA]
 * @param   umax      Pointer to phase voltage circuit scaling (maximal peak voltage) [V]
 * @param   imax      Pointer to phase current circuit scaling (maximal peak current) [I]
 *
 * @remarks Implemented in C-language. Function disables interrupts prior 
 *          execution. Interrupts are re-enabled after function execution.
 *
 ****************************************************************************/
extern void METERLIB3PH_ReadResultsPh3(tMETERLIB3PH_DATA *p, double *urms, 
                                    double *irms, double *w, double *var, 
                                    double *va, double *umax, double *imax);                                                                            
#endif /* __METERLIB_H */
         
