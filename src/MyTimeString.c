#include "MyTimeString.h"
#include "MyRTC.h"
#include <stdio.h>
#include <string.h>

static char ret[23];

static const char* const numToStrLookup[100] =
{
    "00", "01", "02", "03", "04",
    "05", "06", "07", "08", "09",
    "10", "11", "12", "13", "14",
    "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24",
    "25", "26", "27", "28", "29",
    "30", "31", "32", "33", "34",
    "35", "36", "37", "38", "39",
    "40", "41", "42", "43", "44",
    "45", "46", "47", "48", "49",
    "50", "51", "52", "53", "54",
    "55", "56", "57", "58", "59",
    "60", "61", "62", "63", "64",
    "65", "66", "67", "68", "69",
    "70", "71", "72", "73", "74",
    "75", "76", "77", "78", "79",
    "80", "81", "82", "83", "84",
    "85", "86", "87", "88", "89",
    "90", "91", "92", "93", "94",
    "95", "96", "97", "98", "99",
};

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
    char* strPtr = ret;
    #define maxSize 2
    char colon = ':';
    char space = ' ';
    char rightArrow = '>';
    MyTime time = MyRTC_ReadTime();
    // Day
    const char* number = numToStrLookup[time.day];
    memcpy(strPtr, number, maxSize);
    strPtr += 2;
    // Space
    memcpy(strPtr, &space, 1);
    strPtr += 1;
    // Hour
    number = numToStrLookup[time.hour];
    memcpy(strPtr, number, maxSize);
    strPtr += 2;  
    // Colon
    memcpy(strPtr, &colon, 1);
    strPtr += 1;
    // Minute
    number = numToStrLookup[time.minute];
    memcpy(strPtr, number, maxSize);
    strPtr += 2;  
    // Colon
    memcpy(strPtr, &colon, 1);
    strPtr += 1;
    // Second
    number = numToStrLookup[time.second];
    memcpy(strPtr, number, maxSize);
    strPtr += 2;
    // Right arrow
    memcpy(strPtr, &rightArrow, 1);
    strPtr += 1;
    // Space
    memcpy(strPtr, &space, 1);
    strPtr += 1;
    // Null
    *strPtr = '\0';
    
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
