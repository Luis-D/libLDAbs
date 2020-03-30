/** Header for Memory Management using Object Pool **/
/* Luis Delgado. 2019
 * Version 1: July 8th. 2019
 */

/*Minimun Block size: sizeof(uintptr_t)*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
//#include "ObjectPool.h"

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

#define LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE sizeof(void *)

#define LD_OBJECT_POOL_ARENA_SIZE offsetof(struct LD_ObjectPool_Arena_Struct,Data)
#define LD_OBJECT_POOL_ELEMENT_SIZE offsetof(struct LD_ObjectPool_Element_Free_Struct,Next);

#define __ObjectPool_Arena_Destroy free




static void __ObjectPool_Arena_Clear(struct LD_ObjectPool_Arena_Struct* Arena,
					struct LD_ObjectPool * Pool,
					size_t Object_Count,size_t Object_Size)
{
    struct LD_ObjectPool_Element_Free_Struct * EX = 
	(struct LD_ObjectPool_Element_Free_Struct*)&Arena->Data;

    size_t E_SIZE = Object_Size+LD_OBJECT_POOL_ELEMENT_SIZE;


    for(size_t i=Object_Count;i--;)
    {
	EX->Pool = Pool;
//	printf("%p@%p\n",Pool,&EX->Pool);

	EX->Next = (struct LD_ObjectPool_Element_Free_Struct*) 
	    (((uintptr_t)(((char*)EX)+E_SIZE))*(i>0)); //If i==0, then EX->Next = NULL;
	EX=EX->Next;
	
/*	
	void * _NEXT = NULL;
	if(i!=Object_Count-1)
	{_NEXT=(void*)(((char*)EX) + E_SIZE);}
	EX->Next = _NEXT;
	EX=(void*)_NEXT;
*/
    }
}

static struct LD_ObjectPool_Arena_Struct *__ObjectPool_Arena_Create(struct LD_ObjectPool *Pool,size_t Object_Count,size_t Object_Size)
{
    size_t O_SIZE = Object_Size;
    if(O_SIZE < LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE){O_SIZE=LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE;}
    O_SIZE+=LD_OBJECT_POOL_ELEMENT_SIZE;
    struct LD_ObjectPool_Arena_Struct * RETURN =
	malloc(LD_OBJECT_POOL_ARENA_SIZE+(Object_Count*(O_SIZE)));

//    printf("--->%p\n",RETURN);

    __ObjectPool_Arena_Clear(RETURN,Pool,Object_Count,Object_Size);
    return RETURN;
}

struct  LD_ObjectPool_Arena_Struct * __LD_ObjectPool_Destroy_Extra_Arenas(struct LD_ObjectPool * Pool)
{
	struct LD_ObjectPool_Arena_Struct *EX = Pool->Data;

    while(1)
    {
	struct LD_ObjectPool_Arena_Struct * NEXT = EX->Next;
	if(NEXT==NULL){break;}
	__ObjectPool_Arena_Destroy(EX);
	EX=NEXT;
    }
	return EX;
}

void LD_ObjectPool_Clear(struct LD_ObjectPool * Pool)
{
    struct LD_ObjectPool_Arena_Struct *EX = 
__LD_ObjectPool_Destroy_Extra_Arenas(Pool);

    __ObjectPool_Arena_Clear(EX,Pool,Pool->Arena_Capacity,Pool->Object_Size);
    Pool->Data=EX;
    Pool->Free_List =(struct LD_ObjectPool_Element_Free_Struct*) &EX->Data;
}


struct LD_ObjectPool * LD_ObjectPool_Init(struct LD_ObjectPool * Pool,
			    	size_t Object_Count,size_t Object_Size)
{
    if(Object_Size<LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE)
    {Object_Size = LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE;}

    Pool->Arena_Capacity = Object_Count;
    Pool->Object_Size = Object_Size;

    struct LD_ObjectPool_Arena_Struct * ARENA = __ObjectPool_Arena_Create(Pool,Object_Count,Object_Size);
    //printf("--->%p\n",ARENA);
    if(ARENA==NULL){return NULL;}
    ARENA->Next=NULL;
    Pool->Data = ARENA;
    Pool->Free_List =(struct LD_ObjectPool_Element_Free_Struct*) &ARENA->Data;

    return Pool;
}


struct LD_ObjectPool * LD_ObjectPool_Create(size_t Object_Count,size_t Object_Size)
{
    struct LD_ObjectPool * Pool =
	malloc(sizeof(struct LD_ObjectPool));
    if(Pool==NULL){return NULL;}
    
    if(LD_ObjectPool_Init(Pool,Object_Count,Object_Size)==NULL){return NULL;}

    return Pool;
}


void LD_ObjectPool_deInit(struct LD_ObjectPool * Pool)
{
    struct LD_ObjectPool_Arena_Struct *EX = Pool->Data;
    while(EX!=NULL)
    {
	//printf("->%p\n",EX);
	struct LD_ObjectPool_Arena_Struct * NEXT = EX->Next;
	free(EX);
	EX=NEXT;
    }
}

void LD_ObjectPool_Destroy(struct LD_ObjectPool * Pool)
{
    LD_ObjectPool_deInit(Pool);
    free(Pool);
}


void * LD_ObjectPool_New(struct LD_ObjectPool * Pool)
{
    struct LD_ObjectPool_Element_Free_Struct * RETURN = Pool->Free_List;

    if(RETURN==NULL)
    {
	//No more space, creating a new Arena
	struct LD_ObjectPool_Arena_Struct * ARENA = 
	    __ObjectPool_Arena_Create(Pool,Pool->Arena_Capacity,Pool->Object_Size);
	if(ARENA == NULL){return NULL;}

	ARENA->Next=Pool->Data;
	Pool->Data=ARENA;
	RETURN = (struct LD_ObjectPool_Element_Free_Struct*) &ARENA->Data;
	Pool->Free_List=RETURN;
    }

    Pool->Free_List = RETURN->Next;

//    printf("P: %p\n",(void*)*(((uintptr_t*)&RETURN->Next)-1));

    return (void*) &RETURN->Next;
}


void LD_ObjectPool_Delete(void * Object)
{
    struct LD_ObjectPool_Element_Free_Struct * Head =
	(struct LD_ObjectPool_Element_Free_Struct*)
	(
	 ((char*)Object)
	 -offsetof(struct LD_ObjectPool_Element_Free_Struct,Next)
	);

    struct LD_ObjectPool * Pool = 
	(struct LD_ObjectPool*) (Head->Pool);

    //printf("Pool: %p\n",(void*)(Head->Pool));

    Head->Next=Pool->Free_List;

    Pool->Free_List = Head;
//    printf("	Pool: %p\n",(void*)(Head->Pool));
}

