#ifndef SYSTEM_OPERATIONS_H
#define SYSTEM_OPERATIONS_H

#include <stdint.h>
#include "SystemConfigs.h"

#define SO_SUCCESS 0
#define SO_FAIL 1

int SystemOperations_Init(void);
void SystemOperations_OpenLogFile(void);
void SystemOperations_WriteSD(void);
void SystemOperations_FlushSD(void);

#endif
