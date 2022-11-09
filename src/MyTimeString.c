#include "MyTimeString.h"

int MyTimeString_Init(void)
{
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
    return "[2022-10-31 09:13:24.665] ";
}
