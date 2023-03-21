#ifndef FAKE_FILESYSTEM_H_INCLUDED
#define FAKE_FILESYSTEM_H_INCLUDED

#include <stddef.h>

#define FFS_MAX_FILENAME 13
#define FFS_MAX_FILESIZE 50
#define FFS_MAX_FILES   20

void fakefilesystem_reset(void);
int fakefilesystem_fileExists(const char* fileName);
int fakefilesystem_fileSize(const char* fileName);
void fakefilesystem_createFile(const char* fileName);
void fakefilesystem_deleteFile(const char* fileName);
const char* fakefilesystem_readfile(const char* fileName);
void fakefilesystem_writeFile(const char* fileName, const char* data);

#endif