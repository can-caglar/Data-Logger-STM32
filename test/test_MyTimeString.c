#include "unity.h"
#include "MyTimeString.h"
#include "mock_MyRTC.h"

static void expect_rtc_readtime(MyTime time);

void test_init(void)
{
    MyRTC_Init_ExpectAndReturn(0);
    MyTimeString_Init();
}

void test_get_time_stamp_string_representation(void)
{
    expect_rtc_readtime((MyTime){22, 11, 5, 18, 37, 12});
    TEST_ASSERT_EQUAL_STRING("[2022-11-05 18:37:12] ",
        MyTimeString_GetTimeStamp()
    );

    // with 1 digit, will pad
    expect_rtc_readtime((MyTime){2, 1, 9, 1, 7, 1});
    TEST_ASSERT_EQUAL_STRING("[2002-01-09 01:07:01] ",
        MyTimeString_GetTimeStamp()
    );
}

// Helpers

static void expect_rtc_readtime(MyTime time)
{
    MyRTC_ReadTime_ExpectAndReturn(time);
}