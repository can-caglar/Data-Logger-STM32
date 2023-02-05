#ifndef MY_FAKE_SD_CARD_H
#define MY_FAKE_SD_CARD_H

#include "MySD.h"

#define FAKE_MAX_FILE_SIZE 20000000

int fake_SDCard_reset(void);
int fake_SDCard_numFilesOpen(void);
const char* fake_SDCard_getOpenFileName(void);
const char* fake_SDCard_getFileData(void);
int fake_SDCard_isFileEmpty(void);
int fake_SDCard_totalNumOfFilesOpened(void);

#endif
