#include "unity.h"
#include "fakeff.h"
#include "fakefilesystem.h"

void setUp(void)
{
    // Initialize any necessary resources before each test
    fakefilesystem_reset();
}

void tearDown(void)
{
    // Clean up resources after each test
}

void test_fatfs_initialize(void)
{
    // Arrange
    FRESULT expectedResult = FR_OK;

    // Act
    FRESULT result = fatfs_test_double_initialize();

    // Assert
    TEST_ASSERT_EQUAL_INT(expectedResult, result);
    TEST_ASSERT_FALSE(fatfs_test_double_is_mounted());
}

// Test
void test_fatfs_mount(void)
{
    // Arrange
    FRESULT expectedResult = FR_OK;
    FATFS fs;

    // Act
    FRESULT result = f_mount(&fs, "0:", 1);

    // Assert
    TEST_ASSERT_EQUAL_INT(expectedResult, result);
    TEST_ASSERT_TRUE(fatfs_test_double_is_mounted());
}
