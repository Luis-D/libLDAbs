/** Header for Memory Management using Object Pool **/
/* Luis Delgado. 2019
 * Version 1: July 8th. 2019
 */

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

#define LD_OBJECT_POOL struct LD_ObjectPool
#define LD_OBJECTPOOL struct LD_ObjectPool
#define LD_OBJECT_POOL struct LD_ObjectPool
#define LD_ObjectPool LD_ObjectPool

#ifdef __cplusplus
extern "C" 
{
#endif


void LD_ObjectPool_Clear(struct LD_ObjectPool * Pool);

struct LD_ObjectPool * LD_ObjectPool_Init(struct LD_ObjectPool * Pool,
			    	size_t Object_Count,size_t Object_Size);
void LD_ObjectPool_deInit(struct LD_ObjectPool * Pool);

struct LD_ObjectPool * LD_ObjectPool_Create(size_t Object_Count,size_t Object_Size);
void LD_ObjectPool_Destroy(struct LD_ObjectPool * Pool);

void * LD_ObjectPool_New(struct LD_ObjectPool * Pool);
void LD_ObjectPool_Delete(void * Object);

struct  LD_ObjectPool_Arena_Struct * __LD_ObjectPool_Destroy_Extra_Arenas(struct LD_ObjectPool * Pool);

#ifdef __cplusplus
}
#endif

#endif
