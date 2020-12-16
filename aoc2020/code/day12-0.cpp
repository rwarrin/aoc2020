#include "includes.h"

struct ship
{
    v2 P;
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

    struct ship Ship = {};

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
                Ship.P.y += Amount;
            } break;
            case 'S':
            {
                Ship.P.y -= Amount;
            } break;
            case 'E':
            {
                Ship.P.x += Amount;
            } break;
            case 'W':
            {
                Ship.P.x -= Amount;
            } break;
            case 'L':
            {
                Ship.Direction += Amount;
            } break;
            case 'R':
            {
                Ship.Direction -= Amount;
            } break;
            case 'F':
            {
                v2 Direction = {(f32)cos(Ship.Direction*(PI/180.0f)), (f32)sin(Ship.Direction*(PI/180.0f))};
                Ship.P.x += (s32)(Direction.x * (f32)Amount);
                Ship.P.y += (s32)(Direction.y * (f32)Amount);
            } break;

            InvalidDefaultCase;
        }
    }

    s32 ManhattanDistance = (s32)(fabs(Ship.P.x) + fabs(Ship.P.y));
    printf("Answer: %d\n", ManhattanDistance);

    fclose(File);
    return(0);
}
