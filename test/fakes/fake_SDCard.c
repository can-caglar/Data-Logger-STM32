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
#include <stdio.h>

static const uint8_t debugEnabled = 0;
#define MY_LOG(x) if (debugEnabled) printf(x)

static int numFilesOpen;
static int totalNumFilesOpened;
static char openFileName[20];
static char sdCardActualData[FAKE_MAX_FILE_SIZE];
static char sdCardCachedData[FAKE_MAX_FILE_SIZE];
static FSIZE_t openFileSize = 0;
static uint32_t filePointer = 0;    // non flushed
static uint32_t actualFilePointer = 0;    // actual
static FRESULT toReturn = FR_OK;

int fake_SDCard_reset(void)
{
    numFilesOpen = 0;
    memset(openFileName, 0, sizeof(openFileName));
    memset(sdCardActualData, 0, sizeof(sdCardActualData));
    memset(sdCardCachedData, 0, sizeof(sdCardCachedData));
    filePointer = 0;
    actualFilePointer = 0;
    totalNumFilesOpened = 0;
    toReturn = FR_OK;
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
    MY_LOG("FakeSDCard MySD_Init called\n");
    if (toReturn == FR_OK)
    {
        numFilesOpen = 1;
        strcpy(openFileName, filename);
        totalNumFilesOpened++;
        filePointer = 0;
        actualFilePointer = 0;
    }
    return toReturn;
}

void MySD_Close(void)
{
    numFilesOpen = 0;
    memset(openFileName, 0, sizeof(openFileName));
}

FRESULT MySD_WriteString(const char* buf)
{
    MY_LOG("FakeSDCard MySD_WriteString called\n");
    if (toReturn == FR_OK)
    {
        MySD_Write(buf, strlen(buf));
    }
    return toReturn;
}

FRESULT MySD_Write(const uint8_t* buf, uint32_t len)
{
    MY_LOG("FakeSDCard MySD_Write called\n");
    if (toReturn == FR_OK)
    {
        if (numFilesOpen == 1)
        {
            // copies data to global that is pretending
            // to be the sd card storage!
            memcpy(sdCardCachedData + filePointer, buf, len);
            filePointer += len;
        }
    }
    return toReturn;
}

FRESULT MySD_Flush(void)
{
    MY_LOG("FakeSDCard MySD_Flush called\n");
    if (toReturn == FR_OK)
    {
        memcpy(sdCardActualData, sdCardCachedData, FAKE_MAX_FILE_SIZE);
        actualFilePointer = filePointer;
    }
    return toReturn;
}

// Note: known bug, file size will incrementy
// even though it's not flushed!
FSIZE_t MySD_getOpenedFileSize(void)
{
    return actualFilePointer;
}

void fake_SDCard_toReturn(FRESULT newResult)
{
    toReturn = newResult;
}

FRESULT MySD_Read(uint8_t* buf, uint32_t len)
{
    if (toReturn == FR_OK)
    {
        if (numFilesOpen == 1)
        {
            memcpy(buf, sdCardActualData, len);
        }
    }
    return toReturn;
}

// Writes to the fake file directly, bypassing the init, write, flush requirement
// Recommended to reset module before and after use!
void fake_SDCard_helperWriteFileData(const char* str, unsigned int len)
{
    //memset(sdCardActualData, 0, sizeof(sdCardActualData));
    strncpy(sdCardActualData, str, len);
}