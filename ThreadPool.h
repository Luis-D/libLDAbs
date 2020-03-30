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


struct LD_ThreadPool_Queue_Pool
{
    struct
    {
	size_t Capacity;
	size_t Current;
    }Byte_Usage;
    void * Buffer; 
    void * Current;
};

struct LD_ThreadPool_Workers
{
    size_t ThreadsCount;
//    uintptr_t Active_Threads;
    //There be thread structs, void * Data is just for reference the start of Threads List/
    ThreadStruct * Thread_Array;
};

struct LD_ThreadPool_Workpile
{
    MutexStruct Cond_Mutex;
    CondStruct Cond;
    size_t Length; 
    struct LD_ThreadPool_Queue_Pool Workpile;
};

struct LD_ThreadPool			    
{   
    unsigned char Flag;
    struct LD_ThreadPool_Workpile Workpile;
    struct LD_ThreadPool_Workers Workers;	    
};
#define THREADPOOL struct LD_ThreadPool
#define LD_THREADPOOL struct LD_ThreadPool

#ifdef __cplusplus
extern "C" 
{
#endif

void LD_ThreadPool_Queue_Pool_Init(struct LD_ThreadPool_Queue_Pool * Queue_Pool,size_t Initial_Cap);

void * LD_ThreadPool_Work_enqueue(struct LD_ThreadPool *Pool,void* (*Function)(void*), void * Arguments, size_t Arguments_Length);

struct LD_ThreadPool * LD_ThreadPool_Create(size_t ThreadsCount);

void LD_ThreadPool_Join(struct LD_ThreadPool * Pool);

#define LD_ThreadPool_IsEmpty(ThreadPool_ptr) ((ThreadPool_ptr)->Workpile.Length==0)


#ifdef __cplusplus
}
#endif

#endif
