/**
 * @file 
 * @brief Create and use weak pointer variable-size Queue and Stack.
 * @details Stacks and Queues should be used only through the provided MACROS.
 * @details Because of the pointers being 'weak', pointers are always unreliable, but this scheme may be more compact or more suitable for memory critic situations for held objects can have diferent sizes.
 */

/*  Luis Delgado. 2019
 *  Header for weak pointer variable-size Queue and Stack using a Pool.
 * */

/*
 * Version 1: July 7th, 2019.
 * Version 1.1: July 26th, 2020.
 */


#ifndef LD_WEAK_STACK_QUEUE_H
#define LD_WEAK_STACK_QUEUE_H

#ifdef __cplusplus
extern "C" 
{
#endif

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

/** . */
#define LD_STACK_QUEUE struct LD_STACK_QUEUE_Struct

/**
 * @brief Create a Base for Stack or Queue.
 * @details Internally, Stack and Queues are the same.
 * @details (It should be used through provided MACROS).
 * @return It returns a Pointer to the created Stack or Queue Base.
 * @retval NULL It's returned if it fails.
 */
LD_STACK_QUEUE * LD_Stack_Queue_Create();

/**
 * @brief Destroy a Base for Stack or Queue created by LD_Stack_Queue_Create()
 * @details Using it on Bases created by LD_Stack_Queue_Init may trigger errors.
 * @details (It should be used through provided MACROS).
 * @param Pool Pointer to the Stack or Queue
 */
void LD_Stack_Queue_Destroy(LD_STACK_QUEUE * Pool);

/**
 * @brief Initialized a manually allocated Base for Stack of Queue
 * @return It returns a Pointer to the created Stack or Queue Base.
 * @retval NULL It's returned if it fails.
 * @param Pool Pointer to the Stack or Queue
 */
LD_STACK_QUEUE * LD_Stack_Queue_Init(LD_STACK_QUEUE * POOL);

/**
 * @brief Clears the data of the Stack or Queue without Destroying it
 * @details (It should be used through provided MACROS).
 * @param Pool Pointer to the Stack or Queue
 */
void LD_Stack_Queue_Clear(LD_STACK_QUEUE * Pool);

/**
 * @brief Allocate memory in the Stack or Queue pool.
 * @details (It should be used through provided MACROS).
 * @return It returns the pointer to the allocated space.
 * @param Pool Pointer to the Stack or Queue.
 * @param Size Bytes to be allocated.
 */
void * LD_Stack_Queue_Element_alloc(LD_STACK_QUEUE * Pool, size_t Size);

/** 
 * @brief It returns the pointer to the Top element of the pool in the Stack or Queue.
 * @return It returns the pointer to the Top element.
 * @param Pool Pointer to the Stack or Queue.
 */
void* LD_Stack_Queue_Element_Top(LD_STACK_QUEUE * Pool);

/** 
 * @brief It returns the pointer to the Bottom element of the pool in the Stack or Queue.
 * @return It returns the pointer to the Bottom element.
 * @param Pool Pointer to the Stack or Queue.
 */
void* LD_Stack_Queue_Element_Bottom(LD_STACK_QUEUE * Pool);

/**
 * @brief check if the Stack or Queue is empty
 * @retval 1 The Stack or Queue is Empty
 * @retval 0 The Stack or Queue is not Empty
 * @param Pool Pointer to the Stack or Queue.
 */
char LD_Stack_Queue_IsEmpty(LD_STACK_QUEUE * Pool);

/**
 * @brief Push (copy) data to the top of the Stack or Queue pool.
 * @details (It should be used through provided MACROS).
 * @return It returns the pointer to the pushed element.
 * @param Pool Pointer to the Stack or Queue.
 * @param Source Source of the data to be pushed (copied).
 * @param Size Bytes to be allocated.
 */
void* LD_Stack_Queue_Element_Push(LD_STACK_QUEUE * Pool,void * Source, size_t Size);

/**
 * @brief Pop the Top element of the pool of the Stack or Queue.
 * @details (It should be used through provided MACROS).
 * @details it behaves like LD_Stack_Queue_Element_Top(), but the data becomes unreliable as it isn't in the Stack or Queue anymore.
 * @return It returns the pointer to the popped element. It should be re-casted to the proper type.
 * @param Pool Pointer to the Stack or Queue.
 */
void * LD_Stack_Queue_Element_Pop_Top(LD_STACK_QUEUE  * Pool);

/**
 * @brief Pop the Bottom element of the pool of the Stack or Queue.
 * @details (It should be used through provided MACROS).
 * @details it behaves like LD_Stack_Queue_Element_Bottom(), but the data becomes unreliable as it isn't in the Stack or Queue anymore.
 * @return It returns the pointer to the popped element. It should be re-casted to the proper type.
 * @param Pool Pointer to the Stack or Queue.
 */
void * LD_Stack_Queue_Element_Pop_Bottom(LD_STACK_QUEUE * Pool);

/**
 * @brief Get the size in bytes of an element of a Stack or Queue.
 * @details Given a pointer to an element previously pushed and retrieve with LD_Stack_Queue_Element_Top() or LD_Stack_Queue_Element_Bottom(), this function returns it size in bytes. 
 * @details This functions can also works with popped elements if they haven't been recycled yet.
 * @return It returns the size in bytes of the Element.
 * @param Element Pointer to an Element (a.k.a. item or node in the Stack or Queue).
 */
size_t LD_Stack_Queue_Element_Size(void * Element);


#ifdef __cplusplus
}
#endif

