#include "unity.h"
#include "fake_led.h"

void setUp(void)
{
    fakeLed_reset();
}

void test_ledIsOff(void)
{
    TEST_ASSERT_FALSE(fakeLed_isOn());
}

void test_ledRemainsOffIfNotInitted(void)
{
    led_on();
    led_toggle();
    TEST_ASSERT_FALSE(fakeLed_isOn());
}

void test_ledCanOnlyBeOnIfInitted(void)
{
    led_init();
    TEST_ASSERT_FALSE(fakeLed_isOn());

    led_on();
    TEST_ASSERT_TRUE(fakeLed_isOn());

    led_toggle();
    TEST_ASSERT_FALSE(fakeLed_isOn());

    led_toggle();
    TEST_ASSERT_TRUE(fakeLed_isOn());

    led_off();
    TEST_ASSERT_FALSE(fakeLed_isOn());
}