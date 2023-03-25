#include "unity.h"
#include "fakeff.h"
#include <string.h>

// included only to make ceedling compile this dependency
// do not use in tests to prevent tests coupling to fakefilesystem internals
#include "fakefilesystem.h"

#define DONT_CARE   0

void setUp(void)
{
    fakeff_reset();
}

void tearDown(void)
{
}

void test_openReturnsErrorIfNotMounted(void)
{
    FIL myFile;
    TEST_ASSERT_EQUAL_INT(FR_INVALID_DRIVE, f_open(&myFile, "0:", FA_READ));
}

void test_openReturnsErrorIfMountedButNULLFilePointerPassedIn(void)
{
    FATFS fatfs;
    TEST_ASSERT_EQUAL_INT(FR_OK, f_mount(&fatfs, "0:", DONT_CARE));

    TEST_ASSERT_EQUAL_INT(FR_INVALID_PARAMETER, f_open(NULL, "path", FA_READ));
}

void test_openReturnsErrorIfMountedAndValidFilePointerButNULLPath(void)
{
    FATFS fatfs;
    FIL myFile;
    f_mount(&fatfs, "0:", DONT_CARE);

    TEST_ASSERT_EQUAL_INT(FR_INVALID_PARAMETER, f_open(&myFile, NULL, FA_READ));
}

void test_openReturnsErrorIfMountedAndValidFilePointerButBadPath(void)
{
    FATFS fatfs;
    FIL myFile;
    f_mount(&fatfs, "0:", DONT_CARE);

    TEST_ASSERT_EQUAL_INT(FR_INVALID_PARAMETER, f_open(&myFile, "", FA_READ));
}

void test_openSuccessIfMountedAndValidFilePointerAndPathPassedIn(void)
{
    FATFS fatfs;
    FIL myFile;
    f_mount(&fatfs, "0:", DONT_CARE);

    TEST_ASSERT_EQUAL_INT(FR_OK, f_open(&myFile, "abc", FA_READ));
}

void test_OpenFileCannotBeWrittenToUnlessWriteOptionEnabled(void)
{
    FATFS fatfs;
    FIL myFile = { 0 };
    char myBuff[20] = {'a', 'b'};
    char myBuffRead[20] = { 0 };
    memset(myBuffRead, 'X', 19);
    UINT byteCount;

    f_mount(&fatfs, "0:", DONT_CARE);
    f_open(&myFile, "abc", FA_READ);

    TEST_ASSERT_EQUAL_INT(FR_DENIED,
        f_write(&myFile, myBuff, 20, &byteCount));
    
    TEST_ASSERT_EQUAL_INT(0, byteCount);
    
    TEST_ASSERT_EQUAL_INT(FR_OK,
        f_read(&myFile, myBuffRead, 20, &byteCount));
    
    TEST_ASSERT_EQUAL_STRING("", myBuffRead);
}

void test_OpenFileISWrittenToWhenWriteOptionIsEnabled(void)
{
    FATFS fatfs = { 0 };
    FIL myFile = { 0 };
    char myBuff[20] = {'a', 'b'};
    char myBuffRead[20] = { 0 };
    memset(myBuffRead, 'X', 19);
    UINT byteCount;

    f_mount(&fatfs, "0:", DONT_CARE);
    f_open(&myFile, "abc", FA_READ | FA_WRITE);

    TEST_ASSERT_EQUAL_INT(FR_OK,
        f_write(&myFile, myBuff, 20, &byteCount));
    
    TEST_ASSERT_EQUAL_INT(20, byteCount);
    byteCount = 0;

    TEST_ASSERT_EQUAL_INT(FR_OK,
        f_sync(&myFile));
    
    TEST_ASSERT_EQUAL_INT(FR_OK,
        f_read(&myFile, myBuffRead, 20, &byteCount));

    TEST_ASSERT_EQUAL_INT(20, byteCount);
    
    TEST_ASSERT_EQUAL_STRING("ab", myBuffRead);
}

void test_openingSameFileTwiceReturnsError(void)
{
    // not sure exactly how fatfs handles this,
    // but we won't allow it
    TEST_IGNORE();
}

void test_writingTwiceToSameFile(void)
{
    TEST_IGNORE();
}

void test_writingToTwoFilesOpenAtSameTime(void)
{
    TEST_IGNORE();
}

