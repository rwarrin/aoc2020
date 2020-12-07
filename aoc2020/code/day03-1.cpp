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

inline u32
CountTrees(FILE *File, u32 SideStepSize, u32 DownStepSize)
{
    Assert(File);
    fseek(File, 0, SEEK_SET);

    u32 TotalSteps = 0;
    u32 ActualSteps = 0;

    u32 TreeCount = 0;
    char ReadBuffer[64] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        if(TotalSteps % DownStepSize == 0)
        {
            u32 RowLength = StringLength(ReadBuffer);

            char CharacterAtTarget = ReadBuffer[((ActualSteps * SideStepSize) % RowLength)];
            if(CharacterAtTarget == '#')
            {
                ++TreeCount;
            }

            ++ActualSteps;
        }

        ++TotalSteps;
    }

    return(TreeCount);
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

    u32 A = CountTrees(File, 1, 1);
    u32 B = CountTrees(File, 3, 1);
    u32 C = CountTrees(File, 5, 1);
    u32 D = CountTrees(File, 7, 1);
    u32 E = CountTrees(File, 1, 2);

    printf("Trees found in slope A r:1 d:1 = %d\n", A);
    printf("Trees found in slope B r:3 d:1 = %d\n", B);
    printf("Trees found in slope C r:5 d:1 = %d\n", C);
    printf("Trees found in slope D r:7 d:1 = %d\n", D);
    printf("Trees found in slope E r:1 d:2 = %d\n", E);

    r64 Result = A*B*C*D*E;
    printf("Answer: %.0f\n", Result);

    fclose(File);
    return(0);
}