/** @name Weak Queue front-end */
///@{
#define LD_WEAK_QUEUE struct LD_STACK_QUEUE_Struct
#define LD_Weak_Queue_Create	LD_Stack_Queue_Create
#define LD_Weak_Queue_Destroy	LD_Stack_Queue_Destroy
#define LD_Weak_Queue_Init		LD_Stack_Queue_Init
#define LD_Weak_Queue_Clear	LD_Stack_Queue_Clear
#define LD_Weak_Queue_alloc	LD_Stack_Queue_Element_alloc
#define LD_Weak_Queue_Pop		LD_Stack_Queue_Element_Pop_Top
#define LD_Weak_Queue_Push		LD_Stack_Queue_Element_Push
#define LD_Weak_Queue_Enqueue	LD_Stack_Queue_Element_Push
#define LD_Weak_Queue_Dequeue	LD_Stack_Queue_Element_Pop_Top
#define LD_Weak_Queue_First	LD_Stack_Queue_Element_Top
#define LD_Weak_Queue_Last		LD_Stack_Queue_Element_Bottom
#define LD_Weak_Queue_Element_Size LD_Stack_Queue_Element_Size
#define LD_Weak_Queue_IsEmpty	LD_Stack_Queue_IsEmpty
///@}


/** @name Weak Stack front-end */
///@{
#define LD_WEAK_STACK struct LD_STACK_QUEUE_Struct
#define LD_Weak_Stack_Create	LD_Stack_Queue_Create
#define LD_Weak_Stack_Destroy	LD_Stack_Queue_Destroy
#define LD_Weak_Stack_Init		LD_Stack_Queue_Init
#define LD_Weak_Stack_Clear	LD_Stack_Queue_Clear
#define	LD_Weak_Stack_alloc	LD_Stack_Queue_Element_alloc
#define LD_Weak_Stack_Pop		LD_Stack_Queue_Element_Pop_Bottom
#define	LD_Weak_Stack_Push		LD_Stack_Queue_Element_Push
#define LD_Weak_Stack_First	LD_Stack_Queue_Element_Top
#define LD_Weak_Stack_Last		LD_Stack_Queue_Element_Bottom
#define LD_Weak_Stack_Element_Size LD_Stack_Queue_Element_Size
#define LD_Weak_Stack_IsEmpty	LD_Stack_Queue_IsEmpty
///@}

#endif
