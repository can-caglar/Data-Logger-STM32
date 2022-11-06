#include "SerialSnooper.h"
#include "MySD.h"
#include "MyRTC.h"

void SerialSnooper_Init(void)
{
    MyRTC_Init();
    const char* timeStr = MyRTC_GetTimeStr();
    MySD_Init(timeStr);
}

// void SerialSnooper_Run()
// {

// }
