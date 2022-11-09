#include "SerialSnooper.h"
#include "MySD.h"
#include "MyTimeString.h"
#include "MyCircularBuffer.h"
#include "stm32f0xx_hal.h"
#include <string.h>

#define MAX_BUF 32
#define FLUSH_TIME_MS   500

#define STATUS_INIT_FAIL (1 << 0)
#define STATUS_TIMESTAMP (1 << 1)

static uint8_t status;
static uint32_t lastTimeFlushed;

void SerialSnooper_Init(void)
{
    status = STATUS_TIMESTAMP;
    lastTimeFlushed = 0;
    MyCircularBuffer_init();
    MyTimeString_Init();
    const char* fileName = MyTimeString_GetFileName();
    FRESULT err = MySD_Init(fileName);
    if (err != FR_OK)
    {
        status |= STATUS_INIT_FAIL;
    }
}

void SerialSnooper_Run()
{
    if ((status & STATUS_INIT_FAIL) == 0)
    {
        if (!MyCircularBuffer_isEmpty())
        {
            uint8_t val = MyCircularBuffer_read();
            // parse "val"
            if (status & STATUS_TIMESTAMP)
            {
                const char* ts = MyTimeString_GetTimeStamp();
                MySD_WriteString(ts);
                status &= ~STATUS_TIMESTAMP;
            }
            MySD_Write(&val, 1);
            if (val == '\r')
            {
                // time-stamp next one
                status |= STATUS_TIMESTAMP;
            }
        }

        if (HAL_GetTick() >= (lastTimeFlushed + 500))
        {
            MySD_Flush();
            lastTimeFlushed = HAL_GetTick();
        }
    }
}
