#ifndef _LD_SINGLYLINKEDLIST_H_
#define _LD_SINGLYLINKEDLIST_H_

#include "LinkedList/GeneralLinkedList.h"

#define LD_LINKEDLIST struct LD_LinkedList
#define LD_SINGLYLINKEDLIST struct LD_LinkedList

#ifndef LD_LINKEDLISTS_ARENA_SIZE 
    #define LD_LINKEDLISTS_ARENA_SIZE 4096 //<- 4kb
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct LD_LinkedList * LD_LinkedList_Create_EXT (size_t Object_Size, size_t Object_Count);
struct LD_LinkedList * LD_LinkedList_Create (size_t Object_Size);


    //--- Singly Linked List functions---//

#define LD_SinglyLinkedList LD_LinkedList

void * LD_LinkedList_append(struct LD_LinkedList * LinkedList,void * Data);

void * LD_LinkedList_prepend(struct LD_LinkedList * LinkedList,void * Data);

void * LD_LinkedList_insert_Next_to(void * Node, void * Data);

void * LD_LinkedList_detach_First(struct LD_LinkedList * LinkedList);

void LD_LinkedList_attach_prepend(struct LD_LinkedList * LinkedList,void * Node);

void LD_LinkedList_attach_append(struct LD_LinkedList * LinkedList, void * Node);

void LD_LinkedList_attach_Next_to(void* Node_ref ,void * Node);

void LD_LinkedList_remove_First(struct LD_LinkedList * LinkedList);

void LD_LinkedList_remove_Onward_Next_to(void * Node);

void LD_LinkedList_remove_detached(void * Node);

void LD_LinkedList_Clear(struct LD_LinkedList * LinkedList);

void LD_LinkedList_Clear_iterate(struct LD_LinkedList * LinkedList);

void LD_LinkedList_deInit(struct LD_LinkedList * LinkedList);

void LD_LinkedList_Destroy(struct LD_LinkedList * LinkedList);

void LD_LinkedList_Make_Circular(struct LD_LinkedList * LinkedList);

#ifdef __cplusplus
}
#endif

#endif
