#include "../ThreadPool.h"

struct LD_ThreadPool_Work{uintptr_t Len;void (*Function) (void*);void*Args;};


void LD_ThreadPool_Queue_Pool_Init(struct LD_ThreadPool_Queue_Pool * Queue_Pool,uintptr_t Initial_Cap)
{
    Queue_Pool->Buffer = malloc(Initial_Cap);
    Queue_Pool->Byte_Usage.Capacity = Initial_Cap;
    Queue_Pool->Byte_Usage.Current=0;
    Queue_Pool->Current = NULL;
}

static void __LD_ThreadPool_Queue_Pool_add_EXT(struct LD_ThreadPool_Queue_Pool * Pool, void * Load, uintptr_t Len)
{
    uintptr_t OFFSET = sizeof(struct LD_ThreadPool_Queue_Node) 
			-sizeof(void*)
			-(sizeof(void(*)(void*)));
    uintptr_t REAL_LEN = Len+OFFSET;
    uintptr_t CURCUR =Pool->Byte_Usage.Current;
    uintptr_t CURCAP =Pool->Byte_Usage.Capacity ;
    uintptr_t NEWCAP = (CURCUR+REAL_LEN) ;
    uintptr_t IF_GAP = (((uintptr_t) Pool->Current)- ((uintptr_t) Pool->Buffer));
    uintptr_t FC_GAP = -(((uintptr_t) Pool->Current)- (((uintptr_t) Pool->Buffer)+(CURCUR)));
    uintptr_t FM_GAP = -(((uintptr_t) Pool->Current)- (((uintptr_t) Pool->Buffer)+(CURCAP)));



    if(Pool->Current==NULL)
    {
	FC_GAP = 0;
	IF_GAP=0;
    }    

    uintptr_t REAL_TOTAL_USED = FC_GAP;
/*
	    printf("******\n");
	    printf("Current: %p\n",Pool->Current);	    
	    printf("Last: %p\n",Pool->Last);	    
	    printf("Buffer: %p\n",Pool->Buffer); 

	    printf("CURCAP: %d\n",CURCAP);
	    printf("CURCUR: %d\n",CURCUR);
	    printf("NEWCAP: %d\n",NEWCAP);
	    printf("REAL_LEN: %d\n",REAL_LEN);
	    printf("IF: %d\n",IF_GAP);
	    printf("FL: %d\n",FL_GAP);
	    printf("REAL_TOTAL_USED: %d\n",REAL_TOTAL_USED);
*/
    if(NEWCAP> CURCAP)
    {
	
	if((REAL_TOTAL_USED+REAL_LEN) < CURCAP)
	{
//	    printf("ThreadPool. defragmentation @ %p\n",Pool);
	    memmove(Pool->Buffer,Pool->Current,CURCUR-IF_GAP);
	    Pool->Current = Pool->Buffer;
	    Pool->Byte_Usage.Current = REAL_TOTAL_USED;
//	    printf("ThreadPool. defragmentation @ %p (Completed) \n",Pool);
    
	}
	else

	{

	//    printf("ThreadPool. realloc @ %p\n",Pool);
	
	    if(NEWCAP<(CURCAP<<2)){NEWCAP=CURCAP<<2;}
	   // printf("AUGMENTED to: %d\n",NEWCAP);
	    Pool->Buffer = realloc(Pool->Buffer,NEWCAP);
	    Pool->Current =(void*) (((uintptr_t)Pool->Buffer)+IF_GAP);
	    Pool->Byte_Usage.Capacity = NEWCAP;
	//    printf("AUGMENTED to: %d OK\n ",Pool->Byte_Usage.Capacity);
	//    printf("ThreadPool Realloc @ %p (Completed)\n",Pool);
	}

    }

    CURCUR =Pool->Byte_Usage.Current;

    memcpy(((char*)Pool->Buffer)+CURCUR+OFFSET,Load,Len);
    
    struct LD_ThreadPool_Queue_Node *New_Last = 
	    (struct LD_ThreadPool_Queue_Node*)(((uintptr_t) Pool->Buffer)+CURCUR);

    New_Last->Len = Len;

    if(CURCUR==0)
    {
	Pool->Current = New_Last;
    }

    (Pool->Byte_Usage.Current)+=REAL_LEN;

/*    
    IF_GAP = (((uintptr_t) Pool->Current)- ((uintptr_t) Pool->Buffer));
    printf("New Last: %p\n",New_Last); 
    printf("New Current: %p\n",Pool->Current);
    printf("New Buffer: %p\n",Pool->Buffer); 
    printf("New IF: %d\n",IF_GAP); 
    printf("New CURCUR: %d\n",Pool->Byte_Usage.Current); 
*/
}

void * LD_ThreadPool_Work_enqueue(struct LD_ThreadPool *Pool, void * Workload, uintptr_t Workpile_Length)
{
    //printf("START ENQ\n");
    //printf("---> %p\n",*((uintptr_t*)&Pool->Workpile.Cond_Mutex));
    pthread_mutex_lock(&Pool->Workpile.Cond_Mutex);
    //printf("ENQ\n");
	__LD_ThreadPool_Queue_Pool_add_EXT(&Pool->Workpile.Workpile,Workload,Workpile_Length);
	(Pool->Workpile.Length)++;
    pthread_mutex_unlock(&Pool->Workpile.Cond_Mutex);
    pthread_cond_signal(&Pool->Workpile.Cond);
    //getchar();
}

