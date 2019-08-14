#ifndef LD_THREADPOOL_H
#define LD_THREADPOOL_H

#include "Tricks.h"

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>

#ifndef LD_ThreadPool_minimun_Queue_Pool_Space
    #define LD_ThreadPool_minimun_Queue_Pool_Space (1024*4) // 4KB
#endif

#define ThreadStruct pthread_t
#define MutexStruct pthread_mutex_t
#define CondStruct pthread_cond_t

struct LD_ThreadPool_Queue_Node
{
    uintptr_t Len;
    void (*Function) (void*);
    void * Args; //<-- Argument reference;
};

struct LD_ThreadPool_Queue_Pool
{
    struct
    {
	uintptr_t Capacity;
	uintptr_t Current;
    }Byte_Usage;
    void * Buffer; 
    void * Current;
};

struct LD_ThreadPool_Workers
{
    uintptr_t ThreadsCount;
//    uintptr_t Active_Threads;
    //There be thread structs, void * Data is just for reference the start of Threads List/
    ThreadStruct * Thread_Array;
};

struct LD_ThreadPool_Workpile
{
    MutexStruct Cond_Mutex;
    CondStruct Cond;
    uintptr_t Length; 
    struct LD_ThreadPool_Queue_Pool Workpile;
};

struct LD_ThreadPool			    
{   
    unsigned char Flag;
    struct LD_ThreadPool_Workpile Workpile;
    struct LD_ThreadPool_Workers Workers;	    
};
#define THREADPOOL struct LD_ThreadPool


void LD_ThreadPool_Queue_Pool_Init(struct LD_ThreadPool_Queue_Pool * Queue_Pool,uintptr_t Initial_Cap);

void * LD_ThreadPool_Work_enqueue(struct LD_ThreadPool *Pool, void * Workload, uintptr_t Workpile_Length);

void * LD_ThreadPool_Work_HEAD(struct LD_ThreadPool_Queue_Pool * Pool);

void * LD_ThreadPool_Work_dequeue(struct LD_ThreadPool_Queue_Pool * Pool);

struct LD_ThreadPool * ThreadPool_Create(uintptr_t ThreadsCount);

void LD_ThreadPool_Join(struct LD_ThreadPool * Pool);

#define LD_ThreadPool_IsEmpty(ThreadPool_ptr) ((ThreadPool_ptr)->Workpile.Length==0)

#endif
