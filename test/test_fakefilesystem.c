#include "unity.h"
#include "fakefilesystem.h"
#include <string.h>

void setUp(void)
{
    fakefilesystem_reset();
}

void tearDown(void)
{
}

void test_sizeOfFileThatDoesntExistIs0(void)
{
    TEST_ASSERT_EQUAL_INT(0, fakefilesystem_fileSize("file"));
}

void test_fileDoesNotExistAfterInit(void)
{
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("file"));
}

void test_fileExistsOnceCreated(void)
{
    fakefilesystem_createFile("file");

    TEST_ASSERT_TRUE(fakefilesystem_fileExists("file"));
}

void test_fileDoesNotExistOnceDeleted(void)
{
    fakefilesystem_createFile("file");
    
    fakefilesystem_deleteFile("file");

    TEST_ASSERT_FALSE(fakefilesystem_fileExists("file"));
}

void test_queryMultipleFilesCanBeCreated(void)
{
    fakefilesystem_createFile("fileOpen1");
    fakefilesystem_createFile("fileOpen2");
    fakefilesystem_createFile("fileOpen3");

    TEST_ASSERT_TRUE(fakefilesystem_fileExists("fileOpen1"));
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("fileOpen2"));
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("fileOpen3"));
}

void test_queryMultipleFilesCanBeDeleted(void)
{
    fakefilesystem_createFile("fileOpen1");
    fakefilesystem_createFile("fileOpen2");

    fakefilesystem_deleteFile("fileOpen1");
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("fileOpen1"));

    fakefilesystem_deleteFile("fileOpen2");
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("fileOpen2"));
}

void test_createdFileHasNoDataToBeginWithAfterInit(void)
{
    fakefilesystem_createFile("file");

    const char* fileData = fakefilesystem_readfile("file");
    
    TEST_ASSERT_EQUAL_STRING("", fileData);
}

void test_readingADeletedFileReturnsNULL(void)
{
    fakefilesystem_createFile("file");

    fakefilesystem_deleteFile("file");

    const char* fileData = fakefilesystem_readfile("file");

    TEST_ASSERT_EQUAL(NULL, fileData);
}

void test_dataWrittenToFileCanBeReadBackExactly(void)
{
    fakefilesystem_createFile("file");

    fakefilesystem_writeFile("file", "hey");

    const char* fileData = fakefilesystem_readfile("file");

    TEST_ASSERT_EQUAL_STRING("hey", fileData);
}

void test_writingToADeletedFileDoesNotWriteAnythingToIt(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_writeFile("file", "hey");
    fakefilesystem_deleteFile("file");

    fakefilesystem_createFile("file");
    const char* fileData = fakefilesystem_readfile("file");
    
    TEST_ASSERT_EQUAL_STRING("", fileData);
}

void test_createdFileOnlyHasToBeDeletedOnceToNotExist(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_createFile("file");

    fakefilesystem_deleteFile("file");

    TEST_ASSERT_FALSE(fakefilesystem_fileExists("file"));
}

void test_deletedFileLosesAllDataEvenAfterRecreated(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_writeFile("file", "hey");

    const char* fileData = fakefilesystem_readfile("file");
    TEST_ASSERT_EQUAL_STRING("hey", fileData);
    fakefilesystem_deleteFile("file");
    
    fakefilesystem_createFile("file");
    fileData = fakefilesystem_readfile("file");
    TEST_ASSERT_EQUAL_STRING("", fileData);
}

void test_fileSizeIsNumberOfBytesInFile(void)
{
    fakefilesystem_createFile("file");

    fakefilesystem_writeFile("file", "abc");

    int size = fakefilesystem_fileSize("file");
    TEST_ASSERT_EQUAL_INT(3, size);
}

void test_writingTwiceToAFile(void)
{
    fakefilesystem_createFile("file");

    fakefilesystem_writeFile("file", "abc");
    fakefilesystem_writeFile("file", "def");

    const char* fileData = fakefilesystem_readfile("file");
    TEST_ASSERT_EQUAL_STRING("abcdef", fileData);
}


/*
- [x] Want to have files that persist data
    - [x] Want to read data
    - [x] Want to write data
- [x] Files must be created before writing or reading
- [x] Files will write from beginning
- [x] Files will read from beginning
- [x] File will keep appending to end each write
- [x] Want to know size of file
- [x] Want to be able to delete file
- [x] Want to have multiple files at same time
- [x] Files are to be known by their names
*/