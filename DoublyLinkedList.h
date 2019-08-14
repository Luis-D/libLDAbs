#ifndef _LD_DOUBLYLINKEDLIST_H
#define _LD_DOUBLYLINKEDLIST_H

#include "LinkedList/GeneralLinkedList.h"
    

#define LD_DOUBLYLINKEDLIST struct LD_LinkedList

//--- Doubly Linked List functions---//
#define D_LinkedList_Node_Define(Datatype) \
    struct {void * Previous; void * Next; Datatype Data;}
#define __gll_d_ptr(x) ((D_LinkedList_Node_Define(void*)*) x)

#define DoublyLinkedList_Next_to LinkedList_Next_to
#define DoublyLinkedList_Previous_to LinkedList_Previous_to
#define DoublyLinkedList_First	LinkedList_First
#define DoublyLinkedList_Last	LinkedList_Last
#define DoublyLinkedList_Length	LinkedList_Length

struct LD_LinkedList * LD_DoublyLinkedList_Create_EXT (uintptr_t Object_Size, uintptr_t Object_Count);
struct LD_LinkedList * LD_DoublyLinkedList_Create (uintptr_t Object_Size);

void * LD_DoublyLinkedList_append(struct LD_LinkedList * LinkedList, void * Data);

void * LD_DoublyLinkedList_prepend(struct LD_LinkedList* LinkedList,void * Data);


void * LD_DoublyLinkedList_insert_Next_to(struct LD_LinkedList * LinkedList, void * Node,  void * Data);

void * LD_DoublyLinkedList_insert_Previous_to(struct LD_LinkedList * LinkedList, void * Node,  void * Data);

void LD_DoublyLinkedList_detach(struct LD_LinkedList * LinkedList,void * Node);


void LD_DoublyLinkedList_attach_append(struct LD_LinkedList * LinkedList,void * Node);

void LD_DoublyLinkedList_attach_prepend(struct LD_LinkedList * LinkedList,void * Node);


void LD_DoublyLinkedList_attach_Next_to(struct LD_LinkedList * LinkedList,void * Node_ref,void *Node);

void LD_DoublyLinkedList_attach_Previous_to(struct LD_LinkedList * LinkedList,void * Node_ref,void *Node);

void LD_DoublyLinkedList_remove(struct LD_LinkedList * LinkedLst, void * Node);

void LD_DoublyLinkedList_remove_Onward(struct LD_LinkedList * LinkedList, void * Node);

void LD_DoublyLinkedList_remove_Backward(struct LD_LinkedList * LinkedList, void * Node);

void LD_DoublyLinkedList_remove_detached(void * Node);

#define LD_DoublyLinkedList_Clear LD_LinkedList_Clear
#define LD_DoublyLinkedList_Destroy LD_LinkedList_Destroy

void LD_DoublyLinkedList_Clear_iterate(struct LD_LinkedList * LinkedList);

void LD_DoublyLinkedList_Make_Circular(struct LD_LinkedList * LinkedList);

#endif
