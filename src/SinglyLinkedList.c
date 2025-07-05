#include "SinglyLinkedList.h"

#ifndef LD_LINKEDLISTS_ARENA_SIZE 
    #define LD_LINKEDLISTS_ARENA_SIZE 4096 //<- 4kb
#endif


#define S_LinkedList_Node_Define(Datatype) \
    struct {struct LD_LinkedList * List; \
	void * Next; \
       	Datatype Data;}
#define __gll_s_ptr(x) ((S_LinkedList_Node_Define(void*)*) x)


    //--- Single Linked List functions---//
//Structure of a node:
//  -Linked List
//  -Next Node
//  -Actual Data

#define _SLL_Head(Node) (((uintptr_t*)Node)-2)

#define SLL_PRO \
    LL_PRO_PRO(2)\
    void * LLL = LinkedList->Last;\
    void * LLF = LinkedList->First



void LD_LinkedList_Init_EXT(LD_LINKEDLIST * LinkedList,size_t Object_Size, size_t Object_Count){

    size_t Real_Size = Object_Size+(2*sizeof(void*));

    __LinkedList_Init(LinkedList,Real_Size,Object_Count);
    
    LinkedList->Object_Size = Object_Size; //<-- KEEP AN EYE
}

void LD_LinkedList_Init(LD_LINKEDLIST * LinkedList, size_t Object_Size){
    size_t Object_Count = LD_LINKEDLISTS_ARENA_SIZE/(Object_Size);
    LD_LinkedList_Init_EXT(LinkedList,Object_Size,Object_Count);
}

struct LD_LinkedList * LD_LinkedList_Create_EXT (size_t Object_Size, size_t Object_Count)
{
    if(Object_Count==0){return NULL;}

    struct LD_LinkedList * RETURN =
	(struct LD_LinkedList*)malloc(sizeof(struct LD_LinkedList));
    if(RETURN==NULL){return NULL;}

    LD_LinkedList_Init_EXT(RETURN,Object_Size,Object_Count);

    return RETURN;
}

struct LD_LinkedList * LD_LinkedList_Create (size_t Object_Size)
{
    size_t Object_Count = LD_LINKEDLISTS_ARENA_SIZE/(Object_Size);
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
    __gll_s_ptr(_SLL_Head(ptr))->List=LinkedList;

    (LinkedList->Length)++;
    return ptr;
}

void * LD_LinkedList_prepend(struct LD_LinkedList * LinkedList,void * Data)
{
    SLL_PRO;

    __gll_s_ptr(_SLL_Head(ptr))->Next=LLL;
    __gll_s_ptr(_SLL_Head(ptr))->List=LinkedList;
    LinkedList->First=ptr;
    (LinkedList->Length)++;
    return ptr;
}

void * LD_LinkedList_insert_Next_to(void * Node, void * Data)
{
    struct LD_LinkedList * LinkedList = __gll_s_ptr(_SLL_Head(Node))->List;

    if(LinkedList == NULL){return NULL;};


    LL_PRO_PRO(2);

    void * NEXT = __gll_s_ptr(_SLL_Head(Node))->Next ;
    __gll_s_ptr(_SLL_Head(Node))->Next=ptr;
    __gll_s_ptr(_SLL_Head(ptr))->Next=NEXT;
    __gll_s_ptr(_SLL_Head(ptr))->List=LinkedList;
    
    if(Node == LinkedList->Last)
    {
	LinkedList->Last=ptr;
    }
    (LinkedList->Length)++;

    return ptr;
}

void * LD_LinkedList_detach_First(struct LD_LinkedList * LinkedList)
{
    void * RETURN = LinkedList->First;
    __gll_s_ptr(_SLL_Head(RETURN))->List=NULL;
    LinkedList->First=__gll_s_ptr(_SLL_Head(RETURN))->Next;
    (LinkedList->Length)--;
    return RETURN;
} 

void LD_LinkedList_attach_prepend(struct LD_LinkedList * LinkedList,void * Node)
{
    __gll_s_ptr(_SLL_Head(Node))->Next=LinkedList->First;
    __gll_s_ptr(_SLL_Head(Node))->List=LinkedList;
    LinkedList->First=Node;
    (LinkedList->Length)++;
}

void LD_LinkedList_attach_append(struct LD_LinkedList * LinkedList, void * Node)
{
    if(LinkedList->Last != NULL){
        __gll_s_ptr(_SLL_Head(LinkedList->Last))->Next=Node;
    }
    __gll_s_ptr(_SLL_Head(Node))->List=LinkedList;
    LinkedList->Last=Node;
    if(LinkedList->First == NULL){
	LinkedList->First = Node;
    }
    (LinkedList->Length)++;
}

void LD_LinkedList_attach_Next_to(void* Node_ref ,void * Node)
{
    struct LD_LinkedList * LinkedList = __gll_s_ptr(_SLL_Head(Node))->List;
    if(LinkedList == NULL){return;}

    __gll_s_ptr(_SLL_Head(Node))->Next=__gll_s_ptr(_SLL_Head(Node_ref))->Next;
    __gll_s_ptr(_SLL_Head(Node_ref))->Next = Node;
    __gll_s_ptr(_SLL_Head(Node_ref))->List= LinkedList;

    if(LinkedList->Last==Node_ref)
    {
	LinkedList->Last=Node;
    }
    (LinkedList->Length)++;
}

void LD_LinkedList_remove_First(struct LD_LinkedList * LinkedList)
{
    void * First_H = _SLL_Head(LinkedList->First);
    LinkedList->First=__gll_s_ptr(First_H)->Next;
    __LinkedList_node_deallocate(First_H);
    (LinkedList->Length)--;
}

void LD_LinkedList_remove_Onward_Next_to(void * Node)
{
    struct LD_LinkedList * LinkedList = 
	Node=__gll_s_ptr(_SLL_Head(Node))->List;

    if(LinkedList == NULL){return;}

    Node=__gll_s_ptr(_SLL_Head(Node))->Next;
    size_t Count = 0;
    void* _Node = Node;

    while(Node!=NULL)
    {
	void * Head = _SLL_Head(Node);
	void * ptr = __gll_s_ptr(Head)->Next;

	__LinkedList_node_deallocate(Head);
	
	Node = ptr; 
	Count++;
    }

    LinkedList->Last = _Node;
    (LinkedList->Length)-=Count;
}

void LD_LinkedList_remove_detached(void * Node)
{__LinkedList_node_deallocate(_SLL_Head(Node));}


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

void LD_LinkedList_deInit(struct LD_LinkedList * LinkedList)
{LD_ObjectPool_deInit(&LinkedList->Pool);}




void LD_LinkedList_Make_Circular(struct LD_LinkedList * LinkedList)
{
    __gll_s_ptr(_SLL_Head(LinkedList->Last))->Next = LinkedList->First;
}

#undef _SLL_Head 
#undef SLL_PRO

