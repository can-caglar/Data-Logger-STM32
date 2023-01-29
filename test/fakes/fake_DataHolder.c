#include "fake_DataHolder.h"

static uint64_t fakeTime = 0;

void fakeSetTime(uint64_t newTime)
{
    fakeTime = newTime;
}

uint64_t DH_GetTime(DataContext* data)
{
    return fakeTime;
}