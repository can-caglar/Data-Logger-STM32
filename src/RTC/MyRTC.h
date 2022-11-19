#ifndef MY_RTC_H_INCLUDED
#define MY_RTC_H_INCLUDED

#include "stdint.h"

typedef struct MyTime
{
    uint8_t year;   // 0 - 99
    uint8_t month;  // 1 - 12
    uint8_t day;    // 1 - 31
    uint8_t hour;   // 0 - 23
    uint8_t minute; // 0 - 59
    uint8_t second; // 0 - 59
    uint8_t weekday; // 0 - 6
} MyTime;

void MyRTC_Init(void);
MyTime MyRTC_ReadTime(void);    // TODO, return error
int MyRTC_WriteTime(const MyTime* newTime);

#endif
