#include "includes.h"

struct hash_node
{
    u64 Value;
    struct hash_node *Next;
    s32 Key;
};

struct hash_table
{
    struct hash_node *Nodes[512];
};

inline s32
HashKey(s32 Value, struct hash_table *HashTable)
{
    s32 Result = (17*Value) % ArrayCount(HashTable->Nodes);
    return(Result);
}

static void
HashTableSet(s32 Key, s64 Value, struct hash_table *HashTable)
{
    s32 Hash = HashKey(Key, HashTable);

    struct hash_node *Node = HashTable->Nodes[Hash];
    for(; Node; Node = Node->Next)
    {
        if(Node->Key == Key)
        {
            break;
        }
    }

    if(!Node)
    {
        Node = (struct hash_node *)malloc(sizeof(*Node));
        Node->Key = Key;
        Node->Next = HashTable->Nodes[Hash];
        HashTable->Nodes[Hash] = Node;
    }

    Assert(Node);
    Node->Value = Value;
}

#define UNMASKED_SENTINEL -1

int
main(void)
{
    FILE *File = fopen("day14.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file.\n");
        return(1);
    }

    struct hash_table HashTable = {};
    s8 Mask[36] = {};

    char ReadBuffer[64] = {};
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        if(ReadBuffer[1] == 'e')
        {
            s32 Address = 0;
            s64 Value = 0;
            sscanf(ReadBuffer, "mem[%d] = %lld", &Address, &Value);
            for(s32 BitIndex = 0; BitIndex < ArrayCount(Mask); ++BitIndex)
            {
                if(Mask[BitIndex] == 0)
                {
                    Value &= ~((s64)1 << BitIndex);
                }
                else if(Mask[BitIndex] == 1)
                {
                    Value |= ((s64)1 << BitIndex);
                }
            }

            HashTableSet(Address, Value, &HashTable);
        }
        else if(ReadBuffer[1] == 'a')
        {
            char *At = ReadBuffer;
            while((*At != 'X') &&
                  (*At != '1') &&
                  (*At != '0'))
            {
                ++At;
            }

            s8 *MaskIndex = Mask + (ArrayCount(Mask) - 1);
            while(*At != 0)
            {
                if(*At == 'X')
                {
                    *MaskIndex = UNMASKED_SENTINEL;
                }
                else if(*At == '1')
                {
                    *MaskIndex = 1;
                }
                else if(*At == '0')
                {
                    *MaskIndex = 0;
                }
                else
                {
                    break;
                }

                ++At, --MaskIndex;
            }
        }
        else
        {
            InvalidCodePath;
        }
    }

    u64 Sum = 0;
    for(s32 BucketIndex = 0; BucketIndex < ArrayCount(HashTable.Nodes); ++BucketIndex)
    {
        for(struct hash_node *Node = HashTable.Nodes[BucketIndex];
            Node;
            Node = Node->Next)
        {
            Sum += Node->Value;
        }
    }

    printf("Answer: %lld\n", Sum);

    fclose(File);
    return(0);
}
