#include "DoublyLinkedList.h"
    
//--- Doubly Linked List functions---//

#define _DLL_Head(Node) (((uintptr_t*)Node)-3)

#define LD_DoublyLinkedList_foreach LD_LinkedList_foreach

#define DLL_PRO \
    LL_PRO_PRO(3)\
    void * LLL = LinkedList->Last;\
    void * LLF = LinkedList->First

#define D_LinkedList_Node_Define(Datatype) \
    struct {struct LD_LinkedList * List;void * Previous; void * Next; Datatype Data;}
#define __gll_d_ptr(x) ((D_LinkedList_Node_Define(void*)*) x)


struct LD_LinkedList * LD_DoublyLinkedList_Create_EXT(size_t Object_Size, size_t Object_Count)
{
    struct LD_LinkedList * RETURN =
	(struct LD_LinkedList*)malloc(sizeof(struct LD_LinkedList));
    if(RETURN==NULL){return NULL;}

    size_t Real_Size = Object_Size+(3*sizeof(void*));
    __LinkedList_Init(RETURN,Real_Size,Object_Count);
    RETURN->Object_Size = Object_Size;
    return RETURN;
}

void LD_DoublyLinkedList_Init(struct LD_LinkedList * LinkedList,size_t Object_Size)
{
    size_t Object_Count = LD_LINKEDLISTS_ARENA_SIZE/(Object_Size);
    size_t Real_Size = Object_Size+(3*sizeof(void*));
    __LinkedList_Init(LinkedList,Real_Size,Object_Count);
    LinkedList->Object_Size = Object_Size;
}

struct LD_LinkedList * LD_DoublyLinkedList_Create (size_t Object_Size)
{
    size_t Object_Count = LD_LINKEDLISTS_ARENA_SIZE/(Object_Size);
    if(Object_Count<=0){Object_Count=1;}
    return LD_DoublyLinkedList_Create_EXT(Object_Size,Object_Count);
}

void * LD_DoublyLinkedList_append(struct LD_LinkedList * LinkedList, void * Data)
{
    DLL_PRO;

    if(LLL!=NULL && LLF != NULL)
    {
	__gll_d_ptr(_DLL_Head(LLL))->Next=ptr;
    }
    if(LLF == NULL)
    {
	LinkedList->First = ptr;
	__gll_d_ptr(_DLL_Head(ptr))->Previous=NULL;
    }   

    void * ptr_HEAD = _DLL_Head(ptr);

    __gll_d_ptr(ptr_HEAD)->Previous=LinkedList->Last;
    __gll_d_ptr(ptr_HEAD)->Next=NULL;
    __gll_d_ptr(ptr_HEAD)->List=LinkedList;

    LinkedList->Last=ptr;
    (LinkedList->Length)++;
    return ptr;
}

void * LD_DoublyLinkedList_prepend(struct LD_LinkedList* LinkedList,void * Data)
{
    DLL_PRO;

    if(LLL!=NULL && LLF != NULL)
    {
	__gll_d_ptr(_DLL_Head(LLF))->Previous=ptr;
    }
   
    void * ptr_HEAD = _DLL_Head(ptr);

    __gll_d_ptr(ptr_HEAD)->Next=LinkedList->First;
    __gll_d_ptr(ptr_HEAD)->Previous=NULL;
    __gll_d_ptr(ptr_HEAD)->List=LinkedList;
    

    LinkedList->First=ptr;
    (LinkedList->Length)++;
    return ptr;
} 


void * LD_DoublyLinkedList_insert_Next_to(void * Node,void * Data)
{
    void * Node_Head = _DLL_Head(Node);
    
    struct LD_LinkedList * LinkedList = 
	__gll_d_ptr(Node_Head)->List;
    if(LinkedList==NULL){return NULL;}

    LL_PRO_PRO(3);
    void * NEXT = __gll_d_ptr(Node_Head)->Next; 

    if(NEXT!=NULL)
    {__gll_d_ptr(_DLL_Head(NEXT))->Previous = ptr;}

    void * ptr_Head = _DLL_Head(ptr);

    __gll_d_ptr(ptr_Head)->Next = NEXT;

    __gll_d_ptr(ptr_Head)->Previous = Node;
    __gll_d_ptr(Node_Head)->Next=ptr;

    if(Node == LinkedList->Last)
    {
	LinkedList->Last=ptr;
    }

    (LinkedList->Length)++;
    return ptr;  
}


