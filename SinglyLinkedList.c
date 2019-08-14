#include "SinglyLinkedList.h"

#define S_LinkedList_Node_Define(Datatype) \
    struct {void * Next; Datatype Data;}
#define __gll_s_ptr(x) ((S_LinkedList_Node_Define(void*)*) x)


    //--- Single Linked List functions---//


#define _SLL_Head(Node) (((uintptr_t*)Node)-1)

#define SLL_PRO \
    LL_PRO_PRO(1)\
    void * LLL = LinkedList->Last;\
    void * LLF = LinkedList->First

struct LD_LinkedList * LD_LinkedList_Create_EXT (uintptr_t Object_Size, uintptr_t Object_Count)
{
    struct LD_LinkedList * RETURN =
	(struct LD_LinkedList*)malloc(sizeof(struct LD_LinkedList));
    if(RETURN==NULL){return NULL;}

    __LinkedList_Init(RETURN,Object_Size+sizeof(void*),Object_Count);
    
    RETURN->Object_Size = Object_Size; //<-- KEEP AN EYE

    return RETURN;
}

struct LD_LinkedList * LD_LinkedList_Create (uintptr_t Object_Size)
{
    uintptr_t Object_Count = LD_LINKEDLISTS_ARENA_SIZE/(Object_Size);
    if(Object_Count<=0){Object_Count=1;}
    return LD_LinkedList_Create_EXT(Object_Size,Object_Count);
}

void * LD_LinkedList_append(struct LD_LinkedList * LinkedList,void * Data)
{
    SLL_PRO;

    if(LLL!=NULL && LLF != NULL)
    {__gll_s_ptr(_SLL_Head(LLL))->Next=ptr;}
    if(LLF == NULL)
    {LinkedList->First = ptr;}	
    LinkedList->Last=ptr;

    __gll_s_ptr(_SLL_Head(ptr))->Next=NULL;

    (LinkedList->Length)++;
    return ptr;
}

void * LD_LinkedList_prepend(struct LD_LinkedList * LinkedList,void * Data)
{
    SLL_PRO;

    __gll_s_ptr(_SLL_Head(ptr))->Next=LLL;
    LinkedList->First=ptr;
    (LinkedList->Length)++;
    return ptr;
}

void * LD_LinkedList_insert_Next_to(struct LD_LinkedList * LinkedList, void * Node, void * Data)
{
    LL_PRO_PRO(1);

    void * NEXT = __gll_s_ptr(_SLL_Head(Node))->Next ;
    __gll_s_ptr(_SLL_Head(Node))->Next=ptr;
    __gll_s_ptr(_SLL_Head(ptr))->Next=NEXT;
    
    if(LinkedList != NULL)
    {
	if(Node == LinkedList->Last)
	{
	    LinkedList->Last=ptr;
	}
	(LinkedList->Length)++;
    }

    return ptr;
}

void * LD_LinkedList_detach_First(struct LD_LinkedList * LinkedList)
{
    void * RETURN = LinkedList->First;
    LinkedList->First=__gll_s_ptr(_SLL_Head(RETURN))->Next;
    (LinkedList->Length)--;
    return RETURN;
} 

void LD_LinkedList_attach_prepend(struct LD_LinkedList * LinkedList,void * Node)
{
    __gll_s_ptr(_SLL_Head(Node))->Next=LinkedList->First;
    LinkedList->First=Node;
    (LinkedList->Length)++;
}

void LD_LinkedList_attach_append(struct LD_LinkedList * LinkedList, void * Node)
{
    __gll_s_ptr(_SLL_Head(LinkedList->Last))->Next=Node;
    LinkedList->Last=Node;
    (LinkedList->Length)++;
}

void LD_LinkedList_attach_Next_to(struct LD_LinkedList * LinkedList,void* Node_ref ,void * Node)
{
    __gll_s_ptr(_SLL_Head(Node))->Next=__gll_s_ptr(_SLL_Head(Node_ref))->Next;
    __gll_s_ptr(_SLL_Head(Node_ref))->Next = Node;

    if(LinkedList!=NULL)
    {
	if(LinkedList->Last==Node_ref)
	{
	    LinkedList->Last=Node;
	}
	(LinkedList->Length)++;
    }
}

void LD_LinkedList_remove_First(struct LD_LinkedList * LinkedList)
{
    void * First_H = _SLL_Head(LinkedList->First);
    LinkedList->First=__gll_s_ptr(First_H)->Next;
    __LinkedList_node_deallocate(First_H);
    (LinkedList->Length)--;
}

void LD_LinkedList_remove_Onward_Next_to(struct LD_LinkedList * LinkedList,void * Node)
{
    Node=__gll_s_ptr(_SLL_Head(Node))->Next;
    uintptr_t Count = 0;
    void* _Node = Node;

    while(Node!=NULL)
    {
	void * Head = _SLL_Head(Node);
	void * ptr = __gll_s_ptr(Head)->Next;

	__LinkedList_node_deallocate(Head);
	
	Node = ptr; 
	Count++;
    }

    if(LinkedList!=NULL)
    {
	LinkedList->Last = _Node;
	(LinkedList->Length)-=Count;
    }
}

void LD_LinkedList_remove_detached(void * Node)
{__LinkedList_node_deallocate(_SLL_Head(Node));}

void LD_LinkedList_Clear(struct LD_LinkedList * LinkedList)
{
    LD_Object_Pool_Clear(&LinkedList->Pool);
    __LD_LinkedList_Reset(LinkedList);
}

void LD_LinkedList_Clear_iterate(struct LD_LinkedList * LinkedList)
{
    void * EXT = LinkedList->First;
    void * LLL = LinkedList->Last;

    while(EXT!=LLL || EXT!=NULL)
    {
	void * HEAD = _SLL_Head(EXT);
	void * NEXT = __gll_s_ptr(HEAD)->Next;
	__LinkedList_node_deallocate(HEAD);
	EXT=NEXT;
    }

    __LD_LinkedList_Reset(LinkedList);
}

void LD_LinkedList_Destroy(struct LD_LinkedList * LinkedList)
{
    LD_LinkedList_Clear(LinkedList);
    free(LinkedList);
}

void LD_LinkedList_Make_Circular(struct LD_LinkedList * LinkedList)
{
    __gll_s_ptr(_SLL_Head(LinkedList->Last))->Next = LinkedList->First;
}

#undef _SLL_Head 
#undef SLL_PRO

