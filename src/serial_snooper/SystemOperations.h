#ifndef SYSTEM_OPERATIONS_H
#define SYSTEM_OPERATIONS_H

#include <stdint.h>

#define SO_SUCCESS 0
#define SO_FAIL 1

#define MAX_FILE_SIZE   20000000
#define FILE_SIZE_LOWER_THRESHOLD   10000000
#define FLUSH_TIME_MS   30000

int SystemOperations_Init(void);
void SystemOperations_OpenLogFile(void);
void SystemOperations_WriteSD(void);
void SystemOperations_FlushSD(void);

#endif
