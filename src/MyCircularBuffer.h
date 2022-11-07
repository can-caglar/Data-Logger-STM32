#ifndef MY_CIRCULAR_BUFFER_H
#define MY_CIRCULAR_BUFFER_H

#include <stdint.h>

void MyCircularBuffer_init(void);

uint8_t MyCircularBuffer_read(void);

void MyCircularBuffer_write(uint8_t val);

uint8_t MyCircularBuffer_isEmpty(void);

uint8_t MyCircularBuffer_isFull(void);

uint32_t MyCircularBuffer_getCapacity(void);

#endif
