/**
 * @file
 * @brief Create a use Thread Pools.
 * @details A thread pool is an efficient scheme that consist on a queue of jobs and a number of threads than exist in the background and are awoken when there are jobs avaliable in the queue, this way continuous creation and destruction of threads are avoided.
 * @details Is up to the programmer to set up mechanisms to avoid race conditions and dangerous locks in the host code.
 * @For this to work, proper Threading library have to be linked, libpthread for example.
 */

#ifndef LD_THREADPOOL_H
#define LD_THREADPOOL_H


#ifdef __cplusplus
extern "C" 
{
#endif


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

/** . */
#define LD_THREADPOOL struct LD_ThreadPool

/**
 * @brief Creates a Thread Pool
 * @details It creates a given number of threads that will be in the background waiting for instructions. These threads will execute all functions in their Queue when possible and won't stop doing it until they are joined by LD_ThreadPool_Join().
 * @return It returns a pointer to the created Thread Pool.
 * @retval NULL It is returned if it fails.
 * @param ThreadsCount Is the number of threads to be created. This aspect is fixed during the live of the Thread Pool.
 */
LD_THREADPOOL * LD_ThreadPool_Create(size_t ThreadsCount);

/**
 * @brief Enqueue a Function a it's arguments into the Thread Pool Queue.
 * @return (NOT IMPLEMENTED)
 * @param Pool Pointer to the Thread Pool.
 * @param Function Pointer to the function, its only argument correspond to the pointer to the \p Arguments.
 * @param Arguments Ponter to an struct of arguments that will be enqueued (copied) along the pointer to the Function.
 * @param Arguments_Length Size in bytes of the struct of arguments.
 */
void * LD_ThreadPool_enqueue(LD_THREADPOOL *Pool,void* (*Function)(void*), void * Arguments, size_t Arguments_Length);


/**
 * @brief Join all the threads in the Thread Pool
 * @details It will halt the caller thread until the Thread Pool Queue's elements are executed, then the threads join and then the Thread Pool is destroyed.
 * @param Pool Pointer to the Thread Pool
 */
void LD_ThreadPool_Join(LD_THREADPOOL * Pool);


/**
 * @brief Check whether a Thread Pool Queue is Empty
 * @retval 0 It's returned if the Thread Pool Queue is not Empty
 * @retval 1 It's returned if the Thread Pool Queue is Empty
 */
char LD_ThreadPool_IsEmpty(LD_THREADPOOL * ThreadPool); 


//Some arcane:
void LD_ThreadPool_Queue_Pool_Init(struct LD_ThreadPool_Queue_Pool * Queue_Pool,size_t Initial_Cap);


//Deprecated:
//#define LD_ThreadPool_IsEmpty(ThreadPool_ptr) ((ThreadPool_ptr)->Workpile.Length==0)


#ifdef __cplusplus
}
#endif

#endif
