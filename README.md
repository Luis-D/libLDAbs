# libLDAbs
Abstract Data Types and other intricate abstractions

Example: 
```C
#include <stdlib.h>
#include <stdio.h>

#include <LDAbs/ThreadPool.h>

void * Hello(void * args)
{
    printf("Hello from: %d\n",(*(int*)args);
    return NULL;
};

int main()
{
    int Number_of_Threads = 10;
    LD_THREADPOOL * Threadpool = LD_ThreadPool_Create(Number_of_Threads);    

    for(int i = 1; i<=int Number_of_Threads;i++)
    {LD_ThreadPool_enqueue(Threadpool, Hello, &i, sizeof(int));}

    LD_ThreadPool_Join(Threadpool);

    return EXIT_SUCCESS;
}
```
