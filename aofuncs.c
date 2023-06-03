#include "st_pipeline.h"

//PART A: isPrime function
int isPrime(unsigned int x){
    
    //check 0-3 for the protocol, in our program it will not matter
    if(x == 0 || x == 1)
    {
        return 0;
    }

    if(x == 3 || x == 2)
    {
        return 1;
    }

    //check first if divisible by 2, otherwise, go through all odd numbers between 3 and sqrt x
    if(x%2 == 0)
    {
        return 0; 
    }
    else
    {
        for (unsigned int i = 3; i*i <= x; i+= 2)
        {
            if (x%i == 0)
            {
                return 0;
            }
        }

        //returns a non-zero value
        return x;
    }
}


void randNum(void* args, Queue* nextQ) //args- unsigned int seed, int n
{
    //parse argument
    int *input = (int*)args;
    int n = input[0];
    unsigned int seed = (unsigned int)input[1];

    //Check for possible problems
    if(!nextQ)
    {
        printf("Can't enqueue to null pointer\n");
        return;
    }

    //perform operation as detailed in the assignment
    srand(seed);
    
    //generate n numbers between 100,000 and 1,000,000
    for(int i = 0; i < n; i++)
    {
        int *output = malloc(sizeof(int));
        *output = rand() % 900000 + 100000;
        //insert to next queue
        enqueue(nextQ, output);
        usleep(1000);
    }

}

void addEleven(void* args, Queue* nextQ)
{
    //parse argument
    int *num = (int*)args;

    //Check for possible problems
    if(!nextQ)
    {
        printf("Can't enqueue to null pointer\n");
        return;
    }
    
    //perform operation as detailed in the assignement
    printf("%d\n", *num);
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
}

void subtractThirteen(void* args, Queue* nextQ)
{
    //check for possible problems
    if (nextQ == NULL)
    {
        printf("Can't enqueue to a null pointer\n");
        return;
    }

    //parse argument
    int *num = (int*)args;

    //perform operation as detailed in the assignement
    printf("%d\n", *num);
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
}

void addTwo(void* args, Queue* nextQ)
{
    //parse argument
    int *num = (int*)args;

    //perform operation as detailed in the assignement
    printf("%d\n", *num);
    *num+= 2;
    printf("%d\n", *num);
}