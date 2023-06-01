#include "st_pipeline.h"

#define INITIAL_CAPACITY 10

// Function to initialize the queue
void initQueue(Queue* queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->notEmpty, NULL);
    //return queue;
}

// Function to add an element to the queue
void enqueue(Queue* queue, void* element) {
    printf("entered enqueue\n");
    pthread_mutex_lock(&queue->mutex);
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        // Failed to allocate memory, handle the error
        return;
    }
    newNode->data = element;
    newNode->next = NULL;

    if (queue->front == NULL) {
        // Queue is empty, update both front and rear
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        // Add the new node at the rear
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    pthread_mutex_unlock(&queue->mutex);
    pthread_cond_signal(&queue->notEmpty);
}

// Function to remove an element from the queue
void* dequeue(Queue *queue) {
    //printf("entered dequeue\n");
    
    pthread_mutex_lock(&queue->mutex);
    

    while (queue->front == NULL) {
        // Queue is empty, wait for a signal
        printf("Cond wait\n");
        pthread_cond_wait(&queue->notEmpty, &queue->mutex);
    }

    // Remove the node at the front
    Node* nodeToRemove = queue->front;
    void* element = nodeToRemove->data;
    queue->front = nodeToRemove->next;

    if (queue->front == NULL) {
        // Queue is now empty, update the rear
        queue->rear = NULL;
    }
    
    free(nodeToRemove);
    pthread_mutex_unlock(&queue->mutex);
    return element;
}

// Function to free the memory used by the queue
void freeQueue(Queue* queue) {
    pthread_mutex_lock(&queue->mutex);

    Node* current = queue->front;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }

    queue->front = NULL;
    queue->rear = NULL;

    pthread_mutex_unlock(&queue->mutex);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->notEmpty);
}

// Function to display the elements in the queue
void display(Queue* queue) {
    pthread_mutex_lock(&queue->mutex);

    Node* current = queue->front;
    while (current != NULL) {
        int* value = (int*)current->data;
        printf("%d\n", *value);
        current = current->next;
    }

    pthread_mutex_unlock(&queue->mutex);
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