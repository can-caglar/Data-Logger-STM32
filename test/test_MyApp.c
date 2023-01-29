#include "unity.h"
#include "MyApp.h"
#include "mock_MyCLI.h"
#include "mock_AppDecider.h"
#include "mock_SerialSnooper.h"
#include "mock_main.h"
#include "mock_SystemOperations.h"
#include "mock_DataHolder.h"
#include "Loop.h"

// Helpers
void initSerialSnooperHelper(int error);

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
    LOOP_COUNT(3);  // expecting 3 times round the loop

    initSerialSnooperHelper(0);  // todo; magic number

    // expecting 3 calls
    SerialSnooper_Run_Expect();
    SerialSnooper_Run_Expect();
    SerialSnooper_Run_Expect();

    runApp();
}

/*********** Helpers ***********/

void initSerialSnooperHelper(int error)
{
    CubeMX_SystemInit_Expect(CMX_FATFS);
    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_SNOOPING);
    CubeMX_SystemInit_Expect(CMX_UART);

    SerialSnooper_Init_Expect();
    //SystemOperations_Init_ExpectAndReturn(0);
    //DH_Init_ExpectAndReturn(0);

    //SerialSnooper_AddTask_ExpectAndReturn();
}