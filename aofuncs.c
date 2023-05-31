#include "st_pipeline.h"

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

void randNum(void* args, Queue* nextQ)//unsigned int seed, int n)
{
    printf("entered randNum\n");
    //parse argument
    int *input = (int*)args;
    int n = input[0];
    unsigned int seed = (unsigned int)input[1];
    printf("Got input n=%u, seed=%u\n", n, seed);

    //Check for possible problems
    if(!nextQ)
    {
        printf("Can't enqueue to null pointer\n");
    }

    //perform operation as detailed in the assignment
    srand(seed);
    int *output;
    //generate a number between 100,000 and 1,000,000
    for(int i = 0; i < n; i++)
    {
        *output = rand() % 900000 + 100000;
        //insert to next queue
        enqueue(nextQ, (void*)output);
        printf("queued num in randNum %d\n", *output);
        usleep(1000);
    }

    //send signal to end the process
    //enqueue(nextQ, NULL);

}

void addEleven(void* args, Queue* nextQ)//(int num)
{
    printf("entered addEleven\n");
    //parse argument
    int *num = (int*)args;
    printf("parsed input\n");

    //Check for possible problems
    if(!nextQ)
    {
        printf("Can't enqueue to null pointer\n");
    }
    
    //perform operation as detailed in the assignement
    printf("%d is Prime?:", *num);
    if(isPrime(*num))
    {
        printf("True\n");
    }
    else{
        printf("False\n");
    }
    *num += 11;

    //insert to next queue
    enqueue(nextQ, (void*)num);
    printf("queued num in +11 %d\n", *num);
}

void subtractThirteen(void* args, Queue* nextQ)//(int num)
{
    printf("entered subtractThirteen\n");
    //parse argument
    int *num = (int*)args;
    printf("parsed input\n");

    //Check for possible problems
    if(!nextQ)
    {
        printf("Can't enqueue to null pointer\n");
    }

    //perform operation as detailed in the assignement
    printf("%d is Prime?:", *num);
    if(isPrime(*num))
    {
        printf("True\n");
    }
    else{
        printf("False\n");
    }
    *num -= 13;

    //insert to next queue
    enqueue(nextQ, (void*)num);
    printf("queued num in -13 %d\n", *num);
}

void addTwo(void* args, Queue* nextQ)//(int num)
{
    printf("entered addTwo\n");
    //parse argument
    int *num = (int*)args;
    printf("parsed input\n");

    //perform operation as detailed in the assignement
    printf("Num is %d\n", *num);
    *num+= 2;
    printf("Original num: %d\n", *num);
}