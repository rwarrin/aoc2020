#include "includes.h"

struct color_node
{
    char Key[32];
    u32 Index;
    u32 Value;
    struct color_node *Next;
};

struct color_table
{
    struct color_node *Table[569];
    u32 ColorCount;
};

inline s32
StringCompare(char *A, char *B)
{
    s32 Result = INT_MIN;

    if(A && B)
    {
        while(*A == *B)
        {
            if((*A == 0) ||
               (*A == '\r') ||
               (*A == '\n'))
            {
                break;
            }

            ++A, ++B;
        }

        Result = (*A - *B);
    }

    return(Result);
}

inline u32
HashColorKey(struct color_table *ColorTable, char *Key)
{
    u32 Hash = 0;
    u32 Constant = 31;
    while((*Key != 0) &&
          (*Key != '\r') &&
          (*Key != '\n'))
    {
        Hash = (Hash + (*Key - 'a' + 1) * Constant) % ArrayCount(ColorTable->Table);
        ++Key;
    }

    return(Hash);
}

static u32
ColorTableInsert(struct color_table *ColorTable, char *Key, u32 Value)
{
    u32 Hash = HashColorKey(ColorTable, Key);

    struct color_node *Node = ColorTable->Table[Hash];
    for(;
        Node;
        Node = Node->Next)
    {
        if(StringCompare(Key, Node->Key) == 0)
        {
            break;
        }
    }

    if(!Node)
    {
        Node = (struct color_node *)malloc(sizeof(struct color_node));
        strncpy(Node->Key, Key, ArrayCount(Node->Key));
        Node->Index = ColorTable->ColorCount++;
        Node->Value = 0;
        Node->Next = ColorTable->Table[Hash];
        ColorTable->Table[Hash] = Node;
    }

    Assert(Node);

    Node->Value += Value;
    return(Node->Index);
}

static u32
ColorTableGet(struct color_table *ColorTable, char *Key)
{
    u32 Result = UINT_MAX;
    u32 Hash = HashColorKey(ColorTable, Key);
    struct color_node *Node = ColorTable->Table[Hash];
    for(;
        Node;
        Node = Node->Next)
    {
        if(StringCompare(Key, Node->Key) == 0)
        {
            Result = Node->Index;
            break;
        }
    }

    return(Result);
}

static b32
RecursivelyCountBags(u8 *AdjacencyList, u32 GraphNodeIndex, u32 MaxVertexCount)
{
    u32 Result = 0;
    for(u32 GraphVertexIndex = 0;
        GraphVertexIndex < MaxVertexCount;
        ++GraphVertexIndex)
    {
        u32 Value = *(AdjacencyList + (GraphNodeIndex * MaxVertexCount) + GraphVertexIndex);
        if(Value)
        {
            Result += Value + Value*RecursivelyCountBags(AdjacencyList, GraphVertexIndex, MaxVertexCount);
        }
    }

    return(Result);
}

int
main(void)
{
    FILE *File = fopen("day07_formatted.txt", "rb");
    if(!File)
    {
        fprintf(stderr, "Failed to open file\n");
        return(1);
    }

    struct color_table ColorTable = {};

    // TODO(rick): This might suck, we might have to change to a sparse storage
    // scheme (linked-lists).
    // NOTE(rick): Using an adjacency list to construct a directed graph.
    static u8 AdjacencyList[594][594] = {};

    char ReadBuffer[256] = {};
    s32 CurrentGraphNodeIndex = -1;
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        char *At = ReadBuffer;
        if(*At != ' ')
        {
            u32 Index = ColorTableInsert(&ColorTable, At, 0);
            CurrentGraphNodeIndex = Index;
        }
        else
        {
            s32 Number = 0;
            sscanf(At, " %d", &Number);
            while(!((*At >= 'a') && (*At <= 'z')))
            {
                ++At;
            }

            Assert(CurrentGraphNodeIndex != -1);
            u32 Index = ColorTableInsert(&ColorTable, At, Number);
            AdjacencyList[CurrentGraphNodeIndex][Index] = (u8)Number;
        }
    }

    u32 TargetGraphIndex = ColorTableGet(&ColorTable, "shiny gold\r\n");
    Assert(TargetGraphIndex < ColorTable.ColorCount);

    u32 TotalBags = 0;
    for(u32 GraphVertexIndex = 0;
        GraphVertexIndex < ArrayCount(AdjacencyList[0]);
        ++GraphVertexIndex)
    {
        u32 Value = AdjacencyList[TargetGraphIndex][GraphVertexIndex];
        if(Value)
        {
            TotalBags += Value + Value*RecursivelyCountBags(&AdjacencyList[0][0], GraphVertexIndex, ArrayCount(AdjacencyList[0]));
        }
    }

    printf("Answer: %d bags\n", TotalBags);

    fclose(File);
    return(0);
}
