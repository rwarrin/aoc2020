#include "includes.h"

inline s32
CheckForSeatInDirection(char *Map, s32 Width,
                        s32 Y, s32 X,
                        s32 ChangeY, s32 ChangeX,
                        s32 MinX, s32 MinY,
                        s32 MaxX, s32 MaxY)
{
    s32 Result = 0;
    for(;;)
    {
        Y += ChangeY;
        X += ChangeX;

        if( ((Y >= MinY) && (Y < MaxY)) &&
            ((X >= MinX) && (X < MaxX)) )
        {
            char CurrentCell = *(Map + (Y*Width) + X);
            if(CurrentCell == '#')
            {
                Result = 1;
                break;
            }
            else if(CurrentCell == 'L')
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return(Result);
}

int
main(void)
{
    file_contents Data = ReadFile("day11.txt");
    Assert(Data.Size > 0);

    s32 Width = 0;
    s32 Height = 0;
    sscanf(Data.Contents, "%d %d", &Width, &Height);

    char *MapA = (char *)malloc(sizeof(char)*Width*Height + 1);
    MapA[Width*Height] = 0;
    char *MapB = (char *)malloc(sizeof(char)*Width*Height + 1);
    MapB[Width*Height] = 0;

    char *At = Data.Contents;
    while((*At != 0) &&
          (*At != '\r') &&
          (*At != '\n'))
    {
        if(*At == '\r')
        {
            ++At;
        }
        ++At;
    }

    char *Init = MapA;
    while(*At != 0)
    {
        if((*At == '\r') ||
           (*At == '\n'))
        {
            ++At;
        }
        else
        {
            *Init++ = *At++;
        }
    }

    u32 StateChangeCount = 0;
    u32 Iterations = 0;
    do
    {
        StateChangeCount = 0;
        for(s32 Y = 0; Y < Height; ++Y)
        {
            for(s32 X = 0; X < Width; ++X)
            {
                char CellValue = *(MapA + (Y*Width) + X);
                if(CellValue != '.')
                {
                    u32 OccupiedNeighbors = 0;
                    OccupiedNeighbors += CheckForSeatInDirection(MapA, Width, Y, X, -1, -1, 0, 0, Width, Height);  // NW
                    OccupiedNeighbors += CheckForSeatInDirection(MapA, Width, Y, X, -1, 0, 0, 0, Width, Height);  // N
                    OccupiedNeighbors += CheckForSeatInDirection(MapA, Width, Y, X, -1, 1, 0, 0, Width, Height);  // NE

                    OccupiedNeighbors += CheckForSeatInDirection(MapA, Width, Y, X, 0, -1, 0, 0, Width, Height);  // W
                    OccupiedNeighbors += CheckForSeatInDirection(MapA, Width, Y, X, 0, 1, 0, 0, Width, Height);  // E

                    OccupiedNeighbors += CheckForSeatInDirection(MapA, Width, Y, X, 1, -1, 0, 0, Width, Height);  // SW
                    OccupiedNeighbors += CheckForSeatInDirection(MapA, Width, Y, X, 1, 0, 0, 0, Width, Height);  // S
                    OccupiedNeighbors += CheckForSeatInDirection(MapA, Width, Y, X, 1, 1, 0, 0, Width, Height);  // SE

                    if((CellValue == 'L') && (OccupiedNeighbors == 0))
                    {
                        *(MapB + (Y*Width) + X) = '#';
                        ++StateChangeCount;
                    }
                    else if((CellValue == '#') && (OccupiedNeighbors >= 5))
                    {
                        *(MapB + (Y*Width) + X) = 'L';
                        ++StateChangeCount;
                    }
                    else
                    {
                        *(MapB + (Y*Width) + X) = *(MapA + (Y*Width) + X);
                    }
                }
                else
                {
                    *(MapB + (Y*Width) + X) = *(MapA + (Y*Width) + X);
                }
            }
        }

        char *Temp = MapA;
        MapA = MapB;
        MapB = Temp;
        ++Iterations;
    } while(StateChangeCount);

    u32 OccupiedCount = 0;
    At = MapA;
    while(*At != 0)
    {
        if(*At == '#')
        {
            ++OccupiedCount;
        }
        ++At;
    }

    printf("Answer: %d occupied seats\n", OccupiedCount);

    return(0);
}
