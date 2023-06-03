#include "st_pipeline.h"

ActiveObject *ao1,*ao2,*ao3,*ao4;

int main(int argc, char* argv[])
{
    if(argc > 3 || argc < 2)
    {
        printf("Must include 1-2 arguments, no more or less.\n");
        return 0;
    }

    int n = atoi(argv[1]);
    printf("n is %d\n", n);
    int seed;

    //printf("created ints\n");

    if(argc == 3)
    {
        seed = atoi(argv[2]);
        printf("seed is %d\n", seed);
    }
    else
    {
        seed = time(NULL);
    }

    //printf("initialized seed\n");

    //create active objects
    ao1 = createActiveObject((void (*)(void*, Queue*))&randNum);
    //printf("initialized ao1\n");
    ao2 = createActiveObject((void (*)(void*, Queue*))&addEleven);
    ao1->next = ao2;
    //printf("initialized ao2\n");
    ao3 = createActiveObject((void (*)(void*, Queue*))&subtractThirteen);
    ao2->next = ao3;
    //printf("initialized ao3\n");
    ao4 = createActiveObject((void (*)(void*, Queue*))&addTwo);
    ao3->next = ao4;
    //printf("initialized ao4\n");

    if(!ao1 || !ao2 || !ao3 || !ao4)
    {
        printf("not enough activeObjects\n");
        return 0;
    }

    //listener for interrupt signal
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		printf("\ncan't catch SIGINT\n");
	}

    //start the pipeline with the initial enqueue
    int init[2] = {n, seed};
    enqueue(ao1->queue, (void*)init);
    enqueue(ao1->queue, NULL);

    pthread_join(ao4->thread, NULL);
    ao4->running = 0;
    printf("thread 4 is done\n");
    //join threads
    pthread_join(ao1->thread, NULL);
    ao1->running = 0;
    printf("thread 1 is done\n");
    pthread_join(ao2->thread, NULL);
    ao2->running = 0;
    printf("thread 2 is done\n");
    pthread_join(ao3->thread, NULL);
    ao3->running = 0;
    printf("thread 3 is done\n");

    //free
    printf("freeing queue a1\n");
    freeQueue(ao1->queue);
    printf("freeing ao\n");
    free(ao1);
    printf("freeing queue a1\n");
    freeQueue(ao2->queue);
    printf("freeing ao\n");
    free(ao2);
    printf("freeing queue a1\n");
    freeQueue(ao3->queue);
    printf("freeing ao\n");
    free(ao3);
    printf("freeing queue a1\n");
    freeQueue(ao4->queue);
    printf("freeing ao\n");
    free(ao4);

    return 0;
}

void busyWait(ActiveObject* this)
{
    void* task;
    while(task = dequeue(this->queue))
    {
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
    pthread_cond_signal(&(this->queue->notEmpty));

}

ActiveObject* createActiveObject(void (*myfunct)())
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
    //printf("before initQ\n");
    if((this->queue = initQueue()) == NULL)
    {
        free(this);
        return NULL; //did not work
    }
    this->func = myfunct;

    pthread_create(&(this->thread), NULL ,(void* (*)(void*))&busyWait, this);
    this->running = 1;
    return this;
}

Queue* getQueue(ActiveObject* this)
{
    return this->queue;
}

//stop running threads and free memory
void stop(ActiveObject* this)
{
    if(this->running)
    {
        printf("cacelling thread %ld\n", this->thread);
        pthread_cancel(this->thread);
    }
    else
        printf("thread did not need cancelling\n");
    printf("joining thread %ld\n", this->thread);
    pthread_join(this->thread, NULL);
    printf("freeing queue\n");
    freeQueue(this->queue);
    printf("freeing ao\n");
    free(this);
}

//Signal handler function to close everything upon interrupt signal (ctrl+c)
void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("stopping all active objects\n");
        //cleanup
        stop(ao1);
        printf("stopped a1\n");
        stop(ao2);
        printf("stopped a2\n");
        stop(ao3);
        printf("stopped a3\n");
        stop(ao4);
        printf("stopped a4\n");

        // Restore default behavior for SIGINT
        signal(SIGINT, SIG_DFL);

        // Re-raise SIGINT to trigger the default behavior
        raise(SIGINT);
    }  

}