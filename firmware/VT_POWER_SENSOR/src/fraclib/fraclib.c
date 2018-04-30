/******************************************************************************
 *
 * (c) Copyright 2010-1012, Freescale Semiconductor Inc.
 *
 * ALL RIGHTS RESERVED. FREESCALE SEMICONDUCTOR CONFIDENTIAL PROPRIETARY.
 *
 ****************************************************************************//*!
 *
 * @file      FRACLIB.c
 *
 * @author    R55013
 *
 * @version   1.0.3.0
 *
 * @date      Mar-08-2014
 *
 * @brief     Source file containing routines for calculation 32-bit fractional
 *            arithmetic.
 * 
 *******************************************************************************/ 
#include "fraclib.h"

/*******************************************************************************
 * module macro defintion                                                      *
 *******************************************************************************/
#define DIV_STEP(m) { if((tmp+=tmp)>=den){q+=m;tmp-=den;} }
#define LSQR_STEP(k)                                      \
{                                                         \
  if(r1>=0)                                               \
  {                                                       \
    r1-=((q1+(Frac32)FRAC32(1.25/(((Frac32)1)<<k)))>>k);  \
    q1+=((Frac32)FRAC32(0.5)>>(k-1));                     \
  }                                                       \
  else                                                    \
  {                                                       \
    r1+=((q1+(Frac32)FRAC32(0.75/(((Frac32)1)<<k)))>>k);  \
  }                                                       \
}

#define LLSQR_STEP(k)                                     \
{                                                         \
  if(r1>=0)                                               \
  {                                                       \
    r1-=((q1+(Frac64)FRAC64(1.25/(((Frac64)1)<<k)))>>k);  \
    q1+=((Frac64)FRAC64(0.5)>>(k-1));                     \
  }                                                       \
  else                                                    \
  {                                                       \
    r1+=((q1+(Frac64)FRAC64(0.75/(((Frac64)1)<<k)))>>k);  \
  }                                                       \
}

/****************************************************************************//*!
 *
 * @brief   Division of a 32-bit fractional value by a 32-bit fractional value
 *          returning 16-bit fractional result.
 *
 * @param   num    - Input 32-bit fractional value.
 * @param   den    - Input 32-bit fractional value. 
 *
 * @return  Function returns 16-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac16 S_div_ll (register Frac32 num, register Frac32 den)
{
  register Frac32 q=0l;
  register unsigned long tmp=(unsigned long)num;
  
  /* input parameter conditions                                                */
  if (num >= den) { return FRAC16(1.0); }
  if (num <=  0l) { return FRAC16(0.0); }
  
  /* division calculation by comparison or restoring method                    */
  DIV_STEP(0x40000000); DIV_STEP(0x20000000); DIV_STEP(0x10000000); 
  DIV_STEP(0x08000000); DIV_STEP(0x04000000); DIV_STEP(0x02000000); 
  DIV_STEP(0x01000000); DIV_STEP(0x00800000); DIV_STEP(0x00400000); 
  DIV_STEP(0x00200000); DIV_STEP(0x00100000); DIV_STEP(0x00080000);
  DIV_STEP(0x00040000); DIV_STEP(0x00020000); DIV_STEP(0x00010000); 
  return (Frac16)(q>>16);  
}

