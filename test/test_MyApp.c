#include "unity.h"
#include "MyApp.h"
#include "mock_MyCLI.h"
#include "mock_AppDecider.h"
#include "mock_main.h"
#include "Loop.h"

#include "MyScheduler.h"
#include "fake_stm32f0xx_hal.h"
#include "fake_SDCard.h"
#include "mock_MyTimeString.h"

// Helpers
void expectMySchedulerInitHelper(void);

/* Tests for the CLI App */

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


/* Tests for the Serial Snooping App */

void test_App_ShallOpenAFileAndDoNothingWithIt(void)
{
    LOOP_COUNT(1);  // expecting X times round the loop
    
    expectMySchedulerInitHelper();  // the init sequence

    fake_SDCard_reset();    // clean slate

    runApp();

    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getOpenFileName());
    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_numFilesOpen());
}

/*********** Helpers ***********/

void expectMySchedulerInitHelper(void)
{
    CubeMX_SystemInit_Expect(CMX_FATFS);
    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_SNOOPING);
    CubeMX_SystemInit_Expect(CMX_UART);
}

/*
TODO: 
    Perhaps begin from scratch.

    Write some tests, maybe then create the fake modules.

*/