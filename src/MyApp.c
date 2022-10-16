#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"
#include "controller.h"

#include "main.h"

#ifndef TEST
#define runCLI main2    // TODO, change back to main
#endif

/*
int main(void)
*/
int runCLI(void)
{
    MyCLI_Init();
    run_controller();
    
    LOOP
    {
        MyCLI_Run();
    }
}
