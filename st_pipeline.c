#include "st_pipeline.h"

int main(int argc, int* argv)
{
    if(argc > 3 || argc < 2)
    {
        printf("Must include 1-2 arguments, no more or less.\n");
        return 0;
    }

    int n = argv[1];
    unsigned int seed;
    if(argc == 3)
    {
        seed = argv[2];
    }
    else
    {
        seed = (unsigned int)time(NULL);
    }
    return 0;
}

void busyWait(ActiveObject* this)
{
    void* task;
    
    while(task = dequeue(this->queue))
        this->func(task);
}

int createActiveObject(void (*myfunct)())
{
    ActiveObject* this;
    this->running = 0;
    if(initQueue(this->queue) == 0)
    {
        return 0; //did not work
    }
    this->func = myfunct;

    pthread_create(&(this->thread), NULL ,(void* (*)(void*))&busyWait, this);
    this->running = 1;
    return 1;
}

Queue* getQueue(ActiveObject* this)
{
    return this->queue;
}

void stop(ActiveObject* this)
{
    pthread_cancel(this->thread);
    pthread_join(this->thread, NULL);
    freeQueue(this->queue);
}

//Signal handler function to close everything upon interrupt signal (ctrl+c)
void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        //stop();
    }  

}