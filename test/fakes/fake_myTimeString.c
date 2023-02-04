#include "fake_myTimeString.h"
#include <string.h>
#include <stdio.h>

#define FAKE_SIZE_OF_STR 50

static char fileName[FAKE_SIZE_OF_STR];
static char timeStamp[FAKE_SIZE_OF_STR];
static int initialised = 0;

void fake_myTimeString_reset(void)
{
    memset(fileName, 0, FAKE_SIZE_OF_STR);
    memset(timeStamp, 0, FAKE_SIZE_OF_STR);
    initialised = 0;
}

int MyTimeString_Init(void)
{
    initialised = 1;
}

const char* MyTimeString_GetFileName(void)
{
    if (initialised)
    {
        return fileName;
    }
    else
    {
        return NOT_INIT_STRING;
    }
}

const char* MyTimeString_GetTimeStamp(void)
{
    if (initialised)
    {
        return timeStamp;
    }
    else
    {
        return NOT_INIT_STRING;
    }
}

void fake_myTimeString_setFileName(const char* newName)
{
    strcpy(fileName, newName);
}

void fake_myTimeString_setTimestamp(const char* newTS)
{
    strcpy(timeStamp, newTS);
}