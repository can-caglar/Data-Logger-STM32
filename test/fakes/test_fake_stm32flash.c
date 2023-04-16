#include "fake_stm32flash.h"
#include "unity.h"

void setUp(void)
{
    fake_stm32flash_reset();
}

void test_write_read(void)
{
    // given
    write_flash_string(FLASH_DATA_PAGE_0, "hi");
    // when
    char readbuf[20];
    read_flash_string(FLASH_DATA_PAGE_0, readbuf, 3);
    // then
    TEST_ASSERT_EQUAL_STRING("hi", readbuf);
}

void test_read_without_write(void)
{
    // given
    // no read
    // when
    char readbuf[20];
    read_flash_string(FLASH_DATA_PAGE_0, readbuf, 3);
    // then
    // no crash
}
