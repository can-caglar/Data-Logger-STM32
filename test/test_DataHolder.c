#include "unity.h"
#include "DataHolder.h"

#include "MyCircularBuffer.h"
#include "mock_stm32f0xx_hal.h"
#include "mock_MyTimeString.h"
#include "mock_MySD.h"

void resetCircularBuffer(void);

void test_isThereNewDataDependsOnCircularBuffer(void)
{
    resetCircularBuffer();

    // No scenario
    TEST_ASSERT_EQUAL_INT(0, DH_IsThereNewData());

    // Yes scenario
    MyCircularBuffer_write('a');

    TEST_ASSERT_EQUAL_INT(1, DH_IsThereNewData());
}

void test_getLatestDataReturnsAndPopsTheOldestDataInCircBuf(void)
{
    resetCircularBuffer();

    MyCircularBuffer_write('a');
    MyCircularBuffer_write('b');
    MyCircularBuffer_write('c');

    TEST_ASSERT_EQUAL_CHAR('a', DH_GetLatestData());
    TEST_ASSERT_EQUAL_CHAR('b', DH_GetLatestData());
    TEST_ASSERT_EQUAL_CHAR('c', DH_GetLatestData());
}

void test_getFilenameDelegatesToMyTimeString(void)
{
    const char* fileName = "filename.txt";

    MyTimeString_GetFileName_ExpectAndReturn(fileName);

    TEST_ASSERT_EQUAL_STRING(fileName, DH_GetFileName());
}

void test_getFileStampStringDelegatesToMyTimeString(void)
{
    const char* fileName = "[10-10-10]";

    MyTimeString_GetTimeStamp_ExpectAndReturn(fileName);

    TEST_ASSERT_EQUAL_STRING(fileName, DH_GetTimestampString());
}

void test_getOpenedFileSizeGetsItFromMySD(void)
{
    uint32_t fakeFileSize = 1024;

    MySD_getOpenedFileSize_ExpectAndReturn(fakeFileSize);

    TEST_ASSERT_EQUAL_UINT32(fakeFileSize, DH_GetOpenedFileSize());
}


// Helpers

void resetCircularBuffer(void)
{
    MyCircularBuffer_close(); // close incase one was open
    MyCircularBuffer_init();  // inits an empty circ buffer
}