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
        u32 First = 0;
        u32 Second = 0;
        char Character = 0;
        sscanf(ReadBuffer, "%d-%d %c", &First, &Second, &Character);

        char *At = ReadBuffer;
        while(*At != ':') { ++At; }
        ++At;
        ++At;

        char FirstCharacter = At[First-1];
        char SecondCharacter = At[Second-1];

        if((FirstCharacter != SecondCharacter) &&
           ((FirstCharacter == Character) || (SecondCharacter == Character)))
        {
            ++GoodPasswordCount;
        }
    }

    printf("Answer: %d good passwords\n", GoodPasswordCount);

    fclose(File);
    return(0);
}
