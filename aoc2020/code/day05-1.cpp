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

#define SEAT_LIMIT 1024  // NOTE(rick): This is known based on the result of part 1
    u8 Seats[SEAT_LIMIT] = {};

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
        Seats[SeatID] = 1;
    }

    s32 Seat = INT_MIN;
    for(s32 SeatIndex = 1; SeatIndex < SEAT_LIMIT - 1; ++SeatIndex)
    {
        if((Seats[SeatIndex] == 0) &&
           (Seats[SeatIndex - 1] == 1) &&
           (Seats[SeatIndex + 1] == 1))
        {
            Seat = SeatIndex;
            break;
        }
    }

    printf("Answer: %d\n", Seat);

    fclose(File);
    return(0);
}
