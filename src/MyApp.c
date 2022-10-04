#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"
#include "controller.h"

#ifndef TEST
#define runCLI main
#endif

int runCLI(void)
{
    MyCLI_Init();
    run_controller();
    
    LOOP
    {
        MyCLI_Run();
    }
}
