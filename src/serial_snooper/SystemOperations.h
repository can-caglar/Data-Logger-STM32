#ifndef SYSTEM_OPERATIONS_H
#define SYSTEM_OPERATIONS_H

#include <stdint.h>

#define SO_SUCCESS 0
#define SO_FAIL 1

typedef struct DataContext DataContext;

int SystemOperations_Init(void);
int SystemOperations_OpenLogFile(void);
void notifySdCardWriter(const DataContext* data);
void notifySdCardFlusher(const DataContext* data);

#endif
