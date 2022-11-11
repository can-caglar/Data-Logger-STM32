#ifndef MY_RTC_H_INCLUDED
#define MY_RTC_H_INCLUDED

#include "stdint.h"

typedef struct MyTime
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t weekday;
} MyTime;

void MyRTC_Init(void);
MyTime MyRTC_ReadTime(void);    // TODO, return error
int MyRTC_WriteTime(const MyTime* newTime);

#endif
