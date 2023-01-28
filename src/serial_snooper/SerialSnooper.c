#include "SerialSnooper.h"
#include "MyCircularBuffer.h"
#include "SystemOperations.h"

static SubjectData_t mySubjectData;

int SerialSnooper_Init(void)
{
    return SystemOperations_Init();
}

// Only call after a successful Init()
void SerialSnooper_Run()
{
    // Get data
    mySubjectData.isEmpty = MyCircularBuffer_isEmpty();
    
    // Notify Observers
    notifySdCardWriter(&mySubjectData);

    notifySdCardFlusher(&mySubjectData);
}
