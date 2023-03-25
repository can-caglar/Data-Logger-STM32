#include "ff.h"
#include "fakefilesystem.h"
#include <string.h>
#include <stdlib.h>
#include "fakeff.h"

static bool mounted = false;

FRESULT fatfs_test_double_initialize(void)
{
    fakefilesystem_reset();
    mounted = false;
    return FR_OK;
}

bool fatfs_test_double_is_mounted(void)
{
    return mounted;
}

// Provide a new implementation for f_mount()
FRESULT f_mount(FATFS* fs, const TCHAR* path, BYTE opt)
{
    (void) fs; // Unused in this implementation, included to match signature.

    if (opt == 1) // Mount
    {
        if (mounted)
        {
            return FR_INVALID_PARAMETER;
        }
        mounted = true;
        return FR_OK;
    }
    else if (opt == 0) // Unmount
    {
        if (!mounted)
        {
            return FR_INVALID_PARAMETER;
        }
        mounted = false;
        return FR_OK;
    }

    return FR_INVALID_PARAMETER;
}
