/*  Luis Delgado. 2019
 *  Header for weak pointer variable-size Queue and Stack using a Pool.
 * */

/*
 * Version 1: July 7th, 2019.
 *
 */


#ifndef LD_WEAK_STACK_QUEUE_H
#define LD_WEAK_STACK_QUEUE_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct LD_STACK_QUEUE_Struct
{
    uintptr_t Size;
    uintptr_t Used;
    uintptr_t Top;
    uintptr_t Bottom;
    void * Data;
};

struct LD_STACK_QUEUE_Element_Struct
{
    uintptr_t Last_Real_Used;
    uintptr_t Size;
    void * Data;
};



void __Stack_Queue_Clear(struct LD_STACK_QUEUE_Struct * Pool);

struct LD_STACK_QUEUE_Struct * __Stack_Queue_Init(struct LD_STACK_QUEUE_Struct * POOL);

struct LD_STACK_QUEUE_Struct * __Stack_Queue_Create();

void __Stack_Queue_Destroy(struct LD_STACK_QUEUE_Struct * Pool);
   
void * __Stack_Queue_Element_alloc(struct LD_STACK_QUEUE_Struct * Pool, uintptr_t Size);

void* __Stack_Queue_Element_Push(struct LD_STACK_QUEUE_Struct * Pool,void * Source, uintptr_t Size);

void* __Stack_Queue_Element_Top(struct LD_STACK_QUEUE_Struct * Pool);

void* __Stack_Queue_Element_Bottom(struct LD_STACK_QUEUE_Struct * Pool);

char __Stack_Queue_Is_Empty(struct LD_STACK_QUEUE_Struct * Pool);

void * __Stack_Queue_Element_Pop_Top(struct LD_STACK_QUEUE_Struct  * Pool);

void * __Stack_Queue_Element_Pop_Bottom(struct LD_STACK_QUEUE_Struct * Pool);




#define LD_WEAK_QUEUE struct LD_STACK_QUEUE_Struct
#define LD_WEAK_STACK struct LD_STACK_QUEUE_Struct


#define LD_Weak_Queue_Create	__Stack_Queue_Create
#define LD_Weak_Queue_Destroy	__Stack_Queue_Destroy
#define LD_Weak_Queue_Init		__Stack_Queue_Init
#define LD_Weak_Queue_Clear	__Stack_Queue_Clear
#define LD_Weak_Queue_alloc	__Stack_Queue_Element_alloc
#define LD_Weak_Queue_Pop		__Stack_Queue_Element_Pop_Top
#define LD_Weak_Queue_Push		__Stack_Queue_Element_Push
#define LD_Weak_Queue_Enqueue	__Stack_Queue_Element_Push
#define LD_Weak_Queue_Dequeue	__Stack_Queue_Element_Pop_Top
#define LD_Weak_Queue_First	__Stack_Queue_Element_Top
#define LD_Weak_Queue_Last		__Stack_Queue_Element_Bottom
#define LD_Weak_Queue_Is_Empty	__Stack_Queue_Is_Empty

#define LD_Weak_Stack_Create	__Stack_Queue_Create
#define LD_Weak_Stack_Destroy	__Stack_Queue_Destroy
#define LD_Weak_Stack_Init		__Stack_Queue_Init
#define LD_Weak_Stack_Clear	__Stack_Queue_Clear
#define	LD_Weak_Stack_alloc	__Stack_Queue_Element_alloc
#define LD_Weak_Stack_Pop		__Stack_Queue_Element_Pop_Bottom
#define	LD_Weak_Stack_Push		__Stack_Queue_Element_Push
#define LD_Weak_Stack_First	__Stack_Queue_Element_Top
#define LD_Weak_Stack_Last		__Stack_Queue_Element_Bottom
#define LD_Weak_Stack_Is_Empty	__Stack_Queue_Is_Empty

#endif
