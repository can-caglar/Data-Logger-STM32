#include "unity.h"
#include "MyTimeString.h"
#include "mock_MyRTC.h"

static void expect_rtc_readtime(MyTime time);

void test_init(void)
{
    MyRTC_Init_Expect();
    MyTimeString_Init();
}

void test_get_time_stamp_string_representation(void)
{
    expect_rtc_readtime((MyTime){22, 11, 5, 18, 37, 12});
    TEST_ASSERT_EQUAL_STRING("[2022-11-05 18:37:12] ",
        MyTimeString_GetTimeStamp()
    );

    // with 1 digit, will pad 0s to make them 2 digits
    expect_rtc_readtime((MyTime){2, 1, 9, 1, 7, 1});
    TEST_ASSERT_EQUAL_STRING("[2002-01-09 01:07:01] ",
        MyTimeString_GetTimeStamp()
    );

    // with bad input, will return 0s
    expect_rtc_readtime((MyTime){100, 100, 100, 100, 100, 100});
    TEST_ASSERT_EQUAL_STRING("[2000-00-00 00:00:00] ",
        MyTimeString_GetTimeStamp()
    );
}

void test_get_file_name_uses_unix_timestamp(void)
{
    // max is 12 characters (including .txt)
    // 2027 March 12th, at 18:37:12
    expect_rtc_readtime((MyTime){27, 3, 12, 18, 37, 12});
    TEST_ASSERT_EQUAL_STRING("6B943B58.txt",
        MyTimeString_GetFileName()
    );

    // 2017 February 12th at 15:32:10
    expect_rtc_readtime((MyTime){17, 2, 12, 15, 32, 10});
    TEST_ASSERT_EQUAL_STRING("58A07FFA.txt",
        MyTimeString_GetFileName()
    );

    // 2099 February 12th at 15:32:10
    expect_rtc_readtime((MyTime){99, 2, 12, 15, 32, 10});
    TEST_ASSERT_EQUAL_STRING("F2DD5CFA.txt",
        MyTimeString_GetFileName()
    );
}

// Helpers

static void expect_rtc_readtime(MyTime time)
{
    MyRTC_ReadTime_ExpectAndReturn(time);
}