/****************************************************************************//*!
 *
 * @brief   Division of a 32-bit fractional value by a 32-bit fractional value
 *          returning 32-bit fractional result.
 *
 * @param   num    - Input 32-bit fractional value.
 * @param   den    - Input 32-bit fractional value. 
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac32 L_div (register Frac32 num, register Frac32 den)
{
  register Frac32 q=0l;
  register unsigned long tmp=(unsigned long)num;
  
  /* input parameter conditions                                                */
  if (num >= den) { return FRAC32(1.0); }
  if (num <=  0l) { return FRAC32(0.0); }
  
  /* division calculation by comparison or restoring method                    */
  DIV_STEP(0x40000000); DIV_STEP(0x20000000); DIV_STEP(0x10000000); 
  DIV_STEP(0x08000000); DIV_STEP(0x04000000); DIV_STEP(0x02000000); 
  DIV_STEP(0x01000000); DIV_STEP(0x00800000); DIV_STEP(0x00400000); 
  DIV_STEP(0x00200000); DIV_STEP(0x00100000); DIV_STEP(0x00080000);
  DIV_STEP(0x00040000); DIV_STEP(0x00020000); DIV_STEP(0x00010000); 
  DIV_STEP(0x00008000); DIV_STEP(0x00004000); DIV_STEP(0x00002000); 
  DIV_STEP(0x00001000); DIV_STEP(0x00000800); DIV_STEP(0x00000400); 
  DIV_STEP(0x00000200); DIV_STEP(0x00000100); DIV_STEP(0x00000080);
  DIV_STEP(0x00000040); DIV_STEP(0x00000020); DIV_STEP(0x00000010); 
  DIV_STEP(0x00000008); DIV_STEP(0x00000004); DIV_STEP(0x00000002); 
  DIV_STEP(0x00000001);
  return q;  
}

/****************************************************************************//*!
 *
 * @brief   Division of a 64-bit fractional value by a 32-bit fractional value
 *          returning 32-bit fractional result.
 *
 * @param   num    - Input 64-bit fractional value.
 * @param   den    - Input 32-bit fractional value. 
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac32 L_div_lll (register Frac64 num, register Frac32 den)
{
  register Frac64 q=0ll;
  register unsigned long long tmp=(unsigned long long)num;
  
  /* input parameter conditions                                                */
  if (L_extract_hi(num) >= den) { return FRAC32(1.0); }
  if (num               <= 0ll) { return FRAC32(0.0); }
  
  /* division calculation by comparison or restoring method                    */
  DIV_STEP(0x4000000000000000); DIV_STEP(0x2000000000000000);
  DIV_STEP(0x1000000000000000); DIV_STEP(0x0800000000000000);
  DIV_STEP(0x0400000000000000); DIV_STEP(0x0200000000000000);
  DIV_STEP(0x0100000000000000); DIV_STEP(0x0080000000000000);
  DIV_STEP(0x0040000000000000); DIV_STEP(0x0020000000000000);
  DIV_STEP(0x0010000000000000); DIV_STEP(0x0008000000000000);
  DIV_STEP(0x0004000000000000); DIV_STEP(0x0002000000000000); 
  DIV_STEP(0x0001000000000000); DIV_STEP(0x0000800000000000); 
  DIV_STEP(0x0000400000000000); DIV_STEP(0x0000200000000000); 
  DIV_STEP(0x0000100000000000); DIV_STEP(0x0000080000000000); 
  DIV_STEP(0x0000040000000000); DIV_STEP(0x0000020000000000); 
  DIV_STEP(0x0000010000000000); DIV_STEP(0x0000008000000000);
  DIV_STEP(0x0000004000000000); DIV_STEP(0x0000002000000000);
  DIV_STEP(0x0000001000000000); DIV_STEP(0x0000000800000000);  
  DIV_STEP(0x0000000400000000); DIV_STEP(0x0000000200000000);
  DIV_STEP(0x0000000100000000); 
  
  return L_extract_hi (q);  
}

/****************************************************************************//*!
 *
 * @brief   Division of a 64-bit fractional value by a 64-bit fractional value
 *          returning 16-bit fractional result.
 *
 * @param   num    - Input 64-bit fractional value.
 * @param   den    - Input 64-bit fractional value. 
 *
 * @return  Function returns 16-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac16 S_div_llll (register Frac64 num, register Frac64 den)
{
  register Frac64 q=0ll;
  register unsigned long long tmp=(unsigned long long)num;
  
  /* input parameter conditions                                                */
  if (num >= den) { return FRAC16(1.0); }
  if (num <= 0ll) { return FRAC16(0.0); }
  
  /* division calculation by comparison or restoring method                    */
  DIV_STEP(0x4000000000000000); DIV_STEP(0x2000000000000000);
  DIV_STEP(0x1000000000000000); DIV_STEP(0x0800000000000000);
  DIV_STEP(0x0400000000000000); DIV_STEP(0x0200000000000000);
  DIV_STEP(0x0100000000000000); DIV_STEP(0x0080000000000000);
  DIV_STEP(0x0040000000000000); DIV_STEP(0x0020000000000000);
  DIV_STEP(0x0010000000000000); DIV_STEP(0x0008000000000000);
  DIV_STEP(0x0004000000000000); DIV_STEP(0x0002000000000000); 
  DIV_STEP(0x0001000000000000); 
  return (Frac16)(q>>48);
}

