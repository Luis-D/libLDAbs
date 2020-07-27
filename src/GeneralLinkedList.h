/**
 *@file
 * @brief (This header should not be directly included)
 * @details Base header for the Linked Lists systems, it contains general utilities that can be user on the elements that inherit this base.
 * @details Tip: To loop over a Linked List, use the pointer of the Head and compare to NULL or to the pointer to a given Node.
 *
 */



#ifndef _LD_GENERALLINKEDLIST_H
#define _LD_GENERALLINKEDLIST_H

#ifdef __cplusplus
extern "C" 
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ObjectPool.h"

struct LD_LinkedList
{
    void * First;
    void * Last;
    size_t Length;
    size_t Object_Size;
    struct LD_ObjectPool Pool;
};

#ifndef LD_LINKEDLISTS_ARENA_SIZE 
    #define LD_LINKEDLISTS_ARENA_SIZE (4096-32-32) //<- 4kb
#endif

#define G_LinkedList_Node_Define(Datatype) void

#define LL_PRO_PRO(x) \
    size_t Size = (LinkedList->Object_Size);\
    void * ptr = __LinkedList_node_allocate(LinkedList);\
    if(ptr==NULL){return NULL;}\
    ptr=((uintptr_t*)ptr)+x;\
    if(Data != NULL){memcpy(ptr,Data,Size);}\
struct LD_LinkedList * __LinkedList_Init(struct LD_LinkedList*List,size_t Object_Size,size_t Object_Count);


struct LD_LinkedList * __LinkedList_Init(struct LD_LinkedList*List,size_t Object_Size,size_t Object_Count);

void * __LinkedList_node_allocate(struct LD_LinkedList * LL);

void __LinkedList_node_deallocate(void * Node);

void __LD_LinkedList_Reset(struct LD_LinkedList * LinkedList);




/**
 * @brief Destroys a Linked List created by LD_LinkedList_Create() or LD_LinkedList_Create_EXT()
 * @param LinkedList Pointer to the Linked List
 */
void LD_LinkedList_Destroy(struct LD_LinkedList * LinkedList);


/**
 * @brief Clear the Linked List without destroying it.
 * @details It clears the Linked List by clearing directly is Arenas
 * @param LinkedList Pointer to the Linked List
 */
void LD_LinkedList_Clear(struct LD_LinkedList * LinkedList);




    //--- General Macros		---//
    
/** 
 *  @brief Get the lenght of the Linked List 
 *  @details It is recommended to use this to check whether the Linked List is empty.
 *  @param List_ptr Pointer of a Linked List.
 *  @return It returns the number of Nodes in the Linked List
 */
#define LD_LinkedList_Length(List_ptr) ((struct LD_LinkedList*)List_ptr)->Length

/** @brief Given a pointer to a Node, it gets the pointer to the next Node
 * @details It is useful to iterate over the List, getting the Next node at some moment of a cicle.
 * @return It returns a pointer to the Node next to the given one.
 * @param Data_ptr Pointer to ne Node
 * */
#define LD_LinkedList_Next_to(Data_ptr)	(void*) ((uintptr_t*) (*(((uintptr_t*) Data_ptr)-1)))

/** @brief Given a pointer to a Node (Data_ptr), it gets the pointer to the previous Node
 * @details It is useful to iterate over the List, getting the Previous node at some moment of a cicle.
 * @return It returns a pointer to the Node previous to the given one.
 * @param Data_ptr Pointer to the Node
 * */
#define LD_LinkedList_Previous_to(Data_ptr)	(void*) ((uintptr_t*) (*(((uintptr_t*) Data_ptr)-2)))

/** @brief Get the pointer to the first node (Head)
 * @details It is useful to iterate over the List, getting the dirst Node where to start the Loop and goind onwards.
 * @return It returns a pointer to the First Node (Head) of the Linked List
 * @retval NULL There is no Nodes in the Linked List
 * @param List_ptr pointer to the Linked List
 * */
#define LD_LinkedList_First(List_ptr) ((struct LD_LinkedList*)List_ptr)->First

/** @brief Get the pointer to the last node if possible (Tail) 
 * @details It is useful to iterate over the List, getting the first Node where to start the Loop and going backwards.
 * @return It returns a pointer to the Last Node (Tail) of the Linked List. Using this function in a Single Linked List results in abnormal behavior.
 * @retval NULL There is no Nodes in the last Position in the Linked List
 * @param List_ptr pointer to the Linked List.
 * */
#define LD_LinkedList_Last(List_ptr) ((struct LD_LinkedList*)List_ptr)->Last


//Arcane and esoteric:
#define LD_LinkedList_foreach(Item,List,__gll,Start_Node,Next_Node,Cond)	    	    \
    for(void * Item##__LD_LL__Foreach__Iterator__ = __gllptr(List)->Start_Node,		    \
      * Item = &(__gll(Item##__LD_LL__Foreach__Iterator__)->Data);			    \
    Item##__LD_LL__Foreach__Iterator__!=Cond;						    \
    Item##__LD_LL__Foreach__Iterator__ = __gll(Item##__LD_LL__Foreach__Iterator__)->Next_Node,	    \
    Item = &__gll(Item##__LD_LL__Foreach__Iterator__)->Data)




#ifdef __cplusplus
}
#endif


#endif
