#ifndef FAKE_MY_TIME_STRING_H
#define FAKE_MY_TIME_STRING_H

#include "MyTimeString.h"

#define NOT_INIT_STRING "Timestring: not init"

void fake_myTimeString_reset(void);
void fake_myTimeString_setFileName(const char* newName);
void fake_myTimeString_setTimestamp(const char* newTS);

#endif