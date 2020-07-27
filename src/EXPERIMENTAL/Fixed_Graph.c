#include "Fixed_Graph.h"

struct LD_Fixed_Graph * LD_Fixed_Graph_Init(struct LD_Fixed_Graph * Graph,uintptr_t Object_Size, uintptr_t Max_Grade,uintptr_t Order)
{
    void * R=LD_Object_Pool_Init(&Graph->Pool,Order,Object_Size+(sizeof(void*)*Max_Grade)+sizeof(uintptr_t));
    Graph->Object_Size = Object_Size;
    Graph->Max_Grade = Max_Grade;
    Graph->Order = 0; 
    return R;
}

struct LD_Fixed_Graph * LD_Fixed_Graph_Create(uintptr_t Object_Size, uintptr_t Max_Grade,uintptr_t Order)
{
    struct LD_Fixed_Graph * RETURN = 
	(struct LD_Fixed_Graph*) malloc(sizeof(struct LD_Fixed_Graph));
    if(RETURN ==NULL){return NULL;}
    LD_Fixed_Graph_Init(RETURN,Object_Size,Max_Grade,Order);
    return RETURN;
}

void LD_Fixed_Graph_Clear(struct LD_Fixed_Graph * Graph)
{
    LD_Object_Pool_Clear(&Graph->Pool);
    Graph->Order=0;
}
void LD_Fixed_Graph_Destroy(struct LD_Fixed_Graph * Graph)
{
    LD_Fixed_Graph_Clear(Graph);
    free(Graph);
}

void LD_Fixed_Graph_attach(struct LD_Fixed_Graph * Graph,void * Node_ref,uintptr_t Edge, void * Node)
{
    *(((uintptr_t*)Node_ref)-2-(Edge)) = Node;
}

void * LD_Fixed_Graph_add(struct LD_Fixed_Graph * Graph, void * Node, uintptr_t Edge,char Directed,uintptr_t Data_Edge,void * Data);
{
    void * Node = LD_Object_Pool_New(Graph->Object_Pool);
    if(Node==NULL){return NULL;}

    void * Node_Data = ((uintptr_t*)Node)+1+Graph->Max_Grade;

    memcpy(Node_Data,Data,Graph->Object_Size);

    if(Node !== NULL)
    {
	LD_Fixed_Graph_attach(Graph,Node,Edge,Node_Data);
	if(Directed != 0)
	{LD_Fixed_Graph_attach(Graph,Node_Data,Data_Edge,Node);}
    }

    (Graph->Order)++;

    return Node_Data;
}


void* LD_Fixed_Graph_follow(void * Node, uintptr_t Edge)
{
    uintptr_t Grade = *((uintptr_t*)Node-1);
    void * Head = ((uintptr_t*)Node)-Grade-1;
    memset(Head,0,sizeof(void*)*Grade); 
}

void LD_Fixed_Graph_remove(struct LD_Fixed_Graph * Graph, void * Node)
{
    //Maybe detach the Node (?

    uintptr_t Grade = *((uintptr_t*)Node-1);
    void * Head = ((uintptr_t*)Node)-Grade-1;
    LD_Object_Pool_Delete(Head);
}
