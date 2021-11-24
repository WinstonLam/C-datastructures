#include <stdlib.h>

#include "array.h"

struct array {
    int *arr;
    unsigned long size;
    unsigned long used;
};

struct array *array_init(unsigned long initial_capacity) {
    
    struct array *a = malloc(sizeof(struct array));
    if (a == NULL) return NULL;
    
    a->arr = malloc(initial_capacity * sizeof(int));
    if (a->arr == NULL) return NULL;

    a->size = initial_capacity;
    a->used = 0;

    return a;
}

void array_cleanup(struct array *a) {
    if (a == NULL) return;
    free(a);
    return;

}

int array_get(struct array *a, unsigned long index) {
    if (a == NULL) return -1;

    if (index >= a->size) return -1;

    return a->arr[index];
}

/* Note: Although this operation might require the array to be resized and
 * copied, in order to make room for the added element, it is possible to do
 * this in such a way that the amortized complexity is still O(1).
 * Make sure your code is implemented in such a way to guarantee this. */
int array_append(struct array *a, int elem) {
    if (a == NULL) return 0;

    unsigned long index = a->used;
    // Is dit de jusite wijze om de realloc te gebruiken.
    if (index >= a->size)  a->arr = realloc(a->arr, (a->size * 2) * sizeof(int));

    a->arr[index++] = elem;
    a->used++;
    return a->arr[index];
}

unsigned long array_size(struct array *a) {
    if (a == NULL) return 0;
    return a->size;
}
