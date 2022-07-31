#include "unity.h"
#include "MyBitStuff.h"
#include "MyCommon.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testBitStuff(void)
{
    io_register myReg = 0;

    SET_BIT(2, myReg);
    TEST_ASSERT_BIT_HIGH(2, myReg);

    CLR_BIT(2, myReg);
    TEST_ASSERT_BIT_LOW(2, myReg);

    CONF_BITS((0x3 << 2), myReg, (0x6 << 2));

    TEST_ASSERT_BITS(0x3 << 2, 0x2 << 2, myReg);
}

