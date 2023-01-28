#ifndef MY_CIRCULAR_BUFFER_H
#define MY_CIRCULAR_BUFFER_H

#include <stdint.h>

void MyCircularBuffer_init(void);

void MyCircularBuffer_close(void);

uint8_t MyCircularBuffer_read(void);

uint8_t MyCircularBuffer_write(uint8_t val);    // ret 1 if buf was full

uint8_t MyCircularBuffer_isEmpty(void);

uint8_t MyCircularBuffer_isFull(void);

uint32_t MyCircularBuffer_getCapacity(void);

uint8_t MyCircularBuffer_peek(void);

uint16_t MyCircularBuffer_size(void);

#endif
