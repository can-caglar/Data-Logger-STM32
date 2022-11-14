#include "unity.h"
#include "MyCircularBuffer.h"

#define MAX_SIZE (MyCircularBuffer_getCapacity())

uint8_t isPowerOfTwo(uint32_t num);

void setUp(void)
{
    MyCircularBuffer_init();
}

void tearDown(void)
{
    MyCircularBuffer_close();
}

void test_capacityIsPowerOfTwo(void)
{
    // circular buffer shall "waste" a slot to tell full apart from empty
    TEST_ASSERT_TRUE(isPowerOfTwo(MAX_SIZE + 1));
}

void test_startsEmptyAndNotFull(void)
{
    TEST_ASSERT_EQUAL(1, MyCircularBuffer_isEmpty());
    TEST_ASSERT_EQUAL(0, MyCircularBuffer_isFull());
}

void test_with1ItemNotEmpty(void)
{
    TEST_ASSERT_EQUAL(1, MyCircularBuffer_isEmpty());
    MyCircularBuffer_write(5);
    TEST_ASSERT_EQUAL(0, MyCircularBuffer_isEmpty());
}

void test_readsBackWhatWePutInOrder_AndBecomesEmpty(void)
{
    MyCircularBuffer_write(5);
    MyCircularBuffer_write(4);
    MyCircularBuffer_write(3);

    TEST_ASSERT_EQUAL(5, MyCircularBuffer_read());
    TEST_ASSERT_EQUAL(4, MyCircularBuffer_read());
    TEST_ASSERT_EQUAL(3, MyCircularBuffer_read());
    
    TEST_ASSERT_EQUAL(1, MyCircularBuffer_isEmpty());
}

void test_bufferIsFullWhenMaxItemsWritten(void)
{
    for (int j = 0; j < MAX_SIZE; j++)
    {
        MyCircularBuffer_write(j);
    }
    TEST_ASSERT_EQUAL(1, MyCircularBuffer_isFull());
}

void test_bufferCanTakeInMoreThanMaxAsLongAsEmptied(void)
{
    for (int j = 0; j < MAX_SIZE; j++)
    {
        MyCircularBuffer_write(j);
    }

    for (int j = 0; j < MAX_SIZE; j++)
    {
        TEST_ASSERT_EQUAL((uint8_t)j, MyCircularBuffer_read());
    }

    MyCircularBuffer_write(12);
    MyCircularBuffer_write(13);
    MyCircularBuffer_write(14);

    TEST_ASSERT_EQUAL(12, MyCircularBuffer_read());
    TEST_ASSERT_EQUAL(13, MyCircularBuffer_read());
    TEST_ASSERT_EQUAL(14, MyCircularBuffer_read());
}

void test_bufferReturns0_ifEmpty(void)
{
    MyCircularBuffer_write(1);
    TEST_ASSERT_EQUAL(1, MyCircularBuffer_read());
    TEST_ASSERT_EQUAL(0, MyCircularBuffer_read());
    TEST_ASSERT_EQUAL(0, MyCircularBuffer_read());
}

void test_bufferWritingWhenBufferFull(void)
{
    // fill up buffer
    for (int i = 0; i < MAX_SIZE; i++)
    {
        MyCircularBuffer_write((uint8_t)i);
    }
    // overflow buffer
    for (int i = 0; i < MAX_SIZE; i++)
    {
        MyCircularBuffer_write((uint8_t)(42 + i));
    }

    for (int i = 0; i < MAX_SIZE; i++)
    {
        TEST_ASSERT_EQUAL((uint8_t)i, MyCircularBuffer_read());
    }
}

void test_initialisingTwiceDoesNothing(void)
{
    MyCircularBuffer_write('A');
    MyCircularBuffer_init();
    
    TEST_ASSERT_EQUAL('A', MyCircularBuffer_read());
}

/******************** Helper functions and their tests *************/

uint8_t isPowerOfTwo(uint32_t num)
{
    return (num != 0) && ((num & (num - 1)) == 0);
}

void test_isPowerOfTwo(void)
{
    TEST_ASSERT_EQUAL(0, isPowerOfTwo(0));
    TEST_ASSERT_EQUAL(0, isPowerOfTwo(6));
    TEST_ASSERT_EQUAL(0, isPowerOfTwo(130));

    TEST_ASSERT_EQUAL(1, isPowerOfTwo(1));
    TEST_ASSERT_EQUAL(1, isPowerOfTwo(2));
    TEST_ASSERT_EQUAL(1, isPowerOfTwo(1024));
    TEST_ASSERT_EQUAL(1, isPowerOfTwo(131072));
}

/*

- [x] Circular buffer starts out as empty
- [x] Circular buffer with 1 item is not empty
- [x] Circular buffer goes empty again after reading 1 item
- [x] Circular buffer fills up 3 items will read 3 items 
    back in right order then be empty
- [x] Circular buffer with X items is full
- [x] Circular buffer continues to have right order even
    after filling up and reading beyond max
- [x] Circular buffer will return 0 if read when empty
- [x] Circular buffer will overwrite oldest data

*/