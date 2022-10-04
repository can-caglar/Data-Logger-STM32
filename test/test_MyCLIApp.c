#include "unity.h"
#include "MyApp.h"
#include "mock_MyCLI.h"
#include "mock_controller.h"
#include "Loop.h"

void test_App(void)
{
    LOOP_COUNT(3);

    MyCLI_Init_Expect();
    run_controller_Expect();
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();

    runCLI();
}