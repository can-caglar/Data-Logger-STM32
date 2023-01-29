#include "fake_DataHolder.h"
#include <string.h>

static uint64_t fakeTime = 0;
static int isThereNewData = 0;
static uint8_t latestData = 0;

#define MAX_FILE_NAME 20    // arb
static char filename[MAX_FILE_NAME] = { 0 };
static char timestampString[MAX_FILE_NAME] = { 0 };

void fakeSetTime(uint64_t newTime)
{
    fakeTime = newTime;
}

uint64_t DH_GetTime(const DataContext* data)
{
    return fakeTime;
}

int DH_IsThereNewData(const DataContext* data)
{
    return isThereNewData;
}

void fakeSetIsThereNewData(int newState)
{
    isThereNewData = newState;
}

uint8_t DH_GetLatestData(const DataContext* data)
{
    return latestData;
}

void fakeSetLatestData(uint8_t newData)
{
    latestData = newData;
}

const char* DH_GetFileName(const DataContext* data)
{
    return filename;
}

void fakeSetFileName(const char* newFileName)
{
    strncpy(filename, newFileName, MAX_FILE_NAME);
}

const char* DH_GetTimestampString(const DataContext* data)
{
    return timestampString;
}

void fakeSetTimestampString(const char* newFileName)
{
    strncpy(timestampString, newFileName, MAX_FILE_NAME);
}