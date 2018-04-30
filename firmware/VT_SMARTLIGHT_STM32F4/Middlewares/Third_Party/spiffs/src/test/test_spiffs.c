/*
 * test_spiffs.c
 *
 *  Created on: Jun 19, 2013
 *      Author: petera
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params_test.h"
#include "spiffs.h"
#include "spiffs_nucleus.h"

#include "testrunner.h"

#include "test_spiffs.h"
#include "sst25.h"
#include "led.h"
static u32_t fs_check_fixes = 0;

#define NUM_SYS_FD 3

spiffs __fs;
static u8_t _work[LOG_PAGE*2];
static u8_t _fds[DEFAULT_NUM_CACHE_PAGES*DEFAULT_NUM_FD];
static u8_t _cache[sizeof(spiffs_cache) + DEFAULT_NUM_CACHE_PAGES * (sizeof(spiffs_cache_page) + LOG_PAGE)];


static s32_t _read(
#if SPIFFS_HAL_CALLBACK_EXTRA
    spiffs *fs,
#endif
    u32_t addr, u32_t size, u8_t *dst) {
  //printf("rd @ addr %08x => %p\n", addr, &AREA(addr));
  if (addr < SPIFFS_CFG_PHYS_ADDR(&__fs)) {
    printf("FATAL read addr too low %08x < %08x\n", addr, SPIFFS_PHYS_ADDR);
    ERREXIT();
    return -1;
  }
  if (addr + size > SPIFFS_CFG_PHYS_ADDR(&__fs) + SPIFFS_CFG_PHYS_SZ(&__fs)) {
    printf("FATAL read addr too high %08x + %08x > %08x\n", addr, size, SPIFFS_PHYS_ADDR + SPIFFS_FLASH_SIZE);
    ERREXIT();
    return -1;
  }
	return OS_SST25_Read(FLASH_START_ADDR + addr,dst,size);
}

static s32_t _write(
#if SPIFFS_HAL_CALLBACK_EXTRA
    spiffs *fs,
#endif
    u32_t addr, u32_t size, u8_t *src) {
  int i;
  //printf("wr %08x %i\n", addr, size);

  if (addr < SPIFFS_CFG_PHYS_ADDR(&__fs)) {
    printf("FATAL write addr too low %08x < %08x\n", addr, SPIFFS_PHYS_ADDR);
    ERREXIT();
    return -1;
  }
  if (addr + size > SPIFFS_CFG_PHYS_ADDR(&__fs) + SPIFFS_CFG_PHYS_SZ(&__fs)) {
    printf("FATAL write addr too high %08x + %08x > %08x\n", addr, size, SPIFFS_PHYS_ADDR + SPIFFS_FLASH_SIZE);
    ERREXIT();
    return -1;
  }
  return OS_SST25_WriteNoErase(FLASH_START_ADDR + addr,src,size);
}
static s32_t _erase(
#if SPIFFS_HAL_CALLBACK_EXTRA
    spiffs *fs,
#endif
    u32_t addr, u32_t size) {
  if (addr & (SPIFFS_CFG_PHYS_ERASE_SZ(&__fs)-1)) {
    printf("trying to erase at addr %08x, out of boundary\n", addr);
    ERREXIT();
    return -1;
  }
  if (size & (SPIFFS_CFG_PHYS_ERASE_SZ(&__fs)-1)) {
    printf("trying to erase at with size %08x, out of boundary\n", size);
    ERREXIT();
    return -1;
  }
	return OS_SST25_Erase(FLASH_START_ADDR + addr,block4k);
}

static int check_cb_count;
// static u32_t old_perc = 999;
static void spiffs_check_cb_f(
#if SPIFFS_HAL_CALLBACK_EXTRA
    spiffs *fs,
#endif
    spiffs_check_type type, spiffs_check_report report,
    u32_t arg1, u32_t arg2) {
/*  if (report == SPIFFS_CHECK_PROGRESS && old_perc != arg1) {
    old_perc = arg1;
    printf("CHECK REPORT: ");
    switch(type) {
    case SPIFFS_CHECK_LOOKUP:
      printf("LU "); break;
    case SPIFFS_CHECK_INDEX:
      printf("IX "); break;
    case SPIFFS_CHECK_PAGE:
      printf("PA "); break;
    }
    printf("%i%%\n", arg1 * 100 / 256);
  }*/
  if (report != SPIFFS_CHECK_PROGRESS) {
    check_cb_count++;
    if (report != SPIFFS_CHECK_ERROR) fs_check_fixes++;
    printf("   check: ");
    switch (type) {
    case SPIFFS_CHECK_INDEX:
      printf("INDEX  "); break;
    case SPIFFS_CHECK_LOOKUP:
      printf("LOOKUP "); break;
    case SPIFFS_CHECK_PAGE:
      printf("PAGE   "); break;
    default:
      printf("????   "); break;
    }
    if (report == SPIFFS_CHECK_ERROR) {
      printf("ERROR %i", arg1);
    } else if (report == SPIFFS_CHECK_DELETE_BAD_FILE) {
      printf("DELETE BAD FILE %04x", arg1);
    } else if (report == SPIFFS_CHECK_DELETE_ORPHANED_INDEX) {
      printf("DELETE ORPHANED INDEX %04x", arg1);
    } else if (report == SPIFFS_CHECK_DELETE_PAGE) {
      printf("DELETE PAGE %04x", arg1);
    } else if (report == SPIFFS_CHECK_FIX_INDEX) {
      printf("FIX INDEX %04x:%04x", arg1, arg2);
    } else if (report == SPIFFS_CHECK_FIX_LOOKUP) {
      printf("FIX INDEX %04x:%04x", arg1, arg2);
    } else {
      printf("??");
    }
    printf("\n");
  }
}

