/******************************************************************************
*
* freemaster_cfg.h
*
* FreeMASTER Serial Communication Driver configuration file
*
*******************************************************************************/
#ifndef __FREEMASTER_CFG_H
#define __FREEMASTER_CFG_H

/*****************************************************************************
 * FreeMASTER communication interface
 *****************************************************************************/
//#define FMSTR_UART0                             // IR TERMINAL                       
#define FMSTR_UART3                             // 2.4GHz WIRELESS
//#define FMSTR_UART1                             // 2.4GHz WIRELESS


/******************************************************************************
* Select interrupt or poll-driven serial communication
******************************************************************************/
#define FMSTR_LONG_INTR    0        // complete message processing in interrupt
#define FMSTR_SHORT_INTR   1        // SCI FIFO-queuing done in interrupt
#define FMSTR_POLL_DRIVEN  0        // no interrupt needed, polling only

/*****************************************************************************
* Select communication interface (SCI or CAN)
******************************************************************************/
/*
#ifdef FMSTR_UART0
  #define FMSTR_SCI_BASE 0x4006A000   // UART0 base on MKM34Z50
  #define FMSTR_SCI_INTERRUPT 35      // UART0 & UART1 on MKM34Z50
#endif
*/

#ifdef FMSTR_UART1
  #define FMSTR_SCI_BASE 0x4006B000   // UART3 base on MKM34Z50
  #define FMSTR_SCI_INTERRUPT 35      // UART2 & UART3 on MKM34Z50
#endif


//#define FMSTR_SCI_BASE 0x4006B000   // UART1 base on MKM34Z50
//#define FMSTR_SCI_BASE 0x4006C000   // UART2 base on MKM34Z50
#ifdef FMSTR_UART3
  #define FMSTR_SCI_BASE 0x4006D000   // UART3 base on MKM34Z50
  #define FMSTR_SCI_INTERRUPT 36      // UART2 & UART3 on MKM34Z50
#endif


#define FMSTR_USE_SCI     1       // To select SCI communication interface 
#define FMSTR_USE_FLEXCAN 0       // To select FlexCAN communication interface

/******************************************************************************
* Input/output communication buffer size
******************************************************************************/
#define FMSTR_COMM_BUFFER_SIZE 0    // set to 0 for "automatic"

/******************************************************************************
* Receive FIFO queue size (use with FMSTR_SHORT_INTR only)
******************************************************************************/
#define FMSTR_COMM_RQUEUE_SIZE 32   // set to 0 for "default"

/*****************************************************************************
* Support for Application Commands 
******************************************************************************/
#define FMSTR_USE_APPCMD       0    // enable/disable App.Commands support
#define FMSTR_APPCMD_BUFF_SIZE 32   // App.Command data buffer size
#define FMSTR_MAX_APPCMD_CALLS 4    // how many app.cmd callbacks? (0=disable)

/*****************************************************************************
* Oscilloscope support
******************************************************************************/
#define FMSTR_USE_SCOPE       1     // enable/disable scope support
#define FMSTR_MAX_SCOPE_VARS  8     // max. number of scope variables (2..8)

/*****************************************************************************
* Recorder support
******************************************************************************/
#define FMSTR_USE_RECORDER    1     // enable/disable recorder support
#define FMSTR_MAX_REC_VARS    8     // max. number of recorder variables (2..8)
#define FMSTR_REC_OWNBUFF     0     // use user-allocated rec. buffer (1=yes)

// built-in recorder buffer (use when FMSTR_REC_OWNBUFF is 0)
#define FMSTR_REC_BUFF_SIZE   4096  // built-in buffer size

// recorder time base, specifies how often the recorder is called in the user app.
#define FMSTR_REC_TIMEBASE    FMSTR_REC_BASE_MICROSEC(833) // 0 = "unknown"

/*****************************************************************************
* Target-side address translation (TSA)
******************************************************************************/
#define FMSTR_USE_TSA         0     // enable TSA functionality
#define FMSTR_USE_TSA_SAFETY  1     // enable access to TSA variables only
#define FMSTR_USE_TSA_INROM   1     // TSA tables declared as const (put to ROM)

/*****************************************************************************
* Enable/Disable read/write memory commands
******************************************************************************/
#define FMSTR_USE_READMEM      1    // enable read memory commands
#define FMSTR_USE_WRITEMEM     1    // enable write memory commands
#define FMSTR_USE_WRITEMEMMASK 1    // enable write memory bits commands

/*****************************************************************************
* Enable/Disable read/write variable commands (a bit faster than Read Mem)
******************************************************************************/
#define FMSTR_USE_READVAR      0    // enable read variable fast commands
#define FMSTR_USE_WRITEVAR     0    // enable write variable fast commands
#define FMSTR_USE_WRITEVARMASK 0    // enable write variable bits fast commands

/*****************************************************************************
* Enable copy to buffer implementation via function.
******************************************************************************/
#define FMSTR_BUFFER_ACCESS_BY_FUNCT 1

#endif /* __FREEMASTER_CFG_H */