/****************************************************************************//*!
 *
 * @brief   Division of a 64-bit fractional value by a 64-bit fractional value
 *          returning 64-bit fractional result.
 *
 * @param   num    - Input 64-bit fractional value.
 * @param   den    - Input 64-bit fractional value. 
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac64 LL_div (register Frac64 num, register Frac64 den)
{
  register Frac64 q=0ll;
  register unsigned long long tmp=(unsigned long long)num;
  
  /* input parameter conditions                                                */
  if (num >= den) { return FRAC64(1.0); }
  if (num <= 0ll) { return FRAC64(0.0); }
  
  /* division calculation by comparison or restoring method                    */
  DIV_STEP(0x4000000000000000); DIV_STEP(0x2000000000000000);
  DIV_STEP(0x1000000000000000); DIV_STEP(0x0800000000000000);
  DIV_STEP(0x0400000000000000); DIV_STEP(0x0200000000000000);
  DIV_STEP(0x0100000000000000); DIV_STEP(0x0080000000000000);
  DIV_STEP(0x0040000000000000); DIV_STEP(0x0020000000000000);
  DIV_STEP(0x0010000000000000); DIV_STEP(0x0008000000000000);
  DIV_STEP(0x0004000000000000); DIV_STEP(0x0002000000000000); 
  DIV_STEP(0x0001000000000000); DIV_STEP(0x0000800000000000); 
  DIV_STEP(0x0000400000000000); DIV_STEP(0x0000200000000000); 
  DIV_STEP(0x0000100000000000); DIV_STEP(0x0000080000000000); 
  DIV_STEP(0x0000040000000000); DIV_STEP(0x0000020000000000); 
  DIV_STEP(0x0000010000000000); DIV_STEP(0x0000008000000000);
  DIV_STEP(0x0000004000000000); DIV_STEP(0x0000002000000000);
  DIV_STEP(0x0000001000000000); DIV_STEP(0x0000000800000000);  
  DIV_STEP(0x0000000400000000); DIV_STEP(0x0000000200000000);
  DIV_STEP(0x0000000100000000); DIV_STEP(0x0000000080000000);
  DIV_STEP(0x0000000040000000); DIV_STEP(0x0000000020000000); 
  DIV_STEP(0x0000000010000000); DIV_STEP(0x0000000008000000); 
  DIV_STEP(0x0000000004000000); DIV_STEP(0x0000000002000000); 
  DIV_STEP(0x0000000001000000); DIV_STEP(0x0000000000800000); 
  DIV_STEP(0x0000000000400000); DIV_STEP(0x0000000000200000); 
  DIV_STEP(0x0000000000100000); DIV_STEP(0x0000000000080000); 
  DIV_STEP(0x0000000000040000); DIV_STEP(0x0000000000020000); 
  DIV_STEP(0x0000000000010000); DIV_STEP(0x0000000000008000); 
  DIV_STEP(0x0000000000004000); DIV_STEP(0x0000000000002000); 
  DIV_STEP(0x0000000000001000); DIV_STEP(0x0000000000000800); 
  DIV_STEP(0x0000000000000400); DIV_STEP(0x0000000000000200); 
  DIV_STEP(0x0000000000000100); DIV_STEP(0x0000000000000080); 
  DIV_STEP(0x0000000000000040); DIV_STEP(0x0000000000000020); 
  DIV_STEP(0x0000000000000010); DIV_STEP(0x0000000000000008); 
  DIV_STEP(0x0000000000000004); DIV_STEP(0x0000000000000002); 
  DIV_STEP(0x0000000000000001);
  return q;  
}

