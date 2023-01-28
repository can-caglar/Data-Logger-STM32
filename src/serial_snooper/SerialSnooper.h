#ifndef SERIAL_SNOOPER_H
#define SERIAL_SNOOPER_H

#include <stdint.h>

#define SS_SUCCESS 0
#define SS_FAIL 1

typedef struct SubjectData_t SubjectData_t;
struct SubjectData_t
{
    uint64_t tNow;
    uint8_t isEmpty;
    uint8_t circBufData;
};

int SerialSnooper_Init(void);
void SerialSnooper_Run(void);

#endif
