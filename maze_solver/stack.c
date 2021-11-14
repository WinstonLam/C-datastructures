#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct stack{
    size_t capacity;
    int *arr;
    int top, push_count, pop_count, max_elem, used;
};

struct stack *stack_init(size_t capacity){
    struct stack *stack = malloc(sizeof(struct stack));
    // Check if data could be allocated.
    if (stack == NULL) return NULL;
    stack->capacity = capacity;
    stack->top = -1;
    stack->push_count = 0;
    stack->pop_count = 0;
    // Create value for max element and size to be stored
    stack->max_elem = 0;
    stack->used = 0;
    // Check if array could be allocated 
    stack->arr = malloc(stack->capacity * sizeof(int));
    if (stack->arr == NULL){
        free(stack);
        return NULL;
    }
    return stack;
}

/* Check if the stack is full, by checking if 
the top index = capacity, return 1 if full 0 if not.*/
static int is_full(const struct stack *s){
    if (s->top >= 0 && (size_t)s->top == (s->capacity - 1)) return 1;
    return 0;
}

void stack_cleanup(struct stack *s){   
    if (s == NULL) return;
    free(s->arr);
    free(s);
    return;
}

void stack_stats(const struct stack *s){
    fprintf(stderr, "stats %d %d %d\n", s->push_count, s->pop_count, s->max_elem);
}

int stack_push(struct stack *s, int c){
    // Check if stack is not NULL and is not full.
    if (s == NULL) return 1;
    else if (is_full(s) == 1) return 1;
    // Increment top and afterwards add c to the array.
    s->arr[++s->top] = c;
    s->push_count += 1;
    /* Check if current size of s is larger than previously stored size,
    if so incrmement the max_elem */
    if (s->used + 1 > s->used) s->max_elem = s->used + 1;
    s->used += 1;
    return 0;
}

int stack_pop(struct stack *s){
    // Use temp to store top stack value.
    int temp = stack_peek(s);
    // Check if stack is empty.
    if (stack_empty(s) != 0) return -1;
    // Decrement top.
    s->top--;
    s->pop_count += 1;
    s->used -= 1;
    return temp;
}

int stack_peek(const struct stack *s){
    // Check if stack is empty.
    if (stack_empty(s) != 0) return -1;
    return s->arr[s->top];
}

int stack_empty(const struct stack *s){
    // Check if stack can be located.
    if (s == NULL) return -1;
    // Check if stack is empty.
    else if (s->top == -1) return 1;
    // Check if stack is not empty
    else if (s->top >= 0) return 0;
    return -1;
}

size_t stack_size(const struct stack *s){
    return (size_t)s->used;
}
