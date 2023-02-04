#include "unity.h"
#include "fake_myTimeString.h"

void setUp(void)
{
    fake_myTimeString_reset();
}

void test_moduleCanBeReset(void)
{
    fake_myTimeString_setFileName("abc");
    fake_myTimeString_setTimestamp("[1]");

    fake_myTimeString_reset();

    MyTimeString_Init();

    TEST_ASSERT_EQUAL_STRING("", MyTimeString_GetFileName());
    TEST_ASSERT_EQUAL_STRING("", MyTimeString_GetTimeStamp());
}

void test_resetModuleLosesInit(void)
{
    // initialise and set file name
    MyTimeString_Init();
    fake_myTimeString_setFileName("abc");
    TEST_ASSERT_EQUAL_STRING("abc", MyTimeString_GetFileName());

    // reset
    fake_myTimeString_reset();

    // try setting file name again, expect it to act as if not initted
    fake_myTimeString_setFileName("abc");

    TEST_ASSERT_EQUAL_STRING(NOT_INIT_STRING, MyTimeString_GetFileName());
}

void test_moduleDoesNothingIfNotInitted(void)
{
    // no init - // MyTimeString_Init();

    const char fakeName[] = "abc";
    const char fakeStamp[] = "[1]";

    fake_myTimeString_setFileName(fakeName);
    fake_myTimeString_setTimestamp(fakeStamp);

    TEST_ASSERT_EQUAL_STRING(NOT_INIT_STRING, MyTimeString_GetFileName());
    TEST_ASSERT_EQUAL_STRING(NOT_INIT_STRING, MyTimeString_GetTimeStamp());
    
    // Init after, should return the data it would have been set to
    MyTimeString_Init();

    TEST_ASSERT_EQUAL_STRING(fakeName, MyTimeString_GetFileName());
    TEST_ASSERT_EQUAL_STRING(fakeStamp, MyTimeString_GetTimeStamp());
}

void test_FileNameCanBeSet(void)
{
    MyTimeString_Init();

    const char* fileName = "fname";
    const char* fileName2 = "fname2";
    fake_myTimeString_setFileName(fileName);

    TEST_ASSERT_EQUAL_STRING(fileName, MyTimeString_GetFileName());
}

void test_TimeStampCanBeSet(void)
{
    MyTimeString_Init();

    const char* timestamp = "[01]";
    fake_myTimeString_setTimestamp(timestamp);

    TEST_ASSERT_EQUAL_STRING(timestamp, MyTimeString_GetTimeStamp());
}

/*

- [x] Can set time string
- [x] No functions work unless it's initialised
- [x] Can reset state of module
- [x] Can set file name string
- [x] Can set time stamp string

*/