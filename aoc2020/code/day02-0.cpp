#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

int
main(void)
{
    printf("Hello world\n");

    FILE *File = fopen("day02.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file.\n");
        return(1);
    }

    u32 GoodPasswordCount = 0;
    char ReadBuffer[128] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        u32 Min = 0;
        u32 Max = 0;
        char Character = 0;
        sscanf(ReadBuffer, "%d-%d %c", &Min, &Max, &Character);

        char *At = ReadBuffer;
        while(*At != ':') { ++At; }
        ++At;

        u32 CharacterCount[26] = {};
        while((*At != 0) && (*At != '\n'))
        {
            if((Character >= 'a') || (Character <= 'z'))
            {
                ++CharacterCount[*At - 'a'];
            }

            ++At;
        }

        u32 Count = CharacterCount[Character - 'a'];
        if((Count >= Min) && (Count <= Max))
        {
            ++GoodPasswordCount;
        }
    }

    printf("Answer: %d good passwords\n", GoodPasswordCount);

    fclose(File);
    return(0);
}
