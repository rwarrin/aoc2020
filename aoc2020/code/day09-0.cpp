#include "includes.h"

inline b32
CanSum(s64 Target, s64 *Numbers, s32 Length, s64 LastDiffend, s32 Depth = 0)
{
    if(Depth > 2) { return(false); }
    if(Target < 0) { return(false); }
    if(Target == 0) { return(true); }

    for(s32 Index = 0; Index < Length; ++Index)
    {
        s64 Number = Numbers[Index];
        if(Number != LastDiffend)
        {
            s64 Difference = Target - Number;

            s64 Result = CanSum(Difference, Numbers + 1, Length -1, Number, Depth + 1);
            if(Result)
            {
                return(true);
            }
        }
    }

    return(false);
}

int
main(void)
{
    FILE *File = fopen("day09.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file.\n");
        return(1);
    }

    s32 PreambleLength = 25;
    s32 CypherCount = 0;
    s64 Cypher[2048] = {};

    char ReadBuffer[32] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        Assert(CypherCount < ArrayCount(Cypher));
        s64 Number = atos64(ReadBuffer);
        Cypher[CypherCount++] = Number;
    }

    s32 InvalidIndex = -1;
    for(s32 Index = PreambleLength; Index < CypherCount; ++Index)
    {
        s64 Target = Cypher[Index];
        s64 *ArrayStart = (Cypher + Index) - PreambleLength;
        if(CanSum(Target, ArrayStart, PreambleLength, 0) == false)
        {
            InvalidIndex = Index;
            break;
        }
    }

    Assert(InvalidIndex >= 0);

    printf("Answer: %lld\n", Cypher[InvalidIndex]);

    fclose(File);
    return(0);
}

