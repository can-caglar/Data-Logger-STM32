#ifndef MY_RTC_H_INCLUDED
#define MY_RTC_H_INCLUDED

// Module to return:
// Full datetime in format: [2022-10-31 09:13:24.665]
// File name w/date: 2210310913.txt

int MyTimeString_Init(void);
const char* MyTimeString_GetFileName(void);
const char* MyTimeString_GetTimeStamp(void);

#endif
