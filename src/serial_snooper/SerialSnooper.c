/*
The scheduling module.
Call this in a tight loop.
*/

#include "SerialSnooper.h"
#include "DataHolder.h"
#include <string.h>

#define MAX_TASKS 5

typedef struct SSTask_t
{
    FnTask fnPtr;
    bool enabled;
    uint32_t period;
} SSTask_t;

// Task database
static SSTask_t ssTasks[MAX_TASKS];
static uint8_t taskCounter = 0;

// Grab data
static FnGetData getDataFromDataHolder;

void SerialSnooper_Init(FnGetData fnGetData)
{
    taskCounter = 0;
    memset(ssTasks, 0, sizeof(ssTasks));
    getDataFromDataHolder = fnGetData;
}

void SerialSnooper_Run(void)
{
    DataContext* data = NULL;
    uint64_t timeFromContext = DH_GetTime(data);

    if (getDataFromDataHolder)
    {
        data = getDataFromDataHolder();
    }
    for (uint8_t i = 0; i < taskCounter; i++)
    {
        SSTask_t* thisTask = &ssTasks[i];
        if (thisTask->enabled)
        {
            if (timeFromContext == thisTask->period)
            {
                thisTask->fnPtr(data);
            }
        }
    }
}


int SerialSnooper_AddTask(FnTask func, 
    uint32_t period, bool periodic, bool enabled)
{
    ssTasks[taskCounter].fnPtr = func;
    ssTasks[taskCounter].enabled = enabled;
    ssTasks[taskCounter].period = period;
    taskCounter++;
}