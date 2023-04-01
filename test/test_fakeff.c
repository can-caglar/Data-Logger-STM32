#include "unity.h"
#include "fakeff.h"
#include <string.h>
#include <stdbool.h>
#include "fakefilesystem.h"

// Test vars
FIL fileHandle;
FATFS fatfsHandle;
const char const writeBuf[] = "abcdefghijklmnopqrstuvwxyz";
char readBuf[2000];

#define DONT_CARE   0
#define MOUNT(x) f_mount(&(x), "0:", DONT_CARE)
#define MOUNT_FATFS()   MOUNT(fatfsHandle)

FRESULT call_f_open_with(bool isMounted, bool goodFileFP, bool goodPath);
FRESULT f_write_with_sync(FIL* fp, const void* buff, UINT btw, UINT* bw);

void setUp(void)
{
    fakeff_reset();
    memset(&fileHandle, 0, sizeof(fileHandle));
    memset(&fatfsHandle, 0, sizeof(fatfsHandle));
    memset(readBuf, 0, sizeof(readBuf));
}

void tearDown(void)
{
}

void test_mountIsAlwaysSuccessful(void)
{
    TEST_ASSERT_EQUAL_INT(FR_OK, MOUNT(fatfsHandle));
}

void test_openReturnsErrorIfNotMounted(void)
{
    TEST_ASSERT_EQUAL_INT(FR_INVALID_DRIVE, call_f_open_with(false, true, true));
}

void test_openReturnsErrorIfMountedButNULLFilePointerPassedIn(void)
{
    TEST_ASSERT_EQUAL_INT(FR_INVALID_PARAMETER, call_f_open_with(true, false, true));
}

void test_openReturnsErrorIfMountedAndValidFilePointerButNULLPath(void)
{
    TEST_ASSERT_EQUAL_INT(FR_INVALID_PARAMETER, call_f_open_with(true, true, false));
}

void test_openReturnsErrorIfMountedAndValidFilePointerButEmptyPath(void)
{
    MOUNT_FATFS();

    TEST_ASSERT_EQUAL_INT(FR_INVALID_PARAMETER, f_open(&fileHandle, "", FA_READ));
}

void test_openSuccessIfMountedAndValidFilePointerAndPathPassedIn(void)
{
    TEST_ASSERT_EQUAL_INT(FR_OK, call_f_open_with(true, true, true));
}

void test_OpenFileCannotBeWrittenToUnlessWriteOptionEnabled(void)
{
    UINT byteCount = 0xAA;  // so we know if it changes

    MOUNT_FATFS();
    f_open(&fileHandle, "abc", FA_READ | FA_OPEN_APPEND);

    TEST_ASSERT_EQUAL_INT(FR_DENIED,
        f_write(&fileHandle, writeBuf, 20, &byteCount));

    TEST_ASSERT_EQUAL_INT(0, byteCount);
    byteCount = 0xAA;

    TEST_ASSERT_EQUAL_INT(FR_OK,
        f_read(&fileHandle, readBuf, 20, &byteCount));

    TEST_ASSERT_EQUAL_INT(0, byteCount);

    TEST_ASSERT_EQUAL_STRING("", readBuf);
}

void test_OpenFileISWrittenToWhenWriteOptionIsEnabled(void)
{
    UINT byteCount;
    MOUNT_FATFS();

    f_open(&fileHandle, "file", FA_READ | FA_WRITE | FA_OPEN_APPEND);

    TEST_ASSERT_EQUAL_INT(FR_OK,
        f_write(&fileHandle, "abcde", 5, &byteCount));
    TEST_ASSERT_EQUAL_INT(5, byteCount);

    byteCount = 0xAA;

    TEST_ASSERT_EQUAL_INT(FR_OK,
        f_sync(&fileHandle));

    f_lseek(&fileHandle, 0);

    TEST_ASSERT_EQUAL_INT(FR_OK,
        f_read(&fileHandle, readBuf, 5, &byteCount));
    TEST_ASSERT_EQUAL_INT(5, byteCount);

    TEST_ASSERT_EQUAL_STRING("abcde", readBuf);
}

void test_writingMultipleTimesAppendsToEndOfFile(void)
{
    MOUNT_FATFS();

    UINT byteCount = 0xAA;
    f_open(&fileHandle, "path", FA_READ | FA_WRITE | FA_OPEN_APPEND);
    f_write(&fileHandle, "hello", 5, &byteCount);
    TEST_ASSERT_EQUAL_INT(5, byteCount);

    byteCount = 0xAA;
    TEST_ASSERT_EQUAL_INT(FR_OK,
        f_write_with_sync(&fileHandle, " world!", 7, &byteCount));
    TEST_ASSERT_EQUAL_INT(7, byteCount);

    f_lseek(&fileHandle, 0);

    byteCount = 0xAA;
    f_read(&fileHandle, readBuf, (7 + 5), &byteCount);
    TEST_ASSERT_EQUAL_INT(7 + 5, byteCount);

    TEST_ASSERT_EQUAL_STRING("hello world!", readBuf);
}

