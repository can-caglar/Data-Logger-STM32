#include "DataHolder.h"
#include "MyCircularBuffer.h"
#include "MyTimeString.h"
#include "MySD.h"
#include "FileNameIterator.h"

uint8_t DH_IsThereNewData(void)
{
    return !MyCircularBuffer_isEmpty();
}

uint8_t DH_GetLatestData(void)
{
    return MyCircularBuffer_read();
}

const char* DH_GetFileName(void)
{
    static char fileName[MAX_FILE_NAME];
    FileNameIterator_next();
    FileNameIterator_getName(fileName, MAX_FILE_NAME);
    return fileName;
}

const char* DH_GetTimestampString(void)
{
    return MyTimeString_GetTimeStamp();
}

uint32_t DH_GetOpenedFileSize(void)
{
    return (uint32_t)MySD_getOpenedFileSize();
}
