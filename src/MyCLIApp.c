#include "MyCLIApp.h"
#include "MySM.h"
#include "Loop.h"

void runCLI(void)
{
    MySM_Init();
    
    LOOP
    {
        MySM_Run();
    }
}