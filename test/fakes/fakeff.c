#include "fakeff.h"
#include <string.h>
#include <stdint.h>

// Data structure for a single file
typedef struct
{
    FIL* fp;
    char fileData[MAX_FILE_SIZE];
} FakeFile_t;

// Data structure for module
typedef struct
{
    uint32_t numFilesOpen;
    FakeFile_t files[MAX_OPEN_FILES];
} Fakeff_t;

static Fakeff_t internalState;

// Helpers
static uint8_t fileAlreadyOpen(FIL* fp);
static void openFile(FIL* fp);
static int getFileIndex(FIL* fp);

void fakeff_init(void)
{
    memset(&internalState, 0, sizeof(Fakeff_t));
}

void fakeff_destroy(void)
{

}

uint32_t fakeff_numFilesOpen(void)
{
    return internalState.numFilesOpen;
}

FRESULT f_open(FIL* fp, const TCHAR* path, BYTE mode)
{
    FRESULT err = FR_INVALID_PARAMETER;
    if (fp != NULL)
    {
        err = FR_OK;
        if (!fileAlreadyOpen(fp))
        {
            openFile(fp);
        }
        else
        {
            err = FR_DISK_ERR;
        }
    }
    return err;
}

FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw)
{
    int index = getFileIndex(fp);
    strncpy(internalState.files[index].fileData, buff, btw);
    return FR_OK;
}

Fakeff_t fakeff_getInternalState(void)
{
    return internalState;
}

/******* Helpers ********/

uint8_t fileAlreadyOpen(FIL* fp)
{
    uint8_t open = 0;
    if (getFileIndex(fp) > -1)
    {
        open = 1;
    }
    return open;
}

void openFile(FIL* fp)
{
    internalState.files[internalState.numFilesOpen++].fp = fp;
}

static int getFileIndex(FIL* fp)
{
    int index = -1;
    for (int i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (internalState.files[i].fp == fp)
        {
            index = i;
            break;
        }
    }
    return index;
}