s32_t fs_mount_specific(u32_t phys_addr, u32_t phys_size,
    u32_t phys_sector_size,
    u32_t log_block_size, u32_t log_page_size) {
  spiffs_config c;
  c.hal_erase_f = _erase;
  c.hal_read_f = _read;
  c.hal_write_f = _write;
#if SPIFFS_SINGLETON == 0
  c.log_block_size = log_block_size;
  c.log_page_size = log_page_size;
  c.phys_addr = phys_addr;
  c.phys_erase_block = phys_sector_size;
  c.phys_size = phys_size;
#endif
#if SPIFFS_FILEHDL_OFFSET
  c.fh_ix_offset = TEST_SPIFFS_FILEHDL_OFFSET;
#endif
  return SPIFFS_mount(&__fs, &c, _work, _fds, sizeof(_fds), _cache, sizeof(_cache), spiffs_check_cb_f);
}


extern void mUSART3_PutString (char *s);
/**
 * addr_offset
 */
void fs_reset_specific(u32_t phys_addr, u32_t phys_size,
    u32_t phys_sector_size,
    u32_t log_block_size, u32_t log_page_size) {
	char buf[128];
  memset(&__fs, 0, sizeof(__fs));
  s32_t res = fs_mount_specific(phys_addr, phys_size, phys_sector_size, log_block_size, log_page_size);

#if SPIFFS_USE_MAGIC
  if (res == SPIFFS_OK) {
		printf(buf,"Mount OK!\r\n");
		//mUSART3_PutString(buf);
		u32_t total;
		u32_t used;
		SPIFFS_info(&__fs, &total, &used);
		printf(buf,"total:%i\nused:%i\nremain:%i\r\n", total, used, total-used);
		//mUSART3_PutString(buf);
		spiffs_DIR d;
		struct spiffs_dirent e;
		struct spiffs_dirent *pe = &e;
		SPIFFS_opendir(&__fs, "/", &d);
		int spoon_guard = 0;
		while ((pe = SPIFFS_readdir(&d, pe))) {
			printf(buf,"Found a file# Name:%s - Size:%i\r\n", pe->name, pe->size);
			//mUSART3_PutString(buf);
			if(spoon_guard++ >= 3000)
				break;
		}
		return;
   // SPIFFS_unmount(&__fs);
  }
//	BuzzerSetStatus(&buzzer1Ctrl,200, 800,BUZZER_TURN_ON, 500);
	printf(buf,"SD Card is formatting\r\n");
	//mUSART3_PutString(buf);
	printf(buf,"Pls wait around 3 minutes\r\n");
	//mUSART3_PutString(buf);
  res = SPIFFS_format(&__fs);
  if (res != SPIFFS_OK) {
    printf(buf,"format failed, %i\r\n", SPIFFS_errno(&__fs));
		//mUSART3_PutString(buf);
  }
	printf(buf,"Done!\r\n");
	//mUSART3_PutString(buf);
  res = fs_mount_specific(phys_addr, phys_size, phys_sector_size, log_block_size, log_page_size);
  if (res != SPIFFS_OK) {
    printf(buf,"mount failed, %i\r\n", SPIFFS_errno(&__fs));
		//mUSART3_PutString(buf);
		return;
  }
	//BuzzerSetStatus(&buzzer1Ctrl,100, 100,BUZZER_TURN_ON, 1);
	printf(buf,"Mount OK!\r\n");
	//mUSART3_PutString(buf);
#endif
}

