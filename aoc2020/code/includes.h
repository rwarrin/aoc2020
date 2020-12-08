#ifndef INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

#define Assert(Condition) if(!(Condition)) { *(int *)0 = 0; }

#define InvalidCodePath Assert(!"Invalid Code Path")
#define InvalidDefaultCase default: { Assert(!"Invalid Default Case"); } break

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float r32;
typedef double r64;
typedef s32 b32;

#define true 1
#define false 0

#define INCLUDES_H
#endif
