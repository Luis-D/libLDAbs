/** @file 
 * @brief Memory management using Dynamic allocation and binary trees
 * */

// Header for Memory management using binary trees for Dynamic size allocation
// Luis Delgado. 2024
// Version 1: 11th June, 2024

#ifndef LD_OBJECT_DYN_H
    #define LD_OBJECT_DYN_H
#endif

#include <stddef.h>

struct LD_DynamicMem_Tree_Node_Header{
	struct LD_DynamicMem_Tree_Node_Header * Parent;
	struct LD_DynamicMem_Tree_Node_Header * Left;
	struct LD_DynamicMem_Tree_Node_Header * Right;
	char Free; //<- If Free == 1, this node is blank
	size_t Net_Size;
};

struct LD_DynamicMem_Arena_Header{
	void * Parent;
	struct LD_DynamicMem_Arena_Header * Next;
	struct LD_DynamicMem_Tree_Node_Header Root;
};


struct LD_DynamicMemory {

	struct LD_DynamicMem_Arena_Header * Data;
	struct LD_DynamicMem_Arena_Header * Cur_Data;
	struct LD_DynamicMem_Arena_Header * Filled_Arenas;
};


#define LD_DYNAMICMEM struct LD_DynamicMemory
#define LD_DYNAMICMEMORY LD_DYNAMICMEM

#ifdef __cplusplus
extern "C" 
{
#endif

/**
 * @brief Creates a Dynamic Memory Pool.
 * @details The Pool itself is a collection of pre-allocated Arenas, when one runs out of space another is created to hold more objects.
 * @return  Returns a pointer to the created Dynamic Memory System
 * @retval  NULL is returned if fails.
 */
LD_DYNAMICMEM * LD_DynamicMemory_Create();

/**
 *  @brief Destroys a Dynamic Memory System previously created by LD_DynamicMem_Create()
 *  @details It dealocates the pointer
 * @param Pool Pointer to the Object Pool
 */
void LD_DynamicMemory_Destroy( LD_DYNAMICMEM * DynMem);

/**
 *  @brief Initializes a manually allocated Dynamic Memory;
 *  @details It can be used in a nested Dynamic Memory System;
 *  @return It returns the pointer to the Initialized Object Pool.
 *  @retval NULL is returned if fails.
 *  @param DynMem The pointer to the Not-Initialized object pool, this same pointer is returned if succeed.
 */
LD_DYNAMICMEM * LD_DynamicMemory_Init( LD_DYNAMICMEM * DynMem);

/**
 * @brief This funcion clears a Dynamic Memory System of it's data without destroying the pool itself
 * @param DynMem Pointer to the System
 */
void LD_DynamicMemory_Clear( LD_DYNAMICMEM * DynMem);

/**
 * @brief Retrieve an usable pointer to a newly allocated space.
 * @details It behaves much like Malloc.
 * @details The just returned memory space already have the trashed memory.
 * @return It returns a pointer to a new object, this pointer can be re-casted into aproppiate type.
 * @param DynMem Pointer to the base Dynamic memory System.
 * @param Size The Size in Bytes of the required memory space..
 */
void * LD_DynamicMem_Alloc(LD_DYNAMICMEM * DynMem,size_t Size);

/**
 *  @brief Frees a pointer allocated via LD_DynamMem_Alloc() as Free.
 *  @details It behaves much like Free().
 *  @details It does not clears the Object data, but its pointer becomes unreliable.
 *  @param Object Pointer to the instance to "free".
 */
void LD_DynamicMem_Free(void * Object);


/**
 * @brief Retrieve an usable pointer to a newly reallocated space.
 * @details It behaves much like Realloc.
 * @details The just returned memory space will have the same data.
 * @return It returns a pointer to the object, this pointer can be re-casted into aproppiate type.
 * @param DynMem Pointer to the base Dynamic memory System.
 * @param Size The Size in Bytes of the required memory space..
 */
void * LD_DynamicMem_Realloc(void * Object,size_t Size);

#ifdef __cplusplus
}
#endif
#undef LD_OBJECT_DYN_H
