#include "fakefilesystem.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct File {
    char *name;
    char *data;
    int size;
} File;

#define MAX_FILES 100

static File files[MAX_FILES];
static int fileCount = 0;

void fakefilesystem_reset(void) 
{
    for (int i = 0; i < fileCount; i++) {
        free(files[i].name);
        free(files[i].data);
        files[i].name = NULL;
        files[i].data = NULL;
    }
    fileCount = 0;
}

int fakefilesystem_findFile(const char *name) {
    for (int i = 0; i < fileCount; i++) {
        if (files[i].name != NULL && strcmp(files[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

bool fakefilesystem_fileExists(const char *name) {
    return fakefilesystem_findFile(name) != -1;
}

void fakefilesystem_createFile(const char *name) {
    if (fakefilesystem_fileExists(name)) {
        return;
    }

    int index = fileCount++;
    files[index].name = strdup(name);
    files[index].data = calloc(1, 1);
    files[index].size = 0;
}

void fakefilesystem_deleteFile(const char *name) {
    int index = fakefilesystem_findFile(name);
    if (index == -1) {
        return;
    }

    free(files[index].name);
    free(files[index].data);

    // Remove the file from the array and adjust file count
    memmove(&files[index], &files[index + 1], (fileCount - index - 1) * sizeof(File));
    fileCount--;
}

const char *fakefilesystem_readfile(const char *name) {
    int index = fakefilesystem_findFile(name);
    if (index == -1) {
        return NULL;
    }

    return files[index].data;
}

void fakefilesystem_writeFile(const char *name, const char *data) {
    int index = fakefilesystem_findFile(name);
    if (index == -1) {
        return;
    }

    int dataSize = strlen(data);
    files[index].data = realloc(files[index].data, files[index].size + dataSize + 1);
    memcpy(files[index].data + files[index].size, data, dataSize + 1);
    files[index].size += dataSize;
    files[index].data[files[index].size] = '\0';
}

int fakefilesystem_fileSize(const char *name) {
    int index = fakefilesystem_findFile(name);
    if (index == -1) {
        return 0;
    }

    return files[index].size;
}
