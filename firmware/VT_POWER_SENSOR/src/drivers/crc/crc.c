/******************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      crc.c
 * @version   1.0.2.0
 * @date      Jan-18-2014
 * @brief     Cyclic Redundancy Check (CRC) driver source code.
 ******************************************************************************/
#include "common.h"
#include "crc.h"

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
#if defined(__ICCARM__)
  #pragma diag_suppress=Pa082
#endif
void* CRC_INIT (tCRC crc)
{    
  CRC_CTRL  = (crc.CTRL & ~CRC_CTRL_WAS_MASK);
  CRC_GPOLY = crc.GPOLY;
  CRC_CTRL |= CRC_CTRL_WAS_MASK;
  CRC_DATA  = crc.DATA;
  CRC_CTRL &= ~CRC_CTRL_WAS_MASK;
  return (void*)CRC_BASE_PTR;
}

uint32 CRC_CALC8 (const uint8 *ptr, uint32 len)
{    
  register uint32 i;

  for (i = 0; i < len; i++) { *((vuint8*)&CRC_DATA) = *(ptr+i); }  
  if (CRC_CTRL & CRC_CTRL_TCRC_MASK) { return (uint32)(CRC_DATA & 0xFFFFFFFF); }
  else                               { return (uint32)(CRC_DATA & 0x0000FFFF); }
}

uint32 CRC_CALC16 (const uint16 *ptr, uint32 len)
{    
  register uint32 i;

  for (i = 0; i < len; i++) { *((vuint16*)&CRC_DATA) = *(ptr+i); }  
  if (CRC_CTRL & CRC_CTRL_TCRC_MASK) { return (uint32)(CRC_DATA & 0xFFFFFFFF); }
  else                               { return (uint32)(CRC_DATA & 0x0000FFFF); }
}

uint32 CRC_CALC32 (const uint32 *ptr, uint32 len)
{    
  register uint32 i;

  for (i = 0; i < len; i++) { *((vuint32*)&CRC_DATA) = *(ptr+i); }  
  if (CRC_CTRL & CRC_CTRL_TCRC_MASK) { return (uint32)(CRC_DATA & 0xFFFFFFFF); }
  else                               { return (uint32)(CRC_DATA & 0x0000FFFF); }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/