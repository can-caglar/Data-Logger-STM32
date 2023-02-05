/*
Fake SD card module.
Able to write to and read from SD card.
Won't write unless flushed.
Init will open a file and close previous one if
one was open.
*/
#include "fake_SDCard.h"
#include "fatfs.h"
#include <stdint.h>
#include <string.h>

static int numFilesOpen;
static int totalNumFilesOpened;
static char openFileName[20];
static char sdCardActualData[FAKE_MAX_FILE_SIZE];
static char sdCardCachedData[FAKE_MAX_FILE_SIZE];
static FSIZE_t openFileSize = 0;
static uint32_t filePointer = 0;

int fake_SDCard_reset(void)
{
    numFilesOpen = 0;
    memset(openFileName, 0, sizeof(openFileName));
    memset(sdCardActualData, 0, sizeof(sdCardActualData));
    memset(sdCardCachedData, 0, sizeof(sdCardCachedData));
    filePointer = 0;
    totalNumFilesOpened = 0;
}

int fake_SDCard_numFilesOpen(void)
{
    return numFilesOpen;
}

const char* fake_SDCard_getOpenFileName(void)
{
    return openFileName;
}

const char* fake_SDCard_getFileData(void)
{
    return sdCardActualData;
}

int fake_SDCard_isFileEmpty(void)
{
    return filePointer == 0;
}

int fake_SDCard_totalNumOfFilesOpened(void)
{
    return totalNumFilesOpened;
}

FRESULT MySD_Init(const char* filename)
{
    numFilesOpen = 1;
    strcpy(openFileName, filename);
    totalNumFilesOpened++;
    filePointer = 0;
    return FR_OK;
}

void MySD_Close(void)
{
    numFilesOpen = 0;
    memset(openFileName, 0, sizeof(openFileName));
}

FRESULT MySD_WriteString(const char* buf)
{
    MySD_Write(buf, strlen(buf));
    return FR_OK;
}

FRESULT MySD_Write(const uint8_t* buf, uint32_t len)
{
    if (numFilesOpen == 1)
    {
        memcpy(sdCardCachedData + filePointer, buf, len);
        filePointer += len;
    }
    return FR_OK;
}

FRESULT MySD_Flush(void)
{
    memcpy(sdCardActualData, sdCardCachedData, FAKE_MAX_FILE_SIZE);
    return FR_OK;
}

FSIZE_t MySD_getOpenedFileSize(void)
{
    return filePointer;
}
