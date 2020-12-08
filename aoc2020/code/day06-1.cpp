#include "includes.h"

struct group_data
{
    u8 HashTable[26];
    u8 MembersInGroup;
};

int
main(void)
{
    FILE *File = fopen("day06.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file.\n");
        return(1);
    }

    u32 SumOfAllGroups = 0;

    struct group_data Group = {};

    char ReadBuffer[32] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        if((ReadBuffer[0] == '\r') || (ReadBuffer[0] == '\n'))
        {
            u32 PositiveAnswers = 0;
            for(u32 Index = 0; Index < ArrayCount(Group.HashTable); ++Index)
            {
                if(Group.HashTable[Index] == Group.MembersInGroup)
                {
                    ++PositiveAnswers;
                }
            }
            SumOfAllGroups += PositiveAnswers;

            Group = {};
        }
        else
        {
            char *At = ReadBuffer;
            while((*At >= 'a') && (*At <= 'z'))
            {
                u32 Hash = *At - 'a';
                ++Group.HashTable[Hash];

                ++At;
            }
            ++Group.MembersInGroup;
        }
    }

    printf("Answer: %d\n", SumOfAllGroups);

    fclose(File);
    return(0);
}
