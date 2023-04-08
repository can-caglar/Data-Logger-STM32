#include "MySD.h"
#include "LED.h"
#include <stdint.h>
#include <string.h>

static FATFS FatFs; // fats handle
static FIL file; // fats handle
static const BYTE writeMode = FA_WRITE | FA_READ | FA_OPEN_APPEND;
static uint8_t mounted;

FRESULT MySD_Init(const char* filename)
{
    FRESULT err = FR_OK;
    
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

    memset(&file, 0, sizeof(file));

    // Clear state variables
    mounted = 0;
}

// Pass in a c-string, the function will calculate length
FRESULT MySD_WriteString(const char* buf)
{
    int sizeOfBuf = strlen(buf);
    FRESULT err = MySD_Write((const uint8_t*)buf, sizeOfBuf);
    return err;
}

FRESULT MySD_Write(const uint8_t* buf, uint32_t len)
{
    unsigned int bytesWrote = 0;
    FRESULT err = f_write(&file, buf, len, &bytesWrote);
    if (err == FR_OK)
    {
        led_toggle();
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

FRESULT MySD_Read(uint8_t* buf, uint32_t len)
{
    unsigned int bytesRead = 0;
    FRESULT err = f_read(&file, buf, len, &bytesRead);
    return err;
}
