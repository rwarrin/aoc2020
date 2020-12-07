#include "includes.h"

inline char *
ForwardToValue(char *At)
{
    while(*At != ':') { ++At; };
    ++At;

    return(At);
}

inline b32
NumberInRange(s32 Min, s32 Number, s32 Max)
{
    b32 Result = 0;
    if((Number >= Min) && (Number <= Max))
    {
        Result = true;
    }

    return(Result);
}

inline b32
ValidateBYR(char *Str)
{
    // byr (Birth Year) - four digits; at least 1920 and at most 2002.
    b32 Result = false;
    char *At = ForwardToValue(Str);

    u32 Year = 0;
    if(sscanf(At, "%d", &Year) == 1)
    {
        Result = NumberInRange(1920, Year, 2002);
    }

    return(Result);
}

inline b32
ValidateIYR(char *Str)
{
    // iyr (Issue Year) - four digits; at least 2010 and at most 2020.
    b32 Result = false;
    char *At = ForwardToValue(Str);

    u32 Year = 0;
    if(sscanf(At, "%d", &Year) == 1)
    {
        Result = NumberInRange(2010, Year, 2020);
    }

    return(Result);
}

inline b32
ValidateEYR(char *Str)
{
    // eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
    b32 Result = false;
    char *At = ForwardToValue(Str);

    u32 Year = 0;
    if(sscanf(At, "%d", &Year) == 1)
    {
        Result = NumberInRange(2020, Year, 2030);
    }

    return(Result);
}

inline b32
ValidateHGT(char *Str)
{
    // hgt (Height) - a number followed by either cm or in:
    // If cm, the number must be at least 150 and at most 193.
    // If in, the number must be at least 59 and at most 76.
    b32 Result = false;
    char *At = ForwardToValue(Str);

    u32 Height = 0;
    char Unit = 0;
    if(sscanf(At, "%d%c", &Height, &Unit) == 2)
    {
        if(Unit == 'c')
        {
            Result = NumberInRange(150, Height, 193);
        }
        else if(Unit == 'i')
        {
            Result = NumberInRange(59, Height, 76);
        }
    }

    return(Result);
}

inline b32
ValidateHCL(char *Str)
{
    // hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
    b32 Result = false;
    char *At = ForwardToValue(Str);

    s32 RequiredLength = 6;
    char Color[12] = {};
    if(sscanf(At, "#%s", &Color) == 1)
    {
        s32 Length = (s32)strlen(Color);
        if(Length == RequiredLength)
        {
            Result = true;
            for(s32 Index = 0; Index < Length; ++Index)
            {
                char Character = Color[Index];
                if((Character != 0) &&
                   (((Character >= 'a') && (Character <= 'f')) ||
                    ((Character >= '0') && (Character <= '9'))))
                {
                }
                else
                {
                    Result = false;
                    break;
                }
            }
        }
    }

    return(Result);
}

static char *HairColors[] =
{
    "amb",
    "blu",
    "brn",
    "gry",
    "grn",
    "hzl",
    "oth",
};

inline b32
ValidateECL(char *Str)
{
    // ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
    b32 Result = false;
    char *At = ForwardToValue(Str);

    char Color[3] = {};
    if(sscanf(At, "%s", &Color) == 1)
    {
        for(u32 Index = 0; Index < ArrayCount(HairColors); ++Index)
        {
            if(strcmp(Color, HairColors[Index]) == 0)
            {
                Result = true;
                break;
            }
        }
    }

    return(Result);
}

inline b32
ValidatePID(char *Str)
{
    // pid (Passport ID) - a nine-digit number, including leading zeroes.
    b32 Result = false;
    char *At = ForwardToValue(Str);

    u32 RequiredLength = 9;
    u32 Length = 0;
    while((*At >= '0') && (*At <= '9'))
    {
        ++At, ++Length;
    }

    if(Length == RequiredLength)
    {
        Result = true;
    }

    return(Result);
}

inline b32
ValidateCID(char *Str)
{
    // cid (Country ID) - ignored, missing or not.
    return(true);
}

struct field_info
{
    char *Name;
    u32 Flag;
    b32 (*ValidationFunc)(char *);
};

struct field_info FieldInfos[] = 
{
    {"byr", 0x01, ValidateBYR},  // (Birth Year) Required
    {"iyr", 0x02, ValidateIYR},  // (Issue Year) Required
    {"eyr", 0x04, ValidateEYR},  // (Expiration Year) Required
    {"hgt", 0x08, ValidateHGT},  // (Height) Required
    {"hcl", 0x10, ValidateHCL},  // (Hair Color) Required
    {"ecl", 0x20, ValidateECL},  // (Eye Color) Required
    {"pid", 0x40, ValidatePID},  // (Passport ID) Required
    {"cid", 0x80, ValidateCID},  // (Country ID) Not-Required
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

                char *At = 0;
                if((At = strstr(ReadBuffer, FieldInfo->Name)) != 0)
                {
                    b32 Valid = FieldInfo->ValidationFunc(At);
                    if(Valid)
                    {
                        Record |= FieldInfo->Flag;
                    }
                }
            }
        }
    }

    printf("Answer: %d valid passports\n", ValidRecordCount);

    fclose(File);
    return(0);
}
