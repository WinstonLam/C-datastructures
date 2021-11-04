#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue
{
    size_t capacity;
    int *arr;
    int push_count, pop_count, front, rear;
};

struct queue *queue_init(size_t capacity)
{
    struct queue *queue = (struct queue *)malloc(sizeof(struct queue));
    if (queue == NULL)
        return NULL;
    queue->front = 0;
    queue->rear = -1;
    queue->push_count = 0;
    queue->pop_count = 0;
    queue->capacity = capacity;
    queue->arr = (int *)malloc(capacity * sizeof(int));
    if (queue->arr == NULL)
        return NULL;
    return queue;
}

void queue_cleanup(struct queue *q)
{
    free(q);
    free(q->arr);
    return;
}

void queue_stats(const struct queue *q)
{
    /* ... SOME CODE MISSING HERE ... */
}

int queue_push(struct queue *q, int e)
{
    if (is_full(q) == 1)
        return 1;
    q->arr[++q->rear] = e;
    q->push_count += 1;
    return 0;
}

int queue_pop(struct queue *q)
{
    /* ... SOME CODE MISSING HERE ... */
}

int queue_peek(const struct queue *q)
{
    if (queue_empty(q) < 0)
        return -1;
    return q->arr[q->front];
}

int queue_empty(const struct queue *q)
{
    if (q->rear == -1)
        return 1;
    else if (q->rear >= 0)
        return 0;
    return -1;
}
/* Check if the queue is full, by checking if 
the top index = capacity, return 1 if full 0 if not.*/
int is_full(const struct queue *q)
{
    if (q->rear >= 0 && (size_t)q->rear == q->capacity)
        return 0;
    return 1;
}

size_t queue_size(const struct queue *q)
{
    return (size_t)q->rear + 1;
}
