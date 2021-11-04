#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct stack
{
    int top;
    size_t capacity;
    int *arr;
    int push_count;
    int pop_count;
};

struct stack *stack_init(size_t capacity)
{
    struct stack *stack = (struct stack *)malloc(sizeof(struct stack));
    if (stack == NULL)
        return NULL;
    stack->arr = (int *)malloc(stack->capacity * sizeof(int));
    if (stack->arr == NULL)
        return NULL;
    stack->capacity = capacity;
    stack->top = -1;
    stack->push_count = 0;
    stack->pop_count = 0;
    return stack;
}

/* Check if the stack is full, by checking if 
the top index = capacity*/
int is_full(struct stack *s)
{
    return (s->top >= 0 && (size_t)s->top == s->capacity);
}

void stack_cleanup(struct stack *s)
{
    free(s);
    free(s->arr);
    return;
}

void stack_stats(const struct stack *s)
{
    printf("stats");
/*     fprintf("%d,", s->push_count);
    fprintf("%d,", s->pop_count); */
}

int stack_push(struct stack *s, int c)
{
    if (is_full(s)) return 1;
    s->arr[++s->top] = c;
    s->push_count += 1;
    return 0;
}

int stack_pop(struct stack *s)
{
    int temp = stack_peek(s);
    if (stack_empty(s) != 0) return -1;
    s->top--;
    s->pop_count += 1;
    return temp;
}

int stack_peek(const struct stack *s)
{
    if ( stack_empty(s) != 0 ) return -1;
    return s->arr[s->top];
}

int stack_empty(const struct stack *s)
{
    if ( s->top == -1)
    {
        return 1;
    }
    else if ( s->top >= 0 )
    {
        return 0;
    }
    return -1;
}

size_t stack_size(const struct stack *s)
{
    return s->top + 1;
}

void main(){
    struct stack *s = stack_init(5);
    stack_push(s, 5);
    stack_pop(s);
    stack_push(s, 5);
    stack_size(s);
 
}