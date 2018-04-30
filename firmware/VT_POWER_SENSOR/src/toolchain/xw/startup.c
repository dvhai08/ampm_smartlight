/*****************************************************************************
 * (c) Copyright 2010-2012, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      startup.c
 * @version   1.0.1.0
 * @date      May-18-2012      
 * @brief     Generic Kinetis startup code.
 ******************************************************************************/
#include "common.h"

extern char __START_DATA_RAM[];
extern char __START_DATA_ROM[];
extern char __END_DATA_ROM[];
extern char __START_BSS_RAM[];
extern char __END_BSS_RAM[];
extern char __START_FAST_RAM[];
extern char __START_FAST_ROM[];
extern char __END_FAST_ROM[];

/******************************************************************************
 * Common_startup function definition                                         *
 ******************************************************************************/
void startup(void)
{  
    /* Declare a counter we'll use in all of the copy loops                   */
    uint32 n;
    
    /* Get the addresses for the .data section (initialized data section)     */
    unsigned char* data_ram = __START_DATA_RAM;
    unsigned char* data_rom = __START_DATA_ROM;
    unsigned char* data_rom_end = __END_DATA_ROM;
    
    /* Copy initialized data from ROM to RAM                                  */
    n = data_rom_end - data_rom;
    while (n--)
      *data_ram++ = *data_rom++;
 
    /* Get the addresses for the .bss section (zero-initialized data)         */
    unsigned char* bss_start = __START_BSS_RAM;
    unsigned char* bss_end = __END_BSS_RAM;
    
    /* Clear the zero-initialized data section                                */
    n = bss_end - bss_start;
    while(n--)
      *bss_start++ = 0; 
    
    /* Get the addresses for the .fast section (code in RAM)                   */
    unsigned char* fast_ram = __START_FAST_RAM;
    unsigned char* fast_rom = __START_FAST_ROM;
    unsigned char* fast_rom_end = __END_FAST_ROM;
        
    /* Copy functions from ROM to RAM                                        */
    n = fast_rom_end - fast_rom;
    while (n--)
      *fast_ram++ = *fast_rom++;
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
