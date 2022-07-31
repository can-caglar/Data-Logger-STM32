#include "unity.h"
#include "MyBitStuff.h"
#include "MyCommon.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testBitStuffBasics(void)
{
    io_register myReg = 0;

    SET_BIT(2, myReg);
    TEST_ASSERT_BIT_HIGH(2, myReg);

    CLR_BIT(2, myReg);
    TEST_ASSERT_BIT_LOW(2, myReg);
}

void testConfBitsClearsMultipleBits(void)
{
    io_register myReg = 0xFF;
    CONF_BITS(0x3, myReg, 0);
    TEST_ASSERT_BITS(0xFF, 0xFC, myReg);

    myReg = 0xFF;
    CONF_BITS((0x3 << 1), myReg, 0);
    TEST_ASSERT_BITS(0xFF, 0xF9, myReg);
}

void testConfBitsSetsMultipleBits(void)
{
    io_register myReg = 0;
    CONF_BITS(0x3, myReg, 0x2);
    TEST_ASSERT_BITS(0xFF, 0x2, myReg);

    myReg = 0;
    CONF_BITS((0x3 << 1), myReg, (0x2 << 1));
    TEST_ASSERT_BITS(0xFF, (0x2 << 1), myReg);
}

void testConfBitsChangesOnlyMaskedBitsWhenClearing(void)
{
    io_register myReg = 0xFFFF;
    CONF_BITS(0xFF, myReg, 0);
    TEST_ASSERT_BITS(0xFFFF, 0xFF00, myReg);
}

void testConfBitsChangesOnlyMaskedBitsWhenSetting(void)
{
    io_register myReg = 0;
    CONF_BITS(0x00FF, myReg, 0xFFFF);
    TEST_ASSERT_BITS(0xFFFF, 0x00FF, myReg);
}

/*

    CONF_BITS((0x3 << 2), myReg, (0x6 << 2));

    TEST_ASSERT_BITS(~0U, 0x2 << 2, myReg);

*/
