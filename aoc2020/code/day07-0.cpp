#include "includes.h"

struct color_node
{
    char Key[32];
    u32 Index;
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
ColorTableInsert(struct color_table *ColorTable, char *Key)
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
        Node->Next = ColorTable->Table[Hash];
        ColorTable->Table[Hash] = Node;
    }

    Assert(Node);
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
RecursivelyTestBag(u8 *AdjacencyList, u32 GraphNodeIndex, u32 MaxVertexCount, u32 TargetGraphNodeIndex)
{
    if(GraphNodeIndex == TargetGraphNodeIndex)
    {
        return true;
    }

    b32 Result = false;
    for(u32 GraphVertexIndex = 0;
        !Result && (GraphVertexIndex < MaxVertexCount);
        ++GraphVertexIndex)
    {
        u8 Value = *(AdjacencyList + (GraphNodeIndex * MaxVertexCount) + GraphVertexIndex);
        if(Value == 2)
        {
            Result = true;
        }
        else if(Value == 1)
        {
            Result = RecursivelyTestBag(AdjacencyList, GraphVertexIndex, MaxVertexCount, TargetGraphNodeIndex);
            if(Result)
            {
                *(AdjacencyList + (GraphNodeIndex * MaxVertexCount) + GraphVertexIndex) = 2;
            }
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
    u8 AdjacencyList[594][594] = {};

    char ReadBuffer[256] = {};
    s32 CurrentGraphNodeIndex = -1;
    while((fgets(ReadBuffer, ArrayCount(ReadBuffer), File)) != 0)
    {
        char *At = ReadBuffer;
        if(*At != ' ')
        {
            u32 Index = ColorTableInsert(&ColorTable, At);
            CurrentGraphNodeIndex = Index;
        }
        else
        {
            while(!((*At >= 'a') && (*At <= 'z')))
            {
                ++At;
            }

            Assert(CurrentGraphNodeIndex != -1);
            u32 Index = ColorTableInsert(&ColorTable, At);
            AdjacencyList[CurrentGraphNodeIndex][Index] = 1;
        }
    }

    u32 TargetGraphIndex = ColorTableGet(&ColorTable, "shiny gold\r\n");
    Assert(TargetGraphIndex < ColorTable.ColorCount);

    u32 GraphNodesThatLeadToTarget = 0;
    for(u32 GraphNodeIndex = 0;
        GraphNodeIndex < ArrayCount(AdjacencyList);
        ++GraphNodeIndex)
    {
        for(u32 GraphVertexIndex = 0;
            GraphVertexIndex < ArrayCount(AdjacencyList[0]);
            ++GraphVertexIndex)
        {
            u32 Value = AdjacencyList[GraphNodeIndex][GraphVertexIndex];
            if(Value == 2)
            {
                ++GraphNodesThatLeadToTarget;
                break;
            }
            else if(Value == 1)
            {
                u32 TargetReached = RecursivelyTestBag(&AdjacencyList[0][0], GraphVertexIndex, ArrayCount(AdjacencyList[0]), TargetGraphIndex);
                if(TargetReached)
                {
                    ++GraphNodesThatLeadToTarget;
                    break;
                }
            }
        }
    }

    printf("Answer: %d bags\n", GraphNodesThatLeadToTarget);

    fclose(File);
    return(0);
}
