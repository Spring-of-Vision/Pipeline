#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include "math.h"
#include <stdlib.h>
#include <time.h>

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

typedef struct {
    pthread_t thread;
    int running;
    void (*func)();
    Queue* queue;
} ActiveObject;

//Queue operations
int initQueue(Queue *queue);
int isEmpty(Queue *queue);
int isFull(Queue* queue);
void enqueue(Queue* queue, void* data);
void* dequeue(Queue *queue);
void display(Queue* queue);
void freeQueue(Queue* queue);

//AO operations
void busyWait(ActiveObject* this);
int createActiveObject(void (*myfunct)());
Queue* getQueue(ActiveObject* this);
void stop(ActiveObject* this);
void sig_handler(int signo);

//Ao functions and helpers
int isPrime(unsigned int num);

#endif