#include "unity.h"
#include "fake_myTimeString.h"
#include "FileNameIterator.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define FAKE_FILE_NAME "abc.txt"    // desired file name
#define TEST_MAX_BUF_SIZE    20

char fileName[TEST_MAX_BUF_SIZE];

void resetFileNameForTest();    // non-0 value for file name
// this makes it so names are random like they are currently
const char* generateFakeFileName(); 

void test_setUp(void)
{
    fake_myTimeString_reset();

    resetFileNameForTest();
}

void test_FileNameGet(void)
{
    // initialise
    fake_myTimeString_setFileName("abc");
    FileNameIterator_init();
    FileNameIterator_getName(fileName, TEST_MAX_BUF_SIZE);
    TEST_ASSERT_EQUAL_STRING("abc", fileName);

    // call twice after init shall return same str
    fake_myTimeString_setFileName("def");
    FileNameIterator_getName(fileName, TEST_MAX_BUF_SIZE);
    TEST_ASSERT_EQUAL_STRING("abc", fileName);

    // initialise again shall get new str
    fake_myTimeString_setFileName("def");
    FileNameIterator_init();
    FileNameIterator_getName(fileName, TEST_MAX_BUF_SIZE);
    TEST_ASSERT_EQUAL_STRING("def", fileName);
}

void test_FileNameNext(void)
{
    // initialise
    fake_myTimeString_setFileName("abc");
    FileNameIterator_init();
    FileNameIterator_getName(fileName, TEST_MAX_BUF_SIZE);
    TEST_ASSERT_EQUAL_STRING("abc", fileName);

    // do next
    fake_myTimeString_setFileName("def");
    FileNameIterator_next();

    // should get new string
    FileNameIterator_getName(fileName, TEST_MAX_BUF_SIZE);
    TEST_ASSERT_EQUAL_STRING("def", fileName);
}

void test_HasNextAlways(void)
{
    TEST_ASSERT_TRUE(FileNameIterator_hasNext());

    FileNameIterator_init();

    TEST_ASSERT_TRUE(FileNameIterator_hasNext());

    char temp[TEST_MAX_BUF_SIZE];
    FileNameIterator_getName(temp, TEST_MAX_BUF_SIZE);

    TEST_ASSERT_TRUE(FileNameIterator_hasNext());
}

/*********** Helpers *************/

void resetFileNameForTest()
{
    memset(fileName, 1, TEST_MAX_BUF_SIZE);  // fill it up with non-0 values
}

// Will generate a new file name each time it is called
// Never same name twice
const char* generateFakeFileName()
{
    static char generatedFakeFileName[TEST_MAX_BUF_SIZE];
    static int previousNum = 0;
    int maxRand = 9999;

    srand(time(NULL));

    int randNum = 0;
    while (randNum == previousNum)
    {
        randNum = rand() % maxRand;
    }
    previousNum = randNum;

    sprintf(generatedFakeFileName, "rand%u.txt", randNum);

    fake_myTimeString_setFileName(generatedFakeFileName);

    return generatedFakeFileName;
}


/*
File names are output with the iterator pattern.

Creating this iterator abstraction to allow us to change
how the file names are generated in the next update.

- For now, the file names shall be created from the timestring
- currentFileName() - returns current file name by checking timestamp
- nextFileName() - increments "index" to the next file name
    in reality this does nothing for this scenario as name depends on time.
- hasNewFileName() - as per iterator pattern, we check if
    we have a new filename to give. this will always be
    true in this scenario.

*/