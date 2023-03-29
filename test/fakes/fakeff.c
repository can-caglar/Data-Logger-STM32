#include "fakeff.h"
#include <string.h>
#include "fakefilesystem.h"
#include "stdlib.h"

typedef struct FakeFile_t
{
    FIL* fp;
    char internalBuffer[200];
    int writeIndex;
    char filename[20];
} FakeFile_t;

static struct
{
    int mounted;
} internalState;

static FRESULT getMountError(void);

void fakeff_reset(void)
{
    memset(&internalState, 0, sizeof(internalState));
    fakefilesystem_reset();
}

FRESULT f_open(FIL* fp, const TCHAR* path, BYTE mode)
{
    FRESULT ret = getMountError();
    // check parameters
    if (ret == FR_OK)
    {
        if (fp == NULL || path == NULL || 
            (strcmp(path, "") == 0))
        {
            ret = FR_INVALID_PARAMETER;
        }
    }
    // check if file is already open
    if (ret == FR_OK)
    {
        if (fakefilesystem_fileExists(path))
        {
            ret = FR_INVALID_OBJECT;
        }
    }
    // all is good, let's create the file
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
    FRESULT ret = getMountError();
    if (ret == FR_OK)
    {
        // check mode
        if ((fp->flag & FA_WRITE) == 0)
        {
            ret = FR_DENIED;
            *bw = 0;
        }
    }
    if (ret == FR_OK)
    {
        // all is good, let's write
        *bw = btw;
        FakeFile_t* thisFile = (FakeFile_t*)fp->obj.fs;
        strncpy(thisFile->internalBuffer + thisFile->writeIndex, 
            buff, btw);
        thisFile->writeIndex += btw;
    }
    return ret;
}

FRESULT f_read(FIL* fp, void* buff, UINT btr, UINT* br)
{
    FRESULT ret = getMountError();
    if (ret == FR_OK)
    {
        if ((fp->flag & FA_READ) == 0)
        {
            ret = FR_DENIED;
            *br = 0;
        }
    }
    if (ret == FR_OK)
    {
        FakeFile_t* thisFile = (FakeFile_t*)fp->obj.fs;
        const char* data = fakefilesystem_readfile(thisFile->filename);
        strncpy(buff, data, btr);
        *br = strlen(data);
    }
    return ret;
}

FRESULT f_sync(FIL* fp)
{
    FRESULT ret = getMountError();
    if (ret == FR_OK)
    {
        FakeFile_t* thisFile = (FakeFile_t*)fp->obj.fs;
        fakefilesystem_writeFile(thisFile->filename,
            thisFile->internalBuffer);
    }
    return ret;
}

/* Static functions */

static FRESULT getMountError(void)
{
    FRESULT ret = FR_OK;
    // check if file system is mounted
    if (!internalState.mounted)
    {
        ret = FR_INVALID_DRIVE;
    }
    return ret;
}