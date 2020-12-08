#include "includes.h"

struct group_data
{
    u8 HashTable[26];
    u8 PositiveAnswers;
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
            SumOfAllGroups += Group.PositiveAnswers;
            Group = {};
        }
        else
        {
            char *At = ReadBuffer;
            while((*At >= 'a') && (*At <= 'z'))
            {
                u32 Hash = *At - 'a';
                if(Group.HashTable[Hash] == 0)
                {
                    ++Group.PositiveAnswers;
                    Group.HashTable[Hash] = 1;
                }

                ++At;
            }
        }
    }

    printf("Answer: %d\n", SumOfAllGroups);

    fclose(File);
    return(0);
}
