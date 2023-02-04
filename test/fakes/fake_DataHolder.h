#ifndef FAKE_DATA_HOLDER_H
#define FAKE_DATA_HOLDER_H

#include "DataHolder.h"

void fakeSetIsThereNewData(int newState);
void fakeSetLatestData(uint8_t newData);
void fakeSetFileName(const char* newFileName);
void fakeSetTimestampString(const char* timeStamp);
void fakeSetFileSize(uint32_t newFileSize);

#endif