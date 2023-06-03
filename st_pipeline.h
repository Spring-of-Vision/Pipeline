#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include "math.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifndef PIPE
#define PIPE

typedef struct {
    void** items;
    int capacity;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t notEmpty;
} Queue;

typedef struct ActiveObject{
    pthread_t thread;
    int running;
    void (*func)();
    Queue* queue;
    struct ActiveObject* next;
} ActiveObject;

//Queue operations
Queue* initQueue();
int isEmpty(Queue *queue);
int isFull(Queue* queue);
void enqueue(Queue* queue, void* data);
void* dequeue(Queue *queue);
void display(Queue* queue);
void freeQueue(Queue* queue);

//AO operations
void busyWait(ActiveObject* this);
ActiveObject* createActiveObject(void (*myfunct)());
Queue* getQueue(ActiveObject* this);
void stop(ActiveObject* this);
void sig_handler(int signo);

//Ao functions and helpers
int isPrime(unsigned int num);
void randNum(void* args, Queue* nextQ);
void addEleven(void* args, Queue* nextQ);
void subtractThirteen(void* args, Queue* nextQ);
void addTwo(void* args, Queue* nextQ);

#endif