void * LD_DoublyLinkedList_insert_Previous_to(void * Node,  void * Data)
{
    void * Node_Head = _DLL_Head(Node);
    
    struct LD_LinkedList * LinkedList = 
	__gll_d_ptr(Node_Head)->List;
    if(LinkedList==NULL){return NULL;}

    LL_PRO_PRO(3);


    void * PREVIOUS = __gll_d_ptr(Node_Head)->Previous; 



    void * ptr_Head = _DLL_Head(ptr);

    if(PREVIOUS!=NULL)
    {__gll_d_ptr(_DLL_Head(PREVIOUS))->Next = ptr;}

    __gll_d_ptr(ptr_Head)->Previous = PREVIOUS;

    __gll_d_ptr(ptr_Head)->Next = Node;
    __gll_d_ptr(Node_Head)->Previous=ptr;

    if(Node == LinkedList->First)
    {
	LinkedList->First=ptr;
    }

    (LinkedList->Length)++;

    return ptr;  
}

void LD_DoublyLinkedList_detach(void * Node)
{
    void * Node_Head = _DLL_Head(Node);

    struct LD_LinkedList * LinkedList = 
	__gll_d_ptr(Node_Head)->List;

    if(LinkedList==NULL){return;}

    void * PREVIOUS = __gll_d_ptr(Node_Head)->Previous; 
    void * NEXT = __gll_d_ptr(Node_Head)->Next;     


    if(PREVIOUS!=NULL)
    {__gll_d_ptr(_DLL_Head(PREVIOUS))->Next = NEXT;}

    if(NEXT!=NULL)
    {__gll_d_ptr(_DLL_Head(NEXT))->Previous = PREVIOUS;}

    if(Node == LinkedList->First)
    {
	LinkedList->First=NEXT;
    }

    if(Node == LinkedList->Last)
    {
	LinkedList->Last=PREVIOUS;
    }

    (LinkedList->Length)--;
}


void LD_DoublyLinkedList_attach_append(struct LD_LinkedList * LinkedList,void * Node)
{
    void * LAST = LinkedList->Last;
    void * Node_Head = _DLL_Head(Node);
    __gll_d_ptr(Node_Head)->Previous = LAST;
    __gll_d_ptr(Node_Head)->Next=NULL;
    __gll_d_ptr(Node_Head)->List=LinkedList;

    if(LAST!=NULL)
    {
	__gll_d_ptr(_DLL_Head(LAST))->Next = Node;
    }
    
    if(LinkedList->First==NULL){LinkedList->First==Node;}

    LinkedList->Last = Node;
    (LinkedList->Length)++;
}

void LD_DoublyLinkedList_attach_prepend(struct LD_LinkedList * LinkedList,void * Node)
{
    void * FIRST = LinkedList->First;
    void * Node_Head = _DLL_Head(Node);
    __gll_d_ptr(Node_Head)->Next = FIRST;
    __gll_d_ptr(Node_Head)->Previous=NULL;
    __gll_d_ptr(Node_Head)->List=LinkedList;
    
    if(FIRST!=NULL)
    {
	__gll_d_ptr(_DLL_Head(FIRST))->Previous = Node;
    }

    if(LinkedList->Last==NULL){LinkedList->Last==Node;}

    LinkedList->First=Node;
    (LinkedList->Length)++;
}


void LD_DoublyLinkedList_attach_Next_to(void * Node_ref,void *Node)
{
    void * Node_ref_Head = _DLL_Head(Node_ref);
    struct LD_LinkedList * LinkedList = 
	__gll_d_ptr(Node_ref_Head)->List;

    if(LinkedList == NULL){return;}

    void * Node_Head = _DLL_Head(Node);

    void * NEXT = __gll_d_ptr(Node_ref_Head)->Next;
    
    __gll_d_ptr(Node_Head)->Next = NEXT;
    __gll_d_ptr(Node_Head)->Previous = Node_ref;
   
    __gll_d_ptr(Node_ref_Head)->Next = Node;

    if(NEXT != NULL)
    {__gll_d_ptr(_DLL_Head(NEXT))->Previous = Node;}

    if(LinkedList->Last==Node_ref)
    {
	LinkedList->Last=Node;
    }
    (LinkedList->Length)++;
}

