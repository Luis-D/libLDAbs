#ifndef _LD_GENERALLINKEDLIST_H
#define _LD_GENERALLINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Object_Pool.h"

struct LD_LinkedList
{
    void * First;
    void * Last;
    uintptr_t Length;
    uintptr_t Object_Size;
    struct LD_Object_Pool Pool;
};

#ifndef LD_LINKEDLISTS_ARENA_SIZE 
    #define LD_LINKEDLISTS_ARENA_SIZE 4096 //<- 4kb
#endif

#define G_LinkedList_Node_Define(Datatype) void

#define LL_PRO_PRO(x) \
    uintptr_t Size = (LinkedList->Object_Size);\
    void * ptr = __LinkedList_node_allocate(LinkedList);\
    if(ptr==NULL){return NULL;}\
    ptr=((uintptr_t*)ptr)+x;\
    if(Data != NULL){memcpy(ptr,Data,Size);}\
struct LD_LinkedList * __LinkedList_Init(struct LD_LinkedList*List,uintptr_t Object_Size,uintptr_t Object_Count);

struct LD_LinkedList * __LinkedList_Init(struct LD_LinkedList*List,uintptr_t Object_Size,uintptr_t Object_Count);

void * __LinkedList_node_allocate(struct LD_LinkedList * LL);

void __LinkedList_node_deallocate(void * Node);

void __LD_LinkedList_Reset(struct LD_LinkedList * LinkedList);

    //--- General Macros		---//
#define LD_LinkedList_Next_to(Data_ptr)	(void*) ((uintptr_t*) (*(((uintptr_t*) Data_ptr)-1)))
#define LD_LinkedList_Previous_to(Data_ptr)	(void*) ((uintptr_t*) (*(((uintptr_t*) Data_ptr)-2)))
#define LD_LinkedList_First(List_ptr) ((struct LD_LinkedList*)List_ptr)->First
#define LD_LinkedList_Last(List_ptr) ((struct LD_LinkedList*)List_ptr)->Last
#define LD_LinkedList_Length(List_ptr) ((struct LD_LinkedList*)List_ptr)->Length

//This macro iterates over the list
#define LD_LinkedList_foreach(Item,List,__gll,Start_Node,Next_Node,Cond)	    	    \
    for(void * Item##__LD_LL__Foreach__Iterator__ = __gllptr(List)->Start_Node,		    \
      * Item = &(__gll(Item##__LD_LL__Foreach__Iterator__)->Data);			    \
    Item##__LD_LL__Foreach__Iterator__!=Cond;						    \
    Item##__LD_LL__Foreach__Iterator__ = __gll(Item##__LD_LL__Foreach__Iterator__)->Next_Node,	    \
    Item = &__gll(Item##__LD_LL__Foreach__Iterator__)->Data)

#endif
