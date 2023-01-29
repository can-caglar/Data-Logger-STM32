#ifndef FAKE_DATA_HOLDER_H
#define FAKE_DATA_HOLDER_H

#include <stdint.h>

typedef struct DataContext DataContext;

uint64_t DH_GetTime(DataContext* data);
int DH_IsThereNewData(DataContext* data);

#endif