void LD_DoublyLinkedList_attach_Previous_to(void * Node_ref,void *Node)
{
    void * Node_ref_Head = _DLL_Head(Node_ref);
    struct LD_LinkedList * LinkedList = 
	__gll_d_ptr(Node_ref_Head)->List;

    if(LinkedList == NULL){return;}

    void * Node_Head = _DLL_Head(Node);
    void * PREVIOUS = __gll_d_ptr(Node_ref_Head)->Previous;
    
    __gll_d_ptr(Node_Head)->Previous = PREVIOUS;
    __gll_d_ptr(Node_Head)->Next = Node_ref;
   
    __gll_d_ptr(Node_ref_Head)->Previous = Node;

    if(PREVIOUS != NULL)
    {__gll_d_ptr(_DLL_Head(PREVIOUS))->Next = Node;}

    if(LinkedList->First=Node_ref)
    {
	LinkedList->First=Node;
    }
    (LinkedList->Length)++;
}

void LD_DoublyLinkedList_remove(void * Node)
{
    void * Node_Head = _DLL_Head(Node);

    struct LD_LinkedList * LinkedList = 
	__gll_d_ptr(Node_Head)->List;

    if(LinkedList == NULL){return;}

    void * PREVIOUS = __gll_d_ptr(Node_Head)->Previous;
    void * NEXT = __gll_d_ptr(Node_Head)->Next;

    if(PREVIOUS!=NULL)
    {__gll_d_ptr(_DLL_Head(PREVIOUS))->Next=NEXT;}

    if(NEXT!=NULL)
    {__gll_d_ptr(_DLL_Head(NEXT))->Previous=PREVIOUS;}

    __LinkedList_node_deallocate(Node_Head);

    if(LinkedList->First==Node)
    {LinkedList->First=NEXT;}

    if(LinkedList->Last==Node)
    {LinkedList->Last=PREVIOUS;}

    (LinkedList->Length)--;
}

void LD_DoublyLinkedList_remove_Onward(void * Node)
{
    void * Head = _DLL_Head(Node);
    struct LD_LinkedList * LinkedList =     
        __gll_d_ptr(Head)->List;

    if(LinkedList == NULL){return;}

    size_t Count = 0;
    void* _Node = Node;

    while(Node!=NULL)
    {
	Head = _DLL_Head(Node);
	void * ptr = __gll_d_ptr(Head)->Next;
	__LinkedList_node_deallocate(Head);
	Node = ptr;
	Count++;
    }

    if(LinkedList != NULL)
    {
	LinkedList->Last=__gll_d_ptr(_DLL_Head(_Node))->Previous;
	if(LinkedList->First == _Node)
	{LinkedList->First == NULL;}
	(LinkedList->Length)-=Count;
    }
}

void LD_DoublyLinkedList_remove_Backward(void * Node)
{
    void * Head = _DLL_Head(Node);
    struct LD_LinkedList * LinkedList =     
        __gll_d_ptr(Head)->List;

    if(LinkedList == NULL){return;}

    size_t Count = 0;
    void* _Node = Node;

    while(Node!=NULL)
    {
	Head = _DLL_Head(Node);
	void * ptr = __gll_d_ptr(Head)->Previous;
	__LinkedList_node_deallocate(Head);
	Node = ptr;
	Count++;
    }

    if(LinkedList != NULL)
    {
	LinkedList->First = __gll_d_ptr(_DLL_Head(_Node))->Next;
	if(LinkedList->Last == _Node)
	{LinkedList->Last == NULL;}
	(LinkedList->Length)-=Count;
    }
}

void LD_DoublyLinkedList_remove_detached(void * Node)
{__LinkedList_node_deallocate(_DLL_Head(Node));}

void LD_DoublyLinkedList_Clear_iterate(struct LD_LinkedList * LinkedList)
{
    void * EXT = LinkedList->First;
    void * LLL = LinkedList->Last;

    while(EXT!=NULL)
    {
		void * HEAD = _DLL_Head(EXT);
		void * NEXT = __gll_d_ptr(HEAD)->Next;
		__LinkedList_node_deallocate(HEAD);
		if(EXT==LLL){break;}
		EXT=NEXT;
    }

    __LD_LinkedList_Reset(LinkedList);
}

void LD_DoublyLinkedList_Make_Circular(struct LD_LinkedList * LinkedList)
{
    __gll_d_ptr(_DLL_Head(LinkedList->Last))->Next = LinkedList->First;
    __gll_d_ptr(_DLL_Head(LinkedList->First))->Previous = LinkedList->Last;
}

