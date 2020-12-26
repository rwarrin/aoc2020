#include "includes.h"

struct number_range
{
    u32 Min;
    u32 Max;
};

struct field
{
    struct number_range Ranges[2];
    u32 MatchCount[32];
    char Name[32];
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

static b32
PermuteFieldConfigs(struct field *Fields, u32 FieldCount, u32 FieldIndex,
                    s32 *FieldMapping, u32 ValidFieldValue)
{
    if(FieldIndex >= FieldCount)
    {
        return(true);
    }

    b32 Result = false;
    struct field *Field = Fields + FieldIndex;
    for(u32 MatchIndex = 0; MatchIndex < ArrayCount(Field->MatchCount); ++MatchIndex)
    {
        if(Field->MatchCount[MatchIndex] == ValidFieldValue)
        {
            if(FieldMapping[MatchIndex] == -1)
            {
                FieldMapping[MatchIndex] = FieldIndex;
                Result = PermuteFieldConfigs(Fields, FieldCount, FieldIndex + 1, FieldMapping, ValidFieldValue);
                if(Result == true)
                {
                    break;
                }

                FieldMapping[MatchIndex] = -1;
            }
        }
    }

    return(Result);
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
            struct field *Field = Fields + FieldCount++;
            char *At = ReadBuffer;
            char *NameAt = Field->Name;
            while(*At != ':')
            {
                *NameAt++ = *At++;
            }
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

    for(u32 TicketIndex = 0; TicketIndex < OtherTicketCount; )
    {
        b32 IsTicketValid = true;
        struct ticket *Ticket = OtherTickets + TicketIndex;

        for(u32 ValueIndex = 0; IsTicketValid && (ValueIndex < Ticket->ValueCount); ++ValueIndex)
        {
            u32 Value = Ticket->Values[ValueIndex];

            u32 ValidFieldCount = 0;
            for(u32 FieldIndex = 0; (FieldIndex < FieldCount); ++FieldIndex)
            {
                struct field *Field = Fields + FieldIndex;
                struct number_range Lower = Field->Ranges[0];
                struct number_range Upper = Field->Ranges[1];

                if(((Value >= Lower.Min) && (Value <= Lower.Max)) ||
                   ((Value >= Upper.Min) && (Value <= Upper.Max)))
                {
                    ++ValidFieldCount;
                }
            }

            if(!ValidFieldCount)
            {
                IsTicketValid = false;
            }
        }

        if(!IsTicketValid)
        {
            OtherTickets[TicketIndex] = OtherTickets[--OtherTicketCount];
        }
        else
        {
            for(u32 ValueIndex = 0; IsTicketValid && (ValueIndex < Ticket->ValueCount); ++ValueIndex)
            {
                u32 Value = Ticket->Values[ValueIndex];

                for(u32 FieldIndex = 0; (FieldIndex < FieldCount); ++FieldIndex)
                {
                    struct field *Field = Fields + FieldIndex;
                    struct number_range Lower = Field->Ranges[0];
                    struct number_range Upper = Field->Ranges[1];

                    if(((Value >= Lower.Min) && (Value <= Lower.Max)) ||
                       ((Value >= Upper.Min) && (Value <= Upper.Max)))
                    {
                        ++Field->MatchCount[ValueIndex];
                    }
                }
            }

            ++TicketIndex;
        }
    }

    s32 FieldMapping[32] = {};
    for(u32 FieldIndex = 0; FieldIndex < ArrayCount(FieldMapping); ++FieldIndex)
    {
        FieldMapping[FieldIndex] = -1;
    }
    b32 Found = PermuteFieldConfigs(Fields, FieldCount, 0, FieldMapping, OtherTicketCount);
    Assert(Found);

    u64 Answer = 1;
    for(u32 FieldIndex = 0; FieldIndex < ArrayCount(FieldMapping); ++FieldIndex)
    {
        s32 FieldPosition = FieldMapping[FieldIndex];
        struct field *Field = Fields + FieldPosition;
        if(StringCompare("departure", Field->Name) == 0)
        {
            Answer = Answer * (MyTicket.Values[FieldIndex]);
        }
    }

    printf("Answer: %lld\n", Answer);

    fclose(File);
    return(0);
}
