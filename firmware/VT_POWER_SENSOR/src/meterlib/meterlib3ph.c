/******************************************************************************
 *
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 *
 * ALL RIGHTS RESERVED. FREESCALE SEMICONDUCTOR CONFIDENTIAL PROPRIETARY. 
 *  
 ***************************************************************************//*!
 *
 * @file      METERLIB3PH.c
 *
 * @author    R55013
 *
 * @version   1.0.8.0
 *
 * @date      Mar-08-2014
 *
 * @brief     Source file containing routines with implementation metering 
 *            specific calculations and algorithms.
 * 
 ******************************************************************************/ 
#include "fraclib.h"
#include "meterlib.h"

/******************************************************************************
 * Inline function definition                                                 *
 ******************************************************************************/
static inline int CalcEnergy (tENERGY_DATA *p, Frac24 u1, Frac24 i1, 
                                Frac24 u2, Frac24 i2,Frac24 u3, Frac24 i3,
                                Frac32 *pCnt, Frac64 outRes)
{
  register Frac64 energy, tmp, val;

  /* smooth energy measurements                                               */
  energy = LL_add(LL_add(LL_iir_1ord(LL_mac_ll(LL_mac_ll(LL_mul_ll(i1,u1),i2,u2),i3,u3),
                          &p->xIIR,&p->yIIR,p->cofIIR),p->offset),(p->offset<<1));
    
  /* process energy display value                                             */
  tmp = LL_add(p->dispRem,energy);
  if      (tmp >=  p->dispRes) { p->dispRem=LL_sub(tmp,p->dispRes); (*pCnt)++; } 
  else if (tmp <= -p->dispRes) { p->dispRem=LL_add(tmp,p->dispRes); (*pCnt)--; } 
  else                         { p->dispRem=tmp;                               } 
  
  /* process and generate pulse outputs                                       */
  tmp = LL_add(p->pulsRem,energy);
  if      (tmp >=  outRes) { val=LL_abs(p->pulsRem=LL_sub(tmp,outRes)); p->pulsOut^=1; }
  else if (tmp <= -outRes) { val=LL_abs(p->pulsRem=LL_add(tmp,outRes)); p->pulsOut^=1; }
  else                     { val=FRAC64(-1.0); p->pulsRem = tmp;                       }   

  /* calculate fine pulse delay to be generated by the Timer                  */
  if (val >= 0ll) { tmp=LL_abs(energy); p->pulsDly=S_div_llll(LL_sub(tmp,val),tmp); } 
  else            { p->pulsDly=FRAC16(-1.0);                                        } 
   
  /* return rough pulse output state                                          */
  return p->pulsOut;
}

#if defined(__ICCARM__) || defined(__GNUC__)
  static inline unsigned int GetInterruptMask (void)
  { 
    register unsigned int mask;
    __asm volatile("MRS %[Rd],PRIMASK":[Rd]"=r"(mask));
    return mask;
  }
#elif defined(__CC_ARM)
  static inline unsigned int GetInterruptMask (void)
  { 
    register unsigned int mask;
    __asm volatile("MRS mask,PRIMASK");
    return mask;
  }
#endif 

#if defined(__ICCARM__) || defined(__GNUC__)
  static inline void SetInterruptMask (register unsigned int mask)
  {
    __asm volatile("MSR PRIMASK,%[Rs]":[Rs]"+r"(mask));
  }
#elif defined(__CC_ARM)
  static inline void SetInterruptMask (register unsigned int mask)
  {
    __asm volatile("MSR PRIMASK,mask");
  }
