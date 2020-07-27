#ifndef LD_FIXED_GRAPH
#define LD_FIXED_GRAPH

#include "Object_Pool.h"

struct LD_Fixed_Graph
{
    struct LD_Object_Pool Pool;
    uintptr_t Object_Size;
    uintptr_t Max_Grade;
    uintptr_t Order;
};
#define LD_FIXED_GRAPH struct LD_Fixed_Graph

struct LD_Fixed_Graph * LD_Fixed_Graph_Init(struct LD_Fixed_Graph * Graph,uintptr_t Object_Size, uintptr_t Max_Grade,uintptr_t Order);
struct LD_Fixed_Graph * LD_Fixed_Graph_Create(uintptr_t Object_Size, uintptr_t Max_Grade,uintptr_t Order);
void LD_Fixed_Graph_Clear(void LD_Fixed_Graph * Graph);
void LD_Fixed_Graph_Destroy(void LD_Fixed_Graph * Graph);

void LD_Fixed_Graph_attach(struct LD_Fixed_Graph * Graph,void * Node_ref,uintptr_t Edge, void * Node);
void LD_Fixed_Graph_detach(struct LD_Fixed_Graph * Graph,void * Node);

void * LD_Fixed_Graph_add(struct LD_Fixed_Graph * Graph, void * Node, uintptr_t Edge,char Directed,uintptr_t Data_Edge,void * Data);
void LD_Fixed_Graph_remove(struct LD_Fixed_Graph * Graph, void * Node);


void* LD_Fixed_Graph_follow(void * Node, uintptr_t Edge);
uintptr_t LD_Fixed_Graph_get_grade(void * Node);

#endif
