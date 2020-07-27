#include "../ThreadPool.h"

//struct LD_ThreadPool_Work{size_t Len;void* (*Function) (void*);void*Args;};


void * LD_ThreadPool_Work_enqueue(struct LD_ThreadPool *Pool,void* (*Function)(void*),void * Arguments, size_t Arguments_Length)
{
    printf("START ENQ\n");
    printf("---> %u\n",*((uintptr_t*)&Pool->Workpile.Cond_Mutex));

    size_t Workpile_Length = Arguments_Length +(sizeof(void*));

    pthread_mutex_lock(&Pool->Workpile.Cond_Mutex);
    printf("->ENQ\n");

/*	
	char * Workload = (char*) LD_Weak_Queue_alloc(&Pool->Workpile.Workpile,Workpile_Length);
	printf("Alloc'd\n");
    
	memcpy(Workload+(sizeof(size_t)),&Function,sizeof(void*));
	memcpy(Workload+sizeof(void*)+(sizeof(size_t)),Arguments,Arguments_Length);
    
	(Pool->Workpile.Length)++;
	*/
    pthread_mutex_unlock(&Pool->Workpile.Cond_Mutex);
    pthread_cond_signal(&Pool->Workpile.Cond);
    printf("ENQ'd\n");
    //getchar();
}


static void* LD_ThreadPool_Workpile_Get(void * Pool)
{
    struct LD_ThreadPool * ThreadPool = (struct LD_ThreadPool*) Pool;
        printf("-->STARTED @ %p\n",pthread_self());
	printf("@%p; WPL: %lu\n",pthread_self(),ThreadPool->Workpile.Length);
    while(1)
    {
	pthread_mutex_lock(&ThreadPool->Workpile.Cond_Mutex);
	    while((ThreadPool->Workpile.Length == 0 && ThreadPool->Flag==0)) 
	    {pthread_cond_wait(&ThreadPool->Workpile.Cond,&ThreadPool->Workpile.Cond_Mutex);}
    
	    unsigned char EXITCOND = (ThreadPool->Flag&1)==1;

	    if(ThreadPool->Workpile.Length == 0)
	    {
		if(EXITCOND)
		{
		    pthread_mutex_unlock(&ThreadPool->Workpile.Cond_Mutex);
		    break;
		}
	    }

	    printf("Working\n");

	    void * Work = LD_Weak_Queue_Dequeue(&ThreadPool->Workpile.Workpile);
	    size_t WorkSize = LD_Weak_Queue_Element_Size(Work);
	    size_t ArgsSize = WorkSize-sizeof(void*);
	    void*(*Func)(void*) = (void*)(Work);
	    void* Arguments =(void*)(((char*)Work)+sizeof(void*));
	    
	    printf("Function: %p\n",Func);
	    printf("Arguments Size:  %lu",ArgsSize);
	     
	    char ArgBuffer[WorkSize]; //WorkSize is used to add some margin and prevent a 0 sized buffer;
	    memcpy(ArgBuffer,Arguments,ArgsSize);

	    (ThreadPool->Workpile.Length)--;
	pthread_mutex_unlock(&ThreadPool->Workpile.Cond_Mutex);

	Func(ArgBuffer);
    }

    return NULL;
}

static void LD_ThreadPool_Workers_Init(struct LD_ThreadPool_Workers * Workers,size_t ThreadsCount,
    void * ARGS)
{
    Workers->ThreadsCount = ThreadsCount;

    ThreadStruct * tmp = (ThreadStruct*)malloc(sizeof(ThreadStruct)*ThreadsCount);

    for(size_t i = 0; i< ThreadsCount;i++)
    {
	pthread_create(tmp+i,NULL,LD_ThreadPool_Workpile_Get,ARGS);
    }

    Workers->Thread_Array = tmp;
}

struct LD_ThreadPool * LD_ThreadPool_Create(size_t ThreadsCount)
{

//    printf("Creating\n");
    struct LD_ThreadPool * RETURN = (struct LD_ThreadPool*)
			malloc(sizeof(struct LD_ThreadPool));
    if(RETURN == NULL){return NULL;}
    memset(RETURN,0,sizeof(struct LD_ThreadPool)); //Init in 0's

    pthread_mutex_init(&RETURN->Workpile.Cond_Mutex,NULL);
    pthread_cond_init(&RETURN->Workpile.Cond,NULL);
    RETURN->Workpile.Length = 0;

    LD_Weak_Queue_Init(&RETURN->Workpile.Workpile);
    LD_ThreadPool_Workers_Init(&RETURN->Workers,ThreadsCount,RETURN);

//    printf("CREATED\n");
}

void LD_ThreadPool_Join(struct LD_ThreadPool * Pool)
{
    (Pool->Flag)|=1;
    pthread_cond_broadcast(&Pool->Workpile.Cond);
    size_t ThreadsCount = Pool->Workers.ThreadsCount; 
    ThreadStruct * TA = Pool->Workers.Thread_Array;
    for(size_t i = 0; i<ThreadsCount;i++)
    {
	pthread_join(TA[i],NULL);
    }
   
    free(Pool->Workpile.Workpile.Data); 
    free(TA);
    free(Pool);
}

