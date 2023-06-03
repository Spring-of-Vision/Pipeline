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
    int seed;

    //init seed
    if(argc == 3)
    {
        seed = atoi(argv[2]);
    }
    else
    {
        seed = time(NULL);
    }

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

    //main thread waits for all threads
    pthread_join(ao4->thread, NULL);
    ao4->running = 0;
    //printf("thread 4 is done\n");
    pthread_join(ao1->thread, NULL);
    ao1->running = 0;
    //printf("thread 1 is done\n");
    pthread_join(ao2->thread, NULL);
    ao2->running = 0;
    //printf("thread 2 is done\n");
    pthread_join(ao3->thread, NULL);
    ao3->running = 0;
    //printf("thread 3 is done\n");

    //free the queue and ao "objects"
    freeQueue(ao1->queue);
    free(ao1);
    
    freeQueue(ao2->queue);
    free(ao2);

    freeQueue(ao3->queue);
    free(ao3);

    freeQueue(ao4->queue);    
    free(ao4);

    return 0;
}

//Wraps around the call to each AO's individual function, passing the next item to it
void busyWait(ActiveObject* this)
{
    //loop until dequeuing a NULL
    void* task;
    while(task = dequeue(this->queue))
    {
        //if there is a next object, pass along its queue
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
    /*else
    {
        printf("Did not enqueue\n");
    }
    printf("done with n numbers\n");*/

    pthread_cond_signal(&(this->queue->notEmpty));
}

//Create the active object and start its thread using 'bustywait'
ActiveObject* createActiveObject(void (*myfunct)())
{
    ActiveObject* this = malloc(sizeof(ActiveObject));
    if(!this)
    {
        printf("Unable to malloc\n");
        return NULL;
    }
    this->running = 0;
    this->next = NULL;
    
    if((this->queue = initQueue()) == NULL)
    {
        free(this);
        return NULL; //queue malloc did not work
    }
    this->func = myfunct;

    pthread_create(&(this->thread), NULL ,(void* (*)(void*))&busyWait, this);
    this->running = 1;
    return this;
}

//return the queue of the active object
Queue* getQueue(ActiveObject* this)
{
    return this->queue;
}

//stop running threads and free memory
void stop(ActiveObject* this)
{
    if(this->running)
    {
        //printf("cacelling thread %ld\n", this->thread);
        pthread_cancel(this->thread);
    }
    /*else
        printf("thread did not need cancelling\n");*/
    
    pthread_join(this->thread, NULL);
    
    //free queue and object
    freeQueue(this->queue);
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