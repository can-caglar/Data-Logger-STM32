#ifndef MY_LOOP_H_INCLUDED
#define MY_LOOP_H_INCLUDED

#ifdef TEST
extern unsigned int testIndex;
extern unsigned int testIndexMax;
#define LOOP for (testIndex = 0; testIndex < testIndexMax; testIndex++)
#define LOOP_COUNT(x) testIndexMax = (x)
#else
#define LOOP while(1)
#endif

#endif
