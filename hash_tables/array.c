/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * array.c:
 * DESCRIPION:
 *    In this file all the internal functions of 
 *    the struct array are defined. Function such as
 *    Initialization, getting certain values, appending values,
 *    cleaning the array and retrieving it's size.
 * USAGE:
 *    This File can be used by including the header file array.h
 *    And then calling the functions by passing the right variables.
 */
#include <stdlib.h>

#include "array.h"

struct array {
    int *arr;
    unsigned long size;
    unsigned long used;
};

struct array *array_init(unsigned long initial_capacity) {

    // Initialize struct array. And prefrom allocation check.
    struct array *a = malloc(sizeof(struct array));
    if (a == NULL) return NULL;

    // Initialize array for the struct. And prefrom allocation check.
    a->arr = (int*)malloc(initial_capacity * sizeof(int));
    if (a->arr == NULL){
        free(a);
        return NULL;
    } 

    // Set size to 0 and set used to initial_capacity.
    a->size = 0;
    a->used = initial_capacity;
    return a;
}

void array_cleanup(struct array *a) {
    if (a == NULL || a->arr == NULL) return;

    free(a->arr);
    free(a);

    return;
}

int array_get(struct array *a, unsigned long index) {
    if (a == NULL || a->arr == NULL) return -1;

    // Check if index is out of bounds.
    if (index > a->size) return -1;

    return a->arr[index];
}

/* Note: Although this operation might require the array to be resized and
 * copied, in order to make room for the added element, it is possible to do
 * this in such a way that the amortized complexity is still O(1).
 * Make sure your code is implemented in such a way to guarantee this. */
int array_append(struct array *a, int elem) {
    if (a == NULL || a->arr == NULL) return 1;

    /* Check if current size of array is larger than amount allowed to use,
    if so then resize the array. */
    if ((a->size + 1) > a->used){
        a->arr = realloc(a->arr, (a->used * 2) * sizeof(int));
        // Update used variable.
        a->used = a->used * 2;
        if (a->arr == NULL) return 1;
    }  
    // Set the element to the index of the array.
    a->arr[a->size] = elem;
    a->size++;
    return 0;
}

unsigned long array_size(struct array *a) {
    if (a == NULL || a->arr == NULL) return 0;
    return a->size;
}
