#ifndef FILE_NAME_ITERATOR_H_INCLUDED
#define FILE_NAME_ITERATOR_H_INCLUDED

#include <stdint.h>

// 8.3 standard + null terminate
#define MAX_FILE_NAME   12

void FileNameIterator_init(void);
void FileNameIterator_getName(char* str, uint8_t maxSize);
void FileNameIterator_next(void);
uint8_t FileNameIterator_hasNext(void);

#endif
