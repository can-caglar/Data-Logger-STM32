#ifndef DATA_HOLDER_H
#define DATA_HOLDER_H

#include <stdint.h>

typedef struct DataContext DataContext;

uint64_t DH_GetTime(const DataContext* data);
int DH_IsThereNewData(const DataContext* data);
uint8_t DH_GetLatestData(const DataContext* data);

#endif
