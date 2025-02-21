#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    struct data work1, work2, work3;
    work1.a = 5; work1.b = 10;
    work2.a = 7; work2.b = 3;
    work3.a = 20; work3.b = 30;

    pool_init();

    pool_submit(&add,&work1);
    pool_submit(&add,&work2);
    pool_submit(&add,&work3);

    sleep(3);

    pool_shutdown();

    return 0;
}
