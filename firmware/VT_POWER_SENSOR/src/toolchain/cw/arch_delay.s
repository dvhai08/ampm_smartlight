/*****************************************************************************
 * (c) Copyright 2010-2012, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      arch_delay.s
 * @version   1.0.1.0
 * @date      May-18-2012    
 * @brief     Module with generic delay function delaying software execution 
 *            for given number of core clock ticks.
 ******************************************************************************/
      .global arch_delay
      .section .text
      .syntax unified
      .thumb_func
arch_delay:
      subs  r0,#6 	/* subtract function entry and exit code execution ticks	*/
loop: subs  r0,#2
      bpl   loop   
      bx    lr
     .end      
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
