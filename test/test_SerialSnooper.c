#include "unity.h"
#include "SerialSnooper.h"
#include "MyCircularBuffer.h"
#include "SystemOperations.h"
#include "MyTimeString.h"
#include "mock_MySD.h"
#include "mock_MyRTC.h"
#include "mock_stm32f0xx_hal.h"
#include "unity_helper.h"

#if 0

// Continue... reading circ buf here
// is a bug. but tests don't prove it
// improve this module's tests
// mock as litle as possible

void test_SerialSnooper_Init_DelegatesToSystemOperationsInit(void)
{
    MyRTC_Init_Expect();
    TEST_ASSERT_EQUAL_INT(SO_SUCCESS, SerialSnooper_Init());
}

void test_SerialSnooper_Run_PollsLatestData_passesItToObservers(void)
{
    
    // Call function
    SerialSnooper_Run();
}

#endif

/*

*/