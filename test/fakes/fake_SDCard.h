#ifndef MY_FAKE_SD_CARD_H
#define MY_FAKE_SD_CARD_H

#include "MySD.h"

#define FAKE_MAX_FILE_SIZE 200

int fake_SDCard_reset(void);
int fake_SDCard_numFilesOpen(void);
const char* fake_SDCard_getOpenFileName(void);
const char* fake_SDCard_getFileData(void);
void fake_SDCard_setFileSize(FSIZE_t newFileSize);
int fake_SDCard_isFileEmpty(void);

#endif
