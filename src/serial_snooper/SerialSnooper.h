#ifndef SERIAL_SNOOPER_H
#define SERIAL_SNOOPER_H

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

// Fw declare DataContext
typedef struct DataContext DataContext;

// Fn pointer for grabbing data
typedef DataContext* (*FnGetData)(void);

// Scheduler task function pointer
typedef void (*FnTask)(DataContext* data);

void SerialSnooper_Init(FnGetData fnGetData);
void SerialSnooper_Run(void);

int SerialSnooper_AddTask(FnTask func, 
    uint32_t period, bool periodic, bool enabled);

#endif
