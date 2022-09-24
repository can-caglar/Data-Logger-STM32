#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"

#ifndef TEST
#define runCLI main
#endif

int runCLI(void)
{
    MyCLI_Init();
    
    LOOP
    {
        MyCLI_Run();
    }
}
