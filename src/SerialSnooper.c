#include "SerialSnooper.h"
#include "MySD.h"
#include "MyRTC.h"
#include "MyCircularBuffer.h"
#include <string.h>

#define MAX_BUF 32

static uint8_t buf[MAX_BUF];

void SerialSnooper_Init(void)
{
    MyCircularBuffer_init();
    MyRTC_Init();
    const char* timeStr = MyRTC_GetTimeStr();
    FRESULT err = MySD_Init(timeStr);
}

void SerialSnooper_Close(void)
{
    memset(buf, 0, MAX_BUF);
}

void SerialSnooper_Run()
{
    uint32_t len = 0;
    for ( ; (len < MAX_BUF) && !MyCircularBuffer_isEmpty(); len++)
    {
        buf[len] = MyCircularBuffer_read();
    }
    FRESULT err = MySD_Write(buf, len);
}
