#include "unity.h"
#include "SerialSnooper.h"
#include "MyCircularBuffer.h"
#include "mock_SystemOperations.h"
#include "mock_stm32f0xx_hal.h"
#include "unity_helper.h"

void test_SerialSnooper_Init_DelegatesToSystemOperationsInit(void)
{
    SystemOperations_Init_ExpectAndReturn(0);
    TEST_ASSERT_EQUAL_INT(0, SerialSnooper_Init());

    SystemOperations_Init_ExpectAndReturn(1);
    TEST_ASSERT_EQUAL_INT(1, SerialSnooper_Init());
}

void test_SerialSnooper_Run_PollsLatestData_passesItToObservers(void)
{
    // Expected data
    SubjectData_t expected = 
    { 
        .tNow = 30,
        .isEmpty = 0,
        .circBufData = 'B'
    };

    // Get Data (TODO, encapsulate data gathering)
    MyCircularBuffer_close();
    MyCircularBuffer_init();
    MyCircularBuffer_write(expected.circBufData);
    HAL_GetTick_ExpectAndReturn(expected.tNow);

    // Expectation for observers (and notifying observers)
    notifySdCardWriter_Expect(&expected);
    notifySdCardFlusher_Expect(&expected);

    // Call function
    SerialSnooper_Run();
}