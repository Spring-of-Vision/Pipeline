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