#ifndef DATA_HOLDER_H
#define DATA_HOLDER_H

#include <stdint.h>

// Encapsulates the system information
// needed for the serial snooper.

// uint32_t DH_GetTime(void); not encapsualting this, not directly relevant to data being logged
uint8_t DH_IsThereNewData(void);
uint8_t DH_GetLatestData(void);
const char* DH_GetFileName(void);
const char* DH_GetTimestampString(void);
uint32_t DH_GetOpenedFileSize(void);

#endif
