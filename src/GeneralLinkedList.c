#include "GeneralLinkedList.h"
#include "ObjectPool.h"

struct LD_LinkedList * __LinkedList_Init(struct LD_LinkedList*List,size_t Object_Size,size_t Object_Count)
{
    List->First=NULL;
    List->Last=NULL;
    List->Length=0;
    LD_ObjectPool_Init_EXT(&List->Pool,Object_Count,Object_Size);
}

void * __LinkedList_node_allocate(struct LD_LinkedList * LL)
{
    void * tmp = LD_ObjectPool_New(&LL->Pool);
    if(tmp==NULL){return NULL;}  
    return (void*) ((uintptr_t*) tmp);
}

void __LinkedList_node_deallocate(void * Node)
{
    LD_ObjectPool_Delete(((uintptr_t*)Node));
}

void __LD_LinkedList_Reset(struct LD_LinkedList * LinkedList)
{
    LinkedList->First=NULL;
    LinkedList->Last=NULL;
    LinkedList->Length=0;
}



void LD_LinkedList_Destroy(struct LD_LinkedList * LinkedList)
{
    LD_ObjectPool_deInit(&LinkedList->Pool);
    free(LinkedList);
}

void LD_LinkedList_Clear(struct LD_LinkedList * LinkedList)
{
    LD_ObjectPool_Clear(&LinkedList->Pool);
    __LD_LinkedList_Reset(LinkedList);
}
