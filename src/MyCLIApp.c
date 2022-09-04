#include "MyCLIApp.h"
#include "MySM.h"
#include "Loop.h"

#ifndef TEST
#define runCLI main
#endif

int runCLI(void)
{
    MySM_Init();
    
    LOOP
    {
        MySM_Run();
    }
}
