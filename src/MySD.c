#include "MySD.h"
#include <stdint.h>
#include <string.h>

static FATFS FatFs; // fats handle
static FIL file; // fats handle
static const BYTE writeMode = FA_WRITE | FA_OPEN_APPEND;
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

    return err;
}

void MySD_Close(void)
{
    f_close(&file);
    f_mount(NULL, "", 0);

    // Clear state variables
    mounted = 0;
}

FRESULT MySD_Write(char* buf)
{
    int sizeOfBuf = strlen(buf);
    unsigned int bytesWrote = 0;
    FRESULT err = f_write(&file, buf, sizeOfBuf, &bytesWrote);
    return err;
}
