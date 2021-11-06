#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue
{
    size_t capacity, front, rear, used;
    int *arr;
    int push_count, pop_count, max_elem;
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
    queue->max_elem = 0;
    queue->used = 0;
    queue->capacity = capacity;
    queue->arr = (int *)malloc(capacity * sizeof(int));
    if (queue->arr == NULL)
    {
        free(queue);
        return NULL;
    }
    return queue;
}
/* Check if the queue is full, by checking if 
the top index = capacity, return 1 if full 0 if not.*/
static int is_full(const struct queue *q)
{
    if ((size_t)q->used == (q->capacity))
    {
        return 1;
    }
    return 0;
}

void queue_cleanup(struct queue *q)
{
    if (q == NULL)
    {
        return;
    }
    free(q->arr);
    free(q);
    return;
}

void queue_stats(const struct queue *q)
{
    fprintf(stderr, "'stats' %d %d %d", q->push_count, q->pop_count, q->max_elem);
}

int queue_push(struct queue *q, int e)
{
    if (q == NULL)
    {
        return 1;
    }
    else if (is_full(q) == 1)
    {
        return 1;
    }
    else if ((queue_empty(q)) == 1)
    {
        q->front = q->rear = 0;
    }
    q->arr[q->rear] = e;
    q->rear = ((size_t)q->rear + 1) % q->capacity;
    q->push_count += 1;
    q->used += 1;
    q->max_elem += 1;
    return 0;
}

int queue_pop(struct queue *q)
{
    if (q == NULL)
    {
        return -1;
    }
    int temp = q->arr[q->front];

    if (queue_empty(q) == 0)
    {
        q->front = ((size_t)q->front + 1) % q->capacity;
        q->used -= 1;
        return temp;
    }
    else
    {
        return -1;
    }
}

int queue_peek(const struct queue *q)
{
    if (queue_empty(q) < 0)
        return -1;
    return q->arr[q->front];
}

int queue_empty(const struct queue *q)
{
    if (q == NULL)
    {
        return -1;
    }
    else if (q->used > 0)
    {
        return 0;
    }
    return 1;
}

size_t queue_size(const struct queue *q)
{
    return q->used;
}
