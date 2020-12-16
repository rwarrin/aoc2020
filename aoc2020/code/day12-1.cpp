#include "includes.h"

struct object
{
    v2i P;
    f32 Direction;
};

int
main(void)
{
    FILE *File = fopen("day12.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file.\n");
        return(1);
    }

    struct object Ship = {};
    struct object Waypoint = {}; 
    Waypoint.P = {10, 1};
    //Waypoint.Direction = atan2f(Waypoint.P.x, Waypoint.P.y);

    char ReadBuffer[32] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        char Action = ReadBuffer[0];
        char *AmountPtr = &ReadBuffer[1];
        s32 Amount = (s32)atos64(AmountPtr);
        switch(Action)
        {
            case 'N':
            {
                Waypoint.P.y += Amount;
            } break;
            case 'S':
            {
                Waypoint.P.y -= Amount;
            } break;
            case 'E':
            {
                Waypoint.P.x += Amount;
            } break;
            case 'W':
            {
                Waypoint.P.x -= Amount;
            } break;
            case 'L':
            {
                v2i Offset = {Waypoint.P.x - Ship.P.x, Waypoint.P.y - Ship.P.y};

                Amount = -Amount + 360;
                for(s32 i = 0, Times = Amount/90; i < Times; ++i)
                {
                    s32 Temp = Offset.x;
                    Offset.x = Offset.y;
                    Offset.y = -Temp;
                }
                Waypoint.P.x = Ship.P.x + Offset.x;
                Waypoint.P.y = Ship.P.y + Offset.y;
            } break;
            case 'R':
            {
                v2i Offset = {Waypoint.P.x - Ship.P.x, Waypoint.P.y - Ship.P.y};

                Amount = Amount + 360;
                for(s32 i = 0, Times = Amount/90; i < Times; ++i)
                {
                    s32 Temp = Offset.x;
                    Offset.x = Offset.y;
                    Offset.y = -Temp;
                }
                Waypoint.P.x = Ship.P.x + Offset.x;
                Waypoint.P.y = Ship.P.y + Offset.y;
            } break;
            case 'F':
            {
                v2i Offset = {Waypoint.P.x - Ship.P.x, Waypoint.P.y - Ship.P.y};
                for(s32 Moved = 0; Moved < Amount; ++Moved)
                {
                    Ship.P = Waypoint.P;
                    Waypoint.P.x += Offset.x;
                    Waypoint.P.y += Offset.y;
                }
            } break;

            InvalidDefaultCase;
        }
    }

    s32 ManhattanDistance = (s32)(abs(Ship.P.x) + abs(Ship.P.y));
    printf("Answer: %d\n", ManhattanDistance);

    fclose(File);
    return(0);
}
