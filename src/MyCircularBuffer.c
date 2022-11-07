#include "MyCircularBuffer.h"
#include <string.h>

// Max size must be power of 2 for fast ptr advancing
#define SIZE_OF_BUF     64
#define MAX_SIZE    ((SIZE_OF_BUF) - 1)  // "wasting" a slot, so it's 1 less
#define ADVANCED(ptr) (((ptr) + 1) & (SIZE_OF_BUF - 1))

static uint8_t buf[SIZE_OF_BUF]; 
static uint32_t head = 0;
static uint32_t tail = 0;

static uint8_t initialised = 0;

void MyCircularBuffer_init(void)
{
    if (!initialised)
    {
        head = 0;
        tail = 0;
        memset(buf, 0, SIZE_OF_BUF);
        initialised = 1;
    }
}

void MyCircularBuffer_close(void)
{
    initialised = 0;
}

uint32_t MyCircularBuffer_getCapacity(void)
{
    return MAX_SIZE;
}

uint8_t MyCircularBuffer_read(void)
{
    uint8_t ret = buf[tail];
    tail = ADVANCED(tail);
    return ret;
}

void MyCircularBuffer_write(uint8_t val)
{
    if (MyCircularBuffer_isFull())
    {
        tail = ADVANCED(tail);
    }
    buf[head] = val;
    head = ADVANCED(head);
}

uint8_t MyCircularBuffer_isEmpty(void)
{
    return head == tail;
}

uint8_t MyCircularBuffer_isFull(void)
{
    return ADVANCED(head) == (tail);
}
