#include "includes.h"

inline s64
CountConfigurations(u32 CurrentAdapterRating, u32 *Adapters, u32 AdapterCount, s64 *MemoizedTable)
{
    if(MemoizedTable[CurrentAdapterRating] != -1)
    {
        return(MemoizedTable[CurrentAdapterRating]);
    }

    if(AdapterCount == 0) { return(1); }

    s64 Count = 0;
    for(u32 Index = 0; Index < AdapterCount; ++Index)
    {
        u32 Number = Adapters[Index];
        s32 Difference = Number - CurrentAdapterRating;
        if((Difference < 0) || (Difference > 3))
        {
            break;
        }

        Count += CountConfigurations(Number, Adapters + (Index + 1), AdapterCount - (Index + 1), MemoizedTable);
    }

    MemoizedTable[CurrentAdapterRating] = Count;
    return(Count);
}

int
main(void)
{
    FILE *File = fopen("day10.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file.\n");
        return(1);
    }

    u32 AdapterCount = 0;
    u32 Adapters[128] = {};

    s32 MaxAdapter = INT_MIN;

    char ReadBuffer[8] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        Assert(AdapterCount < ArrayCount(Adapters));
        u32 Number = (u32)atos64(ReadBuffer);
        Adapters[AdapterCount++] = Number;
        MaxAdapter = MAX(MaxAdapter, (s32)Number);
    }

    u32 DeviceRating = MaxAdapter + 3;
    Adapters[AdapterCount++] = DeviceRating;

    s64 MemoizedTable[256] = {};
    for(u32 Index = 0; Index < ArrayCount(MemoizedTable); ++Index)
    {
        MemoizedTable[Index] = -1;
    }

    u64 ConfigurationCount = CountConfigurations(0, Adapters, AdapterCount, MemoizedTable);
    printf("Answer: %lld configurations\n", ConfigurationCount);

    fclose(File);
    return(0);
}
