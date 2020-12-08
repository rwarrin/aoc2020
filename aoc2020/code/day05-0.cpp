#include "includes.h"

int
main(void)
{
    FILE *File = fopen("day05.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file.\n");
        return(1);
    }

    s32 MaxSeatNumber = INT_MIN;

    char ReadBuffer[16] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        u32 RowMin = 0;
        u32 RowMax = 127;
        u32 SeatMin = 0;
        u32 SeatMax = 7;

        u32 Row = 0;
        u32 Seat = 0;

        char *At = ReadBuffer;
        while((*At >= 'A') && (*At <= 'Z'))
        {
            u32 RowMidPoint = (RowMin + RowMax) / 2;
            u32 SeatMidPoint = (SeatMin + SeatMax) / 2;
            switch(*At)
            {
                case 'B':
                {
                    RowMin = RowMidPoint + 1;
                    Row = RowMax;
                } break;
                case 'F':
                {
                    RowMax = RowMidPoint;
                    Row = RowMin;
                } break;
                case 'R':
                {
                    SeatMin = SeatMidPoint + 1;
                    Seat = SeatMax;
                } break;
                case 'L':
                {
                    SeatMax = SeatMidPoint;
                    Seat = SeatMin;
                } break;

                InvalidDefaultCase;
            }

            ++At;
        }

        s32 SeatID = (Row * 8) + Seat;
        MaxSeatNumber = MAX(MaxSeatNumber, SeatID);
#if 0
        printf("%s", ReadBuffer);
        printf("RowMin: %d, RowMax: %d, SeatMin: %d, SeatMax: %d\n",
               RowMin, RowMax, SeatMin, SeatMax);
        printf("Row: %d, Seat: %d\n", Row, Seat);
        printf("SeatID: %d\n\n", SeatID);
#endif
    }

    printf("Answer: %d highest seat number\n", MaxSeatNumber);

    fclose(File);
    return(0);
}
