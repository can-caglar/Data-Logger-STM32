#include "unity.h"
#include "AppDecider.h"
#include "mock_MyDipSwitch.h"

void test_init(void)
{
    MyDIP_Init_Expect();
    AppDecider_Init();
}

void test_read(void)
{
    MyDIP_Read_ExpectAndReturn(0);
    TEST_ASSERT_EQUAL(APP_CLI, AppDecider_Decide());

    for (int i = 1; i < 16; i++)
    {
        MyDIP_Read_ExpectAndReturn(i);
        TEST_ASSERT_EQUAL(APP_SNOOPING, AppDecider_Decide());
    }

}