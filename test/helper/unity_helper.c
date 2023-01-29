#include "unity.h"
#include "unity_helper.h"

void AssertEqualMyGPIO(MyGPIO expected, MyGPIO actual, unsigned short line)
{
    UNITY_TEST_ASSERT_EQUAL_PTR(expected.gpio_register, 
        actual.gpio_register,
        line,
        "MyGPIO .gpio_register failed");
    
    UNITY_TEST_ASSERT_EQUAL_HEX32(expected.pin_mask, 
        actual.pin_mask,
        line,
        "MyGPIO .pin_mask failed");
    
    UNITY_TEST_ASSERT_EQUAL_INT(expected.mode, 
        actual.mode,
        line,
        "MyGPIO .mode failed");

    UNITY_TEST_ASSERT_EQUAL_INT(expected.alt_func, 
        actual.alt_func,
        line,
        "MyGPIO .alt_func failed");

    UNITY_TEST_ASSERT_EQUAL_INT(expected.output_type, 
        actual.output_type,
        line,
        "MyGPIO .output_type failed");

    UNITY_TEST_ASSERT_EQUAL_INT(expected.pupd, 
        actual.pupd,
        line,
        "MyGPIO .pupd failed");
}
