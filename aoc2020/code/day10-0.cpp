#include "includes.h"

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

    u32 RatingDifferenceHistogram[4] = {};
    u32 CurrentAdapterRating = 0;
    for(u32 AdapterIndex = 0; AdapterIndex < AdapterCount; ++AdapterIndex)
    {
        u32 Number = Adapters[AdapterIndex];
        u32 Difference = Number - CurrentAdapterRating;
        Assert((Difference > 0) && (Difference <= 3));
        ++RatingDifferenceHistogram[Difference];
        CurrentAdapterRating = Number;
    }

    for(u32 Index = 0; Index < ArrayCount(RatingDifferenceHistogram); ++Index)
    {
        printf("%d) %d\n", Index, RatingDifferenceHistogram[Index]);
    }

    u32 Result = RatingDifferenceHistogram[1] * RatingDifferenceHistogram[3];
    printf("Answer: %d\n", Result);

    fclose(File);
    return(0);
}
