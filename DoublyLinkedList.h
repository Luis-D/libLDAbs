#ifndef _LD_DOUBLYLINKEDLIST_H
#define _LD_DOUBLYLINKEDLIST_H

#include "SinglyLinkedList.h"
#include "LinkedList/GeneralLinkedList.h"


#define LD_DOUBLYLINKEDLIST struct LD_LinkedList

//--- Doubly Linked List functions---//
#define D_LinkedList_Node_Define(Datatype) \
    struct {struct LD_LinkedList * List;void * Previous; void * Next; Datatype Data;}
#define __gll_d_ptr(x) ((D_LinkedList_Node_Define(void*)*) x)

#define LD_DoublyLinkedList_Next_to	LD_LinkedList_Next_to
#define LD_DoublyLinkedList_Previous_to LD_LinkedList_Previous_to
#define LD_DoublyLinkedList_First	LD_LinkedList_First
#define LD_DoublyLinkedList_Last	LD_LinkedList_Last
#define LD_DoublyLinkedList_Length	LD_LinkedList_Length

#ifdef __cplusplus
extern "C" 
{
#endif

struct LD_LinkedList * LD_DoublyLinkedList_Create_EXT (size_t Object_Size, size_t Object_Count);

void LD_DoublyLinkedList_Init(struct LD_LinkedList * LinkedList,size_t Object_Size);

struct LD_LinkedList * LD_DoublyLinkedList_Create (size_t Object_Size);

void * LD_DoublyLinkedList_append(struct LD_LinkedList * LinkedList, void * Data);

void * LD_DoublyLinkedList_prepend(struct LD_LinkedList* LinkedList,void * Data);


void * LD_DoublyLinkedList_insert_Next_to(void * Node,  void * Data);

void * LD_DoublyLinkedList_insert_Previous_to(void * Node,  void * Data);

void LD_DoublyLinkedList_detach(void * Node);


void LD_DoublyLinkedList_attach_append(struct LD_LinkedList * LinkedList,void * Node);

void LD_DoublyLinkedList_attach_prepend(struct LD_LinkedList * LinkedList,void * Node);


void LD_DoublyLinkedList_attach_Next_to(void * Node_ref,void *Node);

void LD_DoublyLinkedList_attach_Previous_to(void * Node_ref,void *Node);

void LD_DoublyLinkedList_remove(void * Node);

void LD_DoublyLinkedList_remove_Onward(void * Node);

void LD_DoublyLinkedList_remove_Backward(void * Node);

void LD_DoublyLinkedList_remove_detached(void * Node);

#define LD_DoublyLinkedList_deInit LD_LinkedList_deInit
#define LD_DoublyLinkedList_Clear LD_LinkedList_Clear
#define LD_DoublyLinkedList_Destroy LD_LinkedList_Destroy

void LD_DoublyLinkedList_Clear_iterate(struct LD_LinkedList * LinkedList);

void LD_DoublyLinkedList_Make_Circular(struct LD_LinkedList * LinkedList);

#ifdef __cplusplus
}
#endif

#endif
