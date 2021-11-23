#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hash_table.h"

struct table {
    /* The (simple) array used to index the table */
    struct node **array;
    /* The function used for computing the hash values in this table */
    unsigned long (*hash_func)(unsigned char *);
    /* Maximum load factor after which the table array should be resized */
    double max_load_factor;
    /* Capacity of the array used to index the table */
    unsigned long capacity;
    /* Current number of elements stored in the table */
    unsigned long load;
};

/* Note: This struct should be a *strong* hint to a specific type of hash table
 * You may implement other options, if you can build them in such a way they
 * pass all tests. However, the other options are generally harder to code. */
struct node {
    /* The string of characters that is the key for this node */
    char *key;
    /* A resizing array, containing the all the integer values for this key */
    struct array *value;
    /* Next pointer */
    struct node *next;
};


struct node *node_init(char *key){
    
    struct node *n = malloc(sizeof(struct node));
    if (n == NULL) return NULL;
    n->value = array_init(1000); 
    n->key = key;
    n->next = NULL;
    return n;
}

struct table *table_init(unsigned long capacity,
                         double max_load_factor,
                         unsigned long (*hash_func)(unsigned char *)) {

    /* Note: The 'array' member of struct table is a pointer to a block of
     * memory that contains pointers to struct nodes. Make sure that the struct
     * node pointers in this block are all set to NULL. A good way to do this is
     * with the calloc() function. Check the manual page 'man calloc' for
     * its usage. */
    struct table *t = malloc(sizeof(struct table));
    if (t == NULL) return NULL;
    
    t->capacity = capacity;
    t->max_load_factor = max_load_factor;
    t->hash_func = hash_func;
    
    t->array = calloc(t->capacity, sizeof(char));
    if (t->array == NULL) return NULL;
    return t;
}

int table_insert(struct table *t, char *key, int value) {
    if (t == NULL) return 1;

    if (t->array[t->hash_func(key)] != NULL) array_append(t->array[t->hash_func(key)]->value, value);
    // hoe moet ik de hash functie gebruiken om de positie op te zoeken.
    t->array[t->hash_func(key)] = key;
    return 0;

}

/* Returns the struct node that matches the given key, if key not
found in the table then return NULL */
static struct node *get_node(struct table *t, char *key){
    if (t == NULL) return NULL;

    struct node* n;
    // Is er een betere manier om de node te achterhalen.
    for (long unsigned int i = 0; i < t->capacity; i++){
        if (t->array[i]->key == key){
            n = t->array[i];
            break;
            }
    } 
    if (n == NULL) return NULL;
    return n;
}

struct array *table_lookup(struct table *t, char *key) {
    if (t == NULL) return NULL;

    struct node* n = get_node(t, key);

    if (n == NULL) return NULL;

    return n->value;
}

double table_load_factor(struct table *t) {
    if (t == NULL) return 0;
    return t->load / t->capacity;
}

int table_delete(struct table *t, char *key) {
    if (t == NULL) return 1;

    struct node *n = get_node(t, key);
    if (n == NULL) return 1;
    // Is dit  de juiste manier om de node te free'en
    free(n);
    return 0;
}

void table_cleanup(struct table *t) {
    if (t == NULL) return;
    free(t->array);
    free(t);
    return;
}
