/** @file 
 * @brief Memory management using Object Pools
 * */

// Header for Memory management using Object Pool
// Luis Delgado. 2019
// Version 1: July 8th. 2019
// Version 2: 25th July, 2020


/*Minimun Block size: sizeof(uintptr_t)*/

#ifndef LD_OBJECT_POOL_H
#define LD_OBJECT_POOL_H

#include <stdlib.h>
#include <stdint.h>


struct LD_ObjectPool_Element_Free_Struct
{
    void * Pool;
    struct LD_ObjectPool_Element_Free_Struct * Next; //<- It is also where data starts
};

struct LD_ObjectPool_Arena_Struct
{
    void * Next;
    void * Data; //<- It is also where data starts
};

struct LD_ObjectPool
{
    size_t Arena_Capacity;
    size_t Object_Size;
    struct LD_ObjectPool_Element_Free_Struct * Free_List;
    struct LD_ObjectPool_Arena_Struct * Data;
};

/** . */
#define LD_OBJECT_POOL struct LD_ObjectPool

/** . */
#define LD_OBJECTPOOL struct LD_ObjectPool

//#define LD_ObjectPool LD_ObjectPool

#ifdef __cplusplus
extern "C" 
{
#endif

/**
 * @brief Creates an Object Pool.
 * @details The Pool itself is a collection of pre-allocated Arenas, when one runs out of space another is created to hold more objects.
 * @return  Returns a pointer to the created Object Pool. 
 * @retval  NULL is returned if fails.
 * @param Object_Count The Quantity of objects to be hold in per Arena.
 * @param Object_Size The size in bytes of each object.
 */
LD_OBJECTPOOL * LD_ObjectPool_Create(size_t Object_Size);

/**
 * @brief Creates an Object Pool with specified Object count per Arena.
 * @details it behaves in the same way than LD_ObjectPool_Create()
 * @details BE CAREFUL as the parameter orders may seen inverted, the Object Count goes first!.
 * @param Object_Count Number of objects per created Arena.
 * @param Object_Size The size in bytes of each object.
 */
LD_OBJECTPOOL * LD_ObjectPool_Create_EXT(size_t Object_Count,size_t Object_Size);

/**
 *  @brief Destroys an Object pool previously created by LD_ObjectPool_Create() or LD_ObjectPool_Create_EXT()
 *  @details It dealocates the pointer
 * @param Pool Pointer to the Object Pool
 */
void LD_ObjectPool_Destroy(LD_OBJECTPOOL * Pool);


/**
 *  @brief Initializes a manually allocated Object Pool
 *  @details Given an arbitrary buffer in memory, this function converts it intro an Object Pool.
 *  @details It can be used in a nested Object Pool that exists in the Stack or inside an struct, for example.
 *  @return It returns the pointer to the Initialized Object Pool.
 *  @retval NULL is returned if fails.
 *  @param Pool The pointer to the Not-Initialized object pool, this same pointer is returned if succeed.
 * @param Object_Count The Quantity of objects to be hold in per Arena.
 * @param Object_Size The size in bytes of each object.
 */
LD_OBJECTPOOL * LD_ObjectPool_Init(LD_OBJECTPOOL * Pool, size_t Object_Size);


/**
 *  @brief Initializes a manually allocated Object Pool specifing Object Count per Arena.
 *  @details It behaves in the same way than LD_ObjectPool_Init()
 *  @details It can be a bit faster than LD_ObjectPool_Init().
 *  @param Pool The pointer to the Not-Initialized object pool, this same pointer is returned if succeed.
 *  @param Object_Count Number of objects per created Arena.
 *  @param Object_Size The size in bytes of each object.
 */
LD_OBJECTPOOL * LD_ObjectPool_Init_EXT(LD_OBJECTPOOL * Pool,
			    	size_t Object_Count,size_t Object_Size);

/**
 * @brief Deinitializes an Object Pool created by LD_ObjectPool_Init_EXT() 
 * @details it does not deallocates the respective memory buffer.
 * @details it should not be used in Object Pools created by LD_ObjectPool_Create()
 * @param Pool Pointer to the Object Pool
 */
void LD_ObjectPool_deInit(LD_OBJECTPOOL * Pool);


/**
 * @brief This funcion clears an Object Pool of it's data without destroying the pool itself
 * @param Pool Pointer to the Object Pool
 */
void LD_ObjectPool_Clear(LD_OBJECTPOOL * Pool);


/**
 * @brief Retrieve an usable pointer to an Object instance
 * @details It behaves much like POO's new().
 * @return It returns a pointer to a new object, this pointer can be re-casted into aproppiate type.
 * @param Pool Pointer to the base Object Pool.
 */
void * LD_ObjectPool_New(LD_OBJECTPOOL* Pool);


/**
 *  @brief Sets a given object instance created by LD_Object_New() as 'deleted'.
 *  @details It behaves much like POO's delete().
 *  @details It does not clears the Object data, but its pointer becomes unreliable.
 *  @param Object Pointer to the instance to "delete".
 */
void LD_ObjectPool_Delete(void * Object);



struct  LD_ObjectPool_Arena_Struct * __LD_ObjectPool_Destroy_Extra_Arenas(struct LD_ObjectPool * Pool);

#ifdef __cplusplus
}
#endif

#endif
