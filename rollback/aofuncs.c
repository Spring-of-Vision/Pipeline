#include "st_pipeline.h"

ActiveObject* createActiveObject(void* (*myfunct)(ActiveObject*))
{
    //printf("creating AO\n");
    ActiveObject* this = malloc(sizeof(ActiveObject));
    if(!this)
    {
        printf("Unable to malloc\n");
        return NULL;
    }
    this->running = 0;
    this->next = NULL;

    /*printf("before initQ\n");
    if((initQueue(this->queue)))
    {
        free(this);
        return NULL; //did not work
    }*/
    printf("ao Q\n");
    this->queue = (Queue*)malloc(sizeof(Queue));
    initQueue(this->queue);
    printf("ao funct\n");
    this->func = myfunct;

    printf("ao thread\n");
    //pthread_create(&(this->thread), NULL ,(void* (*)(void*))&busyWait, this); 
    pthread_create(&(this->thread), NULL ,(void* (*)(void*))(this->func), this->queue);
    this->running = 1;
    return this;
}

/*void busyWait(ActiveObject* this)
{
    void* task;
    while(task = dequeue(this->queue))
    {
        //printf("next? %d\n", this->next != NULL);
        if(this->next == NULL)
        {
            this->func(task, NULL);    
        }
        else
        {
            this->func(task, getQueue(this->next));
        }
    }
    if(this->next != NULL)
    {
        //send signal to end the process
        enqueue(getQueue(this->next), NULL);
    }
    else
    {
        printf("Did not enqueue\n");
    }
    printf("done with n numbers\n");
}*/

Queue* getQueue(ActiveObject* this)
{
    return this->queue;
}

//stop running threads and free memory
void stop(ActiveObject* this)
{
    //pthread_mutex_lock(&mutexstop);
    /*if(this->running)
    {
        printf("cacelling thread %ld\n", this->thread);
        pthread_cancel(this->thread);
        printf("joining thread %ld\n", this->thread);
        pthread_join(this->thread, NULL);
    }
    else
    {
        printf("thread did not need cancelling\n");
    }*/
    pthread_kill(this->thread, SIGINT);
    printf("freeing queue\n");
    freeQueue(this->queue);
    printf("freeing ao\n");
    free(this);
    //pthread_mutex_unlock(&mutexstop);
}

int isPrime(unsigned int num)
{
    //if num is even, return 0
    if(num%2 == 0 && num !=2)
    {
        return 0;
    }

    //iterate over the odds from 3 to sqrt of num and if a factor is found, return 0
    for(int i = 3; i <= sqrt(num); i+2)
    {
        if(num%i == 0)
        {
            return 0;
        }
    }

    //return a positive, nonzero number, and returns 0 if num was 1 (as it is not prime)
    return num-1;
}