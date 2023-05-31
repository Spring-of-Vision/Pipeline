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

void randNum(void* args)//unsigned int seed, int n)
{
    int nums[n];

    srand(seed);
    //generate a number between 100,000 and 1,000,000
    for(int i = 0; i < n; i++)
    {
        nums[i] = rand() % 900000 + 100000;
    }

    //save nums in the queue?

    sleep(1);

    //pass to the next ao
}

void addEleven(void* args)//(int num)
{
    printf("%d is Prime?:", num);
    if(isPrime(num))
    {
        printf("True\n");
    }
    else{
        printf("False\n");
    }

    num += 11;

    //pass to next ao
}

void subtractThirteen(void* args)//(int num)
{
    printf("%d is Prime?:", num);
    if(isPrime(num))
    {
        printf("True\n");
    }
    else{
        printf("False\n");
    }

    num -= 13;

    //pass to next ao
}

void addTwo(void* args)//(int num)
{
    printf("Num is %d", num);
    num+= 2;
    printf("Original num: %d", num);
}