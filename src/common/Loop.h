#ifndef MY_LOOP_H_INCLUDED
#define MY_LOOP_H_INCLUDED

// Configure infinite loops to run X number of times instead
#ifdef TEST
extern unsigned int testIndex;
extern unsigned int testIndexMax;
// Use LOOP instead of while(1)
#define LOOP for (testIndex = 0; testIndex < testIndexMax; testIndex++)
// Sets number of times to run infinite loops
#define LOOP_COUNT(x) testIndexMax = (x)
#define LAST_LOOP_COUNT() testIndex
#else
#define LOOP while(1)
#endif

#endif
