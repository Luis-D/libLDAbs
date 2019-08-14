/** Header for Memory Management using Object Pool **/
/* Luis Delgado. 2019
 * Version 1: July 8th. 2019
 */

/*Minimun Block size: sizeof(uintptr_t)*/

//#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "Object_Pool.h"

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

#define LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE sizeof(void *)

#define LD_OBJECT_POOL_ARENA_SIZE sizeof(struct LD_Object_Pool_Arena_Struct)-sizeof(void*)
#define LD_OBJECT_POOL_ELEMENT_SIZE sizeof(struct LD_Object_Pool_Element_Free_Struct)-sizeof(void*)

#define __Object_Pool_Arena_Destroy free




static void __Object_Pool_Arena_Clear(struct LD_Object_Pool_Arena_Struct* Arena,
					struct LD_Object_Pool * Pool,
					uintptr_t Object_Count,uintptr_t Object_Size)
{
    struct LD_Object_Pool_Element_Free_Struct * EX = 
	(struct LD_Object_Pool_Element_Free_Struct*)&Arena->Data;

    uintptr_t E_SIZE = Object_Size+LD_OBJECT_POOL_ELEMENT_SIZE;


    for(unsigned int i=Object_Count;i--;)
    {
	EX->Pool = Pool;
//	printf("%p@%p\n",Pool,&EX->Pool);
	EX->Next = (struct LD_Object_Pool_Element_Free_Struct*) 
	    (((uintptr_t)(((char*)EX)+E_SIZE))*(i>0)); //If i==0, then EX->Next = NULL;
	EX=EX->Next;
    }
}

static struct LD_Object_Pool_Arena_Struct * __Object_Pool_Arena_Create(struct LD_Object_Pool * Pool,
					uintptr_t Object_Count,uintptr_t Object_Size)
{
    uintptr_t O_SIZE = Object_Size;
    if(O_SIZE < LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE){O_SIZE=LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE;}
    O_SIZE+=LD_OBJECT_POOL_ELEMENT_SIZE;

    struct LD_Object_Pool_Arena_Struct * RETURN =
	(struct LD_Object_Pool_Arena_Struct*) malloc(LD_OBJECT_POOL_ARENA_SIZE+(Object_Count*(O_SIZE)));

    __Object_Pool_Arena_Clear(RETURN,Pool,Object_Count,Object_Size);

    return RETURN;
}


void LD_Object_Pool_Clear(struct LD_Object_Pool * Pool)
{
    struct LD_Object_Pool_Arena_Struct *EX = Pool->Data;

    while(1)
    {
	struct LD_Object_Pool_Arena_Struct * NEXT = EX->Next;
	if(NEXT==NULL){break;}
	__Object_Pool_Arena_Destroy(EX);
	EX=NEXT;
    }

    __Object_Pool_Arena_Clear(EX,Pool,Pool->Arena_Capacity,Pool->Object_Size);
    Pool->Data=EX;
    Pool->Free_List =(struct LD_Object_Pool_Element_Free_Struct*) &EX->Data;
}


struct LD_Object_Pool * LD_Object_Pool_Init(struct LD_Object_Pool * Pool,
			    	uintptr_t Object_Count,uintptr_t Object_Size)
{
    if(Object_Size<LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE)
    {Object_Size = LD_OBJECT_POOL_MINIMUN_OBJECT_SIZE;}

    Pool->Arena_Capacity = Object_Count;
    Pool->Object_Size = Object_Size;

    struct LD_Object_Pool_Arena_Struct * ARENA = __Object_Pool_Arena_Create(Pool,Object_Count,Object_Size);
    if(ARENA==NULL){return NULL;}
    ARENA->Next=NULL;
    Pool->Data = ARENA;
    Pool->Free_List =(struct LD_Object_Pool_Element_Free_Struct*) &ARENA->Data;

    return Pool;
}


struct LD_Object_Pool * LD_Object_Pool_Create(uintptr_t Object_Count,uintptr_t Object_Size)
{
    struct LD_Object_Pool * Pool =
	(struct LD_Object_Pool*) malloc(sizeof(struct LD_Object_Pool_Arena_Struct));
    if(Pool==NULL){return NULL;}
    
    if(LD_Object_Pool_Init(Pool,Object_Count,Object_Size)==NULL){return NULL;}

    return Pool;
}

void LD_Object_Pool_Destroy(struct LD_Object_Pool * Pool)
{
    struct LD_Object_Pool_Arena_Struct *EX = Pool->Data;
    while(EX!=NULL)
    {
	struct LD_Object_Pool_Arena_Struct * NEXT = EX->Next;
	free(EX);
	EX=NEXT;
    }
    free(Pool);
}


void * LD_Object_Pool_New(struct LD_Object_Pool * Pool)
{
    struct LD_Object_Pool_Element_Free_Struct * RETURN = Pool->Free_List;

    if(RETURN==NULL)
    {
	//No more space, creating a new Arena
	struct LD_Object_Pool_Arena_Struct * ARENA = 
	    __Object_Pool_Arena_Create(Pool,Pool->Arena_Capacity,Pool->Object_Size);
	if(ARENA == NULL){return NULL;}

	ARENA->Next=Pool->Data;
	Pool->Data=ARENA;
	RETURN = (struct LD_Object_Pool_Element_Free_Struct*) &ARENA->Data;
	Pool->Free_List=RETURN;
    }

    Pool->Free_List = RETURN->Next;

//    printf("P: %p\n",(void*)*(((uintptr_t*)&RETURN->Next)-1));

    return (void*) &RETURN->Next;
}


void LD_Object_Pool_Delete(void * Object)
{
    struct LD_Object_Pool_Element_Free_Struct * Head =
	(struct LD_Object_Pool_Element_Free_Struct*)(((uintptr_t*)Object)-1);

    struct LD_Object_Pool * Pool = 
	(struct LD_Object_Pool*) (Head->Pool);

//    printf("Pool: %p\n",(void*)(Head->Pool));

    Head->Next=Pool->Free_List;

    Pool->Free_List = Head;
//    printf("	Pool: %p\n",(void*)(Head->Pool));
}

