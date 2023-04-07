#include "fakeff.h"
#include <string.h>
#include "fakefilesystem.h"
#include "stdlib.h"

typedef struct FakeFile_t
{
    char internalBuffer[FFS_MAX_FILESIZE];
    int writeIndex;
    char filename[20];
} FakeFile_t;

static struct
{
    State_fakeff_e state;
    int mounted;
} internalState;

static FRESULT getMountError(void);

void fakeff_reset(void)
{
    memset(&internalState, 0, sizeof(internalState));
    fakefilesystem_reset();
}

void fakeff_setState(State_fakeff_e state)
{
    internalState.state = state;
}

FRESULT f_open(FIL* fp, const TCHAR* path, BYTE mode)
{
    // check mount status
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
    // if params are ok, check mode
    if (ret == FR_OK)
    {
        if ((mode & FA_OPEN_APPEND) == FA_OPEN_APPEND)
        {
            fakefilesystem_createFile(path);
        }
        else if ((mode & FA_OPEN_ALWAYS) == FA_OPEN_ALWAYS)
        {
            fakefilesystem_createFile(path);
            fakefilesystem_seek(path, FFS_BEGIN);
        }
        else if ((mode & FA_CREATE_ALWAYS) == FA_CREATE_ALWAYS)
        {
            fakefilesystem_deleteFile(path);
            fakefilesystem_createFile(path);
        }
        else if ((mode & FA_CREATE_NEW) == FA_CREATE_NEW)
        {
            if (fakefilesystem_fileExists(path))
            {
                ret = FR_EXIST;
            }
            else
            {
                fakefilesystem_createFile(path);
            }
        }
        else
        {
            // the default mode, FA_OPEN_EXISTING
            if (fakefilesystem_fileExists(path))
            {
                fakefilesystem_seek(path, FFS_BEGIN);
            }
            else
            {
                ret = FR_NO_FILE;
            }
        }
    }
    // if mode is okay, let's open file for our use
    if (ret == FR_OK)
    {
        fp->flag = mode;
        // create a new file object
        FakeFile_t* thisFile = calloc(1, sizeof(FakeFile_t));
        // clean previous data ?
        memset(&fp->obj, 0, sizeof(fp->obj));
        // do the set up
        fp->obj.fs = (FATFS*)thisFile;
        strcpy(thisFile->filename, path);
    }
    return ret;
}

FRESULT f_mount(FATFS* fs, const TCHAR* path, BYTE opt)
{
    FRESULT ret = FR_OK;
    if (internalState.state == NO_SD_CARD)
    {
        ret = FR_NOT_READY;
    }
    if (ret == FR_OK)
    {
        internalState.mounted = 1;
    }
    return ret;
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
        // clear and ready internal buffer for next time
        memset(thisFile->internalBuffer, 0, FFS_MAX_FILESIZE);
        thisFile->writeIndex = 0;
        // update file size
        fp->obj.objsize = fakefilesystem_fileSize(thisFile->filename);
    }
    return ret;
}

FRESULT f_lseek(FIL* fp, FSIZE_t ofs)
{
    FRESULT ret = getMountError();
    FakeFile_t* thisFile = (FakeFile_t*)fp->obj.fs;
    if (thisFile)
    {
        fakefilesystem_seek(thisFile->filename, ofs);
    }
    return ret;
}

FRESULT f_close (FIL* fp)
{
    return FR_OK;
}

void fakeff_makeApiReturn(FRESULT err)
{

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