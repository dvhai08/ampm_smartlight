/*****************************************************************************
 * (c) Copyright 2010-2012, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      crt0.s
 * @version   1.0.1.0
 * @date      May-18-2012   
 * @brief     startup function (low level part).
 *
 * Revision History:
 ******************************************************************************/
    .global __gcc_program_start
    .section .text
    .syntax unified
    .thumb_func
__gcc_program_start:
    /* initialize heap                                                        */
    LDR     r0, =__heap_start__
    LDR     r1, =__heap_end__
    SUBS    r1,r1,r0
    CMP     r1,#8
    BLT     __cont
    MOVS    r2,#0
    STR     r2,[r0]
    ADDS    r0,r0,#4
    STR     r1,[r0]
__cont:  
    /* initialize registers                                                   */
    EORS    r0,r0                           /* clear r0                       */
    EORS    r1,r1                           /* clear r1                       */
    EORS    r2,r2                           /* clear r2                       */
    EORS    r3,r3                           /* clear r3                       */
    EORS    r4,r4                           /* clear r4                       */
    EORS    r5,r5                           /* clear r5                       */
    EORS    r6,r6                           /* clear r6                       */
    EORS    r7,r7                           /* clear r7                       */
    CPSID   i                               /* disable interrupts             */
    BL      start                           /* call the C code                */
__done:
    B       __done                          /* stay in the loop               */
    .end
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
