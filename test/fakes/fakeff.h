#ifndef FAKE_FATFS_H_INCLUDED
#define FAKE_FATFS_H_INCLUDED

#include "ff.h"
#include <stdint.h>

// From ff.h
void fakeff_reset(void);

FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt);			/* Mount/Unmount a logical drive */
FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);				/* Open or create a file */
FRESULT f_close (FIL* fp);											/* Close an open file object */
FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);			/* Read data from the file */
FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	/* Write data to the file */
FRESULT f_sync (FIL* fp);											/* Flush cached data of the writing file */


#endif