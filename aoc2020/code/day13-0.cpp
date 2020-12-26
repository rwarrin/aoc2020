#include "includes.h"

inline b32
IsNumber(char C)
{
    b32 Result = ((C >= '0') && (C <= '9'));
    return(Result);
}

inline b32
IsWhitespace(char C)
{
    b32 Result = ((C == ' ') ||
                  (C == '\t') ||
                  (C == '\r') ||
                  (C == '\n'));
    return(Result);
}

int
main(void)
{
    struct file_contents File = ReadFile("day13.txt");
    Assert(File.Size);

    u32 BusCount = 0;
    s32 Bus[128] = {};

    char *At = File.Contents;
    u32 EarliestDeparture = (s32)atos64(At);
    while(!IsWhitespace(*At++));
    while(*At != 0)
    {
        while(!IsNumber(*At)) { ++At; }
        Assert(BusCount < ArrayCount(Bus));
        Bus[BusCount++] = atoi(At);
        while(IsNumber(*At) || IsWhitespace(*At)) { ++At; }
    }

    u32 BestDistance = UINT_MAX;
    u32 BestBusIndex = UINT_MAX;
    for(u32 BusIndex = 0; BusIndex < BusCount; ++BusIndex)
    {
        s32 Interval = Bus[BusIndex];
        u32 DepartureTime = 0;
        while(DepartureTime < EarliestDeparture)
        {
            DepartureTime += Interval;
        }

        u32 Distance =  DepartureTime - EarliestDeparture;
        if(Distance < BestDistance)
        {
            BestDistance = Distance;
            BestBusIndex = BusIndex;
        }
    }

    u32 Answer = BestDistance * Bus[BestBusIndex];
    printf("Answer: %d (%d * %d)\n", Answer, BestDistance, Bus[BestBusIndex]);

    return(0);
}
