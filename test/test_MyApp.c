#include "unity.h"
#include "MyApp.h"
#include "mock_MyCLI.h"
#include "mock_AppDecider.h"
#include "mock_MyScheduler.h"
#include "mock_main.h"
#include "Loop.h"

// Helpers
void expectMySchedulerInitHelper(void);

void test_App_CLI(void)
{
    LOOP_COUNT(3);  // expecting 3 times round the loop

    CubeMX_SystemInit_Expect(CMX_FATFS);
    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_CLI);

    MyCLI_Init_Expect();
    // expecting 3 calls
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();

    runApp();
}

void test_App_SnoopingSuccess(void)
{
    const int loopCount = 3;
    LOOP_COUNT(loopCount);  // expecting 3 times round the loop

    expectMySchedulerInitHelper();

    // expecting this to be looped
    for (int i = 0; i < loopCount; i++)
    {
        MyScheduler_Run_Expect();
    }

    runApp();
}

/*********** Helpers ***********/

void expectMySchedulerInitHelper(void)
{
    CubeMX_SystemInit_Expect(CMX_FATFS);
    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_SNOOPING);
    CubeMX_SystemInit_Expect(CMX_UART);

    MyScheduler_Init_Expect();
}