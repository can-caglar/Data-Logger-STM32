#include "SystemOperations.h"
#include "MySD.h"
#include "stm32f0xx_hal.h"
#include "SerialSnooper.h"
#include "DataHolder.h"

#define MAX_BUF 32
#define FLUSH_TIME_MS   500

#define STATUS_INIT_FAIL (1 << 0)
#define STATUS_TIMESTAMP (1 << 1)

static uint8_t previousData;
static uint32_t lastTimeFlushed;
static uint8_t status;
static uint8_t timestampThisLine(uint8_t thisByte);

int SystemOperations_Init(void)
{
    int ret = SO_SUCCESS;
    status = STATUS_TIMESTAMP;
    lastTimeFlushed = 0;

    return SystemOperations_OpenLogFile(NULL);
}   

int SystemOperations_OpenLogFile(const DataContext* data)
{
    int ret = SO_SUCCESS;

    const char* fileName = DH_GetFileName(NULL);

    // open file
    FRESULT err = MySD_Init(fileName);

    // return any error
    if (err != FR_OK)
    {
        ret = SO_FAIL;
    }
    return ret;
}


void notifySdCardWriter(const DataContext* data)
{
    int thereIsNewData = DH_IsThereNewData(data); // !data->isEmpty;
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
