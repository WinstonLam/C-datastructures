#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue{
    size_t capacity, front, rear, used;
    int *arr;
    int push_count, pop_count, max_elem;
};

struct queue *queue_init(size_t capacity){
    // Allocate memory for the queue.
    struct queue *queue = (struct queue *)malloc(sizeof(struct queue));
    if (queue == NULL) return NULL;
    queue->front = 0;
    queue->rear = (size_t)-1;
    queue->push_count = 0;
    queue->pop_count = 0;
    queue->max_elem = 0;
    // Create value for max element and size to be stored
    queue->used = 0;
    queue->capacity = capacity;
      // Allocate memory for the array of the queue.
    queue->arr = (int *)malloc(capacity * sizeof(int));
    if (queue->arr == NULL){
        free(queue);
        return NULL;
    };
    return queue;
}
/* Check if the queue is full, by checking if 
the top index = capacity, return 1 if full 0 if not.*/
static int is_full(const struct queue *q){
    // Check if used amount of queue is same as capacity.
    if ((size_t)q->used == (q->capacity)) return 1;
    return 0;
}

void queue_cleanup(struct queue *q){
    // Check if q is not NULL.
    if (q == NULL) return;
    // Free the array and the queue itself.
    free(q->arr);
    free(q);
    return;
}

void queue_stats(const struct queue *q){
    fprintf(stderr, "stats %d %d %d\n", q->push_count, q->pop_count, q->max_elem);
}

int queue_push(struct queue *q, int e){
    // Check if queue is not NULL.
    if (q == NULL) return 1;
    // Check if queue is already full.
    else if (is_full(q) == 1){
        return 1;
    } 
    // Check if queue is empty, if so set increment front and rear.
    else if ((queue_empty(q)) == 1) q->front = q->rear = 0;
    // Set rear of queue to value of e.
    q->arr[q->rear] = e;
    // Make queue circular by adjusting rear to left over space in the capacity.
    q->rear = ((size_t)q->rear + 1) % q->capacity;
    q->push_count += 1;
    /* Check if current size of q is larger than previously stored size,
    if so incrmement the max_elem */
    if (q->used + 1 > q->used) q->max_elem = (int)q->used + 1;
    q->used += 1;
    return 0;
}

int queue_pop(struct queue *q){
    // Check if queue is not NULL.
    if (q == NULL) return -1;
    // Store front of queue in temp.
    int temp = q->arr[q->front];
    // Increment front based on capacity space of the queue.
    if (queue_empty(q) == 0){
        q->front = ((size_t)q->front + 1) % q->capacity;
        q->used -= 1;
        // Increment pop_count.
        q->pop_count += 1;
        return temp;
    }
    return -1;

}

int queue_peek(const struct queue *q){
    if (queue_empty(q) < 0) return -1;
    return q->arr[q->front];
}

int queue_empty(const struct queue *q){
    if (q == NULL) return -1;
    else if (q->used > 0) return 0;
    return 1;
}

size_t queue_size(const struct queue *q){
    return q->used;
}
