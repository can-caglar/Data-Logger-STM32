/*
The scheduling module.
Call this in a tight loop.
*/

#include "SerialSnooper.h"
#include "MyCircularBuffer.h"
#include "SystemOperations.h"
#include "stm32f0xx_hal.h"

int SerialSnooper_Init(void)
{
    return SystemOperations_Init();
}

// Only call after a successful Init()
// Gets data and passes to observers
void SerialSnooper_Run()
{
    SubjectData_t mySubjectData = { 0 };

    // Get data
    mySubjectData.isEmpty = MyCircularBuffer_isEmpty();
    mySubjectData.circBufData = MyCircularBuffer_read();
    mySubjectData.tNow = HAL_GetTick();

    // Notify Observers
    notifySdCardWriter(&mySubjectData);
    notifySdCardFlusher(&mySubjectData);
}
