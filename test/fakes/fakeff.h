#ifndef FAKE_FATFS_H_INCLUDED
#define FAKE_FATFS_H_INCLUDED

#include "ff.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_OPEN_FILES 20
#define MAX_FILE_SIZE 200

FRESULT fatfs_test_double_initialize(void);
bool fatfs_test_double_is_mounted(void);

#endif