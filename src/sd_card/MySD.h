#ifndef MY_SD_H_INCLUDED
#define MY_SD_H_INCLUDED

#include "fatfs.h"
#include <stdint.h>

FRESULT MySD_Init(const char* filename);
void MySD_Close(void);
FRESULT MySD_WriteString(const char* buf);
FRESULT MySD_Write(const uint8_t* buf, uint32_t len);
FRESULT MySD_Flush(void);
FSIZE_t MySD_getOpenedFileSize(void);

#endif
