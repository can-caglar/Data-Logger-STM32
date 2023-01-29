#include "unity.h"
#include "DataHolder.h"

#include "MyCircularBuffer.h"
#include "mock_stm32f0xx_hal.h"
#include "mock_MyTimeString.h"
#include "mock_MySD.h"

void successfulInit(void);

void setUp(void)
{
    successfulInit();
}

void test_initShallRefreshCircBuffer(void)
{
    MyCircularBuffer_write('a');

    successfulInit();

    HAL_GetTick_IgnoreAndReturn(0);
    MyTimeString_GetTimeStamp_IgnoreAndReturn(0);
    MyTimeString_GetFileName_IgnoreAndReturn(0);
    MySD_getOpenedFileSize_IgnoreAndReturn(0);

    DataContext* data = DH_RefreshData();
    
    TEST_ASSERT_EQUAL_CHAR(0, DH_GetLatestData(data));
}

void test_badInit(void)
{
    MyTimeString_Init_ExpectAndReturn(1);
    TEST_ASSERT_EQUAL_INT(1,DH_Init());
}

void test_DH_RefreshData(void)
{
    const uint32_t timetoReturn = 77;
    const char* timeStamp = "[00:10:12]";
    const char* filename = "fileName.txt";

    // Set up data
    HAL_GetTick_ExpectAndReturn(timetoReturn);
    MyTimeString_GetTimeStamp_ExpectAndReturn(timeStamp);
    MyTimeString_GetFileName_ExpectAndReturn(filename);
    MySD_getOpenedFileSize_ExpectAndReturn(100);

    // Test function
    DataContext* data = DH_RefreshData();

    // See expectations
    TEST_ASSERT_EQUAL_CHAR(0, DH_GetLatestData(data));
    TEST_ASSERT_EQUAL_CHAR(0, DH_IsThereNewData(data));
    TEST_ASSERT_EQUAL_UINT32(timetoReturn, DH_GetTime(data));
    TEST_ASSERT_EQUAL_STRING(timeStamp, DH_GetTimestampString(data));
    TEST_ASSERT_EQUAL_STRING(filename, DH_GetFileName(data));
    TEST_ASSERT_EQUAL_INT(100, DH_GetOpenedFileSize(data));
}

void test_DH_CircBuffer(void)
{
    HAL_GetTick_IgnoreAndReturn(0);
    MyTimeString_GetTimeStamp_IgnoreAndReturn(0);
    MyTimeString_GetFileName_IgnoreAndReturn(0);
    MySD_getOpenedFileSize_IgnoreAndReturn(0);

    MyCircularBuffer_write('a');
    MyCircularBuffer_write('b');

    DataContext* data = DH_RefreshData();

    TEST_ASSERT_EQUAL_CHAR('a', DH_GetLatestData(data));
   // TEST_ASSERT_EQUAL_INT(1, DH_IsThereNewData(data));
}

void test_DH_NullData(void)
{
    DataContext* data = NULL;

    TEST_ASSERT_EQUAL_CHAR(0, DH_GetLatestData(data));
    TEST_ASSERT_EQUAL_UINT32(0, DH_GetTime(data));
    TEST_ASSERT_EQUAL_STRING(0, DH_GetTimestampString(data));
    TEST_ASSERT_EQUAL_STRING(0, DH_GetFileName(data));
}


/* Helpers */

void successfulInit(void)
{
    MyTimeString_Init_ExpectAndReturn(0);
    DH_Init();
}