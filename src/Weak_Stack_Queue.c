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
#include "Weak_Stack_Queue.h"

#ifndef LD_WEAK_STACK_QUEUE_POOL_SIZE 
    #define LD_WEAK_STACK_QUEUE_POOL_SIZE 4096-32-32 //<-4kb, minus some margin
#endif



#define LD_STACK_QUEUE_ELEMENT_SIZE sizeof(struct LD_STACK_QUEUE_Element_Struct)-sizeof(void*)
#define LD_STACK_QUEUE_SIZE sizeof(struct LD_STACK_QUEUE_Struct)-sizeof(void*)

#define LD_STACK_QUEUE_ptr(x) ((struct LD_STACK_QUEUE_Struct*)x)

#include <stddef.h>
size_t LD_Stack_Queue_Element_Size(void * Element)
{
    return *( (size_t*) 
		(
		    ((char*)Element)
		    -offsetof(struct LD_STACK_QUEUE_Element_Struct,Data)
		    +offsetof(struct LD_STACK_QUEUE_Element_Struct,Size)
		)
	    );
}

void LD_Stack_Queue_Clear(struct LD_STACK_QUEUE_Struct * Pool)
{
    struct LD_STACK_QUEUE_Struct * POOL = (struct LD_STACK_QUEUE_Struct*)Pool;
    POOL->Used=0;
    POOL->Top=0;
    POOL->Bottom=0;
}

struct LD_STACK_QUEUE_Struct * LD_Stack_Queue_Init(struct LD_STACK_QUEUE_Struct * POOL)
{
    void * DATA=malloc(LD_WEAK_STACK_QUEUE_POOL_SIZE);
    if(DATA==NULL){return NULL;}
    POOL->Data = DATA;
    POOL->Size = LD_WEAK_STACK_QUEUE_POOL_SIZE;
    LD_Stack_Queue_Clear(POOL);
    return DATA;
}

struct LD_STACK_QUEUE_Struct * LD_Stack_Queue_Create()
{
    struct LD_STACK_QUEUE_Struct * RETURN = 
	(struct LD_STACK_QUEUE_Struct*)malloc(LD_STACK_QUEUE_SIZE);

    if(LD_Stack_Queue_Init(RETURN)==NULL){return NULL;}
    return RETURN;
}

void LD_Stack_Queue_Destroy(struct LD_STACK_QUEUE_Struct * Pool)
{
    free(Pool->Data);
    free(Pool);
}
   
void * LD_Stack_Queue_Element_alloc(struct LD_STACK_QUEUE_Struct * Pool, size_t Size)
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

void* LD_Stack_Queue_Element_Push(struct LD_STACK_QUEUE_Struct * Pool,void * Source, size_t Size)
{
    void * RETURN = LD_Stack_Queue_Element_alloc(Pool, Size);
    if(RETURN==NULL){return NULL;}
    memcpy(RETURN,Source,Size);
    return RETURN; 
}

void* LD_Stack_Queue_Element_Top(struct LD_STACK_QUEUE_Struct * Pool)
{
    void*RETURN= &((struct LD_STACK_QUEUE_Element_Struct*)(Pool->Data+Pool->Top))->Data;
    return RETURN;
}

void* LD_Stack_Queue_Element_Bottom(struct LD_STACK_QUEUE_Struct * Pool)
{
    return &((struct LD_STACK_QUEUE_Element_Struct*)(Pool->Data+Pool->Bottom))->Data;
}
char LD_Stack_Queue_IsEmpty(struct LD_STACK_QUEUE_Struct * Pool)
{
    if((Pool->Top) > (Pool->Bottom) || Pool->Used==0){return 1;}
    return 0;
}

void * LD_Stack_Queue_Element_Pop_Top(struct LD_STACK_QUEUE_Struct  * Pool)
{
//    if(LD_Stack_Queue_Is_Empty(Pool)){;return NULL;}
    struct LD_STACK_QUEUE_Element_Struct * RETURN =
	(struct LD_STACK_QUEUE_Element_Struct*)(Pool->Data+Pool->Top);
    Pool->Top += (RETURN->Size+LD_STACK_QUEUE_ELEMENT_SIZE);
    return &( RETURN->Data);
}

void * LD_Stack_Queue_Element_Pop_Bottom(struct LD_STACK_QUEUE_Struct * Pool)
{
 //   if(LD_Stack_Queue_Is_Empty(Pool)){return NULL;}
    
    struct LD_STACK_QUEUE_Element_Struct * RETURN =
	(struct LD_STACK_QUEUE_Element_Struct*)(Pool->Data+Pool->Bottom);
    Pool->Bottom = Pool->Top+RETURN->Last_Real_Used;
    Pool->Used -=(RETURN->Size+LD_STACK_QUEUE_ELEMENT_SIZE);
    return &( RETURN->Data);
}







#undef LD_STACK_QUEUE_ptr
