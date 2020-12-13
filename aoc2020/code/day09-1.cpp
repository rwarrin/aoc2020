#include "includes.h"

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

    s32 FirstIndex = 0;
    s32 LastIndex = 0;
    s64 Target = 26796446;
    s32 Found = false;
    for(s32 Index = 0; !Found && (Index < CypherCount); ++Index)
    {
        FirstIndex = Index;
        s64 Sum = Cypher[Index];
        Assert(Sum != Target);

        for(s32 Iter = Index + 1; Iter < CypherCount; ++Iter)
        {
            Sum += Cypher[Iter];
            if(Sum == Target)
            {
                LastIndex = Iter;
                Found = true;
                break;
            }
            else if(Sum > Target)
            {
                break;
            }
        }
    }

    Assert(Found);
    Assert(FirstIndex <= LastIndex);

    s64 Min = INT_MAX;
    s64 Max = INT_MIN;
    for(s32 Index = FirstIndex; Index <= LastIndex; ++Index)
    {
        Min = MIN(Min, Cypher[Index]);
        Max = MAX(Max, Cypher[Index]);
    }
    s64 Sum = Min + Max;

    printf("%d (%d, %d)\n", Found, FirstIndex, LastIndex);
    printf("Answer: %lld (%lld + %lld)\n", Sum, Min, Max);

    fclose(File);
    return(0);
}

