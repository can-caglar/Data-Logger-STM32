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
static int latestDataContextPassedIn = 0;

static uint32_t fakeFileSize = 0;

void fakeSetTime(uint32_t newTime)
{
    fakeTime = newTime;
}

uint32_t DH_GetTime(const DataContext* data)
{
    latestDataContextPassedIn = (int)data;
    return fakeTime;
}

uint8_t DH_IsThereNewData(const DataContext* data)
{
    latestDataContextPassedIn = (int)data;
    return isThereNewData;
}

void fakeSetIsThereNewData(int newState)
{
    isThereNewData = newState;
}

uint8_t DH_GetLatestData(const DataContext* data)
{
    latestDataContextPassedIn = (int)data;
    return latestData;
}

void fakeSetLatestData(uint8_t newData)
{
    latestData = newData;
}

const char* DH_GetFileName(const DataContext* data)
{
    latestDataContextPassedIn = (int)data;
    return filename;
}

void fakeSetFileName(const char* newFileName)
{
    strncpy(filename, newFileName, MAX_FILE_NAME);
}

const char* DH_GetTimestampString(const DataContext* data)
{
    latestDataContextPassedIn = (int)data;
    return timestampString;
}

void fakeSetTimestampString(const char* newFileName)
{
    strncpy(timestampString, newFileName, MAX_FILE_NAME);
}

void fakeSetDataContext(DataContext* newData)
{
    fakeDataContext = (int)newData;
}

DataContext* DH_RefreshData(void)
{
    return (DataContext*)fakeDataContext;
}

DataContext* fakeGetLatestDataContextPassedIn(void)
{
    int ret = latestDataContextPassedIn;
    latestDataContextPassedIn = 0;
    return (DataContext*)ret;
}

void fakeSetFileSize(uint32_t newFileSize)
{
    fakeFileSize = newFileSize;
}

uint32_t DH_GetOpenedFileSize(const DataContext* data)
{
    return fakeFileSize;
}