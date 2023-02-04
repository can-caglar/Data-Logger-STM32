/*
The scheduling module.
Call this in a tight loop.
*/

#include "SerialSnooper.h"
#include "DataHolder.h"
#include "stm32f0xx_hal.h"
#include <string.h>

typedef struct SSTask_t
{
    FnTask fnPtr;
    bool enabled;
    bool isPeriodic;
    uint32_t period;
    uint32_t nextCall;
} SSTask_t;

// Task database
static SSTask_t ssTasks[MAX_TASKS];
static uint8_t taskCounter = 0;

// Internal state variable
static uint32_t schedulerTime = 0;

// Initialises internal variables to 0
void SerialSnooper_Init(void)
{
    taskCounter = 0;
    schedulerTime = 0;
    memset(ssTasks, 0, sizeof(ssTasks));
}

// Run the scheduler
// Calls tasks when it is their time
void SerialSnooper_Run(void)
{
    schedulerTime = HAL_GetTick();

    // Do the scheduling
    for (uint8_t i = 0; i < taskCounter; i++)
    {
        SSTask_t* thisTask = &ssTasks[i];   // for easy access
        if (thisTask->enabled)
        {
            // task is enabled...
            if (schedulerTime >= thisTask->nextCall)
            {
                // it's time to call it, so call it...
                thisTask->fnPtr();
                thisTask->nextCall = schedulerTime + thisTask->period;
                // disable if it was a one-shot
                if (thisTask->isPeriodic == false)
                {
                    thisTask->enabled = false;
                }
            }
        }
    }
}

// Add task to scheduler
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
        ssTasks[taskCounter].isPeriodic = periodic;
        taskCounter++;
    }
    else
    {
        err = SS_ERR_FAULT;
    }
    return err;
}