/****************************************************************************//*!
 *
 * @brief   Square root value of a 32-bit fractional value returning a 16-bit 
 *          result.  
 *
 * @param   x - Input 32-bit fractional value.
 *
 * @return  Function returns 16-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/ 
Frac16 S_sqr_l (register Frac32 x)
{
  register Frac32 q1 = 0l;
  register Frac32 r1 = x-(Frac32)FRAC32(0.25);
  
  /* input parameter conditions                                                */
  if (x <= 0l) { return FRAC32(0.0); }
  
  /* square root calculation using non-restoring method                        */
  LSQR_STEP( 1); LSQR_STEP( 2); LSQR_STEP( 3); LSQR_STEP( 4); LSQR_STEP( 5); 
  LSQR_STEP( 6); LSQR_STEP( 7); LSQR_STEP( 8); LSQR_STEP( 9); LSQR_STEP(10); 
  LSQR_STEP(11); LSQR_STEP(12); LSQR_STEP(13); LSQR_STEP(14); LSQR_STEP(15); 
  
  return S_extract_hi (q1);  
}

/****************************************************************************//*!
 *
 * @brief   Square root value of a 32-bit fractional value returning a 32-bit 
 *          result.  
 *
 * @param   x - Input 32-bit fractional value.
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/ 
Frac32 L_sqr (register Frac32 x)
{
  register Frac32 q1 = 0l;
  register Frac32 r1 = x-(Frac32)FRAC32(0.25);
  
  /* input parameter conditions                                                */
  if (x <= 0l) { return FRAC32(0.0); }
  
  /* square root calculation using non-restoring method                        */
  LSQR_STEP( 1); LSQR_STEP( 2); LSQR_STEP( 3); LSQR_STEP( 4); LSQR_STEP( 5); 
  LSQR_STEP( 6); LSQR_STEP( 7); LSQR_STEP( 8); LSQR_STEP( 9); LSQR_STEP(10); 
  LSQR_STEP(11); LSQR_STEP(12); LSQR_STEP(13); LSQR_STEP(14); LSQR_STEP(15); 
  LSQR_STEP(16); LSQR_STEP(17); LSQR_STEP(18); LSQR_STEP(19); LSQR_STEP(20); 
  LSQR_STEP(21); LSQR_STEP(22); LSQR_STEP(23); LSQR_STEP(24); LSQR_STEP(25); 
  LSQR_STEP(26); LSQR_STEP(27); LSQR_STEP(28); LSQR_STEP(29); LSQR_STEP(30); 
  LSQR_STEP(31);
  
  return q1;  
}

/****************************************************************************//*!
 *
 * @brief   Square root value of a 64-bit fractional value returning a 32-bit 
 *          result.  
 *
 * @param   x - Input 64-bit fractional value.
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/ 
Frac32 L_sqr_ll (register Frac64 x)
{
  register Frac64 q1 = 0ll;
  register Frac64 r1 = x-(Frac64)FRAC64(0.25);
  
  /* input parameter conditions                                                */
  if (x <= 0ll) { return FRAC32(0.0); }
  
  /* square root calculation using non-restoring method                        */
  LLSQR_STEP( 1); LLSQR_STEP( 2); LLSQR_STEP( 3); LLSQR_STEP( 4); LLSQR_STEP( 5); 
  LLSQR_STEP( 6); LLSQR_STEP( 7); LLSQR_STEP( 8); LLSQR_STEP( 9); LLSQR_STEP(10); 
  LLSQR_STEP(11); LLSQR_STEP(12); LLSQR_STEP(13); LLSQR_STEP(14); LLSQR_STEP(15); 
  LLSQR_STEP(16); LLSQR_STEP(17); LLSQR_STEP(18); LLSQR_STEP(19); LLSQR_STEP(20); 
  LLSQR_STEP(21); LLSQR_STEP(22); LLSQR_STEP(23); LLSQR_STEP(24); LLSQR_STEP(25); 
  LLSQR_STEP(26); LLSQR_STEP(27); LLSQR_STEP(28); LLSQR_STEP(29); LLSQR_STEP(30); 
  LLSQR_STEP(31);
  
  return L_extract_hi (q1);  
}

