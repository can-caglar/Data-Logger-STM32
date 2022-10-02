#ifndef _UNITY_HELPER
#define _UNITY_HELPER

#include "MyGPIO.h"
#include "gpio_hal.h"

void AssertEqualMyGPIO(MyGPIO expected, MyGPIO actual, unsigned short line);
void AssertEqualGH_Init_s(GH_Init_s expected, GH_Init_s actual, unsigned short line);

#define UNITY_TEST_ASSERT_EQUAL_MyGPIO(expected, actual, line, message) \
    AssertEqualMyGPIO(expected, actual, line)

#define TEST_ASSERT_EQUAL_MyGPIO(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_MyGPIO(expected, actual, __LINE__, NULL)

#define UNITY_TEST_ASSERT_EQUAL_GH_Init_s(expected, actual, line, message) AssertEqualGH_Init_s(expected, actual, line)


#endif
