#include "fake_myTimeString.h"
#include <string.h>

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
        return "";
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
        return "";
    }
}

void fake_myTimeString_setFileName(const char* newName)
{
    if (initialised)
    {
        strcpy(fileName, newName);
    }
}

void fake_myTimeString_setTimestamp(const char* newTS)
{
    if (initialised)
    {
        strcpy(timeStamp, newTS);
    }
}