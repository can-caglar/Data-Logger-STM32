#include "fake_SDCard.h"
#include "fatfs.h"
#include <stdint.h>
#include <string.h>

static int numFilesOpen;
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

void fake_SDCard_setFileSize(FSIZE_t newFileSize)
{
    openFileSize = newFileSize;
}

int fake_SDCard_isFileEmpty(void)
{
    return filePointer == 0;
}

FSIZE_t fake_SDCard_getFileSize(void)
{
    return openFileSize;   
}

FRESULT MySD_Init(const char* filename)
{
    numFilesOpen = 1;
    strcpy(openFileName, filename);
}

void MySD_Close(void)
{
    numFilesOpen = 0;
    memset(openFileName, 0, sizeof(openFileName));
}

FRESULT MySD_WriteString(const char* buf)
{
    MySD_Write(buf, strlen(buf));
}

FRESULT MySD_Write(const uint8_t* buf, uint32_t len)
{
    if (numFilesOpen == 1)
    {
        memcpy(sdCardCachedData + filePointer, buf, len);
        filePointer += len;
    }
}

FRESULT MySD_Flush(void)
{
    memcpy(sdCardActualData, sdCardCachedData, FAKE_MAX_FILE_SIZE);
}

FSIZE_t MySD_getOpenedFileSize(void)
{
    return openFileSize;
}
