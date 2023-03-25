#include "fakeff.h"
#include <string.h>
#include "fakefilesystem.h"

static struct
{
    int mounted;
    char internalBuffer[200];
    int bufCount;
    char filename[20];
} internalState;

void fakeff_reset(void)
{
    memset(&internalState, 0, sizeof(internalState));
}

FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode)
{
    FRESULT ret = FR_OK;
    if (!internalState.mounted)
    {
        ret = FR_INVALID_DRIVE;
    }
    if (ret == FR_OK)
    {
        if (fp == NULL || path == NULL || 
            (strcmp(path, "") == 0))
        {
            ret = FR_INVALID_PARAMETER;
        }
    }
    if (ret == FR_OK)
    {
        fp->flag = mode;
        strcpy(internalState.filename, path);
        fakefilesystem_createFile(path);
    }
    return ret;
}

FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt)
{
    internalState.mounted = 1;
    return FR_OK;
}

FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw)
{
    if (fp->flag & FA_WRITE)
    {
        *bw = btw;
        strncpy(internalState.internalBuffer,
            buff, btw);
        return FR_OK;
    }
    return FR_DENIED;
}

FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br)
{
    const char* data = fakefilesystem_readfile(internalState.filename);
    strncpy(buff, data, btr);
    *br = btr;
    return FR_OK;
}

FRESULT f_sync (FIL* fp)
{
    fakefilesystem_writeFile(internalState.filename,
        internalState.internalBuffer);
    return FR_OK;
}