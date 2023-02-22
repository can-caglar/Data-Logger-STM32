#include "MySD.h"
#include "LED.h"
#include <stdint.h>
#include <string.h>

static FATFS FatFs; // fats handle
static FIL file; // fats handle
static const BYTE writeMode = FA_WRITE | FA_OPEN_APPEND;
static uint8_t mounted;
static uint8_t sendBuf[WRITE_BUFFER_SIZE];
static uint32_t sendBufIndex = 0;

FRESULT MySD_Init(const char* filename)
{
    FRESULT err = FR_OK;
    sendBufIndex = 0;
    
    if (mounted == 0)
    {
        err = f_mount(&FatFs, "", 1);
    }
    if (err == FR_OK)
    {
        if (mounted == 1)
        {
            f_close(&file); // close the already open file
        }
        mounted = 1;
        err = f_open(&file, filename, writeMode);
    }
    if (err == FR_OK)
    {
        led_init();
        led_on();
    }

    return err;
}

void MySD_Close(void)
{
    f_close(&file);
    f_mount(NULL, "", 0);

    // Clear state variables
    mounted = 0;
}

// Pass in a c-string, the function will calculate length
FRESULT MySD_WriteString(const char* buf)
{
    return MySD_Write((uint8_t*)buf, strlen(buf));
}

FRESULT MySD_Write(const uint8_t* buf, uint32_t len)
{
    // Buffer the data, only send 1024 chunks.
    FRESULT err = FR_OK;
    unsigned int bytesWrote = 0;
    uint32_t charactersLeftToSend = len;
    while (charactersLeftToSend)
    {
        for (int i = 0; (i < len) && (sendBufIndex < WRITE_BUFFER_SIZE) && (charactersLeftToSend); i++)
        {
            sendBuf[sendBufIndex] = *buf;
            sendBufIndex++;
            charactersLeftToSend--;
        }

        if (sendBufIndex == WRITE_BUFFER_SIZE)
        {
            err = f_write(&file, sendBuf, sendBufIndex, &bytesWrote);
            sendBufIndex = 0;
            if (err == FR_OK)
            {
                led_toggle();
            }
        }
    }
    return err;
}

FRESULT MySD_Flush(void)
{
    return f_sync(&file);
}

FSIZE_t MySD_getOpenedFileSize(void)
{
    return f_size(&file);
}
