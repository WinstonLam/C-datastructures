/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * heap.c:
 * DESCRIPION:
 *    In this file all the internal functions of 
 *    the struct heap are defined. Function such as
 *    Initialization, getting certain values, popping values,
 *    cleaning the array and inserting values.
 * USAGE:
 *    This File can be used by including the header file heap.c
 *    And then calling the functions by passing the right variables.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

static struct heap *heap_init(int (*compare)(const void *, const void *)) {
    // Initialize heap and perform malloc check.
    struct heap *h = malloc(sizeof(struct heap));
    if (h == NULL) return NULL;
    
    // Intialize array and perform malloc check.
    struct array* array = array_init(10);
    if (array == NULL){
        free(h);
        return NULL;
    }
    // Set array and the compare function.
    h->array = array;
    h->compare = compare;
    return h;
}

prioq *prioq_init(int (*compare)(const void *, const void *)) {
    return heap_init(compare);
}

long int prioq_size(prioq *q) {
    // Return queue size if queue is not NULL.
    if (q == NULL) return 0;
    return array_size(q->array);
}

static int heap_cleanup(struct heap *h, void free_func(void *)) {
    if (h == NULL) return 0;
    // Cleanup the array using the free func then clean up h.
    array_cleanup(h->array, free_func);
    free(h);
    return 0;
}

int prioq_cleanup(prioq *q, void free_func(void *)) {
    if (q == NULL) return 0;
    heap_cleanup(q, free_func);
    return 0; 
}

static int heap_insert(struct heap *h, void *p) {
    if (h == NULL) return -1;

    // Append new node to array.
    if (array_append(h->array, p) == -1) return -1;
    
    // Store current and parent index for swapping.
    long current = array_size(h->array) - 1;
    long parent = (current - 1) / 2;
    
    // If heap was empty simply return.
    if (current == 0) return 0;

    // If node has a parent, check if it needs to be percolated up.
    while (current >= 0){
       
        // If node is greater than the parent. Then simply return.
        if (h->compare(p, array_get(h->array, parent)) >= 0) {
            return 0;
        }
        // If node is lesser than the parent.
        if (h->compare(p, array_get(h->array, parent)) < 0) {

            // Copy parent to current.
            if (array_set(h->array, current, array_get(h->array, parent)) == -1) return -1;
          
            // Set new node at the index of parent.
            if(array_set(h->array, parent, p) == -1) return -1;

        }
        /* Move current to parent index and
        parent to upper parent to go up the heap. */
        current = parent;
        parent = (parent - 1) / 2;
    }
    return 0;
}

int prioq_insert(prioq *q, void *p) {
    return heap_insert(q, p);

}
/* This function returns the index of the 
smallest child of the given index
on failure it will return -1.*/
static long index_child(struct heap *h, long index) {
    if (h == NULL) return -1;
    // Set left and right child index.
    long l_index = 1;
    long r_index = 2;
    
    // If Index is not at beginning adjust left and right child.
    if (index != 0){
        l_index = index * 2 + 1;
        r_index = l_index + 1;
    }
    // If the index only has one child return.
    if (l_index == array_size(h->array)) return -1;

    // Store left and right child in void pointer for comparison.
    void *left = array_get(h->array, l_index);
    void *right = array_get(h->array, r_index);
    
    // Chech if children exist.
    if (left == NULL && right == NULL) return -1;
    else if (left == NULL) return r_index;
    else if (right == NULL) return l_index;

    // Compare left and right to determine which index is smallest.
    if (h->compare(left, right) <= 0) {
            return l_index;
    } else {
            return r_index;
    }
    return -1;
}

static void *heap_pop(struct heap *h) {
   
    if (h == NULL) return NULL;

    if (array_size(h->array) < 0){
        return NULL;
    }

    // Store the element in array for returning.
    void *elem = array_get(h->array,0);

    // Pop last element in array and replace it with the front.
    array_set(h->array, 0, array_pop(h->array));

    // If heap only has one element.
    if (array_size(h->array) <= 1) {
        return elem;
    }
    // Store current and parent index for swapping.
    long curr_index = 0;
    long child_index = index_child(h, curr_index);

    // Store current and smallest child in void pointer for comparison.
    void *current = array_get(h->array, curr_index);
    void *child = array_get(h->array, child_index);

    // Compare current and child and swap if needed.
    while (h->compare(current, child) > 0) {
        // Set child at current index.
        if (array_set(h->array, curr_index, child) == -1) return NULL;
        // Set current at child index.
        if (array_set(h->array, child_index, current) == -1) return NULL;

        // Update current and child index.
        curr_index = child_index;
        child_index = index_child(h, curr_index);

        // If last child index is reached break out of loop.
        if (child_index < 0) break;

        // Update current and child node pointers.
        current = array_get(h->array, curr_index);
        child = array_get(h->array, child_index);
    }
    return elem;
}


void *prioq_pop(prioq *q) {
    return heap_pop(q);
}


  