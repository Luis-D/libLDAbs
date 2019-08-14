#include "GeneralLinkedList.h"

struct LD_LinkedList * __LinkedList_Init(struct LD_LinkedList*List,uintptr_t Object_Size,uintptr_t Object_Count)
{
    List->First=NULL;
    List->Last=NULL;
    List->Length=0;
    LD_Object_Pool_Init(&List->Pool,Object_Count,Object_Size);
}

void * __LinkedList_node_allocate(struct LD_LinkedList * LL)
{
    void * tmp = LD_Object_Pool_New(&LL->Pool);
    if(tmp==NULL){return NULL;}  
    return (void*) ((uintptr_t*) tmp);
}

void __LinkedList_node_deallocate(void * Node)
{
    LD_Object_Pool_Delete(((uintptr_t*)Node));
}

void __LD_LinkedList_Reset(struct LD_LinkedList * LinkedList)
{
    LinkedList->First=NULL;
    LinkedList->Last=NULL;
    LinkedList->Length=0;
}

