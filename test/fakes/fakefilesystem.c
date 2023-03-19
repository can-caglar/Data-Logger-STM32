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
#define MAX_FILES   20

typedef struct
{
    char name[MAX_FILENAME];
} File_t;

typedef struct
{
    File_t files[MAX_FILES];
} InternalState_t;

InternalState_t internalState;

/* Private functions */
File_t* findFile(char* fileName);
File_t* findEmptyFileSlot(void);

/* Public function declarations */

void fakefilesystem_open(void)
{
    
}

void fakefilesystem_close(void)
{
    memset(&internalState, 0, sizeof(internalState));
}

int fakefilesystem_getsize(char* name)
{
    return 0;
}

int fakefilesystem_isOpen(char* fileName)
{
    bool isOpen = false;
    if (findFile(fileName))
    {
        isOpen = true;
    }
    return isOpen;
}

void fakefilesystem_openFile(char* fileName)
{
    File_t* emptyFile = findEmptyFileSlot();
    strncpy(emptyFile->name, fileName, MAX_FILENAME);
}

void fakefilesystem_closeFile(char* fileName)
{
    File_t* file = findFile(fileName);
    memset(file->name, 0, MAX_FILENAME);
}

/* Private function definitions */

File_t* findFile(char* fileName)
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