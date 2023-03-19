#include "unity.h"
#include "fakeff.h"
#include <string.h>

void setUp(void)
{
    fakeff_init();
}

void tearDown(void)
{
    fakeff_destroy();
}

// Helper functions
static FRESULT helper_failToOpenFile(void);

void test_OpeningFileWithNULLReturnsError(void)
{
    FRESULT result = helper_failToOpenFile();

    TEST_ASSERT_EQUAL(FR_INVALID_PARAMETER, result);
}

void test_OpeningFileWithProperFilePointerReturnsSuccess(void)
{
    FIL file;
    FRESULT result = f_open(&file, "", FA_WRITE);
    TEST_ASSERT_EQUAL(FR_OK, result);
}

void test_AfterInitAllDataIsZero(void)
{
    TEST_ASSERT_EQUAL_INT(0, fakeff_numFilesOpen());
}

void test_FilesOpenIncreasesByOneWhenAFileIsOpened(void)
{
    FIL file;

    f_open(&file, "", FA_WRITE);

    uint32_t numFilesOpen = fakeff_numFilesOpen();

    TEST_ASSERT_EQUAL(1, numFilesOpen);
}

void test_FilesOpenDoesNotIncreaseIfFailed(void)
{
    FIL file;
    
    helper_failToOpenFile();

    int filesOpen = fakeff_numFilesOpen();
    TEST_ASSERT_EQUAL(0, filesOpen);
}

void test_OpeningAnAlreadyOpenFileDoesNotIncrementCounter(void)
{
    FIL file;

    f_open(&file, "", FA_WRITE);
    TEST_ASSERT_EQUAL(FR_DISK_ERR, f_open(&file, "", FA_WRITE));

    int filesOpen = fakeff_numFilesOpen();
    TEST_ASSERT_EQUAL(1, filesOpen);
}

void test_OpeningMultipleFilesIncrementsCounter(void)
{
    FIL file;
    FIL file2;

    f_open(&file, "", FA_WRITE);
    TEST_ASSERT_EQUAL(FR_OK, f_open(&file2, "", FA_WRITE));

    int filesOpen = fakeff_numFilesOpen();
    TEST_ASSERT_EQUAL(2, filesOpen);
}

void test_Write(void)
{
    TEST_IGNORE();

    FIL file;
    char* toWrite = "abcdef";
    UINT bytesWritten = 0;

    f_open(&file, "", FA_WRITE);
    
    TEST_ASSERT_EQUAL(FR_OK, 
        f_write(&file, toWrite, strlen(toWrite), &bytesWritten));

}

// **************************** Helpers ****************************

FRESULT helper_failToOpenFile(void)
{
    return f_open(NULL, "", FA_WRITE);
}


/*
Fake FatFS module
    Simulate an SD card to make it easy to 
    write tests and verify results

From Fatfs:

- [ ] I want to know what files are open (names)
- [ ] I want to know what the data is on my file
- [ ] I want access to the size of the file

FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode); // Open or create a file
- [x] A file can be opened, returns success if non-null file pointer passed in
- [x] Number of files currently open can be ascertained
    - [x] Increases by 1 if opened
    - [x] Doesn't if not opened
- [x] Multiple files can be opened. Current max is two.
- [ ] Number of files opened in total can be ascertained

FRESULT f_close (FIL* fp);											// Close an open file object
- [ ] Closing an open file returns success
- [ ] Closing a file that was not previously opened returns error code
- [ ] Number of files currently open is reduced if success
- [ ] Data is stored in cache and not written to until flush occurs.
- [ ] Flushes cached data

FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	// Write data to the file
- [ ] A closed file cannot be written to
- [ ] Writing to an open file will write data in buff to the file

FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);			// Read data from the file
- [ ] A closed file can not be read from (returns error)
- [ ] Reading from an open file will read its contents: from the beginning each time
- [ ] Can append to end depending on how file is opened
- [ ] Can rewrite everything depending on how file is opened

FRESULT f_sync (FIL* fp);											// Flush cached data of the writing file
- [ ] Flushes cached data so that it can be read with f_read
- [ ] Size is updated accordingly

FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt);			// Mount/Unmount a logical drive
- [ ] Just returns error code that was set

FRESULT

Fake utilities
- [ ] Set error code and all functions will return this error code
- [ ] A copy of the files information will be returned
- [ ] Helper functions can be made?

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