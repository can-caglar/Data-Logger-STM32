#include "SystemOperations.h"
#include "MySD.h"
#include "stm32f0xx_hal.h"
#include "SerialSnooper.h"
#include "DataHolder.h"

#define MAX_BUF 32

#define STATUS_INIT_FAIL (1 << 0)
#define STATUS_TIMESTAMP (1 << 1)

static uint8_t previousData;
static uint32_t lastTimeFlushed;
static uint8_t status;
static uint8_t aFileIsOpen = 0;
static uint8_t timestampThisLine(uint8_t thisByte);

int SystemOperations_Init(void)
{
    status = STATUS_TIMESTAMP;
    lastTimeFlushed = 0;
    aFileIsOpen = 0;

    return SO_SUCCESS;
}   

void SystemOperations_OpenLogFile(const DataContext* data)
{
    // get size of file
    uint32_t dataSize = DH_GetOpenedFileSize(data);
    // find out if we have data to handle
    uint8_t newDataToHandle = DH_IsThereNewData(data);
    // open new file if size is greater than maximum allowed
    uint8_t maxDataSizeReached = (dataSize >= MAX_FILE_SIZE);
    uint8_t lowerBoundSizeReached = (dataSize >= FILE_SIZE_LOWER_THRESHOLD);
    // open file "early" if system has no new data to handle
    uint8_t openNewFileEarly = (lowerBoundSizeReached && !newDataToHandle);

    if (!aFileIsOpen || maxDataSizeReached || openNewFileEarly)
    {
        // get file name
        const char* fileName = DH_GetFileName(data);

        // open file
        FRESULT err = MySD_Init(fileName);

        if (err == FR_OK)
        {
            aFileIsOpen = 1;
        }
        else
        {
            aFileIsOpen = 0;
        }
    }
}


void notifySdCardWriter(const DataContext* data)
{
    uint8_t thereIsNewData = DH_IsThereNewData(data);
    if (thereIsNewData)
    {
        // get top item from circular buffer
        uint8_t val = DH_GetLatestData(data); //MyCircularBuffer_read();
        // parse it (determine if need to timestamp)
        if (timestampThisLine(val))
        {
            // write timestamp
            const char* ts = DH_GetTimestampString(data);
            MySD_WriteString(ts);
            status &= ~STATUS_TIMESTAMP;
        }
        // write data to SD card
        MySD_Write(&val, 1);
        previousData = val;
    }
}

void notifySdCardFlusher(const DataContext* data)
{
    // Device may be unplugged at any moment.
    // Flush every 500 ms.
    uint32_t tNow = DH_GetTime(data);
    if (tNow >= (lastTimeFlushed + FLUSH_TIME_MS))
    {
        MySD_Flush();
        lastTimeFlushed = tNow;
    }
}


// Should behave like the "auto" setting on Teraterm
// Should timestamp the first ever character
// Then will timestamp the first character
// after a line feed.
// A line feed is either: CR, LF, CRLF, LFCR
static uint8_t timestampThisLine(uint8_t thisByte)
{
    uint8_t ret = 0;
    if (status & STATUS_TIMESTAMP)
    {
        ret = 1;
    }
    if (previousData == '\n')
    {
        if (thisByte != '\r')
        {
            // If previously received LF and this
            // new line is not CR...
            ret = 1;
        }
        else
        {
            status |= STATUS_TIMESTAMP; // ts the next one
        }
    }
    if (previousData == '\r')
    {
        if (thisByte != '\n')
        {
            // If previously received CR and this
            // new line is not LF...
            ret = 1;
        }
        else
        {
            status |= STATUS_TIMESTAMP; // ts the next one
        }
    }
    return ret;
}
