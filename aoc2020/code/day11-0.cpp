#include "includes.h"

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
                    b32 YMinusOne = ((Y - 1) >= 0);
                    b32 YPlusOne = ((Y + 1) < Height);
                    b32 XMinusOne = ((X - 1) >= 0);
                    b32 XPlusOne = ((X + 1) < Width);

                    if(YMinusOne)
                    {
                        if(XMinusOne)
                        {
                            OccupiedNeighbors += ((*(MapA + ((Y - 1)*Width) + (X - 1)) == '#') ? 1 : 0);
                        }

                        OccupiedNeighbors += ((*(MapA + ((Y - 1)*Width) + X) == '#') ? 1 : 0);

                        if(XPlusOne)
                        {
                            OccupiedNeighbors += ((*(MapA + ((Y - 1)*Width) + (X + 1)) == '#') ? 1 : 0);
                        }
                    }

                    if(XMinusOne)
                    {
                        OccupiedNeighbors += ((*(MapA + (Y*Width) + (X - 1)) == '#') ? 1 : 0);
                    }
                    if(XPlusOne)
                    {
                        OccupiedNeighbors += ((*(MapA + (Y*Width) + (X + 1)) == '#') ? 1 : 0);
                    }

                    if(YPlusOne)
                    {
                        if(XMinusOne)
                        {
                            OccupiedNeighbors += ((*(MapA + ((Y + 1)*Width) + (X - 1)) == '#') ? 1 : 0);
                        }

                        OccupiedNeighbors += ((*(MapA + ((Y + 1)*Width) + X) == '#') ? 1 : 0);

                        if(XPlusOne)
                        {
                            OccupiedNeighbors += ((*(MapA + ((Y + 1)*Width) + (X + 1)) == '#') ? 1 : 0);
                        }
                    }

                    if((CellValue == 'L') && (OccupiedNeighbors == 0))
                    {
                        *(MapB + (Y*Width) + X) = '#';
                        ++StateChangeCount;
                    }
                    else if((CellValue == '#') && (OccupiedNeighbors >= 4))
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
