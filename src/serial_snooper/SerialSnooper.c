/*
The scheduling module.
Call this in a tight loop.
*/

#include "SerialSnooper.h"
#include "DataHolder.h"
#include <string.h>

typedef struct SSTask_t
{
    FnTask fnPtr;
    bool enabled;
    uint32_t period;
    uint64_t nextCall;
} SSTask_t;

// Task database
static SSTask_t ssTasks[MAX_TASKS];
static uint8_t taskCounter = 0;

// Grab data
static FnGetData getDataFromDataHolder;

// Internal state variable
static uint64_t schedulerTime = 0;

void SerialSnooper_Init(FnGetData fnGetData)
{
    taskCounter = 0;
    schedulerTime = 0;
    memset(ssTasks, 0, sizeof(ssTasks));
    getDataFromDataHolder = fnGetData;
}

void SerialSnooper_Run(void)
{
    DataContext* data = NULL;
    schedulerTime = DH_GetTime(data);

    if (getDataFromDataHolder)
    {
        data = getDataFromDataHolder();
    }
    for (uint8_t i = 0; i < taskCounter; i++)
    {
        SSTask_t* thisTask = &ssTasks[i];
        if (thisTask->enabled)
        {
            if (schedulerTime >= thisTask->nextCall)
            {
                thisTask->fnPtr(data);
                thisTask->nextCall = schedulerTime + thisTask->period;
            }
        }
    }
}


int SerialSnooper_AddTask(FnTask func, 
    uint32_t period, bool periodic, bool enabled)
{
    int err = SS_ERR_NONE;
    if (taskCounter < MAX_TASKS)
    {
        ssTasks[taskCounter].fnPtr = func;
        ssTasks[taskCounter].enabled = enabled;
        ssTasks[taskCounter].period = period;
        ssTasks[taskCounter].nextCall = schedulerTime + period;
        taskCounter++;
    }
    else
    {
        err = SS_ERR_FAULT;
    }
    return err;
}
