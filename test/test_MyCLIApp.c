#include "unity.h"
#include "MyCLIApp.h"
#include "mock_MySM.h"
#include "Loop.h"

void test_App(void)
{
    LOOP_COUNT(3);

    MySM_Init_Expect();
    MySM_Run_Expect();
    MySM_Run_Expect();
    MySM_Run_Expect();

    runCLI();
}