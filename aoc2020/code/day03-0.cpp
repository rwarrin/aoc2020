#include "includes.h"

/** NOTE(rick):
 * . = open
 * # = tree
 * right 3, down 1
 **/

inline u32
StringLength(char *Str)
{
    u32 Length = 0;
    while((Str[0] != 0) &&
          (Str[0] != '\r') &&
          (Str[0] != '\n'))
    {
        ++Str, ++Length;
    }

    return(Length);
}

int
main(void)
{
    FILE *File = fopen("day03.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file.\n");
        return(1);
    }

    u32 StepSize = 3;
    u32 Step = 0;

    u32 TreeCount = 0;
    char ReadBuffer[64] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        u32 RowLength = StringLength(ReadBuffer);

        char CharacterAtTarget = ReadBuffer[((Step * StepSize) % RowLength)];
        if(CharacterAtTarget == '#')
        {
            ++TreeCount;
        }

        ++Step;
    }

    printf("Answer: %d trees run into\n", TreeCount);

    fclose(File);
    return(0);
}
