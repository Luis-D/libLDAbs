#include "ThreadPool.h"

char LD_ThreadPool_IsEmpty(LD_THREADPOOL * ThreadPool)
{return (ThreadPool->Workpile.Length == 0);} 
