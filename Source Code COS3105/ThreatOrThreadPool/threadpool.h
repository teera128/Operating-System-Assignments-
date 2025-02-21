#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <semaphore.h>

typedef struct 
{
    void (*function)(void *p);
    void *data;
} task;

void execute(void (*somefunction)(void *p), void *p);
int pool_submit(void (*somefunction)(void *p), void *p);
void *worker(void *param);
void pool_init(void);
void pool_shutdown(void);

#endif
