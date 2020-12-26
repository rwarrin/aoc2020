#include "includes.h"

struct number_range
{
    u32 Min;
    u32 Max;
};

struct field
{
    char *Fields;
    struct number_range Ranges[2];
};

struct ticket
{
    u32 ValueCount;
    u32 Values[32];
};

enum parse_mode
{
    ParseMode_Fields,
    ParseMode_MyTicket,
    ParseMode_NearbyTickets,
};

inline void
ParseTicket(struct ticket *Ticket, char *String)
{
    char *At = String;
    for(;;)
    {
        sscanf(At, "%d", &Ticket->Values[Ticket->ValueCount++]);
        while((*At >= '0') && (*At <= '9')) { ++At; }
        ++At;
        if((*At == 0) || (*At == '\r') || (*At == '\n'))
        {
            break;
        }
    }
}

int
main(void)
{
    FILE *File = fopen("day16.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file.\n");
        return(1);
    }

    u32 FieldCount = 0;
    u32 OtherTicketCount = 0;
    struct field Fields[32] = {};
    struct ticket MyTicket = {};
    struct ticket OtherTickets[256] = {};

    u32 ParseMode = ParseMode_Fields;
    char ReadBuffer[128] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        if((ReadBuffer[0] == '\r') ||
           (ReadBuffer[0] == '\n'))
        {
            ParseMode++;
        }

        if(ParseMode == ParseMode_Fields)
        {
            char *At = ReadBuffer;
            while(*At != ':') { ++At; }
            struct field *Field = Fields + FieldCount++;
            sscanf(At, ": %d-%d or %d-%d",
                   &Field->Ranges[0].Min, &Field->Ranges[0].Max,
                   &Field->Ranges[1].Min, &Field->Ranges[1].Max);
        }
        else if(ParseMode == ParseMode_MyTicket)
        {
            if((ReadBuffer[0] >= '0') && (ReadBuffer[0] <= '9'))
            {
                ParseTicket(&MyTicket, ReadBuffer);
            }
        }
        else if(ParseMode == ParseMode_NearbyTickets)
        {
            if((ReadBuffer[0] >= '0') && (ReadBuffer[0] <= '9'))
            {
                ParseTicket(&OtherTickets[OtherTicketCount++], ReadBuffer);
            }
        }
        else
        {
            InvalidCodePath;
        }
    }

    u32 ErrorRate = 0;
    for(u32 OtherTicketIndex = 0;
        OtherTicketIndex < OtherTicketCount;
        ++OtherTicketIndex)
    {
        struct ticket *OtherTicket = OtherTickets + OtherTicketIndex;
        for(u32 OtherTicketFieldIndex = 0;
            OtherTicketFieldIndex < OtherTicket->ValueCount;
            ++OtherTicketFieldIndex)
        {
            u32 OtherTicketFieldValue = OtherTicket->Values[OtherTicketFieldIndex];

            b32 FitARange = false;
            for(u32 FieldIndex = 0;
                !FitARange && (FieldIndex < FieldCount);
                ++FieldIndex)
            {
                struct field *Field = Fields + FieldIndex;
                for(u32 RangeIndex = 0; RangeIndex < ArrayCount(Field->Ranges); ++RangeIndex)
                {
                    struct number_range *Range = Field->Ranges + RangeIndex;
                    if((OtherTicketFieldValue >= Range->Min) && (OtherTicketFieldValue <= Range->Max))
                    {
                        FitARange = true;
                        break;
                    }
                }
            }

            if(!FitARange)
            {
                ErrorRate += OtherTicketFieldValue;
            }
        }
    }

    printf("Answer: %d\n", ErrorRate);

    fclose(File);
    return(0);
}