void * LD_ThreadPool_Work_HEAD(struct LD_ThreadPool_Queue_Pool * Pool)
{
    void * RETURN = NULL;

    RETURN = &((struct LD_ThreadPool_Queue_Node*)Pool->Current)->Len;

    return RETURN;
}

void* LD_ThreadPool_Work_dequeue(struct LD_ThreadPool_Queue_Pool * Pool)
{
    void * RETURN = LD_ThreadPool_Work_HEAD(Pool);
    Pool->Current = ((char*)RETURN)+((struct LD_ThreadPool_Queue_Node*)RETURN)->Len+sizeof(uintptr_t);
    return RETURN;
}

static void* LD_ThreadPool_Workpile_Get(void * Pool)
{
    //    printf("STARTED @ %p\n",pthread_self());
    struct LD_ThreadPool * ThreadPool = (struct LD_ThreadPool*) Pool;

    struct LD_ThreadPool_Work * Work = NULL;

    while(1)
    {
//	printf("Loop start @ %p\n",pthread_self());	
	pthread_mutex_lock(&ThreadPool->Workpile.Cond_Mutex);
//	printf("Mutex taken @ %p\n",pthread_self());
	while((ThreadPool->Workpile.Length == 0 && ThreadPool->Flag==0)) 
	{
	    //Mutex UNLOCKED at sleep. Mutex LOCKED at wake//
	    //		printf("LOCKED @ %p\n",pthread_self());
	    pthread_cond_wait(&ThreadPool->Workpile.Cond,&ThreadPool->Workpile.Cond_Mutex);
	   //		printf("UNLOCKED @ %p\n",pthread_self());
	}

	char EXITCOND = (ThreadPool->Flag&1)==1;
	//printf("\tWLEN: %d @ %p\n",ThreadPool->Workpile.Length,pthread_self());
	//printf("\tEXIT: %d @ %p\n",EXITCOND,pthread_self());

	    if(ThreadPool->Workpile.Length == 0)
	    {
		if(EXITCOND)
		{
		    pthread_mutex_unlock(&ThreadPool->Workpile.Cond_Mutex);
		    break;
		}
	    }

	    
	     Work= (struct LD_ThreadPool_Work*) LD_ThreadPool_Work_dequeue(&ThreadPool->Workpile.Workpile);

	    //printf("Work: %ld\n",(uintptr_t)Work);
	
	    uintptr_t WorkSize = Work->Len + sizeof(uintptr_t);
	    
	    //printf("Len: %d\n",Work->Len);
	    //printf("Func:%p\n",Work->Function);
	    char WorkBuffer[WorkSize];
	    memcpy(WorkBuffer,Work,WorkSize);
	    Work = (struct LD_ThreadPool_Work*) WorkBuffer;

	    (ThreadPool->Workpile.Length)--;
	pthread_mutex_unlock(&ThreadPool->Workpile.Cond_Mutex);
	
	Work->Function(&Work->Args);

//	pthread_mutex_lock(&ThreadPool->Workpile.Cond_Mutex);
//	    (ThreadPool->Pool.Active_Threads)--;
//	pthread_mutex_unlock(&ThreadPool->Workpile.Cond_Mutex);
    }
    //printf("EXITED @ %p\n",pthread_self());
    return NULL;
}

static void LD_ThreadPool_Workers_Init(struct LD_ThreadPool_Workers * Workers,uintptr_t ThreadsCount,
    void * ARGS)
{
    Workers->ThreadsCount = ThreadsCount;

    ThreadStruct * tmp = (ThreadStruct*)malloc(sizeof(ThreadStruct)*ThreadsCount);

    for(uintptr_t i = 0; i< ThreadsCount;i++)
    {
	pthread_create(tmp+i,NULL,LD_ThreadPool_Workpile_Get,ARGS);
    }

    Workers->Thread_Array = tmp;
}

struct LD_ThreadPool * ThreadPool_Create(uintptr_t ThreadsCount)
{
//    printf("Creating\n");
    struct LD_ThreadPool * RETURN = (struct LD_ThreadPool*)
			malloc(sizeof(struct LD_ThreadPool));
    if(RETURN == NULL){return NULL;}
    memset(RETURN,0,sizeof(struct LD_ThreadPool)); //Init in 0's

    pthread_mutex_init(&RETURN->Workpile.Cond_Mutex,NULL);
    pthread_cond_init(&RETURN->Workpile.Cond,NULL);


    
    LD_ThreadPool_Queue_Pool_Init(&RETURN->Workpile.Workpile,
	    LD_ThreadPool_minimun_Queue_Pool_Space);
    LD_ThreadPool_Workers_Init(&RETURN->Workers,ThreadsCount,RETURN);

//    printf("CREATED\n");

    return RETURN;    
}

void LD_ThreadPool_Join(struct LD_ThreadPool * Pool)
{
    (Pool->Flag)|=1;
    pthread_cond_broadcast(&Pool->Workpile.Cond);
    uintptr_t ThreadsCount = Pool->Workers.ThreadsCount; 
    ThreadStruct * TA = Pool->Workers.Thread_Array;
    for(uintptr_t i = 0; i<ThreadsCount;i++)
    {
	pthread_join(TA[i],NULL);
    }
    
    free(Pool->Workpile.Workpile.Buffer);
    free(TA);
    free(Pool);
}

