/*
 * test_spiffs.h
 *
 *  Created on: Jun 19, 2013
 *      Author: petera
 */

#ifndef TEST_SPIFFS_H_
#define TEST_SPIFFS_H_

#include "spiffs.h"

#define FS &__fs

extern spiffs __fs;


#define CHECK(r) if (!(r)) return -1;
#define CHECK_RES(r) if (r < 0) return -1;
#define FS_PURE_DATA_PAGES(fs) \
    (SPIFFS_CFG_PHYS_SZ(fs) / SPIFFS_CFG_LOG_PAGE_SZ(fs)- (fs)->block_count * SPIFFS_OBJ_LOOKUP_PAGES(fs))
#define FS_PURE_DATA_SIZE(fs) \
    FS_PURE_DATA_PAGES(fs) * SPIFFS_DATA_PAGE_SIZE(fs)

#define O_RDONLY	0x0000
#define O_WRONLY	0x0001
#define O_RDWR		0x0002
#define O_ACCMODE	0x0003

#define O_BINARY	0x0004	/* must fit in char, reserved by dos */
#define O_TEXT		0x0008	/* must fit in char, reserved by dos */
#define O_NOINHERIT	0x0080	/* DOS-specific */

#define O_CREAT		0x0100	/* second byte, away from DOS bits */
#define O_EXCL		0x0200
#define O_NOCTTY	0x0400
#define O_TRUNC		0x0800
#define O_APPEND	0x1000
#define O_NONBLOCK	0x2000

int _open_r(int *fd,const char *filename, int flags);
int _read_r(int *fd, void *buf, size_t len);
int _write_r(int *fd, void *buf, size_t len);
int _lseek_r(int *fd, int where, int whence);
int _close_r(int *fd);
int _rename_r(const char *from, const char *to);
int _unlink_r(const char *filename);
int _fsize_r(int *fd);
int _opendir_r(spiffs_DIR *d,const char *name);
int _readdir_r(spiffs_DIR *d,struct spiffs_dirent *e);
int _closedir_r(spiffs_DIR *d);

void spiffs_setup(void);
void fs_reset(void);
void clear_flash_ops_log(void);
int test_create_file(char *name);
#endif /* TEST_SPIFFS_H_ */
