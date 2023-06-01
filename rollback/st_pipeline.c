#include "st_pipeline.h"


Queue *queue1;

void* testQ(Queue *q)
{
    //Queue *q = (Queue*)arg;
    printf("Hello from thread\n");
    srand(time(NULL));
    int items = rand() % 5 + 1;
    int y;
    for(int i = 0; i < items;  i++)
    {
        y = rand() % 900000 + 100000;
        printf("y is %d\n", y);
        enqueue(q, (void*)&y);
    }
    display(q);

    void* data = dequeue(q);
    int* x = (int*)data;
    printf("x is %d\n", *x);
}

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

    if(argc == 3)
    {
        seed = atoi(argv[2]);
        printf("seed is %d\n", seed);
    }
    else
    {
        seed = time(NULL);
        printf("seed is %d\n", seed);
    }

    //listener for interrupt signal
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		printf("\ncan't catch SIGINT\n");
	}

    pthread_t th[1];
    queue1 = initQueue();

    printf("Created quque\n");

    for(int i = 0; i < 1; i++)
    {
        if (pthread_create(&th[i], NULL, (void* (*)(void*))&testQ, queue1) != 0) {
            return 1;
        }
    }
    for(int i = 0; i < 1; i++)
    {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        printf("Thread %d has finished execution\n", i);
    }

    freeQueue(queue1);

    return 0;
}

//Signal handler function to close everything upon interrupt signal (ctrl+c)
void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("stopping all active objects\n");
        freeQueue(queue1);

        // Restore default behavior for SIGINT
        signal(SIGINT, SIG_DFL);

        // Re-raise SIGINT to trigger the default behavior
        raise(SIGINT);
    }
}