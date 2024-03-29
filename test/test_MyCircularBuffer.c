#include "unity.h"
#include "MyCircularBuffer.h"

#define MAX_SIZE (MyCircularBuffer_getCapacity())

void fillCircularBuffer(uint32_t amountOfData, uint8_t filledItem);
void drainCircularBuffer(uint32_t amountOfDataToDrain);
void emptyEntireCircularBuffer(void);

void setUp(void)
{
    MyCircularBuffer_init();
}

void tearDown(void)
{
    MyCircularBuffer_close();
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
    uint8_t err = 0;
    // fill up buffer
    for (int i = 0; i < MAX_SIZE; i++)
    {
        err = MyCircularBuffer_write((uint8_t)i);
        TEST_ASSERT_EQUAL_UINT(0, err);
    }
    // overflow buffer
    for (int i = 0; i < MAX_SIZE; i++)
    {
        err = MyCircularBuffer_write((uint8_t)(42 + i));
        TEST_ASSERT_EQUAL_UINT(1, err);
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

void test_circularBufferPeek(void)
{
    MyCircularBuffer_write('A');
    MyCircularBuffer_write('B');

    TEST_ASSERT_EQUAL(0, MyCircularBuffer_isEmpty());
    TEST_ASSERT_EQUAL('A', MyCircularBuffer_read());
    TEST_ASSERT_EQUAL('B', MyCircularBuffer_peek());
    TEST_ASSERT_EQUAL('B', MyCircularBuffer_read());
    TEST_ASSERT_EQUAL(1, MyCircularBuffer_isEmpty());
}

void test_circularBufferSizeIncrementsBy1EachWriteWhenStartingFromEmptyBuffer(void)
{
    TEST_ASSERT_EQUAL(0, MyCircularBuffer_size());

    fillCircularBuffer(3, 0);

    TEST_ASSERT_EQUAL(3, MyCircularBuffer_size());
}

void test_circularBufferSizeIncrementsBy1EachWriteWhenStartingFromFullBuffer(void)
{
    // Fill up until full capacity
    uint32_t capacity = MyCircularBuffer_getCapacity();
    fillCircularBuffer(capacity, 0);
    TEST_ASSERT_EQUAL(capacity, MyCircularBuffer_size());

    // Empty it
    emptyEntireCircularBuffer();
    TEST_ASSERT_EQUAL(0, MyCircularBuffer_size());

    // Fill with some more data (should cause pointers to loop round)
    fillCircularBuffer(3, 0);
    TEST_ASSERT_EQUAL(3, MyCircularBuffer_size());
}

/******************** Helper functions and their tests *************/

void fillCircularBuffer(uint32_t amountOfData, uint8_t filledItem)
{
    for (uint32_t i = 0; i < amountOfData; i++)
    {
        MyCircularBuffer_write(filledItem);
    }
}

void drainCircularBuffer(uint32_t amountOfDataToDrain)
{
    for (uint32_t i = 0; i < amountOfDataToDrain; i++)
    {
        MyCircularBuffer_read();
    }
}

void emptyEntireCircularBuffer(void)
{
    while (!MyCircularBuffer_isEmpty())
    {
        MyCircularBuffer_read();
    }
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