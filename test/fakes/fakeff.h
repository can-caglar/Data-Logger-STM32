#ifndef FAKE_FATFS_H_INCLUDED
#define FAKE_FATFS_H_INCLUDED

#include "ff.h"
#include <stdint.h>

#define MAX_OPEN_FILES 20
#define MAX_FILE_SIZE 200

void fakeff_init(void);
uint32_t fakeff_numFilesOpen(void);
void fakeff_destroy(void);

#endif