/** Header for Memory Management using Object Pool **/
/* Luis Delgado. 2019
 * Version 1: July 8th. 2019
 */

/*Minimun Block size: sizeof(uintptr_t)*/

#ifndef LD_OBJECT_POOL_H
#define LD_OBJECT_POOL_H

#include <stdlib.h>
#include <stdint.h>


struct LD_Object_Pool_Element_Free_Struct
{
    void * Pool;
    struct LD_Object_Pool_Element_Free_Struct * Next; //<- It is also where data starts
};

struct LD_Object_Pool_Arena_Struct
{
    void * Next;
    void * Data; //<- It is also where data starts
};

struct LD_Object_Pool
{
    uintptr_t Arena_Capacity;
    uintptr_t Object_Size;
    struct LD_Object_Pool_Element_Free_Struct * Free_List;
    struct LD_Object_Pool_Arena_Struct * Data;
};

#define LD_OBJECT_POOL struct LD_Object_Pool

void LD_Object_Pool_Clear(struct LD_Object_Pool * Pool);

struct LD_Object_Pool * LD_Object_Pool_Init(struct LD_Object_Pool * Pool,
			    	uintptr_t Object_Count,uintptr_t Object_Size);

struct LD_Object_Pool * LD_Object_Pool_Create(uintptr_t Object_Count,uintptr_t Object_Size);

void LD_Object_Pool_Destroy(struct LD_Object_Pool * Pool);

void * LD_Object_Pool_New(struct LD_Object_Pool * Pool);

void LD_Object_Pool_Delete(void * Object);

#endif