#endif 

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
void METERLIB3PH_RemoveDcBias (tMETERLIB3PH_DATA *p, Frac24 u1Q, Frac24 i1Q, 
                               Frac24 u2Q, Frac24 i2Q, Frac24 u3Q, Frac24 i3Q)
{   
  register Frac64 tmp;
  
  if (p->sens == 1) /* current sensor with proportional output characteristic */
  {   
    /* Remove DC bias from phase voltage and phase current measurements       */
    LL_iir_1ord_ll (u1Q,&p->uDcb1[1].xIIR,&p->uDcb1[1].yIIR,p->bias.cofIIR);
    LL_iir_1ord_ll (u2Q,&p->uDcb2[1].xIIR,&p->uDcb2[1].yIIR,p->bias.cofIIR);
    LL_iir_1ord_ll (u3Q,&p->uDcb3[1].xIIR,&p->uDcb3[1].yIIR,p->bias.cofIIR);
    LL_iir_1ord_ll (i1Q,&p->iDcb1[1].xIIR,&p->iDcb1[1].yIIR,p->bias.cofIIR);
    LL_iir_1ord_ll (i2Q,&p->iDcb2[1].xIIR,&p->iDcb2[1].yIIR,p->bias.cofIIR);
    LL_iir_1ord_ll (i3Q,&p->iDcb3[1].xIIR,&p->iDcb3[1].yIIR,p->bias.cofIIR);
  }
  else              /* current sensor with derivative output characteristic   */
  {
    /* Remove DC bias from phase voltage measurements                         */
    tmp = LL_iir_1ord_ll (u1Q,&p->uDcb1[0].xIIR,&p->uDcb1[0].yIIR,p->bias.cofIIR);
    LL_iir_1ord_ll (L_extract_hi(tmp),&p->uDcb1[1].xIIR,&p->uDcb1[1].yIIR,p->bias.cofIIR);
    tmp = LL_iir_1ord_ll (u2Q,&p->uDcb2[0].xIIR,&p->uDcb2[0].yIIR,p->bias.cofIIR);
    LL_iir_1ord_ll (L_extract_hi(tmp),&p->uDcb2[1].xIIR,&p->uDcb2[1].yIIR,p->bias.cofIIR);
    tmp = LL_iir_1ord_ll (u3Q,&p->uDcb3[0].xIIR,&p->uDcb3[0].yIIR,p->bias.cofIIR);
    LL_iir_1ord_ll (L_extract_hi(tmp),&p->uDcb3[1].xIIR,&p->uDcb3[1].yIIR,p->bias.cofIIR);
  
    /* Remove DC bias from phase current measurements and integrate           */
    tmp = LL_iir_1ord_ll (i1Q,&p->iDcb1[0].xIIR,&p->iDcb1[0].yIIR,p->bias.cofIIR);
    tmp = LL_iir_1ord_ll (L_extract_hi(tmp),&p->intd1.xIIR,&p->intd1.yIIR,p->intg.cofIIR);
    LL_iir_1ord_ll (L_extract_hi(tmp),&p->iDcb1[1].xIIR,&p->iDcb1[1].yIIR,p->bias.cofIIR); 
    tmp = LL_iir_1ord_ll (i2Q,&p->iDcb2[0].xIIR,&p->iDcb2[0].yIIR,p->bias.cofIIR);
    tmp = LL_iir_1ord_ll (L_extract_hi(tmp),&p->intd2.xIIR,&p->intd2.yIIR,p->intg.cofIIR);
    LL_iir_1ord_ll (L_extract_hi(tmp),&p->iDcb2[1].xIIR,&p->iDcb2[1].yIIR,p->bias.cofIIR);   
    tmp = LL_iir_1ord_ll (i3Q,&p->iDcb3[0].xIIR,&p->iDcb3[0].yIIR,p->bias.cofIIR);
    tmp = LL_iir_1ord_ll (L_extract_hi(tmp),&p->intd3.xIIR,&p->intd3.yIIR,p->intg.cofIIR);
    LL_iir_1ord_ll (L_extract_hi(tmp),&p->iDcb3[1].xIIR,&p->iDcb3[1].yIIR,p->bias.cofIIR);       
  }
}

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
 *          used for calculation IIR high-pass and FIR filter paremeters.
 *          This function must be called prior METERLIB3PH_CalcWattHours and 
 *          METERLIB3PH_CalcAuxiliary functions.
 *
 ****************************************************************************/  
