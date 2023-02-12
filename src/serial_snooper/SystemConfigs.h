#ifndef SYSTEM_CONFIGS_H
#define SYSTEM_CONFIGS_H

// The least hacky way I could think of...

#ifndef TEST
#define MAX_FILE_SIZE   23068672
#define FILE_SIZE_LOWER_THRESHOLD   20971520
#define FLUSH_TIME_MS   30000
#else
// Smaller values to make testing possible
// Should be smaller than circular buf capacity
#define MAX_FILE_SIZE   200
#define FILE_SIZE_LOWER_THRESHOLD   100
#define FLUSH_TIME_MS   500
#endif

#endif
