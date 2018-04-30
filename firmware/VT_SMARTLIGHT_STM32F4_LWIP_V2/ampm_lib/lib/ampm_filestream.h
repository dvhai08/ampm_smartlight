

#ifndef __AMPM_FILESTREAM_H__
#define __AMPM_FILESTREAM_H__
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib/ringbuf.h"
 
#define AMPM_FILE_HANDLE_CODE 0xA111B111 

extern int fputc_stdout( int iChar, FILE *f );

typedef struct
{
	uint32_t ampmFileHandleCode;
	void  (*callback)(void);
	int offset;
	int size;
	int tell;
	uint8_t *data;
	//RINGBUF io;
} AMPM_FILEHANDLE;


void AmpmFileHandle_Init(FILE *f,uint8_t *buf,uint32_t len);
int AmpmFileHandle_Write(FILE *f,uint8_t *data,uint32_t len);
int AmpmFileHandle_Read(FILE *f,uint8_t *data,uint32_t len);


#endif