int METERLIB3PH_CalcWattHours (tMETERLIB3PH_DATA *p, Frac32 *pCnt, Frac64 outRes)
{   
  register Frac32 tmp32_1, tmp32_2, tmp32_3, tmp32_4, tmp32_5, tmp32_6;
  
  tmp32_1 = L_extract_hi(p->uDcb1[1].yIIR); tmp32_2 = L_extract_hi(p->iDcb1[1].yIIR); 
  tmp32_3 = L_extract_hi(p->uDcb2[1].yIIR); tmp32_4 = L_extract_hi(p->iDcb2[1].yIIR); 
  tmp32_5 = L_extract_hi(p->uDcb3[1].yIIR); tmp32_6 = L_extract_hi(p->iDcb3[1].yIIR);

  /* active energy calculation and pulse state output                         */
  return CalcEnergy (&p->Wh, tmp32_1, tmp32_2, tmp32_3, tmp32_4, tmp32_5, tmp32_6,
                      pCnt, outRes);  
}

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
int METERLIB3PH_CalcVarHours (tMETERLIB3PH_DATA *p, Frac32 *pCnt, Frac64 outRes)
{
  register Frac32 tmp32_1, tmp32_2, tmp32_3, tmp32_4, tmp32_5, tmp32_6;
  
  tmp32_1 = L_extract_hi(p->uDcb1[1].yIIR); tmp32_2 = L_extract_hi(p->iDcb1[1].yIIR);
  tmp32_3 = L_extract_hi(p->uDcb2[1].yIIR); tmp32_4 = L_extract_hi(p->iDcb2[1].yIIR); 
  tmp32_5 = L_extract_hi(p->uDcb3[1].yIIR); tmp32_6 = L_extract_hi(p->iDcb3[1].yIIR); 
  
  /* calculate 90 degree instantaneous voltage phase shifts and postprocess   */
  /* instantaneous current measurement to compensate for Hilbert FIR filter   */
  /* group delay                                                              */ 
  p->phSh1.yFIR1 =  LL_fir_ll(tmp32_1,p->phSh1.xFIR1,p->hilb.cofFIR1,p->hilb.lenFIR1);
  p->phSh1.yFIR2 = -L_fir_ls (tmp32_2,p->phSh1.xFIR2,p->hilb.cofFIR2,p->hilb.lenFIR2); 
  p->phSh2.yFIR1 =  LL_fir_ll(tmp32_3,p->phSh2.xFIR1,p->hilb.cofFIR1,p->hilb.lenFIR1);
  p->phSh2.yFIR2 = -L_fir_ls (tmp32_4,p->phSh2.xFIR2,p->hilb.cofFIR2,p->hilb.lenFIR2);
  p->phSh3.yFIR1 =  LL_fir_ll(tmp32_5,p->phSh3.xFIR1,p->hilb.cofFIR1,p->hilb.lenFIR1);
  p->phSh3.yFIR2 = -L_fir_ls (tmp32_6,p->phSh3.xFIR2,p->hilb.cofFIR2,p->hilb.lenFIR2);
    
  /* reactive energy calculation and pulse generation                         */
  return CalcEnergy (&p->VARh,
                      L_extract_hi(p->phSh1.yFIR1),
                      p->phSh1.yFIR2,
                      L_extract_hi(p->phSh2.yFIR1),
                      p->phSh2.yFIR2,
                      L_extract_hi(p->phSh3.yFIR1),
                      p->phSh3.yFIR2,
                      pCnt,
                      outRes);
}

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
void METERLIB3PH_CalcAuxiliary (tMETERLIB3PH_DATA *p)
{
  register Frac32 tmp32_1, tmp32_2, tmp32_3, tmp32_4;
  register Frac64 tmp64_1;
  register unsigned int mask;
  
  /* disable interrupt and load variables                                     */
  mask = GetInterruptMask(); 
  __asm volatile ("CPSID i"); /* disable interrupts to keep data consistancy  */
  tmp32_1=p->iDcb1[1].yIIR>>24; tmp32_2=p->uDcb1[1].yIIR>>24; 
  tmp32_3=p->phSh1.yFIR1>>24; tmp32_4=p->phSh1.yFIR2<<8;
  SetInterruptMask (mask);    /* re-enable interrupts                         */
    
  /* Active power calculation                                                 */
  tmp64_1 = LL_mul_ll(tmp32_1,tmp32_2);
  tmp64_1 = LL_add(LL_iir_1ord (tmp64_1,&p->PAve1.xIIR1,&p->PAve1.yIIR1,p->aver.cofIIR1),
                   (p->Wh.offset<<16));   
  LL_iir_1ord_ll(L_extract_hi(tmp64_1),&p->PAve1.xIIR2,&p->PAve1.yIIR2,p->aver.cofIIR2);
  
  /* Reactive power calculation                                               */
  tmp64_1 = LL_mul_ll(tmp32_4,tmp32_3); 
  tmp64_1 = LL_add(LL_iir_1ord (tmp64_1,&p->QAve1.xIIR1,&p->QAve1.yIIR1,p->aver.cofIIR1),
                   (p->VARh.offset<<16));   
  LL_iir_1ord_ll(L_extract_hi(tmp64_1),&p->QAve1.xIIR2,&p->QAve1.yIIR2,p->aver.cofIIR2);
  
  /* Voltage RMS calculation - direct RMS converter                           */
  tmp64_1 = LL_mul_ll   (tmp32_2,tmp32_2);
  tmp64_1 = LL_iir_1ord (tmp64_1,&p->uRms1.xIIR1,&p->uRms1.yIIR1,p->aver.cofIIR1);
  LL_iir_1ord_ll(L_sqr_ll(tmp64_1),&p->uRms1.xIIR2,&p->uRms1.yIIR2,p->aver.cofIIR2); 
  
  /* Current RMS calculation                                                  */
  if (p->sens == 1) /* current sensor with proportional output characteristic */
  {
    /* Current RMS calculation - direct RMS converter                         */
    tmp64_1 = LL_mul_ll   (tmp32_1,tmp32_1);
    tmp64_1 = LL_iir_1ord (tmp64_1,&p->iRms1.xIIR1,&p->iRms1.yIIR1,p->aver.cofIIR1);         
    LL_iir_1ord_ll(L_sqr_ll(tmp64_1),&p->iRms1.xIIR2,&p->iRms1.yIIR2,p->aver.cofIIR2);
  } 
  else              /* current sensor with derivative output characteristic   */ 
  {
    /* Current RMS calculation from power triangle                            */
    tmp32_1 = L_extract_hi(p->PAve1.yIIR2); tmp32_2 = L_extract_hi(p->QAve1.yIIR2);
    tmp32_3 = L_sqr_ll(LL_mac_ll(LL_mul_ll(tmp32_1, tmp32_1), tmp32_2, tmp32_2));
    tmp32_4 = L_div(tmp32_3,L_extract_hi(p->uRms1.yIIR2));
    LL_iir_1ord_ll(tmp32_4,&p->iRms1.xIIR2,&p->iRms1.yIIR2,p->aver.cofIIR2);    
  }
  
  /* disable interrupt and load variables                                     */
  mask = GetInterruptMask(); 
  __asm volatile ("CPSID i"); /* disable interrupts to keep data consistancy  */
  tmp32_1=p->iDcb2[1].yIIR>>24; tmp32_2=p->uDcb2[1].yIIR>>24; 
  tmp32_3=p->phSh2.yFIR1>>24; tmp32_4=p->phSh2.yFIR2<<8;
  SetInterruptMask (mask);    /* re-enable interrupts                         */
  
  /* Active power calculation                                                 */
  tmp64_1 = LL_mul_ll(tmp32_1,tmp32_2);
  tmp64_1 = LL_add(LL_iir_1ord (tmp64_1,&p->PAve2.xIIR1,&p->PAve2.yIIR1,p->aver.cofIIR1),
                   (p->Wh.offset<<16));   
  LL_iir_1ord_ll(L_extract_hi(tmp64_1),&p->PAve2.xIIR2,&p->PAve2.yIIR2,p->aver.cofIIR2);
  
  /* Reactive power calculation                                               */
  tmp64_1 = LL_mul_ll(tmp32_4,tmp32_3); 
  tmp64_1 = LL_add(LL_iir_1ord (tmp64_1,&p->QAve2.xIIR1,&p->QAve2.yIIR1,p->aver.cofIIR1),
                   (p->VARh.offset<<16));   
  LL_iir_1ord_ll(L_extract_hi(tmp64_1),&p->QAve2.xIIR2,&p->QAve2.yIIR2,p->aver.cofIIR2);
  
  /* Voltage RMS calculation - direct RMS converter                           */
  tmp64_1 = LL_mul_ll   (tmp32_2,tmp32_2);
  tmp64_1 = LL_iir_1ord (tmp64_1,&p->uRms2.xIIR1,&p->uRms2.yIIR1,p->aver.cofIIR1);
  LL_iir_1ord_ll(L_sqr_ll(tmp64_1),&p->uRms2.xIIR2,&p->uRms2.yIIR2,p->aver.cofIIR2);
  
  /* Current RMS calculation                                                  */
  if (p->sens == 1) /* current sensor with proportional output characteristic */
  {
    /* Current RMS calculation - direct RMS converter                         */
    tmp64_1 = LL_mul_ll   (tmp32_1,tmp32_1);
    tmp64_1 = LL_iir_1ord (tmp64_1,&p->iRms2.xIIR1,&p->iRms2.yIIR1,p->aver.cofIIR1);         
    LL_iir_1ord_ll(L_sqr_ll(tmp64_1),&p->iRms2.xIIR2,&p->iRms2.yIIR2,p->aver.cofIIR2);
  }
  else              /* current sensor with derivative output characteristic   */ 
  {
    /* Current RMS calculation from power triangle                            */
    tmp32_1 = L_extract_hi(p->PAve2.yIIR2); tmp32_2 = L_extract_hi(p->QAve2.yIIR2);
    tmp32_3 = L_sqr_ll(LL_mac_ll(LL_mul_ll(tmp32_1, tmp32_1), tmp32_2, tmp32_2));
    tmp32_4 = L_div(tmp32_3,L_extract_hi(p->uRms2.yIIR2));
    LL_iir_1ord_ll(tmp32_4,&p->iRms2.xIIR2,&p->iRms2.yIIR2,p->aver.cofIIR2);    
  }
  
  /* disable interrupt and load variables                                     */
  mask = GetInterruptMask(); 
  __asm volatile ("CPSID i"); /* disable interrupts to keep data consistancy  */
  tmp32_1=p->iDcb3[1].yIIR>>24; tmp32_2=p->uDcb3[1].yIIR>>24; 
  tmp32_3=p->phSh3.yFIR1>>24; tmp32_4=p->phSh3.yFIR2<<8;
  SetInterruptMask (mask);    /* re-enable interrupts                         */
  
  /* Active power calculation                                                 */
  tmp64_1 = LL_mul_ll(tmp32_1,tmp32_2);
  tmp64_1 = LL_add(LL_iir_1ord (tmp64_1,&p->PAve3.xIIR1,&p->PAve3.yIIR1,p->aver.cofIIR1),
                   (p->Wh.offset<<16));   
  LL_iir_1ord_ll(L_extract_hi(tmp64_1),&p->PAve3.xIIR2,&p->PAve3.yIIR2,p->aver.cofIIR2);
  
  /* Reactive power calculation                                               */
  tmp64_1 = LL_mul_ll(tmp32_4,tmp32_3); 
  tmp64_1 = LL_add(LL_iir_1ord (tmp64_1,&p->QAve3.xIIR1,&p->QAve3.yIIR1,p->aver.cofIIR1),
                   (p->VARh.offset<<16));   
  LL_iir_1ord_ll(L_extract_hi(tmp64_1),&p->QAve3.xIIR2,&p->QAve3.yIIR2,p->aver.cofIIR2);
  
  /* Voltage RMS calculation - direct RMS converter                           */
  tmp64_1 = LL_mul_ll   (tmp32_2,tmp32_2);
  tmp64_1 = LL_iir_1ord (tmp64_1,&p->uRms3.xIIR1,&p->uRms3.yIIR1,p->aver.cofIIR1);
  LL_iir_1ord_ll(L_sqr_ll(tmp64_1),&p->uRms3.xIIR2,&p->uRms3.yIIR2,p->aver.cofIIR2); 
    
  /* Current RMS calculation                                                  */
  if (p->sens == 1) /* current sensor with proportional output characteristic */
  {
    /* Current RMS calculation - direct RMS converter                         */
  tmp64_1 = LL_mul_ll   (tmp32_1,tmp32_1);
  tmp64_1 = LL_iir_1ord (tmp64_1,&p->iRms3.xIIR1,&p->iRms3.yIIR1,p->aver.cofIIR1);         
  LL_iir_1ord_ll(L_sqr_ll(tmp64_1),&p->iRms3.xIIR2,&p->iRms3.yIIR2,p->aver.cofIIR2);
  }
  else              /* current sensor with derivative output characteristic   */ 
  {
    /* Current RMS calculation from power triangle                            */
    tmp32_1 = L_extract_hi(p->PAve3.yIIR2); tmp32_2 = L_extract_hi(p->QAve3.yIIR2);
    tmp32_3 = L_sqr_ll(LL_mac_ll(LL_mul_ll(tmp32_1, tmp32_1), tmp32_2, tmp32_2));
    tmp32_4 = L_div(tmp32_3,L_extract_hi(p->uRms3.yIIR2));
    LL_iir_1ord_ll(tmp32_4,&p->iRms3.xIIR2,&p->iRms3.yIIR2,p->aver.cofIIR2);    
  }  
}

