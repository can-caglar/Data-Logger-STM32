#ifndef _UNITY_HELPER
#define _UNITY_HELPER

#include "MyGPIO.h"

void AssertEqualMyGPIO(MyGPIO expected, MyGPIO actual, unsigned short line);

#define UNITY_TEST_ASSERT_EQUAL_MyGPIO(expected, actual, line, message) \
    AssertEqualMyGPIO(expected, actual, line)

#define TEST_ASSERT_EQUAL_MyGPIO(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_MyGPIO(expected, actual, __LINE__, NULL)

#endif
