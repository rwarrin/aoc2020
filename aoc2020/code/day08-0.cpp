#include "includes.h"

enum instruction_mnemonic
{
    Instruction_Invalid,

    Instruction_Accumulate,
    Instruction_Jump,
    Instruction_Noop,

    Instruction_Count,
};

struct instruction
{
    u32 TimesExecuted;
    instruction_mnemonic Mnemonic;
    s32 Parameter;
    char *CodeBytes;
};

struct program
{
    s32 ECX;
    s32 PC;
    s32 InstructionCount;
    struct instruction Instructions[1024];
};

struct file_contents
{
    u32 Size;
    char *Contents;
};

static struct file_contents
ReadFile(char *FileName)
{
    struct file_contents Result = {};

    FILE *File = fopen(FileName, "rb");
    if(File)
    {
        fseek(File, 0, SEEK_END);
        Result.Size = (u32)ftell(File);
        fseek(File, 0, SEEK_SET);

        Result.Contents = (char *)malloc(Result.Size + 1);
        fread(Result.Contents, 1, Result.Size, File);
        Result.Contents[Result.Size] = 0;

        fclose(File);
    }
    else
    {
        fprintf(stderr, "Failed to open file.\n");
    }

    return(Result);
}

int
main(void)
{
    struct file_contents File = ReadFile("day08.txt");
    Assert(File.Size);

    struct program Program = {};
    char *At = File.Contents;
    while(*At != 0)
    {
        if((*At == '\r') ||
           (*At == '\n'))
        {
            if(*At == '\r')
            {
                ++At;
            }
            ++At;

            if(*At == 0)
            {
                break;
            }

            Assert(Program.InstructionCount < ArrayCount(Program.Instructions));
            struct instruction *Instruction = &Program.Instructions[Program.InstructionCount++];

            Instruction->CodeBytes = At;
            if(strncmp("acc", At, 3) == 0)
            {
                Instruction->Mnemonic = Instruction_Accumulate;
                At += 3;
            }
            else if(strncmp("jmp", At, 3) == 0)
            {
                Instruction->Mnemonic = Instruction_Jump;
                At += 3;
            }
            else if(strncmp("nop", At, 3) == 0)
            {
                Instruction->Mnemonic = Instruction_Noop;
                At += 3;
            }

            sscanf(At, "%d", &Instruction->Parameter);
        }

        ++At;
    }

    for(;;)
    {
        struct instruction *Instruction = &Program.Instructions[Program.PC];
        if(Instruction->TimesExecuted)
        {
            break;
        }

        switch(Instruction->Mnemonic)
        {
            case Instruction_Accumulate:
            {
                Program.ECX += Instruction->Parameter;
                ++Program.PC;
            } break;
            case Instruction_Jump:
            {
                Program.PC += Instruction->Parameter;
            } break;
            case Instruction_Noop:
            {
                ++Program.PC;
            } break;

            InvalidDefaultCase;
        }

        ++Instruction->TimesExecuted;
    }

    printf("Answer: %d in the accumulator\n", Program.ECX);

    return(0);
}
