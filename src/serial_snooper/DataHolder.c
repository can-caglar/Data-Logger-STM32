#include "DataHolder.h"
#include "MyCircularBuffer.h"
#include "MyTimeString.h"
#include "MySD.h"
#include "stm32f0xx_hal.h"

struct DataContext
{
    uint32_t tNow;
    FSIZE_t fileSize;
    uint8_t isThereNewData;
    uint8_t theLatestData;
    const char* fileName;
    const char* timestampStr;
};

static DataContext thisContext;

int DH_Init(void)
{
    int ret = MyTimeString_Init();
    // Circular buffer to start out empty
    MyCircularBuffer_close();
    MyCircularBuffer_init();

    return ret;
}

DataContext* DH_RefreshData(void)
{
    thisContext.tNow = HAL_GetTick();
    thisContext.isThereNewData = !MyCircularBuffer_isEmpty();
    thisContext.theLatestData = MyCircularBuffer_read();
    thisContext.timestampStr = MyTimeString_GetTimeStamp();
    thisContext.fileName = MyTimeString_GetFileName();
    thisContext.fileSize = MySD_getOpenedFileSize(); // not initted it, problem?

    return &thisContext;
}

uint32_t DH_GetTime(const DataContext* data)
{
    if (data)
    {
        return data->tNow;
    }
    return 0;
}

uint8_t DH_IsThereNewData(const DataContext* data)
{
    if (data)
    {
        return data->isThereNewData;
    }
    return 0;
}

uint8_t DH_GetLatestData(const DataContext* data)
{
    if (data)
    {
        return data->theLatestData;
    }
    return 0;
}

const char* DH_GetFileName(const DataContext* data)
{
    if (data)
    {
        return data->fileName;
    }
    return 0;
}

const char* DH_GetTimestampString(const DataContext* data)
{
    if (data)
    {
        return data->timestampStr;
    }
    return 0;
}

uint32_t DH_GetOpenedFileSize(const DataContext* data)
{
    if (data)
    {
        return data->fileSize;
    }
    return 0;
}