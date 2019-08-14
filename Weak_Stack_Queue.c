/*  Luis Delgado. 2019
 *  Header for weak pointer variable-size Queue and Stack using a Pool.
 * */

/*
 * Version 1: July 7th, 2019.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//#include "Weak_Stack_Queue.h"

#ifndef LD_WEAK_STACK_QUEUE_POOL_SIZE 
    #define LD_WEAK_STACK_QUEUE_POOL_SIZE 4096 //<-4kb
#endif

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



#define LD_STACK_QUEUE_ELEMENT_SIZE sizeof(struct LD_STACK_QUEUE_Element_Struct)-sizeof(void*)
#define LD_STACK_QUEUE_SIZE sizeof(struct LD_STACK_QUEUE_Struct)-sizeof(void*)

#define LD_STACK_QUEUE_ptr(x) ((struct LD_STACK_QUEUE_Struct*)x)


void __Stack_Queue_Clear(struct LD_STACK_QUEUE_Struct * Pool)
{
    struct LD_STACK_QUEUE_Struct * POOL = (struct LD_STACK_QUEUE_Struct*)Pool;
    POOL->Used=0;
    POOL->Top=0;
    POOL->Bottom=0;
}

struct LD_STACK_QUEUE_Struct * __Stack_Queue_Init(struct LD_STACK_QUEUE_Struct * POOL)
{
    void * DATA=malloc(LD_WEAK_STACK_QUEUE_POOL_SIZE);
    if(DATA==NULL){return NULL;}
    POOL->Data = DATA;
    POOL->Size = LD_WEAK_STACK_QUEUE_POOL_SIZE;
    __Stack_Queue_Clear(POOL);
    return DATA;
}

struct LD_STACK_QUEUE_Struct * __Stack_Queue_Create()
{
    struct LD_STACK_QUEUE_Struct * RETURN = 
	(struct LD_STACK_QUEUE_Struct*)malloc(LD_STACK_QUEUE_SIZE);

    if(__Stack_Queue_Init(RETURN)==NULL){return NULL;}
    return RETURN;
}

void __Stack_Queue_Destroy(struct LD_STACK_QUEUE_Struct * Pool)
{
    free(Pool->Data);
    free(Pool);
}
   
void * __Stack_Queue_Element_alloc(struct LD_STACK_QUEUE_Struct * Pool, uintptr_t Size)
{
    uintptr_t New_Size = Size+LD_STACK_QUEUE_ELEMENT_SIZE;
    uintptr_t Used = Pool->Used;
    uintptr_t Top = Pool->Top;
    uintptr_t Real_Used = Used-Top;
    uintptr_t Bottom_Ref = Pool->Bottom-Top;
    void * Pool_Data = Pool->Data;

    if(Used+New_Size > Pool->Size)
    {
	if(New_Size<=Pool->Top)
	{
	    if(Real_Used!=0)
	    {memmove(Pool_Data,Pool_Data + Top, Real_Used);}

	    Pool->Top = 0;
	    Used = Real_Used;
	}
	else
	{
	    uintptr_t Pool_New_Size = New_Size+Pool->Size;
	    void * tmp = realloc(Pool_Data,Pool_New_Size);
	    if(tmp==NULL){return NULL;}
	    Pool->Data=tmp;
	    Pool->Size=Pool_New_Size;
	}

    }

    Pool->Bottom = Used;

    struct LD_STACK_QUEUE_Element_Struct* RETURN =
       (struct LD_STACK_QUEUE_Element_Struct*)(Pool_Data + Used);
    
    Pool->Used=Used+New_Size;

    RETURN->Size = Size;
    RETURN->Last_Real_Used=Bottom_Ref;

//    printf("Allocated: %ld,%ld,%ld\n",Pool->Top,Pool->Bottom,Pool->Used);

    return &(RETURN->Data);
}

void* __Stack_Queue_Element_Push(struct LD_STACK_QUEUE_Struct * Pool,void * Source, uintptr_t Size)
{
    void * RETURN = __Stack_Queue_Element_alloc(Pool, Size);
    if(RETURN==NULL){return NULL;}
    memcpy(RETURN,Source,Size);
    return RETURN; 
}

void* __Stack_Queue_Element_Top(struct LD_STACK_QUEUE_Struct * Pool)
{
    void*RETURN= &((struct LD_STACK_QUEUE_Element_Struct*)(Pool->Data+Pool->Top))->Data;
    return RETURN;
}

void* __Stack_Queue_Element_Bottom(struct LD_STACK_QUEUE_Struct * Pool)
{
    return &((struct LD_STACK_QUEUE_Element_Struct*)(Pool->Data+Pool->Bottom))->Data;
}
char __Stack_Queue_Is_Empty(struct LD_STACK_QUEUE_Struct * Pool)
{
    if((Pool->Top) > (Pool->Bottom) || Pool->Used==0){return 1;}
    return 0;
}

void * __Stack_Queue_Element_Pop_Top(struct LD_STACK_QUEUE_Struct  * Pool)
{
//    if(__Stack_Queue_Is_Empty(Pool)){;return NULL;}
    struct LD_STACK_QUEUE_Element_Struct * RETURN =
	(struct LD_STACK_QUEUE_Element_Struct*)(Pool->Data+Pool->Top);
    Pool->Top += (RETURN->Size+LD_STACK_QUEUE_ELEMENT_SIZE);
    return &( RETURN->Data);
}

void * __Stack_Queue_Element_Pop_Bottom(struct LD_STACK_QUEUE_Struct * Pool)
{
 //   if(__Stack_Queue_Is_Empty(Pool)){return NULL;}
    
    struct LD_STACK_QUEUE_Element_Struct * RETURN =
	(struct LD_STACK_QUEUE_Element_Struct*)(Pool->Data+Pool->Bottom);
    Pool->Bottom = Pool->Top+RETURN->Last_Real_Used;
    Pool->Used -=(RETURN->Size+LD_STACK_QUEUE_ELEMENT_SIZE);
    return &( RETURN->Data);
}




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







#undef LD_STACK_QUEUE_ptr
