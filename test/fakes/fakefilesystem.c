#include "fakefilesystem.h"
#include <string.h>
#include <stdint.h>

typedef uint8_t bool;
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

typedef struct File_t
{
    char name[FFS_MAX_FILENAME];
    char data[FFS_MAX_FILESIZE];
    int fileSize;
} File_t;
typedef struct InternalState_t
{
    File_t files[FFS_MAX_FILES];
} InternalState_t;

// Module variables
static InternalState_t internalState;

/* Private functions */

File_t* findFile(const char* fileName);
File_t* findEmptyFileSlot(void);

/* Public function definitions */

void fakefilesystem_reset(void)
{
    memset(&internalState, 0, sizeof(internalState));
}

int fakefilesystem_fileExists(const char* fileName)
{
    return findFile(fileName) != NULL;
}

int fakefilesystem_fileSize(const char* name)
{
    int size = 0;
    File_t* file = findFile(name);
    if (file)
    {
        size = file->fileSize;
    }
    return size;
}

void fakefilesystem_createFile(const char* fileName)
{
    File_t* file = findFile(fileName);
    if (file == NULL)
    {
        file = findEmptyFileSlot();
        if (file)
        {
            strncpy(file->name, fileName, FFS_MAX_FILENAME);
        }
    }
}

void fakefilesystem_deleteFile(const char* fileName)
{
    File_t* file = findFile(fileName);
    memset(file, 0, sizeof(*file));
}

const char* fakefilesystem_readfile(const char* fileName)
{
    File_t* file = findFile(fileName);
    if (file)
    {

        return file->data;
    }
    return NULL;
}

void fakefilesystem_writeFile(const char* fileName, const char* data)
{
    File_t* file = findFile(fileName);
    if (file != NULL)
    {
        char* writeIndex = file->data + file->fileSize;
        strcpy(writeIndex, data);
        file->fileSize = strlen(file->data);
    }
}

/* Private function definitions */

File_t* findFile(const char* fileName)
{
    File_t* file = NULL;
    for (int i = 0; i < FFS_MAX_FILES; i++)
    {
        if (strcmp(fileName, internalState.files[i].name) == 0)
        {
            file = &internalState.files[i];
            break;
        }
    }
    return file;
}

File_t* findEmptyFileSlot(void)
{
    File_t* file = NULL;
    int i;
    for (i = 0; i < FFS_MAX_FILES; i++)
    {
        if (strcmp(internalState.files[i].name, "") == 0)
        {
            file = &internalState.files[i];
            break;
        }
    }
    return file;
}