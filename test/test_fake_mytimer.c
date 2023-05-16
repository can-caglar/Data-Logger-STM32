#include "unity.h"
#include "fake_MyTimer.h"

static uint8_t timerCbCalled;
static void timerCb(uint32_t);

void setUp(void)
{
    fake_mytimer_reset();
    timerCbCalled = 0;
}

void test_init(void)
{
    // given
    MyTimer_init(timerCb);
    // when
    fake_mytimer_pretendToReceiveString("abc", 100);
    // then
    TEST_ASSERT_TRUE(timerCbCalled);
}

void test_if_not_init_wont_crash(void)
{
    // given
    // when
    fake_mytimer_pretendToReceiveString("abc", 100);
}


/*

- [ ] takes in callback
- [ ] callback will be called based on config in header

*/

void timerCb(uint32_t arg)
{
    timerCbCalled = 1;
}