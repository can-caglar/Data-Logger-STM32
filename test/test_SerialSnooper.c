#include "unity.h"
#include "SerialSnooper.h"
#include "fake_DataHolder.h"
#include "fake_stm32f0xx_hal.h"
#include "unity_helper.h"
#include <stdint.h>

static uint8_t task1Called = 0;
static uint8_t task2Called = 0;
static uint8_t fnGetDataCalled = 0;

// Tasks for testing
static void resetTaskData(void);
static void task1(void);
static void task2(void);
static uint8_t wasTask1Called(void);    // auto clears state
static uint8_t wasTask2Called(void);    // auto clears state
static const int fakeDataContext = 777;

// Test functions
static const FnTask task1ptr = task1;
static const FnTask task2ptr = task2;

// TODO, test for overflow conditions

// Set up
void setUp(void)
{
    SerialSnooper_Init();
    resetTaskData();
}

void test_NoTaskAddedWontCallAnyTasks(void)
{
    SerialSnooper_Run();

    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
    TEST_ASSERT_EQUAL_INT(0, wasTask2Called());
}

void test_AddTask_TaskAddedAndIsEnabledWillBeCalledEverytimeIfPeriod0(void)
{
    SerialSnooper_AddTask(task1ptr, 0, true, true);

    // Run loop X times
    for (int i = 0; i < 3; i++)
    {
        SerialSnooper_Run();
        TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
    }
}

void test_AddTask_AddedTasksWhichAreDisabledWontBeCalled(void)
{
    SerialSnooper_AddTask(task1ptr, 0, true, false);

    SerialSnooper_Run();

    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
}

void test_AddTask_MultipleTasksCanBeAddedWillCallSequentially(void)
{
    SerialSnooper_AddTask(task1ptr, 0, true, true);
    SerialSnooper_AddTask(task2ptr, 0, true, true);

    // Run loop X times
    for (int i = 0; i < 3; i++)
    {
        SerialSnooper_Run();
        TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
        TEST_ASSERT_EQUAL_INT(1, wasTask2Called());
    }
}

void test_SchedulerWillOnlyCallTaskWhenItsTheRightTime(void)
{
    const int period = 100;
    SerialSnooper_AddTask(task1ptr, period, true, true);

    // Not time yet, don't call
    setHalGetTickReturnValue(period - 1);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());

    // Now's the exact time, should be called
    setHalGetTickReturnValue(period);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    // Not time again, don't call
    setHalGetTickReturnValue(period + 1);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
}

void test_SchedulerWillCallIfItIsLateToCall(void)
{
    const int period = 100;
    SerialSnooper_AddTask(task1ptr, period, true, true);

    // Late
    setHalGetTickReturnValue(period + 1);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
}

void test_SchedulerWithTwoTasksKnowsWhenToCallThem(void)
{
    const int period1 = 100;
    const int period2 = 150;

    SerialSnooper_AddTask(task1ptr, period1, true, true);
    SerialSnooper_AddTask(task2ptr, period2, true, true);

    // Task 1 time to call
    setHalGetTickReturnValue(period1);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
    TEST_ASSERT_EQUAL_INT(0, wasTask2Called());

    // Task 2 time to call
    setHalGetTickReturnValue(period2);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
    TEST_ASSERT_EQUAL_INT(1, wasTask2Called());
}

void test_AddingTaskAfterInitialisation(void)
{
    const int startTime = 500;
    const int period = 100;

    // Scheduler already running
    setHalGetTickReturnValue(startTime);
    SerialSnooper_Run();

    // Task added afterwards
    SerialSnooper_AddTask(task1ptr, period, true, true);

    // Shouldn't call the task until "period" has elapsed from then
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
}

void test_AddingSameFunctionAgainCreatesMultipleEntriesInScheduler(void)
{
    const int period1 = 75;
    const int period2 = 100;

    SerialSnooper_AddTask(task1ptr, period1, true, true);
    SerialSnooper_AddTask(task1ptr, period2, true, true);

    // period1 has elapsed, should call function
    setHalGetTickReturnValue(period1);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    setHalGetTickReturnValue(period2);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    setHalGetTickReturnValue(period1 * 2);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    setHalGetTickReturnValue(period2 * 2);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
}

void test_ThereIsALimitToHowManyFunctionsToAdd(void)
{
    // Adding tasks up to MAX_TASKS
    for (int i = 0; i < MAX_TASKS; i++)
    {
        TEST_ASSERT_EQUAL_INT(SS_ERR_NONE,
            SerialSnooper_AddTask(task1ptr, 0, true, true));
    }
    // Anymore will just return an eror
    TEST_ASSERT_EQUAL_INT(SS_ERR_FAULT,
        SerialSnooper_AddTask(task2ptr, 0, true, true));

    SerialSnooper_Run();

    // Task 1 is called
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
    // Task 2 isn't
    TEST_ASSERT_EQUAL_INT(0, wasTask2Called());
}

void test_OneShotTasksAreCalledOnce(void)
{
    const int period = 100;

    SerialSnooper_AddTask(task1ptr, period, false, true);

    setHalGetTickReturnValue(100);
    SerialSnooper_Run();

    // expect to be called
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    // should not be called anymore
    setHalGetTickReturnValue(200);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());

}

/* Helpers */

void task1(void)
{
    task1Called = 1;
}

void task2(void)
{
    task2Called = 1;
}

static void resetTaskData(void)
{
    task1Called = 0;
    task2Called = 0;
    fnGetDataCalled = 0;
}

static uint8_t wasTask1Called(void)
{
    uint8_t ret = task1Called;
    task1Called = 0;
    return ret;
}

static uint8_t wasTask2Called(void)
{
    uint8_t ret = task2Called;
    task2Called = 0;
    return ret;
}


/*
- This module will be a scheduler
- Serial Snooper is passed in function pointers during init
- It is then passed in a struct of data
- It will iterate and call them based on the 
frequency we decide during the dependency injection

- Tasks will have lastCall, nextCallPeriod and fn pointer
- Scheduler will check if time has elapsed (lastCall + nextCallPeriod)
- For simplicity, these parameters can't be changed once set

- In the end, the scheduler will call the tasks when it's their time
and pass in the time to them

- [x] Has a DataStore
- [x] Gets DataHandle* from DataStore
- [x] Passes this to Tasks
- [x] Tasks are function pointers with periods for how often, oneshot and enable/disable
to call (may take longer depending on time of other tasks)
- [x] Oneshot tasks are called only once
- [x] Tasks are initialised with addTask(fnPointer, period, oneshot)
- [x] There is a maximum number of tasks fails if could not add task
- [x] If a function pointer already exists, will just add task

*/