#include "unity.h"
#include "MyApp.h"
#include "mock_MyCLI.h"
#include "mock_controller.h"
#include "mock_AppDecider.h"
#include "mock_SerialSnooper.h"
#include "Loop.h"

void test_App_CLI(void)
{
    LOOP_COUNT(3);  // expecting 3 times round the loop

    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_CLI);

    MyCLI_Init_Expect();
    // expecting 3 calls
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();

    runApp();
}

void test_App_Snooping(void)
{
    LOOP_COUNT(3);  // expecting 3 times round the loop

    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_SNOOPING);

    // expecting 3 calls
    SerialSnooper_Run_Expect();
    SerialSnooper_Run_Expect();
    SerialSnooper_Run_Expect();

    runApp();
}