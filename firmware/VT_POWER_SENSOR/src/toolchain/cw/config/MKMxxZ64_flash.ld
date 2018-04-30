/*
*********************************************************************************
**
**  File        : MKMxxZ64_flash.ld
**
**  Default linker command file for Flash targets
**
*********************************************************************************
*/
/* Entry Point                                                                  */
ENTRY(__gcc_program_start)

/* Generate a link error if heap and stack don't fit into RAM                   */
__stack_size = 0x200;     /* required amount of stack                           */
__heap_size  = 0x200;     /* required amount of heap                            */                                  

/* Specify the memory areas                                                     */
MEMORY
{
  m_interrupts(rx)  : ORIGIN = 0x00000000, LENGTH = 0x400
  m_memcfg    (rx)  : ORIGIN = 0x00000400, LENGTH = 0x10
  m_text      (rx)  : ORIGIN = 0x00000410, LENGTH = 0xFBF0
  m_data      (rwx) : ORIGIN = 0x1FFFF000, LENGTH = 0x4000  /* SRAM             */  
  m_rtc       (rwx) : ORIGIN = 0x40051000, LENGTH = 0x20    /* 32bytes RTC SRAM */
}
        
/* Define output sections                                                       */
SECTIONS
{
  /* placing my named section at given address:                                 */   
  .RtcRamBlock (NOLOAD) :   
  {     
    KEEP(*(.RtcRamSection)) /* keep my variable even if not referenced          */   
  } > m_rtc
  
  /* Interrupt vector table                                                     */
  .interrupts :
  {
    __vector_table = .;
    KEEP(*(.intvec))
    . = ALIGN(4);
  } > m_interrupts
  
  /* Flash Configuration Field (FCF)                                            */    
  .memcfg :
  {
    KEEP(*(.memcfg))   
    . = ALIGN(4);
  } > m_memcfg 

  /* The program code and other constant data goes into Flash                   */
  .text :
  {
    *(.text)            /* .text sections (code)                                */
    *(.text*)           /* .text* sections (code)                               */
    *(.rodata)          /* .rodata sections (constants, strings, etc.)          */
    *(.rodata*)         /* .rodata* sections (constants, strings, etc.)         */
    . = ALIGN(4);
    __data_ROM_addr = .;
  } > m_text

  /* reserve MTB memory at the end of m_data                                    */
  .mtbram :             /* MTB buffer address as defined by the hardware        */
  {                    
    KEEP(*(.mtbram))
  } > m_data  

  /* Data sections goes into RAM, but initaialization data goes to Flash        */
  .data : AT(__data_ROM_addr)
  {
    . = ALIGN(4);       /* align to long word due to preceeding MTB buffer      */
    _sdata = .;         /* create symbol at data start                          */
    *(.data)            /* .data sections                                       */
    *(.data*)           /* .data* sections                                      */
    . = ALIGN(4);
    _edata = .;         /* create symbol at data end                            */
  } > m_data
    
  __fast_ROM_addr = __data_ROM_addr + SIZEOF(.data);
  PROVIDE ( __START_DATA_RAM = _sdata );
  PROVIDE ( __START_DATA_ROM = __data_ROM_addr );
  PROVIDE ( __END_DATA_ROM = __fast_ROM_addr);

  /* Fast sections goes into RAM, but initaialization code goes to Flash        */
  .fast : AT(__fast_ROM_addr)
  {
    . = ALIGN(4);      
    _sfast = .;         /* create symbol at fast start                          */
    *(.fast)            /* .fast sections                                       */
    *(.fast*)           /* .fast* sections                                      */
    . = ALIGN(4);
    _efast = .;         /* create symbol at fast end                            */
  } > m_data 
    
  __end_ROM_addr = __fast_ROM_addr + SIZEOF(.fast);  
  PROVIDE ( __START_FAST_RAM = _sfast );
  PROVIDE ( __START_FAST_ROM = __fast_ROM_addr );
  PROVIDE ( __END_FAST_ROM = __end_ROM_addr );
    
  /* Uninitialized data section                                                 */
  .bss :
  {
    /* This is used by the startup to initialize the .bss section               */
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > m_data
  
  PROVIDE ( __START_BSS_RAM = _sbss );
  PROVIDE ( __END_BSS_RAM = _ebss );

  .noinit (NOLOAD) :
  {
    /* This section will not be initialized                                     */
    . = ALIGN(4);
    _snoinit = .;
    *(.noinit)	
    *(.noinit*)
    . = ALIGN(4);
    _enoinit = .;
  } > m_data

  PROVIDE ( __START_NOINIT_RAM = _snoinit );
  PROVIDE ( __END_NOINIT_RAM = _enoinit );
 
  /* User_heap_stack section is used to check that there is enough RAM left     */
  ._user_heap_stack :
  {
    . = ALIGN(4);
    __heap_addr = .;
    . = . + __heap_size;
    . = . + __stack_size;
    . = ALIGN(4);
  } > m_data
  
  PROVIDE ( _end = __heap_addr ); 
}