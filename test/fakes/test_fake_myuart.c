#include "unity.h"
#include "fake_myuart.h"

void setUp(void)
{
    fake_myuart_reset();
}

void test_noInitWillReturnBadBR(void)
{
    uint32_t invalidBr = 0;
    TEST_ASSERT_EQUAL_UINT32(invalidBr, fake_myuart_getBr());
}

void test_initWillUpdateBR(void)
{
    // given
    // when
    MyUART_Config config;
    config.baudrate = 19200;
    MyUART_init(&config);
    // then
    TEST_ASSERT_EQUAL_INT(19200, fake_myuart_getBr());
}

void test_initTwiceDoesNothing(void)
{
    // given
    MyUART_Config config;
    config.baudrate = 19200;
    MyUART_init(&config);
    // when
    config.baudrate = 200;
    MyUART_init(&config);
    // then
    TEST_ASSERT_EQUAL_INT(19200, fake_myuart_getBr());
}