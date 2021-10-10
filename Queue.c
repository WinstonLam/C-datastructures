#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Queue{
    int rear, front, size;
    unsigned capacity;
    int* array;
};

// create pointer to struct Queue
struct Queue* createQueue(unsigned capacity){
    // locate memory for the pointer
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    // set capacity, front, size and rear
    queue->capacity= capacity;
    queue->front = queue->size = 0;
    queue->rear= capacity -1;
    queue->array = (int*)malloc(queue->capacity * (sizeof(int)));

    return queue;

};

int isEmpty(struct Queue* queue){
    return (queue->size ==0);
}
int isFull(struct Queue* queue){
    return (queue->size == queue->capacity);
}
void Enqueue(struct Queue* queue, int x){
    //check if queue is already full
    if (isFull(queue))
        return;
    
    queue->rear = (queue->rear +1)%queue->capacity;
    queue->array[queue->rear] = x;
    queue->size += 1;
}
int Dequeue(struct Queue* queue){
    if (isEmpty(queue)){
        return;}
    int item = queue->array[queue->front];    
    queue->front =(queue->front +1)%queue->capacity;
    queue->size -=1;
}

int front(struct Queue* queue){
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
int rear(struct Queue* queue){
    if (isEmpty(queue))
        return  INT_MIN;
    return queue->array[queue->rear];
}
void print(struct Queue* queue){
    if(isEmpty(queue))
        return printf("queue is empty");
    
    printf("queue is: ");
    for(int i=queue->front;i<=queue->size;i++){
        printf("%d, ",queue->array[i]);
    }
    printf("\n");
}

int main(){
    struct Queue* queue = createQueue(10);
    isEmpty(queue);
    Enqueue(queue, 5);
    Enqueue(queue, 4);
    Enqueue(queue, 3);
    Enqueue(queue, 2);
    Enqueue(queue, 1);
    printf("front of the queue is now %d\n",front(queue));
    printf("rear of the queue is now %d\n", rear(queue));
    print(queue);
    Dequeue(queue);
    Dequeue(queue);
    Enqueue(queue, 7);
    printf("front of the queue is now %d\n",front(queue));
    printf("rear of the queue is now %d\n", rear(queue));
    print(queue);
}