void fs_reset(void) {
  fs_reset_specific(SPIFFS_PHYS_ADDR, SPIFFS_FLASH_SIZE, SECTOR_SIZE, LOG_BLOCK, LOG_PAGE);
}




int test_create_file(char *name) {
  spiffs_stat s;
  spiffs_file fd;
  int res = SPIFFS_creat(FS, name, 0);
  CHECK_RES(res);
  fd = SPIFFS_open(FS, name, SPIFFS_RDONLY, 0);
  CHECK(fd >= 0);
  res = SPIFFS_fstat(FS, fd, &s);
  CHECK_RES(res);
  CHECK(strcmp((char*)s.name, name) == 0);
  CHECK(s.size == 0);
#if SPIFFS_OBJ_META_LEN
  {
    int i;
    for (i = 0; i < SPIFFS_OBJ_META_LEN; i++) {
      CHECK(s.meta[i] == 0xff);
    }
  }
#endif
  SPIFFS_close(FS, fd);
  return 0;
}


void spiffs_setup(void) {
  fs_reset();
}



int _opendir_r(spiffs_DIR *d,const char *name)
{
	if(SPIFFS_opendir(&__fs,name,d) == NULL)
		return -1;
	return 0;
}

int _readdir_r(spiffs_DIR *d,struct spiffs_dirent *e)
{
	if(SPIFFS_readdir(d,e) == e)
		return 0;
	return - 1;
}

int _closedir_r(spiffs_DIR *d)
{
	SPIFFS_closedir(d);
	return 0;
}

int _open_r(int *fd,const char *filename, int flags)
{
    spiffs_mode sm = 0;
    int res;
    int rw = (flags & 3);

    if (rw == O_RDONLY || rw == O_RDWR) {
        sm |= SPIFFS_RDONLY;
    }

    if (rw == O_WRONLY || rw == O_RDWR) {
        sm |= SPIFFS_WRONLY;
    }

    if (flags & O_CREAT) {
        sm |= SPIFFS_CREAT;
    }

    if (flags & O_TRUNC) {
        sm |= SPIFFS_TRUNC;
    }

    if (flags & O_APPEND) {
        sm |= SPIFFS_APPEND;
    }

    /* Supported in newer versions of SPIFFS. */
    /* if (flags && O_EXCL) sm |= SPIFFS_EXCL; */
    /* if (flags && O_DIRECT) sm |= SPIFFS_DIRECT; */

    *fd = SPIFFS_open(&__fs, (char *) filename, sm, 0);

    if (*fd >= 0) {
        *fd += NUM_SYS_FD;
				return SPIFFS_OK;
    }

    return *fd;
}

int _read_r(int *fd, void *buf, size_t len)
{

    int res;

    if (*fd < NUM_SYS_FD) {
        res = -1;
    } else {
        res = SPIFFS_read(&__fs, *fd - NUM_SYS_FD, buf, len);
    }

    return res;
}

int _write_r(int *fd, void *buf, size_t len)
{

    if (*fd < NUM_SYS_FD) {
        return -1;
    }

    int res = SPIFFS_write(&__fs, *fd - NUM_SYS_FD, (char *) buf, len);
    return res;
}

int _lseek_r(int *fd, int where, int whence)
{

    int res;

    if (*fd < NUM_SYS_FD) {
        res = -1;
    } else {
        res = SPIFFS_lseek(&__fs, *fd - NUM_SYS_FD, where, whence);
    }

    return res;
}

int _close_r(int *fd)
{

    if (*fd < NUM_SYS_FD) {
        return -1;
    }

    SPIFFS_close(&__fs, *fd - NUM_SYS_FD);
    return 0;
}

int _rename_r(const char *from, const char *to)
{

    int res = SPIFFS_rename(&__fs, (char *) from, (char *) to);
    return res;
}

int _unlink_r(const char *filename)
{

    int res = SPIFFS_remove(&__fs, (char *) filename);
    return res;
}

int _fsize_r(int *fd)
{
    int res;
    spiffs_stat ss;
    if (*fd < NUM_SYS_FD) {
        return 0;
    }
    res = SPIFFS_fstat(&__fs, *fd - NUM_SYS_FD, &ss);
    if (res < 0) {
        return 0;
    }
    return ss.size;
}


