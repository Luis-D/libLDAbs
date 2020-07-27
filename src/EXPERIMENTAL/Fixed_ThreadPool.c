#include "Fixed_ThreadPool.h"

struct LD_Fixed_ThreadPool_Work{void (*Function) (void*);void*Args;};

static void* LD_Fixed_ThreadPool_Workpile_Get(void * Pool)
{
    struct LD_Fixed_ThreadPool * ThreadPool = Pool;
    uintptr_t Argument_Size = Pool->WorkLoad_Size;
    uintptr_t WorkLoad_Size = Argument_Size;
    Argument_Size -=sizeof(void*);
    struct LD_Fixed_ThreadPool_Work * Work = NULL;

    while(1)
    {
	pthread_mutex_lock(&ThreadPool->Workpile.Cond_Mutex);

	    while((ThreadPool->Workpile.Length == 0 && ThreadPool->Flag==0)) 
	    {
		//Mutex UNLOCKED at sleep. Mutex LOCKED at wake//
		//		printf("LOCKED @ %p\n",pthread_self());
		pthread_cond_wait(&ThreadPool->Workpile.Cond,&ThreadPool->Workpile.Cond_Mutex);
	       //		printf("UNLOCKED @ %p\n",pthread_self());
	    }

	    char EXITCOND = (ThreadPool->Flag&1)==1;

	    if(ThreadPool->Workpile.Length == 0)
	    {
		if(EXITCOND)
		{
		    pthread_mutex_unlock(&ThreadPool->Workpile.Cond_Mutex);
		    break;
		}
	    }


	    Work = (struct LD_Fixed_ThreadPool_Work*)
		LD_Queue_Front(&ThreadPool->Workpile.Workpile);

	    char WorkBuffer[WorkLoad_Size];
	    memcpy(WorkBuffer,Work,WorkLoad_Size);
	    Work = (void*) WorkBuffer;
	   
		LD_Queue_dequeue(&ThreadPool->Workpile.Workpile);

	    (ThreadPool->Workpile.Length)--;
	pthread_mutex_unlock(&ThreadPool->Workpile.Cond_Mutex);

	Work->Function(&Work->Args);
    }
    return NULL;
}

void LD_ThreadPool_Work_enqueue(struct LD_Fixed_ThreadPool *Pool,void * (*Function)(void*) ,void * WorkLoad)
{
    uintptr_t WorkLoad_Size = Pool->WorkLoad_Size;
    char Buffer[Pool->WorkLoad_Size];
    memcpy(Buffer,Function,sizeof(void*));
    WorkLoad_Size -= sizeof(void*);
    memcpy(Buffer+(sizeof(void*)),WorkLoad,WorkLoad_Size);
    pthread_mutex_lock(&Pool->Workpile.Cond_Mutex);
	LD_Queue_enqueue(Pool->Workpile.Workpile,BUFFER);
	(Pool->Workpile.Length)++;
    pthread_mutex_unlock(&Pool->Workpile.Cond_Mutex);
}

static void __LD_Fixed_ThreadPool_Workers_Init(struct LD_Fixed_ThreadPool_Workers * Workers,size_t ThreadsCount,
    void * ARGS)
{
    Workers->ThreadsCount = ThreadsCount;

    ThreadStruct * tmp = (ThreadStruct*)malloc(sizeof(ThreadStruct)*ThreadsCount);

    for(size_t i = 0; i< ThreadsCount;i++)
    {
	pthread_create(tmp+i,NULL,LD_Fixed_ThreadPool_Workpile_Get,ARGS);
    }

    Workers->Thread_Array = tmp;
}

struct LD_Fixed_ThreadPool * LD_Fixed_ThreadPool_Create(size_t ThreadsCount,size_t WorkLoad_Size)
{
    struct LD_ThreadPool * RETURN = 
	malloc(sizeof(struct LD_Fixed_ThreadPool));
    if(RETURN == NULL){return NULL;}
    memset(RETURN,0,sizeof(struct LD_Fixed_ThreadPool)); 

    pthread_mutex_init(&RETURN->Workpile.Cond_Mutex,NULL);
    pthread_cond_init(&RETURN->Workpile.Cond,NULL);

    RETURN->WorkLoad_Size = WorkLoad_Size+(sizeof(void*));

    RETURN->Workpile.Workpile = LD_Queue_Create(WorkLoad_Size+(sizeof(void*)));

    LD_Fixed_ThreadPool_Workers_Init(&RETURN->Workers,ThreadsCount,RETURN);


    return RETURN;    
}
