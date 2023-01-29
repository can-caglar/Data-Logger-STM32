#ifndef DATA_HOLDER_H
#define DATA_HOLDER_H

#include <stdint.h>

typedef struct DataContext DataContext;

int DH_Init(void);
DataContext* DH_RefreshData(void);
uint32_t DH_GetTime(const DataContext* data);
uint8_t DH_IsThereNewData(const DataContext* data);
uint8_t DH_GetLatestData(const DataContext* data);
const char* DH_GetFileName(const DataContext* data);
const char* DH_GetTimestampString(const DataContext* data);
uint32_t DH_GetOpenedFileSize(const DataContext* data);

#endif
