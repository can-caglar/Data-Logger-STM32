#include "SystemOperations.h"
#include "MySD.h"
#include "stm32f3xx_hal.h"
#include "DataHolder.h"
#include "FileNameIterator.h"
#include "MyStm32Flash.h"
#include <string.h>
#include <stdio.h>

#define STATUS_INIT_FAIL (1 << 0)
#define STATUS_TIMESTAMP (1 << 1)

static uint8_t previousData;
static uint8_t status;
static uint8_t bLogFileIsOpen = 0;
static uint8_t timestampThisLine(uint8_t thisByte);
static char fileName[MAX_FILE_NAME];
static uint8_t isNvmFilenameValid(void);

int SystemOperations_Init(void)
{
    status = STATUS_TIMESTAMP;
    bLogFileIsOpen = 0;
    memset(fileName, 0, sizeof(fileName));
    FileNameIterator_init();
    return SO_SUCCESS;
}   

void SystemOperations_OpenLogFile(void)
{
    uint32_t dataSize = DH_GetOpenedFileSize();
    uint8_t bThereIsNewDataToHandle = DH_IsThereNewData();
    uint8_t bMaxFileSizeReached = (dataSize >= MAX_FILE_SIZE);
    uint8_t bLowerBoundaryFileSizeReached = (dataSize >= FILE_SIZE_LOWER_THRESHOLD);
    uint8_t bOpenNewFileEarly = (bLowerBoundaryFileSizeReached && !bThereIsNewDataToHandle);
    if (!bLogFileIsOpen || bMaxFileSizeReached || bOpenNewFileEarly)
    {
        if (bLogFileIsOpen)
        {
            // generate a new file name
            strncpy(fileName, DH_GetFileName(), MAX_FILE_NAME);
        }
        else
        {
            char data[13 + 4 + 1] = "";
            read_flash_string(FLASH_DATA_PAGE_0, data, 13 + 4 + 1);
            if (strncmp(data, "file", 4) == 0)
            {
                strcpy(fileName, data + 4);
            }
            else
            {
                // generate a new file name
                strncpy(fileName, DH_GetFileName(), MAX_FILE_NAME);
            }
        }
        // open file
        FRESULT err = MySD_Init(fileName);
        if (err == FR_OK)
        {
            bLogFileIsOpen = 1;
            char data[13 + 4 + 1] = "file";
            strcpy(data + 4, fileName);
            write_flash_string(FLASH_DATA_PAGE_0, data);
        }
        else
        {
            bLogFileIsOpen = 0;
        }
    }
}

void SystemOperations_WriteSD(void)
{
    // Write to SD card from a circular buffer
    // Parse each letter one by one and determine if
    // need to also write a timestamp.
    if (DH_IsThereNewData())
    {
        // get top item from circular buffer
        uint8_t val = DH_GetLatestData();
        // parse it (determine if need to timestamp)
        if (timestampThisLine(val))
        {
            // write timestamp
            const char* ts = DH_GetTimestampString();
            MySD_WriteString(ts);
            status &= ~STATUS_TIMESTAMP;
        }
        // write data to SD card
        MySD_Write(&val, 1);
        previousData = val;
    }
}

void SystemOperations_FlushSD(void)
{
    MySD_Flush();
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

static uint8_t isNvmFilenameValid(void)
{
    char data[13 + 4 + 1] = "";
    read_flash_string(FLASH_DATA_PAGE_0, data, 13 + 4 + 1);
    return strncmp(data, "file", 4) == 0;
}
