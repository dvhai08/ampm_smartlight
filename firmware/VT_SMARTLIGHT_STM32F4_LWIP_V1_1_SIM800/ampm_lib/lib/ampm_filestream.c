

#include "ampm_filestream.h"

__weak int fgetc_stdin(FILE *f)
{
 return 0;
}

int fputc( int iChar, FILE *f )
{ 
	AMPM_FILEHANDLE *ampm_f = (AMPM_FILEHANDLE *)f;
	if(f == stdout)
	{
		fputc_stdout(iChar,f);
	}
	else if(ampm_f->ampmFileHandleCode == AMPM_FILE_HANDLE_CODE)
	{
		if(ampm_f->tell >= ampm_f->size)
			return EOF;
		ampm_f->data[ampm_f->tell++] = iChar;
		ampm_f->offset = ampm_f->tell;
		if(ampm_f->tell >= ampm_f->size)
			return EOF;
	}
	else
	{
		return EOF;
	}
	return( iChar );
}

int feof(FILE *f)
{
	AMPM_FILEHANDLE *ampm_f = (AMPM_FILEHANDLE *)f;
	if(ampm_f->ampmFileHandleCode != AMPM_FILE_HANDLE_CODE)
		return 0;
	if(ampm_f->tell >= ampm_f->size)
	{
		return EOF;
	}
	return ampm_f->tell;
}

long int ftell(FILE *f)
{
	AMPM_FILEHANDLE *ampm_f = (AMPM_FILEHANDLE *)f;
	if(ampm_f->ampmFileHandleCode != AMPM_FILE_HANDLE_CODE)
		return 0;
	return ampm_f->tell;
}


int fflush(FILE *f)
{
	AMPM_FILEHANDLE *ampm_f = (AMPM_FILEHANDLE *)f;
	if(ampm_f->ampmFileHandleCode != AMPM_FILE_HANDLE_CODE)
		return 0;
	ampm_f->offset = 0;
	ampm_f->tell = 0;
	return 0;
}

int fseek(FILE *f, long int offset, int whence)
{
	AMPM_FILEHANDLE *ampm_f = (AMPM_FILEHANDLE *)f;
	if(ampm_f->ampmFileHandleCode != AMPM_FILE_HANDLE_CODE)
		return 0;
	switch(whence)
	{
		case SEEK_SET: /* start of stream (see fseek) */
			ampm_f->offset = offset;
		break;
		case SEEK_CUR: /* current position in stream (see fseek) */
			
		break;
		case SEEK_END: /* end of stream (see fseek) */
			ampm_f->offset = ampm_f->tell;
		break;
		default:
			break;
	}
	return 0;
}

int fgetc(FILE *f)
{ 
	AMPM_FILEHANDLE *ampm_f = (AMPM_FILEHANDLE *)f;
	uint8_t iChar;
	if(f == stdin)
	{
		return fgetc_stdin(f);
	}
	else if(ampm_f->ampmFileHandleCode == AMPM_FILE_HANDLE_CODE)
	{
		if(ampm_f->offset >= ampm_f->tell)
		{
			return EOF;
		}
		else
		{
			iChar = ampm_f->data[ampm_f->offset++];
		}
	}
	else
	{
		return EOF;
	}
	return( iChar );
}


void AmpmFileHandle_Init(FILE *f,uint8_t *buf,uint32_t len)
{
	AMPM_FILEHANDLE *ampm_f = (AMPM_FILEHANDLE *)f;
	//RINGBUF_Init(&ampm_f->io,buf,len);
	ampm_f->data = buf;
	ampm_f->offset = 0;
	ampm_f->size = len;
	ampm_f->callback = NULL;
	ampm_f->ampmFileHandleCode = AMPM_FILE_HANDLE_CODE;
}


