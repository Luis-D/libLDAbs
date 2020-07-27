#ifndef LD_FIXED_THREADPOOL
#define LD_FIXED_THREADPOOL

#include "Tricks.h"
#include "Stack_Queue.h"

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>


#ifndef LD_Fixed_ThreadPool_minimun_Queue_Pool_Space
    #define LD_Fixed_ThreadPool_minimun_Queue_Pool_Space (1024*4) // 4KB
#endif

#define ThreadStruct pthread_t
#define MutexStruct pthread_mutex_t
#define CondStruct pthread_cond_t

struct LD_Fixed_ThreadPool_Queue_Node
{
    void (*Function) (void*);
    void * Args; //<-- Argument reference;
};

struct LD_Fixed_ThreadPool
{
    unsigned char Flag;
    uintptr_t WorkLoad_Size;
    struct LD_Fixed_ThreadPool_Workpile
    {
	MutexStruct Cond_Mutex;
	CondStruct Cond;
	uintptr_t Length;
	LD_QUEUE * Workpile;
    } Workpile;

    struct LD_Fixed_ThreadPool_Workers
    {
	uintptr_t ThreadsCount;
	ThreadStruct * Thread_Array;
    }Workers;
};
#define FIXED_THREADPOOL struct LD_Fixed_ThreadPool

#ifdef __cplusplus
extern "C"
{
#endif

void LD_Fixed_ThreadPool_Queue_Pool_Init(struct LD_ThreadPool_Queue_Pool *Queue_Pool,size_t Initial_Cap);

void LD_Fixed_ThreadPool_Work_enqueue(struct LD_ThreadPool *Pool, void*(*Function)(void*),void * Workload);

void * LD_Fixed_ThreadPool_Work_HEAD(struct LD_ThreadPool_Queue_Pool * Pool);

void LD_Fixed_ThreadPool_Work_dequeue(struct LD_ThreadPool_Queue_Pool * Pool);

struct LD_Fixed_ThreadPool * LD_Fixed_ThreadPool_Create(size_t ThreadsCount,size_t WorkLoad_Size);

void LD_Fixed_ThreadPool_Join(struct LD_ThreadPool * Pool);

#define LD_Fixed_ThreadPool_IsEmpty(ThreadPool_ptr) ((ThreadPool_ptr)->Workpile.Length==0)

#ifdef __cplusplus
}
#endif

#endif
