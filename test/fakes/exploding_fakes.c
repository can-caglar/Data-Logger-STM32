#include <stdio.h>
#include <stdlib.h>
 
#define BOOM_MESSAGE printf("BOOM! time to write a better fake for %s\n", __func__)
#define EXPLODING_FAKE_FOR(f) void f(void); void f(void) { BOOM_MESSAGE; exit(1); }
#define NULL_VOID_FAKE_FOR(f) void f(void); void f(void) {}
#define NULL_VALUE_FAKE_FOR(value_type, f, result) value_type f(void); value_type f(void) { return result; }
 
EXPLODING_FAKE_FOR(HAL_IncTick)
EXPLODING_FAKE_FOR(HAL_GetTick)

