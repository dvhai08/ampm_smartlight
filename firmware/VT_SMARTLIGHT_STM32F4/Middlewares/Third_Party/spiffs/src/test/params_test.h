/*
 * params_test.h
 *
 *  Created on: May 26, 2013
 *      Author: petera
 */

#ifndef PARAMS_TEST_H_
#define PARAMS_TEST_H_
#include "sst25.h"
//////////////// TEST PARAMS ////////////////

// spiffs file system size
#define SPIFFS_FLASH_SIZE     FLASH_SIZE_MAX
// spiffs file system offset in emulated spi flash
#define SPIFFS_PHYS_ADDR      0
// sector size
#define SECTOR_SIZE         4096
// logical block size
#define LOG_BLOCK           4096
// logical page size
#define LOG_PAGE            256
// number of filedescs
#define DEFAULT_NUM_FD            8
// number of cache pages
#define DEFAULT_NUM_CACHE_PAGES   2

// When testing, test bench create reference files for comparison on
// the actual hard drive. By default, put these on ram drive for speed.
#define TEST_PATH "/dev/shm/spiffs/test-data/"

#define ASSERT(c, m) 


/////////// SPIFFS BUILD CONFIG  ////////////

// test using filesystem magic
#ifndef SPIFFS_USE_MAGIC
#define SPIFFS_USE_MAGIC    0
#endif
// test using filesystem magic length
#ifndef SPIFFS_USE_MAGIC_LENGTH
#define SPIFFS_USE_MAGIC_LENGTH   1
#endif
// test using extra param in callback
#ifndef SPIFFS_HAL_CALLBACK_EXTRA
#define SPIFFS_HAL_CALLBACK_EXTRA       1
#endif
// test using filehandle offset
#ifndef SPIFFS_FILEHDL_OFFSET
#define SPIFFS_FILEHDL_OFFSET           1
// use this offset
#define TEST_SPIFFS_FILEHDL_OFFSET      0x1000
#endif


#define SPIFFS_LOCK(fs)
#define SPIFFS_UNLOCK(fs)


// dbg output
#define SPIFFS_DBG(_f, ...) //printf("\x1b[32m" _f "\x1b[0m", ## __VA_ARGS__)
#define SPIFFS_API_DBG(_f, ...) //printf("\n\x1b[1m\x1b[7m" _f "\x1b[0m", ## __VA_ARGS__)
#define SPIFFS_GC_DBG(_f, ...) //printf("\x1b[36m" _f "\x1b[0m", ## __VA_ARGS__)
#define SPIFFS_CACHE_DBG(_f, ...) //printf("\x1b[33m" _f "\x1b[0m", ## __VA_ARGS__)
#define SPIFFS_CHECK_DBG(_f, ...) //printf("\x1b[31m" _f "\x1b[0m", ## __VA_ARGS__)

// needed types
typedef signed int s32_t;
typedef unsigned int u32_t;
typedef signed short s16_t;
typedef unsigned short u16_t;
typedef signed char s8_t;
typedef unsigned char u8_t;

#endif /* PARAMS_TEST_H_ */
