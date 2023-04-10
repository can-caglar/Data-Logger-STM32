#ifndef MY_SD_H_INCLUDED
#define MY_SD_H_INCLUDED

#include "fatfs.h"
#include <stdint.h>
#include <string.h>

FRESULT MySD_Init(const char* filename);
void MySD_Close(void);
FRESULT MySD_Write(const uint8_t* buf, uint32_t len);
#define MySD_WriteString(buf) MySD_Write((buf), strlen(buf))
FRESULT MySD_Flush(void);
FSIZE_t MySD_getOpenedFileSize(void);
FRESULT MySD_Read(uint8_t* buf, uint32_t len);

#endif
