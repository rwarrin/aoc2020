#include "includes.h"

struct hash_node
{
    u64 Value;
    struct hash_node *Next;
    u64 Key;
};

struct hash_table
{
    struct hash_node *Nodes[512];
};

inline u64
HashKey(u64 Value, struct hash_table *HashTable)
{
    s32 Result = (31 + (17*Value)) % ArrayCount(HashTable->Nodes);
    return(Result);
}

static void
HashTableSet(u64 Key, s64 Value, struct hash_table *HashTable)
{
    u64 Hash = HashKey(Key, HashTable);

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

inline void
PermuteFloatingAddress(struct hash_table *HashTable,
                       s8 *Mask, s32 MaskSize,
                       u64 Value)
{
    b32 DidPermutation = false;

    for(s32 BitIndex = 0; BitIndex < MaskSize; ++BitIndex)
    {
        if(Mask[BitIndex] == UNMASKED_SENTINEL)
        {
            Mask[BitIndex] = 0;
            PermuteFloatingAddress(HashTable, Mask, MaskSize, Value);
            Mask[BitIndex] = 1;
            PermuteFloatingAddress(HashTable, Mask, MaskSize, Value);
            Mask[BitIndex] = UNMASKED_SENTINEL; 
            DidPermutation = true;
            break;
        }
    }

    if(DidPermutation == false)
    {
        u64 Address = 0;
        for(s32 BitIndex = 0; BitIndex < MaskSize; ++BitIndex)
        {
            if(Mask[BitIndex] == 1)
            {
                Address |= ((u64)1 << BitIndex);
            }
            else if(Mask[BitIndex] == 0)
            {
                Address &= ~((u64)1 << BitIndex);
            }
            else
            {
                Assert(Mask[BitIndex] != UNMASKED_SENTINEL);
            }
        }

        HashTableSet(Address, Value, HashTable);
    }
}

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
            u64 Address = 0;
            u64 Value = 0;
            sscanf(ReadBuffer, "mem[%lld] = %lld", &Address, &Value);

            s8 MaskedAddress[36] = {};
            for(s32 BitIndex = 0; BitIndex < ArrayCount(Mask); ++BitIndex)
            {
                if(Mask[BitIndex] == UNMASKED_SENTINEL)
                {
                    MaskedAddress[BitIndex] = UNMASKED_SENTINEL;
                }
                else if(Mask[BitIndex] == 1)
                {
                    MaskedAddress[BitIndex] = 1;
                }
                else
                {
                    MaskedAddress[BitIndex] = ((Address >> BitIndex) & 0x1);
                }
            }

            PermuteFloatingAddress(&HashTable, MaskedAddress, ArrayCount(MaskedAddress), Value);
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
