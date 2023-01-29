#include "unity.h"
#include "SerialSnooper.h"
#include "fake_DataHolder.h"
#include "unity_helper.h"
#include <stdint.h>

static uint8_t task1Called = 0;
static uint8_t task2Called = 0;
static uint8_t fnGetDataCalled = 0;

// Tasks for testing
static void resetTaskData(void);
static void task1(DataContext* data);
static void task2(DataContext* data);
static uint8_t wasTask1Called(void);    // auto clears state
static uint8_t wasTask2Called(void);    // auto clears state
static int getTask1DataPassedIn(void);  // auto clears state
static int getTask2DataPassedIn(void);  // auto clears state
static DataContext* fnForGettingData(void);
static const int fakeDataContext = 777;

// Test functions
static const FnTask task1ptr = task1;
static const FnTask task2ptr = task2;
static DataContext* task1dataPassedIn;
static DataContext* task2dataPassedIn;

// Set up
void setUp(void)
{
    SerialSnooper_Init(NULL);
    resetTaskData();
    fakeSetTime(0);
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

void test_SchedulerWillGrabDataAndPassToTasks(void)
{
    // init with a "data grabber"
    SerialSnooper_Init(fnForGettingData);  

    // add some task
    SerialSnooper_AddTask(task1ptr, 0, true, true);

    // run the function under test
    SerialSnooper_Run();

    // task should be passed in the data grabbed
    TEST_ASSERT_EQUAL_INT(fakeDataContext, getTask1DataPassedIn());
}

void test_SchedulerWillOnlyCallTaskWhenItsTheRightTime(void)
{
    const int period = 100;
    SerialSnooper_AddTask(task1ptr, period, true, true);

    // Not time yet, don't call
    fakeSetTime(period - 1);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());

    // Now's the exact time, should be called
    fakeSetTime(period);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    // Not time again, don't call
    fakeSetTime(period + 1);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
}

void test_SchedulerWillCallIfItIsLateToCall(void)
{
    const int period = 100;
    SerialSnooper_AddTask(task1ptr, period, true, true);

    // Late
    fakeSetTime(period + 1);
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
    fakeSetTime(period1);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());
    TEST_ASSERT_EQUAL_INT(0, wasTask2Called());

    // Task 2 time to call
    fakeSetTime(period2);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(0, wasTask1Called());
    TEST_ASSERT_EQUAL_INT(1, wasTask2Called());
}

void test_AddingTaskAfterInitialisation(void)
{
    const int startTime = 500;
    const int period = 100;

    // Scheduler already running
    fakeSetTime(startTime);
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
    fakeSetTime(period1);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    fakeSetTime(period2);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    fakeSetTime(period1 * 2);
    SerialSnooper_Run();
    TEST_ASSERT_EQUAL_INT(1, wasTask1Called());

    fakeSetTime(period2 * 2);
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

/* Helpers */

void task1(DataContext* data)
{
    task1Called = 1;
    task1dataPassedIn = data;
}

void task2(DataContext* data)
{
    task2Called = 1;
    task2dataPassedIn = data;
}

static void resetTaskData(void)
{
    task1Called = 0;
    task2Called = 0;
    fnGetDataCalled = 0;
    task1dataPassedIn = 0;
    task2dataPassedIn = 0;
}

static DataContext* fnForGettingData(void)
{
    // return some fake fata
    return (DataContext*)fakeDataContext;
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

static int getTask1DataPassedIn(void)
{
    int ret = (int)task1dataPassedIn;
    task1dataPassedIn = 0;
    return ret;
}

static int getTask2DataPassedIn(void)
{
    int ret = (int)task2dataPassedIn;
    task2dataPassedIn = 0;
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

- [ ] Has a DataStore
- [ ] Gets DataHandle* from DataStore
- [ ] Passes this to Tasks
- [ ] Tasks are function pointers with periods for how often, oneshot and enable/disable
to call (may take longer depending on time of other tasks)
- [ ] Tasks are initialised with addTask(fnPointer, period, oneshot)
- [ ] There is a maximum number of tasks fails if could not add task
- [ ] If a function pointer already exists, will update its period and oneshot
- [ ] If doesnt exist, will add to list

// Extras
- [ ] Function can be removed which will just NULL its fn pointer.
    This maybe not necessary for now
*/