#include "SerialSnooper.h"
#include "MyCircularBuffer.h"
#include "SystemOperations.h"

int SerialSnooper_Init(void)
{
    return SystemOperations_Init();
}

// Only call after a successful Init()
void SerialSnooper_Run()
{
    SubjectData_t mySubjectData;

    // Get data
    mySubjectData.isEmpty = MyCircularBuffer_isEmpty();
    
    // Notify Observers
    notifySdCardWriter(&mySubjectData);
    notifySdCardFlusher(&mySubjectData);
}
