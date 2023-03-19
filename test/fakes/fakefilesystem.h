#ifndef FAKE_FILESYSTEM_H_INCLUDED
#define FAKE_FILESYSTEM_H_INCLUDED

#include <stddef.h>

void fakefilesystem_open(void);
void fakefilesystem_close(void);
int fakefilesystem_getsize(const char* fileName);
int fakefilesystem_isOpen(const char* fileName);
void fakefilesystem_openFile(const char* fileName);
void fakefilesystem_closeFile(const char* fileName);
const char* fakefilesystem_readfile(const char* fileName);
void fakefilesystem_writeFile(const char* fileName, const char* data);

#endif