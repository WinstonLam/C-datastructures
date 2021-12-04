#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

static struct heap *heap_init(int (*compare)(const void *, const void *)) {
    struct heap *h = malloc(sizeof(struct heap));
    if (h == NULL) return NULL;
    
    struct array* array = array_init(10);
    if (array == NULL) return NULL;
    h->array = array;
    /* Store the function pointer compare argument in the struct here.
       If the struct pointer is called 'h' the assignment is:
       h->compare = compare; */
    h->compare = compare;
    return h;
}

prioq *prioq_init(int (*compare)(const void *, const void *)) {
    return heap_init(compare);
}

long int prioq_size(prioq *q) {
    if (q == NULL) return 0;
    return array_size(q->array);
}

static int heap_cleanup(struct heap *h, void free_func(void *)) {
    if (h == NULL) return 0;
    
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
    
    long size = array_size(h->array) - 1;
    long parent = (size - 1) / 2;
    
    if (size == 0) return 0;
    
    // If node has a parent, check if it needs to be percolated up.
    while (parent >= 0){
       
        // If node is greater than the parent.
        if (h->compare(p, array_get(h->array, parent)) > 0){
     
            // Copy parent to end.
            if (array_set(h->array, size, array_get(h->array, parent)) == -1) return -1;
          
            // Set new child at the index of parent.
            if(array_set(h->array, parent, p) == -1) return -1;
    

        }
        // If node is lesser than the parent. Then simply return.
        if (h->compare(p, array_get(h->array, parent)) <= 0){
            return 0;
        }
        // Move parent to upper parent to go up the heap.
        parent = parent;
    }
    return 0;
}

int prioq_insert(prioq *q, void *p) {
    return heap_insert(q, p);

}

static void *heap_pop(struct heap *h) {
   
    if (h == NULL) return NULL;
    if (array_size(h->array) == 0){
        return NULL;
    }
    // If heap only has one element.
    if (array_size(h->array) == 1){
        // Store the element in array for returning.
        void *elem = array_get(h->array,0);
        // Pop the only index.
        array_pop(h->array);
        return elem;
    }

    // Pop last element in array and replace it with the front.
    array_set(h->array, 0, array_pop(h->array));

    long curr_index = 1;
    long l_index = 2;
    long r_index = 3;
    
    for(long i = 0; i < array_size(h->array); i++){

        // Store the three nodes for percolating down.
        void *current = array_get(h->array, curr_index);
        void *left = array_get(h->array, l_index);
        void *right = array_get(h->array, r_index);

        // If node is at final spot, not smaller than left child and not larger than right child.
        if (h->compare(current, left) > 0 
        && h->compare(current, right) < 0){
            break;
        }

        // Check if node is smaller than left child.
        if (h->compare(current, left) < 0 ){
            // Set given node on position of its child.
            if (array_set(h->array, l_index, current) == -1) return NULL;
            // Set child on postion of given node.
            if (array_set(h->array, curr_index, left) == -1) return NULL;

            // Update indices to traverse the heap.
            curr_index = l_index;
            l_index = curr_index * 2;
            r_index = l_index + 1;
        }

        // Check if node is larger than right child.
        if (h->compare(current, right) > 0 ){
            // Set given node on position of its child.
            if (array_set(h->array, r_index, current) == -1) return NULL;
            // Set child on postion of given node.
            if (array_set(h->array, curr_index, right) == -1) return NULL;

            // Update indices to traverse the heap.
            curr_index = r_index;
            l_index = curr_index * 2;
            r_index = l_index + 1;
        }

    }
    return array_get(h->array, 0);
}

void *prioq_pop(prioq *q) {
    return heap_pop(q);
}


/* 
  int int_compare(const void *a, const void *b) {
    int x = *((const int *) a);
    int y = *((const int *) b);

    return x - y;
}
void main(){

    prioq *p = prioq_init(int_compare);

    int values[3] = { 2, 5, 2};

    for (int i = 0; i < 3; i++){
        prioq_insert(p, values + i);
    }
    for (int i = 0; i < 3; i++){
        printf("value: %ld\n",*((int *)array_get(p->array, i)));
    } 


}

 */
