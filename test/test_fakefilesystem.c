#include "unity.h"
#include "fakefilesystem.h"
#include <string.h>

void setUp(void)
{
    fakefilesystem_open();
}

void tearDown(void)
{
    fakefilesystem_close();
}

void test_sizeOfFileThatDoesntExistIs0(void)
{
    TEST_ASSERT_EQUAL_INT(0, fakefilesystem_getsize("file"));
}

void test_fileIsNotOpenAfterInit(void)
{
    TEST_ASSERT_FALSE(fakefilesystem_isOpen("file"));
}

void test_fileIsOpenOnceOpened(void)
{
    fakefilesystem_openFile("file");

    TEST_ASSERT_TRUE(fakefilesystem_isOpen("file"));
}

void test_fileIsNotOpenOnceClosed(void)
{
    fakefilesystem_openFile("file");
    
    fakefilesystem_closeFile("file");

    TEST_ASSERT_FALSE(fakefilesystem_isOpen("file"));
}

void test_queryMultipleFilesCanBeOpened(void)
{
    fakefilesystem_openFile("fileOpen1");
    fakefilesystem_openFile("fileOpen2");
    fakefilesystem_openFile("fileOpen3");

    TEST_ASSERT_TRUE(fakefilesystem_isOpen("fileOpen1"));
    TEST_ASSERT_TRUE(fakefilesystem_isOpen("fileOpen2"));
    TEST_ASSERT_TRUE(fakefilesystem_isOpen("fileOpen3"));
}

void test_queryMultipleFilesCanBeClosed(void)
{
    fakefilesystem_openFile("fileOpen1");
    fakefilesystem_openFile("fileOpen2");

    fakefilesystem_closeFile("fileOpen1");
    TEST_ASSERT_FALSE(fakefilesystem_isOpen("fileOpen1"));

    fakefilesystem_closeFile("fileOpen2");
    TEST_ASSERT_FALSE(fakefilesystem_isOpen("fileOpen2"));
}

/*
void test_sizeOfFileIncrementsOnlyWhenItsOpen(void)
{
    fakefilesystem_writeTo("file", "abc");

    TEST_ASSERT_EQUAL_INT(0, fakefilesystem_getsize("file"));

    fakefilesystem_open("file", 0);

    //TEST_ASSERT_EQUAL_INT(strlen("abc"), fakefilesystem_getsize("file"));
}
*/

/*
- [ ] Want to have files that persist data
    - [ ] Want to read data
    - [ ] Want to write data
- [ ] Files must be opened before writing or reading
- [ ] Files are opened at index 0
- [ ] File index can be changed:
    - Begin: 0
    - End: -1
    - Val = num > 0
- [ ] Want to know size of file
- [ ] Want to be able to write to any location in file
- [ ] Want to be able to clear data in a file
- [ ] Want to be able to get size of file
- [ ] Want to have multiple files at same time
- [ ] Files are to be known by their names
*/