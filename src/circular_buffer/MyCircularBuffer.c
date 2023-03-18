#include "MyCircularBuffer.h"
#include <string.h>

#define SIZE_OF_BUF     10200
#define MAX_SIZE    ((SIZE_OF_BUF) - 1)  // "wasting" a slot to tell apart full/empty
#define ADVANCED(ptr) (((ptr) + 1) % (SIZE_OF_BUF))

#define CB_SUCCESS 0
#define CB_FAIL 1

static uint8_t buf[SIZE_OF_BUF]; 
static uint16_t head = 0;
static uint16_t tail = 0;
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

uint8_t MyCircularBuffer_write(uint8_t val)
{
    if (MyCircularBuffer_isFull())
    {
        #if 0
        tail = ADVANCED(tail); // this is to overwrite
        #endif
        return 1;   // indicate that buffer was full
    }
    buf[head] = val;
    head = ADVANCED(head);
    return 0;
}

uint8_t MyCircularBuffer_isEmpty(void)
{
    return head == tail;
}

uint8_t MyCircularBuffer_isFull(void)
{
    return ADVANCED(head) == (tail);
}

uint8_t MyCircularBuffer_peek(void)
{
    return buf[tail];
}

uint16_t MyCircularBuffer_size(void)
{
    uint16_t ret;
    if (head < tail)
    {
        // 0 indexed, so + 1
        ret = ((MAX_SIZE - tail) + head) + 1;  
    }
    else
    {
        ret = head - tail;
    }
    return ret;
}
