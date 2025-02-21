#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

task task_queue[QUEUE_SIZE];
int queue_front = 0;
int queue_rear = 0;
int queue_size = 0;


pthread_t threads[NUMBER_OF_THREADS];
pthread_mutex_t mutex;
sem_t sem_queue;
int is_pool_shutting_down = 0;

int enqueue(task t) 
{
    pthread_mutex_lock(&mutex);
    if(queue_size == QUEUE_SIZE)
    {
        pthread_mutex_unlock(&mutex);
        return 1;
    }
    task_queue[queue_rear] = t;
    queue_rear = (queue_rear + 1) % QUEUE_SIZE;
    queue_size++;
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_queue);
    return 0;
}

task dequeue() 
{
    pthread_mutex_lock(&mutex);
    task t = task_queue[queue_front];
    queue_front = (queue_front + 1) % QUEUE_SIZE;
    queue_size--;
    pthread_mutex_unlock(&mutex);
    return t;
}

void *worker(void *param)
{
    while (TRUE)
    {
        sem_wait(&sem_queue);
        pthread_mutex_lock(&mutex);
        if (is_pool_shutting_down && queue_size == 0)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        task work = dequeue();
        execute(work.function, work.data);
    }
    pthread_exit(0);
}

void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

int pool_submit(void (*somefunction)(void *p), void *p)
{
    task new_task;
    new_task.function = somefunction;
    new_task.data = p;
    return enqueue(new_task);
}

void pool_init(void)
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_queue, 0, 0);
    is_pool_shutting_down = 0;

    for(int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, worker, NULL);
    }
}

void pool_shutdown(void)
{
    pthread_mutex_lock(&mutex);
    is_pool_shutting_down = 1;
    pthread_mutex_unlock(&mutex);

    for(int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        sem_post(&sem_queue); 
    }

    for(int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_queue);
}
