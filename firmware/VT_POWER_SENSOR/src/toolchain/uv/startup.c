/******************************************************************************
 * (c) Copyright 2010-2014, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      startup.c
 * @version   1.0.4.0
 * @date      May-10-2014      
 * @brief     Generic Kinetis startup code.
 ******************************************************************************/
#include "common.h"

extern char Image$$ER_RAM_DATA$$RW$$Base[];
extern char Load$$ER_RAM_DATA$$RW$$Base[];
extern char Load$$ER_RAM_DATA$$RW$$Limit[];

extern char Image$$ER_RAM_BSS$$ZI$$Base[];
extern char Image$$ER_RAM_BSS$$ZI$$Limit[];

extern char Image$$ER_RAM_FAST$$RO$$Base[];
extern char Load$$ER_RAM_FAST$$RO$$Base[];
extern char Load$$ER_RAM_FAST$$RO$$Limit[];

/******************************************************************************
 * Common_startup function definition                                         *
 ******************************************************************************/
void startup(void)
{   
    /* Declare a counter we'll use in all of the copy loops                   */
    uint32 n;
    
    /* Get the addresses for the .data section (initialized data section)     */
    unsigned char* data_ram = (unsigned char*)Image$$ER_RAM_DATA$$RW$$Base;
    unsigned char* data_rom = (unsigned char*)Load$$ER_RAM_DATA$$RW$$Base;
    unsigned char* data_rom_end = (unsigned char*)Load$$ER_RAM_DATA$$RW$$Limit;
    
    /* Copy initialized data from ROM to RAM                                  */
    n = data_rom_end - data_rom;
    while (n--)
      *data_ram++ = *data_rom++;
    
    /* Get the addresses for the .bss section (zero-initialized data)         */
    unsigned char* bss_start = (unsigned char*)Image$$ER_RAM_BSS$$ZI$$Base;
    unsigned char* bss_end = (unsigned char*)Image$$ER_RAM_BSS$$ZI$$Limit;
    
    /* Clear the zero-initialized data section                                */
    n = bss_end - bss_start;
    while(n--)
      *bss_start++ = 0; 
    
    /* Get the addresses for the .fast section (code in RAM)                  */
    unsigned char* fast_ram = (unsigned char*)Image$$ER_RAM_FAST$$RO$$Base;
    unsigned char* fast_rom = (unsigned char*)Load$$ER_RAM_FAST$$RO$$Base;
    unsigned char* fast_rom_end = (unsigned char*)Load$$ER_RAM_FAST$$RO$$Limit;
        
    /* Copy functions from ROM to RAM                                         */
    n = fast_rom_end - fast_rom;
    while (n--)
      *fast_ram++ = *fast_rom++; 
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
