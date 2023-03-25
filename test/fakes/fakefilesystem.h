#ifndef FAKEFILESYSTEM_H
#define FAKEFILESYSTEM_H

#include <stdbool.h>

void fakefilesystem_reset(void);
bool fakefilesystem_fileExists(const char *name);
void fakefilesystem_createFile(const char *name);
void fakefilesystem_deleteFile(const char *name);
const char *fakefilesystem_readfile(const char *name);
void fakefilesystem_writeFile(const char *name, const char *data);
int fakefilesystem_fileSize(const char *name);

#endif // FAKEFILESYSTEM_H
