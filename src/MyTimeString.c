#include "MyTimeString.h"
#include "MyRTC.h"
#include <stdio.h>

static char ret[30];

int MyTimeString_Init(void)
{
    MyRTC_Init();
    return 0;
}

const char* MyTimeString_GetFileName(void)
{
    // max 8 characters (+ .txt)
    // 20911845
    return "20911845.txt";
}

const char* MyTimeString_GetTimeStamp(void)
{
    MyTime time = MyRTC_ReadTime();

    sprintf(ret, "[20%02d-%02d-%02d %02d:%02d:%02d] ",
        time.year,
        time.month,
        time.day,
        time.hour,
        time.minute,
        time.second);
    return ret;
}
