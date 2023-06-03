#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define INITIAL_CAPACITY 20

typedef struct {
    void** items;
    int capacity;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t notEmpty;
} Queue;

// Function to initialize the queue
Queue* initQueue()
{
    Queue* queue = malloc(sizeof(Queue));
    if(queue == NULL)
    {
        printf("Unable to malloc\n");
        return NULL;
    }
    queue->front = -1;
    queue->rear = -1;
    queue->capacity = INITIAL_CAPACITY;
    queue->items = malloc(INITIAL_CAPACITY*sizeof(void*)); //init size is 20 items
    if(queue->items == NULL)
    {
        printf("Unable to malloc\n");
        return NULL;
    }
    //set up mutex and cond
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->notEmpty, NULL);
    return queue;
}

// Function to check if the queue is empty
int isEmpty(Queue *queue) {
    return (queue->front == -1 && queue->rear == -1);
}

// Function to check if the queue is full
int isFull(Queue* queue) {
    return (queue->rear == queue->capacity -1);
}

// Function to add an element to the queue
void enqueue(Queue* queue, void* data) {

    pthread_mutex_lock(&queue->mutex);

    if (isFull(queue)) {
        queue->capacity = queue->capacity * 2;
        void** newItems = realloc(queue->items, queue->capacity * sizeof(void*));
        if (newItems == NULL) {
            printf("Unable to realloc\n");
            pthread_mutex_unlock(&queue->mutex);
            return;
        }
        queue->items = newItems;
    }
    if (isEmpty(queue)) {
        queue->front = 0;
        queue->rear = 0;
    } else {
        queue->rear = queue->rear + 1;
    }
    queue->items[queue->rear] = data;
    pthread_cond_signal(&queue->notEmpty);
    pthread_mutex_unlock(&queue->mutex);
}

// Function to remove an element from the queue
void* dequeue(Queue *queue) {
    pthread_mutex_lock(&queue->mutex);

    while (isEmpty(queue)) {
        // Queue is empty, wait for a signal
        //printf("Cond wait\n");
        pthread_cond_wait(&queue->notEmpty, &queue->mutex);
    }
    void* data;
    if (queue->front == queue->rear) {
        data = queue->items[queue->front];
        queue->front = -1;
        queue->rear = -1;
    } else {
        data = queue->items[queue->front];
        queue->front = queue->front + 1;
    }
    
    pthread_mutex_unlock(&queue->mutex);
    return data;
}

// Function to display the elements in the queue
/*void display(Queue* queue) {
    pthread_mutex_lock(&queue->mutex);
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        pthread_mutex_unlock(&queue->mutex);
        return;
    }
    int i = queue->front;
    while (i != queue->rear) {
        printf("%p ", queue->items[i]);
        i++;
    }
    printf("%p\n", queue->items[i]);
    pthread_mutex_unlock(&queue->mutex);
}*/

// Function to free the memory used by the queue
void freeQueue(Queue* queue) {
    //pthread_mutex_lock(&queue->mutex);
    free(queue->items);
    queue->items = NULL;
    queue->capacity = 0;
    queue->front = -1;
    queue->rear = -1;
    //pthread_mutex_unlock(&queue->mutex);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->notEmpty);

    free(queue);
}

/*int main() {
    Queue queue;
    initQueue(&queue);

    int num1 = 10;
    int num2 = 20;
    char ch = 'A';
    char* str = "Hello, World!";

    enqueue(&queue, &num1);
    enqueue(&queue, &num2);
    enqueue(&queue, &ch);
    enqueue(&queue, &str);

    printf("Queue elements: ");
    display(&queue);

    int* dequeuedInt = (int*)dequeue(&queue);
    if (dequeuedInt != NULL) {
        printf("Dequeued integer: %d\n", *dequeuedInt);
    }

    dequeuedInt = (int*)dequeue(&queue);
    if (dequeuedInt != NULL) {
        printf("Dequeued integer: %d\n", *dequeuedInt);
    }

    char* dequeuedChar = (char*)dequeue(&queue);
    if (dequeuedChar != NULL) {
        printf("Dequeued character: %c\n", *dequeuedChar);
    }

    char** dequeuedString = (char**)dequeue(&queue);
    if (dequeuedString != NULL) {
        printf("Dequeued string: %s\n", *dequeuedString);
    }

    printf("Queue elements after dequeue: ");
    display(&queue);

    freeQueue(&queue);

    return 0;
}*/