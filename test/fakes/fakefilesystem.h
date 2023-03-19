#ifndef FAKE_FILESYSTEM_H_INCLUDED
#define FAKE_FILESYSTEM_H_INCLUDED

#include <stddef.h>

void fakefilesystem_open(void);
void fakefilesystem_close(void);
int fakefilesystem_getsize(char* fileName);
int fakefilesystem_isOpen(char* fileName);
void fakefilesystem_openFile(char* fileName);
void fakefilesystem_closeFile(char* fileName);

#endif