/*************************************************************************//*
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
void METERLIB3PH_ReadResultsPh1 (tMETERLIB3PH_DATA *p, double *urms, double *irms, 
                           double *w, double *var, double *va, double *umax, 
                           double *imax)
{ 
  register Frac32 tmp_1, tmp_2, tmp_3, tmp_4;
  register unsigned int mask;
  
  /* disable interrupt and load variables                                     */
  mask = GetInterruptMask(); 
  __asm volatile ("CPSID i"); /* disable interrupts to keep data consistancy  */
  tmp_1 = L_extract_hi(p->uRms1.yIIR2); tmp_2 = L_extract_hi(p->iRms1.yIIR2);
  tmp_3 = L_extract_hi(p->PAve1.yIIR2); tmp_4 = L_extract_hi(p->QAve1.yIIR2);
  SetInterruptMask (mask);    /* re-enable interrupts                         */
  
  /* sensitivity threshold logic - clears powers and RMS current if values    */
  /* are below sensitivity threshold                                          */
  if ((tmp_3 < p->thrd) && (tmp_3 > -p->thrd)) { tmp_3 = 0l; }
  if ((tmp_4 < p->thrd) && (tmp_4 > -p->thrd)) { tmp_4 = 0l; }
  if ((tmp_3 ==     0l) && (tmp_4 ==      0l)) { tmp_2 = 0l; }
  
  /* convert fractional quantities to double precision format                 */
  *urms = F32TODBL(tmp_1)*p->umax;       
  *irms = F32TODBL(tmp_2)*p->imax;
  *w    = F32TODBL(tmp_3)*p->imax*p->umax; 
  *var  = F32TODBL(tmp_4)*p->imax*p->umax;
  *va   = (*urms)*(*irms);
  *umax = p->umax;
  *imax = p->imax;  
}

