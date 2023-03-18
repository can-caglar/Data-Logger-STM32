#include "FileNameIterator.h"
#include "MyTimeString.h"
#include <string.h>

static char fileName[MAX_FILE_NAME];    

void FileNameIterator_init(void)
{
    MyTimeString_Init();
    memset(fileName, 0, MAX_FILE_NAME);
}

void FileNameIterator_getName(char* str, uint8_t maxSize)
{
    if (fileName[0] == 0)
    {
        FileNameIterator_next();
    }
    strncpy(str, fileName, maxSize);
}

void FileNameIterator_next(void)
{
    strncpy(fileName, MyTimeString_GetFileName(), MAX_FILE_NAME);
}

uint8_t FileNameIterator_hasNext(void)
{
    return 1;
}