void test_callingAnyOfTheApiBeforeMountShouldReturnError(void)
{
    UINT byteCount = 0;

    TEST_ASSERT_EQUAL_INT(FR_INVALID_DRIVE,
        f_write(&fileHandle, "test", 7, &byteCount));

    TEST_ASSERT_EQUAL_INT(FR_INVALID_DRIVE,
        f_read(&fileHandle, readBuf, 7, &byteCount));

    TEST_ASSERT_EQUAL_INT(FR_INVALID_DRIVE,
        f_sync(&fileHandle));

    TEST_ASSERT_EQUAL_INT(FR_INVALID_DRIVE,
        f_lseek(&fileHandle, 0));

    // f_open is tested thoroughly in the above tests
}

void test_readingWithoutCorrectModeReturnsDeniedError(void)
{
    UINT byteCount = 0xAA;

    MOUNT_FATFS();

    f_open(&fileHandle, "myfile", FA_WRITE | FA_OPEN_APPEND);

    TEST_ASSERT_EQUAL_INT(FR_DENIED,
        f_read(&fileHandle, readBuf, 10, &byteCount));
    TEST_ASSERT_EQUAL_INT(0, byteCount);
    TEST_ASSERT_EQUAL_STRING("", readBuf);
}

void test_writingToTwoFilesOpenAtSameTime(void)
{
    MOUNT_FATFS();

    FIL fp2 = { 0 };
    UINT byteCount = 0; // not being used

    f_open(&fileHandle, "path1", FA_READ | FA_WRITE | FA_OPEN_APPEND);
    f_open(&fp2, "path2", FA_READ | FA_WRITE | FA_OPEN_APPEND);

    f_write_with_sync(&fileHandle, "writing to file 1", 17, &byteCount);
    f_write_with_sync(&fp2, "writing to file 2", 17, &byteCount);

    f_lseek(&fileHandle, 0);
    f_lseek(&fp2, 0);

    f_read(&fileHandle, readBuf, 17, &byteCount);
    TEST_ASSERT_EQUAL_STRING("writing to file 1", readBuf);
    f_read(&fp2, readBuf, 17, &byteCount);
    TEST_ASSERT_EQUAL_STRING("writing to file 2", readBuf);
}

void test_abletoReadAndWriteLongMessagesToAndFromFile(void)
{
    MOUNT_FATFS();
    UINT byteCount = 0xAA;

    const uint32_t LONG_MSG_LEN = 1000;
    char longMessage[LONG_MSG_LEN];
    memset(longMessage, 'x', LONG_MSG_LEN);
    longMessage[LONG_MSG_LEN] = '\0';

    f_open(&fileHandle, "path1", FA_READ | FA_WRITE | FA_OPEN_APPEND);
    f_write_with_sync(&fileHandle, longMessage, LONG_MSG_LEN, &byteCount);
    TEST_ASSERT_EQUAL_INT(LONG_MSG_LEN, byteCount);
    byteCount = 0xAA;   // reset it

    f_lseek(&fileHandle, 0);

    f_read(&fileHandle, readBuf, LONG_MSG_LEN, &byteCount);
    TEST_ASSERT_EQUAL_INT(LONG_MSG_LEN, byteCount);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(longMessage, readBuf, LONG_MSG_LEN);
}

void test_modeFlags_OpenExistingOnlyOpensIfExistingLeavesFilePointerAtBeginning(void)
{
    MOUNT_FATFS();
    UINT byteCount = 0xAA;

    // when file does not exist:

    FRESULT err = f_open(&fileHandle, "newfile",
        FA_READ | FA_OPEN_EXISTING);

    TEST_ASSERT_EQUAL_INT(FR_NO_FILE, err);
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("newfile"));

    // when file exists:
    fakefilesystem_createFile("exists");
    fakefilesystem_writeFile("exists", "hello");

    err = f_open(&fileHandle, "exists",
        FA_READ | FA_OPEN_EXISTING);

    TEST_ASSERT_EQUAL_INT(FR_OK, err);
    f_read(&fileHandle, readBuf, 5, &byteCount);
    TEST_ASSERT_EQUAL_STRING("hello", readBuf);
}

void test_modeFlags_CreateNewCreatesNewFileOnlyIfItDoesntExist(void)
{
    MOUNT_FATFS();

    UINT byteCount = 0;

    // when file exists:
    fakefilesystem_createFile("fileexists");

    FRESULT err = f_open(&fileHandle, "fileexists", FA_WRITE | FA_CREATE_NEW);

    TEST_ASSERT_EQUAL_INT(FR_EXIST, err);
    TEST_ASSERT_EQUAL_INT(FR_DENIED, f_write(&fileHandle, "abc", 3, &byteCount));

    // when file does not exist:
    err = f_open(&fileHandle, "filenotexist", FA_WRITE | FA_CREATE_NEW);

    TEST_ASSERT_EQUAL_INT(err, FR_OK);
    TEST_ASSERT_EQUAL_INT(FR_OK, f_write(&fileHandle, "abc", 3, &byteCount));
}

