#include "SystemOperations.h"
#include "MySD.h"
#include "MyTimeString.h"
#include "MyCircularBuffer.h"
#include "stm32f0xx_hal.h"
#include "SerialSnooper.h"

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
    int ret = SS_SUCCESS;
    status = STATUS_TIMESTAMP;
    lastTimeFlushed = 0;

    MyCircularBuffer_init();
    MyTimeString_Init();

    const char* fileName = MyTimeString_GetFileName();
    FRESULT err = MySD_Init(fileName);
    if (err != FR_OK)
    {
        ret = SS_FAIL;
    }
    return ret;
}   

int SystemOperations_OpenNewFile()
{
    int ret = SO_SUCCESS;

    // get file name
    MyTimeString_Init();
    const char* fileName = MyTimeString_GetFileName();

    // open file
    FRESULT err = MySD_Init(fileName);

    // return any error
    if (err != FR_OK)
    {
        ret = SO_FAIL;
    }
    return ret;
}


void notifySdCardWriter(SubjectData_t* data)
{
    if (!data->isEmpty)
    {
        // get top item from circular buffer
        uint8_t val = MyCircularBuffer_read();
        // parse it (determine if need to timestamp)
        if (timestampThisLine(val))
        {
            // write timestamp
            const char* ts = MyTimeString_GetTimeStamp();
            MySD_WriteString(ts);
            status &= ~STATUS_TIMESTAMP;
        }
        // write data to SD card
        MySD_Write(&val, 1);
        previousData = val;
    }
}

void notifySdCardFlusher(SubjectData_t* data)
{
    // Device may be unplugged at any moment.
    // Flush every 500 ms.
    if (HAL_GetTick() >= (lastTimeFlushed + 500))
    {
        MySD_Flush();
        lastTimeFlushed = HAL_GetTick();
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
