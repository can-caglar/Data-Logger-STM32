#include "fake_DataHolder.h"

static uint64_t fakeTime = 0;
static int isThereNewData = 0;
static uint8_t latestData = 0;

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