void test_modeFlags_CreateAlwaysWillAlwaysCreateFileAndTruncateIt(void)
{
    MOUNT_FATFS();

    UINT byteCount = 0xAA;

    // when file exists:
    fakefilesystem_createFile("fileexists");
    fakefilesystem_writeFile("fileexists", "hello!");

    FRESULT err = f_open(&fileHandle, "fileexists",
        FA_READ | FA_WRITE | FA_CREATE_ALWAYS);

    TEST_ASSERT_EQUAL_INT(FR_OK, err);

    f_read(&fileHandle, readBuf, 10, &byteCount);
    TEST_ASSERT_EQUAL_STRING("", readBuf);  // truncated

    f_write_with_sync(&fileHandle, "abc", 3, &byteCount);
    f_lseek(&fileHandle, 0);
    f_read(&fileHandle, readBuf, 10, &byteCount);
    TEST_ASSERT_EQUAL_STRING("abc", readBuf);

    // when file does not exist:
    err = f_open(&fileHandle, "existsLater",
        FA_READ | FA_WRITE | FA_CREATE_ALWAYS);

    TEST_ASSERT_TRUE(fakefilesystem_fileExists("existsLater"));
}

void test_modeFlags_OpenAppendSetsFilePointerAtEnd(void)
{
    MOUNT_FATFS();

    UINT byteCount = 0xAA;
    fakefilesystem_createFile("fileexists");
    fakefilesystem_writeFile("fileexists", "hello");

    FRESULT err = f_open(&fileHandle, "fileexists",
        FA_READ | FA_WRITE | FA_OPEN_APPEND);
    TEST_ASSERT_EQUAL_INT(FR_OK, err);

    f_write_with_sync(&fileHandle, " there", 6, &byteCount);
    f_lseek(&fileHandle, 0);
    f_read(&fileHandle, readBuf, 20, &byteCount);
    TEST_ASSERT_EQUAL_STRING("hello there", readBuf);
}

void test_modeFlags_OpenAlwaysOpensAFileAtBeginningIfExistsOrNot(void)
{
    MOUNT_FATFS();

    UINT byteCount = 0xAA;

    // when file doesn't exist:
    FRESULT err = f_open(&fileHandle, "file", FA_READ | FA_OPEN_ALWAYS);
    
    TEST_ASSERT_EQUAL_INT(FR_OK, err);
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("file"));

    // when file exists:
    fakefilesystem_createFile("exists");
    fakefilesystem_writeFile("exists", "hello");

    err = f_open(&fileHandle, "exists", FA_READ | FA_OPEN_ALWAYS);
    TEST_ASSERT_EQUAL_INT(FR_OK, err);
    f_read(&fileHandle, readBuf, 20, &byteCount);
    TEST_ASSERT_EQUAL_STRING("hello", readBuf);
}

void test_syncMultipleTimesBetweenWriting(void)
{
    MOUNT_FATFS();

    UINT byteCount = 0xAA;
    f_open(&fileHandle, "file", FA_READ | FA_WRITE | FA_CREATE_NEW);

    f_write_with_sync(&fileHandle, "hey", 3, &byteCount);
    f_sync(&fileHandle);
    f_write_with_sync(&fileHandle, " there", 6, &byteCount);

    f_lseek(&fileHandle, 0);
    f_read(&fileHandle, readBuf, 20, &byteCount);
    TEST_ASSERT_EQUAL_STRING("hey there", readBuf);
}

void test_writeMultipleTimesWithoutSyncing(void)
{
    MOUNT_FATFS();

    UINT byteCount = 0xAA;
    f_open(&fileHandle, "file", FA_READ | FA_WRITE | FA_CREATE_NEW);

    f_write(&fileHandle, "hey", 3, &byteCount);
    f_write_with_sync(&fileHandle, " there", 6, &byteCount);

    f_lseek(&fileHandle, 0);
    f_read(&fileHandle, readBuf, 20, &byteCount);

    TEST_ASSERT_EQUAL_STRING("hey there", readBuf);
}

void test_acquireFileSize(void)
{
    TEST_IGNORE();
}

// Private

FRESULT call_f_open_with(bool isMounted, bool goodFileFP, bool goodPath)
{
    FIL* fp = &fileHandle;
    TCHAR* path = "test";
    if (isMounted)
    {
        MOUNT_FATFS();
    }
    if (!goodFileFP)
    {
        fp = NULL;
    }
    if (!goodPath)
    {
        path = NULL;
    }
    return f_open(fp, path, FA_READ | FA_OPEN_APPEND);
}

FRESULT f_write_with_sync(FIL* fp, const void* buff, UINT btw, UINT* bw)
{
    FRESULT err_f_write = f_write(fp, buff, btw, bw);
    f_sync(fp);
    return err_f_write;
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