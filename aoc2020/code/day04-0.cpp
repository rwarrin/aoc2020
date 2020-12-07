#include "includes.h"

struct field_info
{
    char *Name;
    u32 Flag;
};

struct field_info FieldInfos[] = 
{
    {"byr", 0x01},  // (Birth Year) Required
    {"iyr", 0x02},  // (Issue Year) Required
    {"eyr", 0x04},  // (Expiration Year) Required
    {"hgt", 0x08},  // (Height) Required
    {"hcl", 0x10},  // (Hair Color) Required
    {"ecl", 0x20},  // (Eye Color) Required
    {"pid", 0x40},  // (Passport ID) Required
    {"cid", 0x80},  // (Country ID) Not-Required
};

int
main(void)
{
    FILE *File = fopen("day04.txt", "rb");
    if(!File)
    {
        printf("Failed to open file.\n");
        return(1);
    }

    u32 ValidRecord = (0x100 - 1) & (~FieldInfos[ArrayCount(FieldInfos) - 1].Flag);
    u32 ValidRecordCount = 0;

    u32 Record = 0;
    char ReadBuffer[128] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        if((ReadBuffer[0] == '\r') || (ReadBuffer[0] == '\n'))
        {
            if((Record == ValidRecord) || (Record == (0x100 - 1)))
            {
                ++ValidRecordCount;
            }
            Record = 0;
        }
        else
        {
            for(u32 FieldIndex = 0; FieldIndex < ArrayCount(FieldInfos); ++FieldIndex)
            {
                struct field_info *FieldInfo = FieldInfos + FieldIndex;
                if(strstr(ReadBuffer, FieldInfo->Name) != 0)
                {
                    Record |= FieldInfo->Flag;
                }
            }
        }
    }

    printf("Answer: %d valid passports\n", ValidRecordCount);

    fclose(File);
    return(0);
}
