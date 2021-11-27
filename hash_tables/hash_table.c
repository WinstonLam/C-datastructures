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
        free(n);
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
    t->load = 0;
    t->max_load_factor = max_load_factor;
    t->hash_func = hash_func;
    
    t->array = calloc(t->capacity, sizeof(struct node *));
    if (t->array == NULL) return NULL;
    return t;
}

static struct table *resize (struct table *t){
    if (t == NULL) return NULL;

    printf("load: %ld\n", t->load);
    printf("old %ld\n", t->capacity);
    unsigned long new_capacity = t->capacity * 2;
    

    struct table *new_t = table_init(new_capacity, t->max_load_factor, t->hash_func);
    if (new_t == NULL) return NULL;
    printf("new %ld\n", new_capacity);
    printf("first item %s\n", t->array[0]->key);
    long unsigned int i;
    
    for (i = 0; i < t->capacity; i++){
        if (t->array[i] != NULL){
            new_t->array[i] = t->array[i];
        }
    }
    printf("first item %s\n", t->array[0]->key);
    table_cleanup(t);
    return new_t;
}

int table_insert(struct table *t, char *key, int value) {
    printf("Inserting %d\n", value);
    if (t == NULL) return 1;

    // Copy the key given in a string value.
    char* string = malloc((strlen(key) + 1) * sizeof(char));
    if (string == NULL) return 1;
    strcpy(string, key);
    printf("table load factor %f\n",table_load_factor(t));

    // Check if max load factor is reached, if so reallocate the hash table array.  
    if( table_load_factor(t) >= t->max_load_factor ) {
        printf("table load %ld\n",t->load);
        struct table *new_t = resize(t);
        struct table *t = new_t;
        if (t == NULL) {
            free(string);
            return 1;
        }
        printf("test %ld\n", new_t->load);
    }
    
    // Store hashed key in an index that fits in the capacity of the array.
    unsigned long index = t->hash_func((unsigned char *)string) % t->capacity;
    
    // Initialize node which is to be added in the hash table since it is not in the table yet.
    struct node *n = node_init(string);
    if (n == NULL) return 1;

    struct node* temp = t->array[index];
    // If index is already in hash table, append value to existing struct node.
    if (temp != NULL){
        
        // In case of bucket collision add node to next pointer.
        if (strcmp(temp->key, key) != 0){
            while (temp->next != NULL){
                temp = temp->next;              
            }
            temp->next = n;
            array_append(temp->next->value, value);
            t->load++;
            printf("A0: succesfully appended %s to index %s\n",temp->next->key, temp->key);
            return 0;
        } else {
            array_append(temp->value, value);
            t->load++;
            printf("A: succesfully appended to index %s\n",t->array[index]->key);
            return 0;
        }
        
    }

   
    // Add struct node n to the specified index in the hash table.
    t->array[index] = n;
    // Append value to newly added struct node.
    array_append(t->array[index]->value, value);
    t->load++;

    printf("B: succesfully appended new index %s\n",t->array[index]->key);
    return 0; 
}

/* Returns the struct node that matches the given key, if key not
found in the table then return NULL */
static struct node *get_node(struct table *t, char *key){
    if (t == NULL) return NULL;
   

    if (t->array == NULL) return NULL;
    // Loop through the array of t to check each index.
    for (unsigned long int i = 0; i < t->capacity; i++){

        if (t->array[i] == NULL) continue;
        
        struct node *current = t->array[i];
        // Check for if desired key is chained in the linked list.
        while (current != NULL){
            // Compare key of ith node with given key.
            if (strcmp(current->key, key) == 0){
                return current;
            }
            // Update current to next node.
            current = current->next;
        }
    } 
    return NULL;
}

struct array *table_lookup(struct table *t, char *key) {
    if (t == NULL) return NULL;

    struct node* n = get_node(t, key);

    if (n == NULL) return NULL;

    return n->value;
}

double table_load_factor(struct table *t) {
    if (t == NULL) return 0;
    return ((double)t->load / (double)t->capacity);
}

int table_delete(struct table *t, char *key) {
    if (t == NULL) return 1;

    if (t->array == NULL) return 1;

    // Loop through the array of t to check each index.
    for (unsigned long int i = 0; i < t->capacity; i++){

        if (t->array[i] == NULL) return 1;
        
        struct node *current = t->array[i];
        struct node *prev = current;
        // Check for if desired key is chained in the linked list.
        while (current != NULL){

            // Compare key of ith node with given key.
            if (strcmp(current->key, key) == 0){
                break;
            }
            // Update current to next node.
            prev = current;
            current = current->next;
        }
    
        if (current->next == NULL){
            prev->next = NULL;
        } 
        else if (current == prev){
            t->array[i] = current->next;
        }

        free(current->key);
        array_cleanup(current->value);
        free(current);
    } 
}

void table_cleanup(struct table *t) {
    if (t == NULL) return;

    unsigned long i;
    for (i = 0; i < t->capacity; i++){

        struct node *current = t->array[i];
        
        while (current != NULL){
            struct node* temp = current;
            current = current->next;

            array_cleanup(temp->value);
            free(temp->key);
            free(temp);
        }
    }
    
    free(t->array);
    // clean table
    free(t);
    return;
}
 void main(){
 struct table *t;
    t = table_init(2, 0.6, hash_too_simple);
    

    char *a = malloc(sizeof(char) * 4);
    memcpy(a, "abc", sizeof(char) * 4);
    char *b = malloc(sizeof(char) * 4);
    memcpy(b, "ade", sizeof(char) * 4);
    char *c = malloc(sizeof(char) * 4);
    memcpy(c, "afg", sizeof(char) * 4);
    char *d = malloc(sizeof(char) * 4);
    memcpy(d, "ahi", sizeof(char) * 4);

    table_insert(t, a, 3);
    table_insert(t, b, 5);
    table_insert(t, c, 7);
    

    printf("%d\n",array_get(table_lookup(t, a), 0));
    printf("%d\n",array_get(table_lookup(t, b), 0));

} 