/****************************************************************************//*!
 *
 * @brief   Square root value of a 64-bit fractional value returning a 64-bit 
 *          result.  
 *
 * @param   x - Input 64-bit fractional value.
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/ 
Frac64 LL_sqr (register Frac64 x)
{
  register Frac64 q1 = 0ll;
  register Frac64 r1 = x-(Frac64)FRAC64(0.25);
  
  /* input parameter conditions                                                */
  if (x <= 0l) { return FRAC64(0.0); }
  
  /* square root calculation using non-restoring method                        */
  LLSQR_STEP( 1); LLSQR_STEP( 2); LLSQR_STEP( 3); LLSQR_STEP( 4); LLSQR_STEP( 5); 
  LLSQR_STEP( 6); LLSQR_STEP( 7); LLSQR_STEP( 8); LLSQR_STEP( 9); LLSQR_STEP(10); 
  LLSQR_STEP(11); LLSQR_STEP(12); LLSQR_STEP(13); LLSQR_STEP(14); LLSQR_STEP(15); 
  LLSQR_STEP(16); LLSQR_STEP(17); LLSQR_STEP(18); LLSQR_STEP(19); LLSQR_STEP(20); 
  LLSQR_STEP(21); LLSQR_STEP(22); LLSQR_STEP(23); LLSQR_STEP(24); LLSQR_STEP(25); 
  LLSQR_STEP(26); LLSQR_STEP(27); LLSQR_STEP(28); LLSQR_STEP(29); LLSQR_STEP(30); 
  LLSQR_STEP(31); LLSQR_STEP(32); LLSQR_STEP(33); LLSQR_STEP(34); LLSQR_STEP(35);
  LLSQR_STEP(36); LLSQR_STEP(37); LLSQR_STEP(38); LLSQR_STEP(39); LLSQR_STEP(40); 
  LLSQR_STEP(41); LLSQR_STEP(42); LLSQR_STEP(43); LLSQR_STEP(44); LLSQR_STEP(45); 
  LLSQR_STEP(46); LLSQR_STEP(47); LLSQR_STEP(48); LLSQR_STEP(49); LLSQR_STEP(50); 
  LLSQR_STEP(51); LLSQR_STEP(52); LLSQR_STEP(53); LLSQR_STEP(54); LLSQR_STEP(55); 
  LLSQR_STEP(56); LLSQR_STEP(57); LLSQR_STEP(58); LLSQR_STEP(59); LLSQR_STEP(60);
  LLSQR_STEP(61); LLSQR_STEP(62); LLSQR_STEP(63);
  
  return q1;  
}
           
