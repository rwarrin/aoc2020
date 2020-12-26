#include "includes.h"

struct hash_node
{
    u64 Number;
    u64 LastSeen;
    struct hash_node *Next;
};

struct hash_table
{
    struct hash_node *Table[122777];
};

inline u32
HashKey(struct hash_table *HashTable, u64 Key)
{
    u32 Hash = (31 + (Key * 8951)) % ArrayCount(HashTable->Table);
    return(Hash);
}

static u64
HashInsert(struct memory_arena *Arena, struct hash_table *HashTable, u64 Value, u32 Iteration)
{
    u64 Result = 0;

    u32 Bucket = HashKey(HashTable, Value);
    struct hash_node *Node = HashTable->Table[Bucket];
    for(; Node; Node = Node->Next)
    {
        if(Node->Number == Value)
        {
            break;
        }
    }

    if(!Node)
    {
        struct hash_node *Node = PushStruct(Arena, hash_node);
        Assert(Node);
        Node->Number = Value;
        Node->LastSeen = Iteration;
        Node->Next = HashTable->Table[Bucket];
        HashTable->Table[Bucket] = Node;
    }
    else
    {
        Result = Node->LastSeen;
        Node->LastSeen = Iteration;
    }

    return(Result);
}

int
main(void)
{
    u32 SeedNumbers[] = {16, 11, 15, 0, 1, 7};
    u32 TargetIterations = 30000000 - 1;
    u32 Iteration = 0;
    u64 LastNumber = 0;
    struct hash_table HashTable = {};
    struct memory_arena Arena = InitializeArena(Gigabytes(1));

    for(s32 Index = 0; Index < ArrayCount(SeedNumbers); ++Index)
    {
        LastNumber = HashInsert(&Arena, &HashTable, SeedNumbers[Index], ++Iteration);
    }

    while(Iteration != TargetIterations)
    {
        ++Iteration;
        LastNumber = HashInsert(&Arena, &HashTable, LastNumber, Iteration);
        LastNumber = (LastNumber ? (LastNumber = Iteration - LastNumber) : LastNumber);

#if 0
        if(Iteration % 1000000 == 0)
        {
            printf("%8u\n", Iteration);
        }
#endif
    }

    printf("Answer: %lld\n", LastNumber);

    return(0);
}
