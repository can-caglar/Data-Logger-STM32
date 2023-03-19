#include "fakefilesystem.h"
#include <string.h>
#include <stdint.h>

/* Useful defines */

typedef uint8_t bool;
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#define MAX_FILENAME 13
#define MAX_FILESIZE 50
#define MAX_FILES   20

typedef struct
{
    char name[MAX_FILENAME];
    char data[MAX_FILESIZE];
} File_t;

typedef struct
{
    File_t files[MAX_FILES];
} InternalState_t;

InternalState_t internalState;

/* Private functions */
File_t* findFile(const char* fileName);
File_t* findEmptyFileSlot(void);

/* Public function declarations */

void fakefilesystem_open(void)
{
    
}

void fakefilesystem_close(void)
{
    memset(&internalState, 0, sizeof(internalState));
}

int fakefilesystem_getsize(const char* name)
{
    return 0;
}

int fakefilesystem_exists(const char* fileName)
{
    File_t* file = findFile(fileName);
    bool isOpen = false;
    if (file)
    {
        isOpen = true;
    }
    return isOpen;
}

void fakefilesystem_createFile(const char* fileName)
{
    File_t* file = findFile(fileName);
    if (file == NULL)
    {
        file = findEmptyFileSlot();
    }
    strncpy(file->name, fileName, MAX_FILENAME);
}

void fakefilesystem_deleteFile(const char* fileName)
{
    File_t* file = findFile(fileName);
    memset(file, 0, sizeof(*file));
}

const char* fakefilesystem_readfile(const char* fileName)
{
    File_t* file;
    if ( (file = findFile(fileName) ))
    {
        return file->data;
    }
    return NULL;
}

void fakefilesystem_writeFile(const char* fileName, const char* data)
{
    if (fakefilesystem_exists(fileName))
    {
        File_t* file = findFile(fileName);
        strcpy(file->data, data);
    }
}

/* Private function definitions */

File_t* findFile(const char* fileName)
{
    File_t* file = NULL;
    for (int i = 0; i < MAX_FILES; i++)
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
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (strcmp(internalState.files[i].name, "") == 0)
        {
            file = &internalState.files[i];
            break;
        }
    }
    return file;
}