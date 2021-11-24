#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hash_table.h"
#include "hash_func.h"

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
    if (n->value == NULL) {
        printf("array init failed\n");
        return NULL;
    }
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
    
    t->array = calloc(t->capacity, sizeof(struct node *));
    if (t->array == NULL) return NULL;
    return t;
}

int table_insert(struct table *t, char *key, int value) {
    printf("Inserting %d\n", value);
    if (t == NULL) return 1;

    // Copy the key given in a string value.
    char* string = malloc((strlen(key) + 1) * sizeof(char));
    if (string == NULL) return 1;
    strcpy(string, key);
    
    // Initialize node which is to be added in the hash table.
    struct node *n = node_init(string);
    if (n == NULL) return 1;

    // Check if max load factor is reached, if so reallocate the hash table array.  
    if( table_load_factor(t) >= t->max_load_factor ) {
        t->array = realloc(t->array, ( t->capacity * 2) * sizeof(struct node*));
        if (t->array == NULL) return 1;

        t->capacity = t->capacity * 2;
    }
    
    // Store hashed key in an index that fits in the capacity of the array.
    unsigned long index = t->hash_func((unsigned char *)string) % t->capacity;
    
    // If index is already in hash table, append value to existing struct node.
    if (t->array[index] != NULL){
        array_append(t->array[index]->value, value);
        t->load++;
        printf("succesfully appended to index %s\n",t->array[index]->key);
        return 0;
    }

    // Add struct node n to the specified index in the hash table.
    t->array[index] = n;
    printf("succesfully appended to index %s\n",t->array[index]->key);
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
    return (double)(t->load / t->capacity);
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
/*
    void main(){
    struct table *t;
    t = table_init(2, 0.6, hash_too_simple);

    char *a = malloc(sizeof(char) * 4);
    memcpy(a, "abc", sizeof(char) * 4);
    char *b = malloc(sizeof(char) * 4);
    memcpy(b, "def", sizeof(char) * 4);

    table_insert(t, a, 3);
    table_insert(t, b, 5);

    array_get(table_lookup(t, a), 0);
    array_get(table_lookup(t, b), 0);

    table_cleanup(t);
    free(a);
    free(b);
}    */