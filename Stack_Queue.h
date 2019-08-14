#ifndef LD_STACK_QUEUE_H
#define LD_STACK_QUEUE_H

#include "SinglyLinkedList.h"



//--- Queue Macros	---//
#define LD_QUEUE LD_LINKEDLIST

#define LD_Queue_Create		LD_LinkedList_Create
#define LD_Queue_Clear		LD_LinkedList_Clear
#define LD_Queue_Clear_iterate	LD_LinkedList_Clear_iterate
#define LD_Queue_Destroy	LD_LinkedList_Destroy

#define LD_Queue_Front		LD_LinkedList_First
#define LD_Queue_Top		LD_LinkedList_First
#define LD_Queue_Back		LD_LinkedList_Last
#define LD_Queue_Bottom		LD_LinkedList_Last
#define LD_Queue_Length		LD_LinkedList_Length

#define LD_Queue_enqueue	LD_LinkedList_append
#define LD_Queue_dequeue	LD_LinkedList_remove_First



//--- Queue Macros	---//
#define LD_STACK LD_LINKEDLIST

#define LD_Stack_Create		LD_LinkedList_Create
#define LD_Stack_Clear		LD_LinkedList_Clear
#define LD_Stack_Clear_iterate	LD_LinkedList_Clear_iterate
#define LD_Stack_Destroy	LD_LinkedList_Destroy

#define LD_Stack_Front		LD_LinkedList_First
#define LD_Stack_Top		LD_LinkedList_First
#define LD_Stack_Bottom		LD_LinkedList_Last
#define LD_Stack_Back		LD_LinkedList_Last
#define LD_Stack_Length		LD_LinkedList_Length

#define LD_Stack_push		LD_LinkedList_preppend
#define LD_Stack_pop		LD_LinkedList_remove_First

#endif
