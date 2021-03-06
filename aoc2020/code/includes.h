#ifndef INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <math.h>

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
typedef r32 f32;
typedef r64 f64;
typedef s32 b32;
typedef size_t umm;

#define true 1
#define false 0

#define PI 3.14159265f

struct v2
{
    f32 x;
    f32 y;
};

struct v2i
{
    s32 x;
    s32 y;
};

struct file_contents
{
    u32 Size;
    char *Contents;
};

static struct file_contents
ReadFile(char *FileName)
{
    struct file_contents Result = {};

    FILE *File = fopen(FileName, "rb");
    if(File)
    {
        fseek(File, 0, SEEK_END);
        Result.Size = (u32)ftell(File);
        fseek(File, 0, SEEK_SET);

        Result.Contents = (char *)malloc(Result.Size + 1);
        fread(Result.Contents, 1, Result.Size, File);
        Result.Contents[Result.Size] = 0;

        fclose(File);
    }
    else
    {
        fprintf(stderr, "Failed to open file.\n");
    }

    return(Result);
}

inline s64
atos64(char *Str)
{
    s64 Result = 0;

    while((*Str != 0) &&
          (*Str != '\r') &&
          (*Str != '\n'))
    {
        s32 Digit = *Str - '0';
        Result = (Result*10) + Digit;
        ++Str;
    }

    return(Result);
}

inline s32
StringCompare(char *A, char *B)
{
    s32 Result = 0;
    if(A && B)
    {
        for(;;)
        {
            if(*A == 0)
            {
                Result = 0;
                break;
            }
            else if(*A != *B)
            {
                Result = (*A - *B);
                break;
            }

            ++A, ++B;
        }
    }

    return(Result);
}

#define Kilobytes(Count) ((Count)*1024LL)
#define Megabytes(Count) (Kilobytes(Count)*1024LL)
#define Gigabytes(Count) (Megabytes(Count)*1024LL)

struct memory_arena
{
    umm Size;
    umm Used;
    u8 *Base;
};

static struct memory_arena
InitializeArena(umm Size)
{
    struct memory_arena Result = {};
    Result.Size = Size;
    Result.Used = 0;
    Result.Base = (u8 *)malloc(Size);

    return(Result);
};

#define PushStruct(Arena, Type) (Type *)Push_(Arena, sizeof(Type))
#define PushArray(Arena, Count, Type) (Type *)Push_(Arena, sizeof(Type)*Count)
inline u8 *
Push_(struct memory_arena *Arena, umm Size)
{
    Assert((Arena->Used + Size) < Arena->Size);
    
    u8 *Result = Arena->Base + Arena->Used;
    Arena->Used += Size;

    return(Result);
}

#define INCLUDES_H
#endif
