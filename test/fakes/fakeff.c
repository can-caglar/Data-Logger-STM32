#include "fakeff.h"
#include <string.h>
#include "fakefilesystem.h"
#include "stdlib.h"

typedef struct FakeFile_t
{
    FIL* fp;
    char internalBuffer[200];
    int bufCount;
    char filename[20];
} FakeFile_t;

static struct
{
    int mounted;
} internalState;

void fakeff_reset(void)
{
    memset(&internalState, 0, sizeof(internalState));
    fakefilesystem_reset();
}

FRESULT f_open(FIL* fp, const TCHAR* path, BYTE mode)
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
        // create a new file object
        FakeFile_t* thisFile = calloc(1, sizeof(FakeFile_t));
        fp->obj.fs = (FATFS*)thisFile;
        strcpy(thisFile->filename, path);
        fakefilesystem_createFile(thisFile->filename);
    }
    return ret;
}

FRESULT f_mount(FATFS* fs, const TCHAR* path, BYTE opt)
{
    internalState.mounted = 1;
    return FR_OK;
}

FRESULT f_write(FIL* fp, const void* buff, UINT btw, UINT* bw)
{
    if (fp->flag & FA_WRITE)
    {
        *bw = btw;
        FakeFile_t* thisFile = (FakeFile_t*)fp->obj.fs;
        strncpy(thisFile->internalBuffer,
            buff, btw);
        return FR_OK;
    }
    *bw = 0;
    return FR_DENIED;
}

FRESULT f_read(FIL* fp, void* buff, UINT btr, UINT* br)
{
    FakeFile_t* thisFile = (FakeFile_t*)fp->obj.fs;
    const char* data = fakefilesystem_readfile(thisFile->filename);
    strncpy(buff, data, btr);
    *br = strlen(data);
    return FR_OK;
}

FRESULT f_sync(FIL* fp)
{
    FakeFile_t* thisFile = (FakeFile_t*)fp->obj.fs;
    fakefilesystem_writeFile(thisFile->filename,
        thisFile->internalBuffer);
    return FR_OK;
}