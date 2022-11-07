#ifndef MY_CIRCULAR_BUFFER_H
#define MY_CIRCULAR_BUFFER_H

#include <stdint.h>

uint8_t MyCircularBuffer_read(void);
uint8_t MyCircularBuffer_isEmpty(void);

#endif

/*

write
    adds to the circular buffer
read
    reads from the circular buffer
free
    frees the data to be used by producer again
length 
*/