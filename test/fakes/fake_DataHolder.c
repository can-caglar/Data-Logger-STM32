#include "fake_DataHolder.h"

static uint64_t fakeTime = 0;
static int isThereNewData = 0;

void fakeSetTime(uint64_t newTime)
{
    fakeTime = newTime;
}

uint64_t DH_GetTime(DataContext* data)
{
    return fakeTime;
}

int DH_IsThereNewData(DataContext* data)
{
    return isThereNewData;
}

void fakeSetIsThereNewData(int newState)
{
    isThereNewData = newState;
}