void test_writingVeryLongMessageToFile(void)
{
    TEST_IGNORE();
}



/*
Fake FatFS module
    Simulate the FATFS module make it easy to 
    write tests and verify results.

    - Returns error codes as FatFS would do
    - Writes / reads files

From Fatfs:

FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt);			// Mount/Unmount a logical drive
- [...] If this is not called first, all other implemented functions in API return FR_INVALID_DRIVE

FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode); // Open or create a file
- [x] Return error if not mounted. FR_INVALID_DRIVE
- [x] Return error if null fp passed in. FR_INVALID_OBJECT
- [x] Return error if invalid path. FR_INVALID_DRIVE
- [x] Return success if non-null fp passed in
- [x] Return success if valid file path
- [ ] Opening same file twice

FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);			// Read data from the file
- [ ] Reading a file before it is opened returns FR_NOT_OPENED
- [ ] Reading a file that was not opened with FA_READ access code returns FR_DENIED
- [ ] Reading a file that is opened with correct access code returns data in file

FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	// Write data to the file
- [ ] Writing to a file before it is opened returns FR_NOT_OPENED
- [ ] Writing to a file before it is opened does not write anything
- [ ] Writing to an open file will write data at end of the file
- [ ] Writing to data to a file opened to overwrite will overwrite
- [ ] Writing to a file without correct access code returns FR_DENIED
    and doesn't write anything
- [ ] Writing only works if after a flush (f_sync)
 
FRESULT f_sync (FIL* fp);											// Flush cached data of the writing file
- [ ] 

Maybe implement:

FRESULT f_close (FIL* fp);											// Close an open file object

Extras:

Very large files,
Invalid names
Invalid paths

Unimplemented:

FRESULT f_lseek (FIL* fp, FSIZE_t ofs);								// Move file pointer of the file object
FRESULT f_truncate (FIL* fp);										// Truncate the file
FRESULT f_opendir (DIR* dp, const TCHAR* path);						// Open a directory
FRESULT f_closedir (DIR* dp);										// Close an open directory
FRESULT f_readdir (DIR* dp, FILINFO* fno);							// Read a directory item
FRESULT f_findfirst (DIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern);	// Find first file
FRESULT f_findnext (DIR* dp, FILINFO* fno);							// Find next file
FRESULT f_mkdir (const TCHAR* path);								// Create a sub directory
FRESULT f_unlink (const TCHAR* path);								// Delete an existing file or directory
FRESULT f_rename (const TCHAR* path_old, const TCHAR* path_new);	// Rename/Move a file or directory
FRESULT f_stat (const TCHAR* path, FILINFO* fno);					// Get file status
FRESULT f_chmod (const TCHAR* path, BYTE attr, BYTE mask);			// Change attribute of a file/dir
FRESULT f_utime (const TCHAR* path, const FILINFO* fno);			// Change timestamp of a file/dir
FRESULT f_chdir (const TCHAR* path);								// Change current directory
FRESULT f_chdrive (const TCHAR* path);								// Change current drive
FRESULT f_getcwd (TCHAR* buff, UINT len);							// Get current directory
FRESULT f_getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs);	// Get number of free clusters on the drive
FRESULT f_getlabel (const TCHAR* path, TCHAR* label, DWORD* vsn);	// Get volume label
FRESULT f_setlabel (const TCHAR* label);							// Set volume label
FRESULT f_forward (FIL* fp, UINT(*func)(const BYTE*,UINT), UINT btf, UINT* bf);	// Forward data to the stream
FRESULT f_expand (FIL* fp, FSIZE_t szf, BYTE opt);					// Allocate a contiguous block to the file
FRESULT f_mkfs (const TCHAR* path, BYTE opt, DWORD au, void* work, UINT len);	// Create a FAT volume
FRESULT f_fdisk (BYTE pdrv, const DWORD* szt, void* work);			// Divide a physical drive into some partitions
int f_putc (TCHAR c, FIL* fp);										// Put a character to the file
int f_puts (const TCHAR* str, FIL* cp);								// Put a string to the file
int f_printf (FIL* fp, const TCHAR* str, ...);						// Put a formatted string to the file
TCHAR* f_gets (TCHAR* buff, int len, FIL* fp);						// Get a string from the file


*/