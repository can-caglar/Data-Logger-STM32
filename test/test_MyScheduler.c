#include "unity.h"
#include "MyScheduler.h"
#include "fake_DataHolder.h"
#include "fake_stm32f0xx_hal.h"
#include "unity_helper.h"
#include <stdint.h>

static uint8_t task1Called = 0;
static uint8_t task2Called = 0;
static uint8_t fnGetDataCalled = 0;
static uint32_t task1CallCount = 0;

// Tasks for testing
static void resetTaskData(void);
static void task1(void);
static void task2(void);
static uint8_t wasTask1Called(void);    // auto clears state
static uint8_t wasTask2Called(void);    // auto clears state
static uint8_t getTask1CallCount(void);
static const int fakeDataContext = 777;

// Test functions
static const FnTask task1ptr = task1;
static const FnTask task2ptr = task2;

// Set up
void setUp(void)
{
    MyScheduler_Init();
    resetTaskData();
    fake_halTick_enableAutoIncrement(0);
}

void test_NoTaskAddedWontCallAnyTasks(void)
{
    MyScheduler_Run();

    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
    TEST_ASSERT_EQUAL_INT(0, wasTask2Called());
}

void test_AddTask_TaskAddedAndIsEnabledWillBeCalledEverytimeIfPeriod0(void)
{
    MyScheduler_AddTask(task1ptr, 0, true, true);

    // Run loop X times
    for (int i = 0; i < 3; i++)
    {
        MyScheduler_Run();
        TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
    }
}

void test_AddTask_AddedTasksWhichAreDisabledWontBeCalled(void)
{
    MyScheduler_AddTask(task1ptr, 0, true, false);

    MyScheduler_Run();

    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
}

void test_AddTask_MultipleTasksCanBeAddedWillCallSequentially(void)
{
    MyScheduler_AddTask(task1ptr, 0, true, true);
    MyScheduler_AddTask(task2ptr, 0, true, true);

    // Run loop X times
    for (int i = 0; i < 3; i++)
    {
        MyScheduler_Run();
        TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
        TEST_ASSERT_EQUAL_INT(1, wasTask2Called());
    }
}

void test_SchedulerWillOnlyCallTaskWhenItsTheRightTime(void)
{
    const int period = 100;
    MyScheduler_AddTask(task1ptr, period, true, true);

    // Not time yet, don't call
    fake_halTick_setTickValue(period - 1);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());

    // Now's the exact time, should be called
    fake_halTick_setTickValue(period);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    // Not time again, don't call
    fake_halTick_setTickValue(period + 1);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
}

void test_SchedulerWillCallIfItIsLateToCall(void)
{
    const int period = 100;
    MyScheduler_AddTask(task1ptr, period, true, true);

    // Late
    fake_halTick_setTickValue(period + 1);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
}

void test_SchedulerWithTwoTasksKnowsWhenToCallThem(void)
{
    const int period1 = 100;
    const int period2 = 150;

    MyScheduler_AddTask(task1ptr, period1, true, true);
    MyScheduler_AddTask(task2ptr, period2, true, true);

    // Task 1 time to call
    fake_halTick_setTickValue(period1);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
    TEST_ASSERT_EQUAL_INT(0, wasTask2Called());

    // Task 2 time to call
    fake_halTick_setTickValue(period2);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
    TEST_ASSERT_EQUAL_INT(1, wasTask2Called());
}

void test_AddingTaskAfterInitialisation(void)
{
    const int startTime = 500;
    const int period = 100;

    // Scheduler already running
    fake_halTick_setTickValue(startTime);
    MyScheduler_Run();

    // Task added afterwards
    MyScheduler_AddTask(task1ptr, period, true, true);

    // Shouldn't call the task until "period" has elapsed from then
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
}

void test_AddingSameFunctionAgainCreatesMultipleEntriesInScheduler(void)
{
    const int period1 = 75;
    const int period2 = 100;

    MyScheduler_AddTask(task1ptr, period1, true, true);
    MyScheduler_AddTask(task1ptr, period2, true, true);

    // period1 has elapsed, should call function
    fake_halTick_setTickValue(period1);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    fake_halTick_setTickValue(period2);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    fake_halTick_setTickValue(period1 * 2);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    fake_halTick_setTickValue(period2 * 2);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
}

void test_ThereIsALimitToHowManyFunctionsToAdd(void)
{
    // Adding tasks up to MAX_TASKS
    for (int i = 0; i < MAX_TASKS; i++)
    {
        TEST_ASSERT_EQUAL_INT(SS_ERR_NONE,
            MyScheduler_AddTask(task1ptr, 0, true, true));
    }
    // Anymore will just return an eror
    TEST_ASSERT_EQUAL_INT(SS_ERR_FAULT,
        MyScheduler_AddTask(task2ptr, 0, true, true));

    MyScheduler_Run();

    // Task 1 is called
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
    // Task 2 isn't
    TEST_ASSERT_EQUAL_INT(0, wasTask2Called());
}

void test_OneShotTasksAreCalledOnce(void)
{
    const int period = 100;

    MyScheduler_AddTask(task1ptr, period, false, true);

    fake_halTick_setTickValue(100);
    MyScheduler_Run();

    // expect to be called
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    // should not be called anymore
    fake_halTick_setTickValue(200);
    MyScheduler_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
}

void test_tasksCanBeCalledPeriodically(void)
{
    // given
    const int period = 100;
    MyScheduler_AddTask(task1ptr, period, true, true);
    fake_halTick_enableAutoIncrement(period);
    // when
    MyScheduler_Run();
    MyScheduler_Run();
    MyScheduler_Run();
    // then
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
    TEST_ASSERT_EQUAL_INT(3, getTask1CallCount());
}


/* Helpers */

void task1(void)
{
    task1Called = 1;
    task1CallCount++;
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
    task1CallCount = 0;
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

static uint8_t getTask1CallCount(void)
{
    return task1CallCount;
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