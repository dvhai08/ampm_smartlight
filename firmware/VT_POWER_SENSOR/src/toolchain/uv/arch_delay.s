;/******************************************************************************
; * (c) Copyright 2010-2014, Freescale Semiconductor Inc.
; * ALL RIGHTS RESERVED.
; ***************************************************************************//*!
; * @file      arch_delay.s
; * @version   1.0.2.0
; * @date      Jan-18-2014    
; * @brief     Module with generic delay function delaying software execution 
; *            for given number of core clock ticks.
; ******************************************************************************/
            EXPORT arch_delay
            AREA  |.text|, CODE, READONLY
            THUMB
arch_delay
            subs  r0,#6 ; subtract function entry and exit code execution ticks
loop
            subs  r0,#2
            bpl   loop   
            bx    lr
            END
;/******************************************************************************
; * End of module                                                              *
; ******************************************************************************/
