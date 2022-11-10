#include "MyTimeString.h"
#include "MyRTC.h"
#include <stdio.h>
#include <time.h>

static char ret[30];

static uint32_t secondsSinceEpoch(const MyTime* time);

int MyTimeString_Init(void)
{
    MyRTC_Init();
    return 0;
}

const char* MyTimeString_GetFileName(void)
{
    // max 8 characters (+ .txt)
    // Unix timestamp, should be good for many, many decades
    // as of 2022
    MyTime time = MyRTC_ReadTime();
    uint32_t unixTs = secondsSinceEpoch(&time);
    sprintf(ret, "%X.txt\0", unixTs);
    return ret;
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

// Helper functions
static uint32_t secondsSinceEpoch(const MyTime* time)
{
    struct tm t;
    uint32_t timeEpoch; 

    t.tm_year = (2000 + time->year) - 1900;  // Year - 1900
    t.tm_mon = time->month - 1;  // Month, where 0 = jan
    t.tm_mday = time->day;       // Day of the month
    t.tm_hour = time->hour;
    t.tm_min = time->minute;
    t.tm_sec = time->second;
    t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    timeEpoch = mktime(&t);

    return timeEpoch;
}