/*************************************************************************//*
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
void METERLIB3PH_ReadResultsPh2 (tMETERLIB3PH_DATA *p, double *urms, double *irms, 
                           double *w, double *var, double *va, double *umax, 
                           double *imax)
{ 
  register Frac32 tmp_1, tmp_2, tmp_3, tmp_4;
  register unsigned int mask;
  
  /* disable interrupt and load variables                                     */
  mask = GetInterruptMask(); 
  __asm volatile ("CPSID i"); /* disable interrupts to keep data consistancy  */
  tmp_1 = L_extract_hi(p->uRms2.yIIR2); tmp_2 = L_extract_hi(p->iRms2.yIIR2);
  tmp_3 = L_extract_hi(p->PAve2.yIIR2); tmp_4 = L_extract_hi(p->QAve2.yIIR2);
  SetInterruptMask (mask);    /* re-enable interrupts                         */
  
  /* sensitivity threshold logic - clears powers and RMS current if values    */
  /* are below sensitivity threshold                                          */
  if ((tmp_3 < p->thrd) && (tmp_3 > -p->thrd)) { tmp_3 = 0l; }
  if ((tmp_4 < p->thrd) && (tmp_4 > -p->thrd)) { tmp_4 = 0l; }
  if ((tmp_3 ==     0l) && (tmp_4 ==      0l)) { tmp_2 = 0l; }
  
  /* convert fractional quantities to double precision format                 */
  *urms = F32TODBL(tmp_1)*p->umax;       
  *irms = F32TODBL(tmp_2)*p->imax;
  *w    = F32TODBL(tmp_3)*p->imax*p->umax; 
  *var  = F32TODBL(tmp_4)*p->imax*p->umax;
  *va   = (*urms)*(*irms);
  *umax = p->umax;
  *imax = p->imax;  
}

