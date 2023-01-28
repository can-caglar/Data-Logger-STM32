#ifndef SERIAL_SNOOPER_H
#define SERIAL_SNOOPER_H

#include <stdint.h>

typedef struct SubjectData_t
{
    uint64_t tNow;
    uint8_t isEmpty;
    uint8_t circBufData;
} SubjectData_t;

int SerialSnooper_Init(void);
void SerialSnooper_Run(void);

#endif
