#ifndef MY_SD_H_INCLUDED
#define MY_SD_H_INCLUDED

#include "fatfs.h"

FRESULT MySD_Init(const char* filename);
void MySD_Close(void);
FRESULT MySD_WriteString(char* buf);
FRESULT MySD_Write(unsigned char* buf, uint32_t len);

#endif
