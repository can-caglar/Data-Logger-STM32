#include "unity.h"
#include "fake_stm32f0xx_hal.h"
#include "autobaudrate.h"
#include "fake_MyTimer.h"

void stubCallback(uint32_t);

void setUp(void)
{
    fake_mytimer_reset();
    autobaudrate_init();
}

void test_auto_baud_rate_br_is_undefined_initially(void)
{
    // given
    // when
    // then
    TEST_ASSERT_EQUAL_UINT32(UNDEFINED_BR, autobaudrate_getBr());
}

void test_baud_rate_is_not_calculated_until_min_data_received(void)
{
    // given
    // when
    fake_mytimer_pretendToReceiveString("UU", 19200);
    // then
    TEST_ASSERT_EQUAL_UINT32(UNDEFINED_BR, autobaudrate_getBr());
}

void test_baud_rate_is_calculated_when_min_num_data_received(void)
{
    // given
    autobaudrate_init();
    uint32_t expected = 19200;
    // when
    fake_mytimer_pretendToReceiveString("UUUUUUUUU", expected);
    // then
    TEST_ASSERT_UINT32_WITHIN(expected * 0.05, expected, autobaudrate_getBr());
}

void test_baud_rate_doesnt_change_once_found(void)
{
    // given
    uint32_t expected = 19200;
    uint32_t notExpected = 800;
    // when
    fake_mytimer_pretendToReceiveString("UUUUUUUUUUU", expected);
    fake_mytimer_pretendToReceiveString("UUUUUUUUUUU", notExpected);
    // then
    TEST_ASSERT_UINT32_WITHIN(expected * 0.05, expected, autobaudrate_getBr());
}

/*
- assume IRQ is set up and will call this function
- so all we have to do is call it repeatedly, with fake times
- the function shall calculate baud rate and update an internal variable
- getBR function here will return an invalid one until the correct
one
- the correct value is essentially the smallest of 50 readings
*/