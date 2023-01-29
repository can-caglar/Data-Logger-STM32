#ifndef SYSTEM_OPERATIONS_H
#define SYSTEM_OPERATIONS_H

#include <stdint.h>

#define SO_SUCCESS 0
#define SO_FAIL 1

typedef struct SubjectData_t SubjectData_t;

int SystemOperations_Init(void);
int SystemOperations_OpenLogFile(void);
void notifySdCardWriter(const SubjectData_t* data);
void notifySdCardFlusher(const SubjectData_t* data);

#endif
