#include "SerialSnooper.h"
#include "MySD.h"
#include "MyRTC.h"
#include "MyCircularBuffer.h"
#include <string.h>

#define MAX_BUF 2048

static uint8_t buf[MAX_BUF];

void SerialSnooper_Init(void)
{
    MyRTC_Init();
    const char* timeStr = MyRTC_GetTimeStr();
    MySD_Init(timeStr);
}

void SerialSnooper_Close(void)
{
    memset(buf, 0, MAX_BUF);
}

void SerialSnooper_Run()
{
    uint32_t bufSize = 0;
    while (!MyCircularBuffer_isEmpty() && bufSize < MAX_BUF)
    {
        buf[bufSize] = MyCircularBuffer_read();
        bufSize++;
    }
    MySD_Write(buf, bufSize);
}
