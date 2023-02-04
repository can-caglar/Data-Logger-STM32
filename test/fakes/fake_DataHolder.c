#include "unity.h"
#include "fake_DataHolder.h"
#include <string.h>

static uint32_t fakeTime = 0;
static int isThereNewData = 0;
static uint8_t latestData = 0;

#define MAX_FILE_NAME 20    // arb
static char filename[MAX_FILE_NAME] = { 0 };
static char timestampString[MAX_FILE_NAME] = { 0 };

static int fakeDataContext = 0;

static uint32_t fakeFileSize = 0;

uint8_t DH_IsThereNewData(void)
{
    return isThereNewData;
}

void fakeSetIsThereNewData(int newState)
{
    isThereNewData = newState;
}

uint8_t DH_GetLatestData(void)
{
    return latestData;
}

void fakeSetLatestData(uint8_t newData)
{
    latestData = newData;
}

const char* DH_GetFileName(void)
{
    return filename;
}

void fakeSetFileName(const char* newFileName)
{
    strncpy(filename, newFileName, MAX_FILE_NAME);
}

const char* DH_GetTimestampString(void)
{
    return timestampString;
}

void fakeSetTimestampString(const char* newFileName)
{
    strncpy(timestampString, newFileName, MAX_FILE_NAME);
}

void fakeSetFileSize(uint32_t newFileSize)
{
    fakeFileSize = newFileSize;
}

uint32_t DH_GetOpenedFileSize(void)
{
    return fakeFileSize;
}