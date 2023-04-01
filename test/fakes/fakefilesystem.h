#ifndef FAKE_FILESYSTEM_H_INCLUDED
#define FAKE_FILESYSTEM_H_INCLUDED

#include <stddef.h>

#define FFS_MAX_FILENAME 13
#define FFS_MAX_FILESIZE 5000
#define FFS_MAX_FILES   20

#define FFS_BEGIN 0
#define FFS_END -1

void fakefilesystem_reset(void);
int fakefilesystem_fileExists(const char* fileName);
int fakefilesystem_fileSize(const char* fileName);
void fakefilesystem_createFile(const char* fileName);
void fakefilesystem_deleteFile(const char* fileName);
const char* fakefilesystem_readfile(const char* fileName);
void fakefilesystem_writeFile(const char* fileName, const char* data);
void fakefilesystem_seek(const char* fileName, int pos);

#endif