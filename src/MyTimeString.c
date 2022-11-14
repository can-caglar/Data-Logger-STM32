#include "MyTimeString.h"
#include "MyRTC.h"
#include <stdio.h>
// #include <time.h>

static char ret[23];

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
    sprintf(ret, "%X.txt", unixTs);
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
    // Seconds passed since 1st January 1970
    uint32_t timeEpoch = 0;
    
    // year
    for (uint32_t i = 1970; i < (2000 + time->year); i++)
    {
        uint32_t days = 365;
        if ((i & 3) == 0)   // modulus 4
        {
            days = 366;
        }
        timeEpoch += (days * 86400);
    }
    // month
    for (uint32_t i = 1; i < time->month; i++)
    {
        uint32_t days = 31;
        if (i == 2)
        {
            if ((time->year & 3) == 0)
            {
                days = 29;
            }
            else
            {
                days = 28;
            }
        }
        if ((i == 4) || (i == 6) || (i == 9) || (i == 11))
        {
            days = 30;
        }
        timeEpoch += (days * 86400);
    }
    // day
    timeEpoch += (time->day - 1) * 86400;

    // hour
    timeEpoch += (time->hour * 3600);

    // minute
    timeEpoch += (time->minute * 60);

    // second
    timeEpoch += time->second;

//    t.tm_year = (2000 + time->year) - 1900;  // Year - 1900
//    t.tm_mon = time->month - 1;  // Month, where 0 = jan
//    t.tm_mday = time->day;       // Day of the month
//    t.tm_hour = time->hour;
//    t.tm_min = time->minute;
//    t.tm_sec = time->second;
//    t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
//    //timeEpoch = mktime(&t);

    return timeEpoch;
}
