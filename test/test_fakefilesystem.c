#include "unity.h"
#include <string.h>
#include "fakefilesystem.h"

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

void test_queryDeletingAFileDoesNotDeleteOthers(void)
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

void test_readingADeletedFileReturnsEmptyString(void)
{
    fakefilesystem_createFile("file");

    fakefilesystem_deleteFile("file");

    const char* fileData = fakefilesystem_readfile("file");

    TEST_ASSERT_EQUAL_STRING("", fileData);
}

void test_dataWrittenToFileCanBeReadBackExactly(void)
{
    fakefilesystem_createFile("file");

    fakefilesystem_writeFile("file", "hey");

    fakefilesystem_seek("file", FFS_BEGIN);

    const char* fileData = fakefilesystem_readfile("file");

    TEST_ASSERT_EQUAL_STRING("hey", fileData);
}

void test_writingToADeletedFileDoesNotWriteAnythingToIt(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_writeFile("file", "hey");
    fakefilesystem_deleteFile("file");

    fakefilesystem_writeFile("file", "hey");
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
    fakefilesystem_seek("file", FFS_BEGIN);

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
    fakefilesystem_seek("file", FFS_BEGIN);

    const char* fileData = fakefilesystem_readfile("file");
    TEST_ASSERT_EQUAL_STRING("abcdef", fileData);
}

void test_maxFiles(void)
{
    for (int i = 0; i < FFS_MAX_FILES; i++)
    {
        char fileName[FFS_MAX_FILENAME];
        sprintf(fileName, "file%i", i);
        fakefilesystem_createFile(fileName);
        TEST_ASSERT_TRUE(fakefilesystem_fileExists(fileName));
    }
    fakefilesystem_createFile("full");
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("full"));
}

void test_fileSystemSeekToChangeReadWritePointerToBegining(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_writeFile("file", "mansion");

    fakefilesystem_seek("file", FFS_BEGIN);

    fakefilesystem_writeFile("file", "pen");
    fakefilesystem_writeFile("file", "cil");
    fakefilesystem_seek("file", FFS_BEGIN);

    TEST_ASSERT_EQUAL_STRING("pencil", fakefilesystem_readfile("file"));
}

void test_fileSystemSeekDoesNothingToNonExistingFile(void)
{
    // exercise a potential crash
    fakefilesystem_seek("file", FFS_BEGIN);
}

void test_fileSystemSeekToChangeReadWritePointerToEnd(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_writeFile("file", "man");

    fakefilesystem_seek("file", FFS_BEGIN);
    fakefilesystem_seek("file", FFS_END);

    fakefilesystem_writeFile("file", "sion");
    fakefilesystem_seek("file", FFS_BEGIN);

    TEST_ASSERT_EQUAL_STRING("mansion", fakefilesystem_readfile("file"));
}

void test_fileSystemSeekToChangeReadWritePointerToMiddle(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_writeFile("file", "mansion");

    fakefilesystem_seek("file", 3);

    fakefilesystem_writeFile("file", "ner");
    fakefilesystem_seek("file", FFS_BEGIN);

    TEST_ASSERT_EQUAL_STRING("manner", fakefilesystem_readfile("file"));
}

void test_fileSystemSeekBeyondSizeLimitsSeekageToEnd(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_writeFile("file", "garden");

    fakefilesystem_seek("file", 77);

    fakefilesystem_writeFile("file", "er");
    fakefilesystem_seek("file", FFS_BEGIN);

    TEST_ASSERT_EQUAL_STRING("gardener", fakefilesystem_readfile("file"));

}

void test_fileSystemSeekAnyNegativeValueWillJustSeekEnd(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_writeFile("file", "garden");

    fakefilesystem_seek("file", -2);

    fakefilesystem_writeFile("file", "er");
    fakefilesystem_seek("file", FFS_BEGIN);

    TEST_ASSERT_EQUAL_STRING("gardener", fakefilesystem_readfile("file"));
}

void test_fileSystemReadIsAffectedBySeek(void)
{
    fakefilesystem_createFile("file");
    fakefilesystem_writeFile("file", "understand");

    fakefilesystem_seek("file", 5);

    TEST_ASSERT_EQUAL_STRING("stand", fakefilesystem_readfile("file"));
}

void test_deletingAFileThatDoesntExist(void)
{
    // just to exercise it
    fakefilesystem_deleteFile("file");
}

void test_dumpInfo(void)
{
    // just to see what the print looks like
    // switch in to use
    #if 0   
    fakefilesystem_createFile("file");
    fakefilesystem_createFile("file2");
    fakefilesystem_createFile("file3");
    fakefilesystem_createFile("file4");
    fakefilesystem_createFile("file5");
    
    fakefilesystem_writeFile("file", "a");
    fakefilesystem_writeFile("file2", 
        "hello this is a long-ish string being printed.");
    fakefilesystem_writeFile("file5", "hello");
    
    fakefilesystem_deleteFile("file3");
    fakefilesystem_seek("file5", 2);
    
    fakefilesystem_dump();
    #endif
}