/*************************************************************************//*
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
void METERLIB3PH_ReadResultsPh3 (tMETERLIB3PH_DATA *p, double *urms, double *irms, 
                           double *w, double *var, double *va, double *umax, 
                           double *imax)
{ 
  register Frac32 tmp_1, tmp_2, tmp_3, tmp_4;
  register unsigned int mask;
  
  /* disable interrupt and load variables                                     */
  mask = GetInterruptMask(); 
  __asm volatile ("CPSID i"); /* disable interrupts to keep data consistancy  */
  tmp_1 = L_extract_hi(p->uRms3.yIIR2); tmp_2 = L_extract_hi(p->iRms3.yIIR2);
  tmp_3 = L_extract_hi(p->PAve3.yIIR2); tmp_4 = L_extract_hi(p->QAve3.yIIR2);
  SetInterruptMask (mask);    /* re-enable interrupts                         */
  
  /* sensitivity threshold logic - clears powers and RMS current if values    */
  /* are below sensitivity threshold                                          */
  if ((tmp_3 < p->thrd) && (tmp_3 > -p->thrd)) { tmp_3 = 0l; }
  if ((tmp_4 < p->thrd) && (tmp_4 > -p->thrd)) { tmp_4 = 0l; }
  if ((tmp_3 ==     0l) && (tmp_4 ==      0l)) { tmp_2 = 0l; }
  
  /* convert fractional quantities to double precision format                 */
  *urms = F32TODBL(tmp_1)*p->umax;       
  *irms = F32TODBL(tmp_2)*p->imax;
  *w    = F32TODBL(tmp_3)*p->imax*p->umax; 
  *var  = F32TODBL(tmp_4)*p->imax*p->umax;
  *va   = (*urms)*(*irms);
  *umax = p->umax;
  *imax = p->imax;  
}
/******************************************************************************
 * end of module                                                              *
 ******************************************************************************/