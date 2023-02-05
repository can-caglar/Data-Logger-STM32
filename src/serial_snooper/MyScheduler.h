#ifndef MY_SCHEDULER_H
#define MY_SCHEDULER_H

#include <stdint.h>

#ifndef bool
#define bool uint8_t
#define true 1
#define false 0
#endif

enum
{
    SS_ERR_NONE,
    SS_ERR_FAULT,
};

#define MAX_TASKS 3

// Fw declare DataContext
typedef struct DataContext DataContext;

// Scheduler task function pointer
typedef void (*FnTask)(void);

void MyScheduler_Init(void);
void MyScheduler_Run(void);

int MyScheduler_AddTask(FnTask func, 
    uint32_t period, bool periodic, bool enabled);

#endif
