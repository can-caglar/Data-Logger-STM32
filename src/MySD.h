#ifndef MY_SD_H_INCLUDED
#define MY_SD_H_INCLUDED

#include "fatfs.h"

FRESULT MySD_Init(const char* filename);
void MySD_Close(void);
FRESULT MySD_Write(char* buf);

#endif