/****************************************************************************//*!
 *
 * @brief   Execute finite impulse response filter (FIR) iteration using the 
 *          following equation:
 *
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *
 *          Internal accumulations don't saturate. The FIR filter output is within 
 *          32-bit fractional range from 0x80000000 to 0x7fffffff.
 *
 * @param   x        - Input fractional value represented in 16-bit fractional 
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 16-bit 
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 16-bit fractional 
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb". 
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac32 L_fir_ss (register Frac16 x, register Frac16 *px, const Frac16 *pcoef, 
                 register Word16 len)
{
  register Frac32 tmp = 0;
  
  while ((--len)>0) 
  {
    *(px+len)=*(px+len-1);
    if (*(pcoef+len) != 0) { tmp = L_mac_ss (tmp,*(pcoef+len),*(px+len)); }
  }
  return L_mac_ss (tmp, *pcoef, *px = x);
} 
  
/****************************************************************************//*!
 *
 * @brief   Execute finite impulse response filter (FIR) iteration using the 
 *          following equation:
 *
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *
 *          Internal accumulations don't saturate. The FIR filter output is within 
 *          32-bit fractional range from 0x80000000 to 0x7fffffff.
 *
 * @param   x        - Input fractional value represented in 16-bit fractional 
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 16-bit 
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional 
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb". 
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac32 L_fir_sl (register Frac16 x, register Frac16 *px, const Frac32 *pcoef, 
                 register Word16 len)
{
  register Frac32 tmp = 0;
  
  while ((--len)>0) 
  { 
    *(px+len)=*(px+len-1);
    if (*(pcoef+len) != 0l) { tmp = L_mac_ls (tmp,*(pcoef+len),*(px+len)); }
  }
  return L_mac_ls (tmp, *pcoef, *px = x);
}

/****************************************************************************//*!
 *
 * @brief   Execute finite impulse response filter (FIR) iteration using the 
 *          following equation:
 *
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *
 *          Internal accumulations don't saturate. The FIR filter output is within 
 *          32-bit fractional range from 0x80000000 to 0x7fffffff.
 *
 * @param   x        - Input fractional value represented in 32-bit fractional 
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit 
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 16-bit fractional 
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb". 
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac32 L_fir_ls (register Frac32 x, register Frac32 *px, const Frac16 *pcoef, 
                 register Word16 len)
{
  register Frac32 tmp = 0;
  
  while ((--len)>0) 
  { 
    *(px+len)=*(px+len-1);
    if (*(pcoef+len) != 0) { tmp = L_mac_ls (tmp,*(px+len),*(pcoef+len)); }
  }
  return L_mac_ls (tmp, *px = x, *pcoef);
}

/****************************************************************************//*!
 *
 * @brief   Execute finite impulse response filter (FIR) iteration using the 
 *          following equation:
 *
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *
 *          Internal accumulations don't saturate. The FIR filter output is within 
 *          32-bit fractional range from 0x80000000 to 0x7fffffff.
 *
 * @param   x        - Input fractional value represented in 32-bit fractional 
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit 
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional 
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb". 
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac32 L_fir (register Frac32 x, register Frac32 *px, const Frac32 *pcoef, 
              register Word16 len)
{
  register Frac64 tmp = 0;
  
  while ((--len)>0) 
  { 
    *(px+len)=*(px+len-1);
    if (*(pcoef+len) != 0)  { tmp = LL_mac_ll (tmp,*(px+len),*(pcoef+len)); } 
  } 
  
  return L_extract_hi (LL_mac_ll (tmp, *px = x, *pcoef));
}

/****************************************************************************//*!
 *
 * @brief   Execute finite impulse response filter (FIR) iteration using the 
 *          following equation:
 *
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *
 *          Internal accumulations don't saturate. The FIR filter output is within 
 *          64-bit fractional range from 0x8000000000000000 to 0x7ffffffffffffff.
 *
 * @param   x        - Input fractional value represented in 32-bit fractional 
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit 
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional 
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb". 
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac64 LL_fir_ll (register Frac32 x, register Frac32 *px, const Frac32 *pcoef, 
                  register Word16 len)
{
  register Frac64 tmp = 0;
  
  while ((--len)>0) 
  { 
    *(px+len)=*(px+len-1);
    if (*(pcoef+len) != 0)  { tmp = LL_mac_ll (tmp,*(px+len),*(pcoef+len)); } 
  } 
  
  return LL_mac_ll (tmp, *px = x, *pcoef);
}

/****************************************************************************//*!
 *
 * @brief   Execute finite impulse response filter (FIR) iteration using the 
 *          following equation:
 *
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *
 *          Internal accumulations don't saturate. The FIR filter output is within 
 *          64-bit fractional range from 0x8000000000000000 to 0x7ffffffffffffff.
 *
 * @param   x        - Input fractional value represented in 64-bit fractional 
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 64-bit 
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional 
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb". 
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac64 LL_fir_lll (register Frac64 x, register Frac64 *px, const Frac32 *pcoef, 
                   register Word16 len)
{
  register Frac64 tmp = 0;
  
  while ((--len)>0) 
  { 
    *(px+len)=*(px+len-1);
    if (*(pcoef+len) != 0)  { tmp = LL_mac_lll (tmp,*(px+len),*(pcoef+len)); } 
  } 
  
  return LL_mac_lll (tmp, *px = x, *pcoef);
}

/****************************************************************************//*!
 *
 * @brief   Execute nth order infinite impulse response filter (IIR) iteration 
 *          using the following equation:
 *
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(na+1)*y(n-na)
 *
 *          Internal accumulations don't saturate. The IIR filter output is within 
 *          32-bit fractional range from 0x80000000 to 0x7fffffff..
 *
 * @param   x       - Input fractional value represented in 16-bit fractional 
 *                    format "x(n)".
 * @param   *px     - Pointer to previous input values represented in 16-bit 
 *                    fractional format "x(n-1) ... x(n-nb)".
 * @param   len_x   - Filter length "nb".
 * @param   *py     - Pointer to previous output values represented in 16-bit 
 *                    fractional format "y(n-1) ... y(n-na)".
 * @param   len_y   - Filter length "na".
 * @param   *pcoef  - Pointer to filter constants represented in 16-bit fractional 
 *                    format "b(1) ... b(nb+1), a(2) ... a(na+1)".
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac32 L_iir_ss (register Frac16 x, register Frac16 *px, register Word16 len_x, 
                 register Frac32 *py, register Word16 len_y, const Frac16 *pcoef)
{ 
  register Frac32 tmp = 0;
  register Word16 i;
  
  for (i=(Word16)(len_x-1); i > 0 ;  i--) { *(px+i) = *(px+i-1); } 
  *px = x;
  
  for (i=0; i < len_x;  i++) { tmp = L_mac_ss (tmp, *(px+i), *(pcoef      +i)); }
  for (i=0; i < len_y;  i++) { tmp = L_msu_ls (tmp, *(py+i), *(pcoef+i+len_x)); }
  
  for (i=(Word16)(len_y-1); i > 0 ;  i--) { *(py+i) = *(py+i-1); }
    
  return *py = tmp;
}

/****************************************************************************//*!
 *
 * @brief   Execute nth order infinite impulse response filter (IIR) iteration 
 *          using the following equation:
 *
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(na+1)*y(n-na)
 *
 *          Internal accumulations don't saturate. The IIR filter output is within 
 *          32-bit fractional range from 0x80000000 to 0x7fffffff..
 *
 * @param   x       - Input fractional value represented in 32-bit fractional 
 *                    format "x(n)".
 * @param   *px     - Pointer to previous input values represented in 32-bit 
 *                    fractional format "x(n-1) ... x(n-nb)".
 * @param   len_x   - Filter length "nb".
 * @param   *py     - Pointer to previous output values represented in 32-bit 
 *                    fractional format "y(n-1) ... y(n-na)".
 * @param   len_y   - Filter length "na".
 * @param   *pcoef  - Pointer to filter constants represented in 32-bit fractional 
 *                    format "b(1) ... b(nb+1), a(2) ... a(na+1)".
 *
 * @return  Function returns 32-bit value in fractional format.
 *
 * @remarks Implementation in C-language.
 *
 *******************************************************************************/
Frac32 L_iir (register Frac32 x, register Frac32 *px, register Word16 len_x, 
              register Frac32 *py, register Word16 len_y, const Frac32 *pcoef)
{ 
  register Frac64 tmp = 0;
  register Word16 i;
  
  for (i=(Word16)(len_x-1); i > 0 ;  i--) { *(px+i) = *(px+i-1); } 
  *px = x;
  
  for (i=0; i < len_x;  i++) { tmp = LL_mac_ll (tmp, *(px+i), *(pcoef      +i)); }
  for (i=0; i < len_y;  i++) { tmp = LL_msu_ll (tmp, *(py+i), *(pcoef+i+len_x)); }
  
  for (i=(Word16)(len_y-1); i > 0 ;  i--) { *(py+i) = *(py+i-1); }
    
  return *py = L_extract_hi (tmp);
}
/*******************************************************************************
 * end of module                                                               *
 *******************************************************************************/