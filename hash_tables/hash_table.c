/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * hash_table.c:
 * DESCRIPION:
 *    In this file all the internal functions of 
 *    the hash table are defined. Function such as
 *    Initialization, resizing the hash table, getting specific nodes,
 *    calculating the load factor, inserting elements into the hash table 
 *    and cleaning the hash table up.
 * USAGE:
 *    This File can be used by including the header file hash_table.h
 *    And then calling the functions by passing the right variables.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

/* This function initializes a node with the given key,
if unsuccesfull it will return NULL */
struct node *node_init(char *key){
    
    // Allocate memory for the struct node.
    struct node *n = malloc(sizeof(struct node));
    if (n == NULL) return NULL;

    // Initialize array of the node with beginning capacity of 1000.
    n->value = array_init(1000); 
    // Check if memory allocation was succesfull.
    if (n->value == NULL) {
        free(n);
        return NULL;
    }

    // Set key value to given key and next pointer to NULL.
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

    // Intialize the table and check for memory allocation.
    struct table *t = malloc(sizeof(struct table));
    if (t == NULL) return NULL;
    
    // Set capacity, max_load_factor and hash_func to given values.
    t->capacity = capacity;
    t->max_load_factor = max_load_factor;
    t->hash_func = hash_func;
    // Initialize load.    
    t->load = 0;

    // Initialize array with calloc and check for memory allocation.
    t->array = calloc(t->capacity, sizeof(struct node *));
    if (t->array == NULL) return NULL;
    return t;
}

/* This function takes care of creating a new array with a new size
and moving the elements in the old array over to this new array. After which
the old array will be cleared. If failed it will return 1.*/
static int resize(struct table *t){
    
    // Table NULL check.
    if (t == NULL) return 1;

    struct node **new_arr = calloc(t->capacity * 2, sizeof(struct node *));
    if (new_arr == NULL) return 1;
    
     // Update capacity to new capacity.
    t->capacity = t->capacity * 2;

    for (unsigned long i = 0; i < t->capacity / 2; i++){
        struct node *n = t->array[i];
        
        // If node n is not NULL add it to new array.
        while (n){
            // Store next pointer.
            struct node *next = n->next;
            assert(array_size(n->value));
            // Get the index with given hash function.
            unsigned long index = (t->hash_func((unsigned char *)n->key) % t->capacity);
            // If there is already a node on the index move this one to the next.
            n->next = new_arr[index];
            // Insert given new found node at the beginning of linked list.
            new_arr[index] = n;
            n = next;
        }
    }
    free(t->array);
    t->array = new_arr;
    return 0;
}

int table_insert(struct table *t, char *key, int value) {
    if (t == NULL) return 1;

    // Copy the key given in a string value.
    char* string = malloc((strlen(key) + 1) * sizeof(char));
    if (string == NULL) return 1;
    strcpy(string, key);

  
    // Check if max load factor is reached, if so reallocate the hash table array.  
    if( table_load_factor(t) >= t->max_load_factor ) {
        // Store new resized array in new_arr
        if (resize(t) == 1){
            free(string);
            return 1;
        }
    }
    // Store hashed key in an index that fits in the capacity of the array.
    unsigned long index = t->hash_func((unsigned char *)string) % t->capacity;
    
    // Initialize node which is to be added in the hash table since it is not in the table yet.
    struct node *n = node_init(string);
    if (n == NULL) return 1;
    struct node* temp = t->array[index];
    
    // If index is already in hash table, append value to existing struct node.
    if (temp){
        
        // Check for every node in the linked list if the key is same as given key.
        while (temp && strcmp(temp->key, key) != 0) {
            temp = temp->next;
        }
        /* If given key has bucket collision but is not 
        in the linked list add it to the front of the list. */
        if (temp == NULL) {

            // Update next pointer to current front of the linked list.
            n->next = t->array[index];
            // Add node to front of linked list.
            t->array[index] = n;
            // Append given value to the node.
            array_append(n->value, value);

            // Update load.
            t->load++;
            return 0;
        }


        /* If key is already present delete the node n
            and only add the given value to the existing key. */
        array_cleanup(n->value);
        free(n->key);
        free(n);

        array_append(temp->value, value);
        t->load++;
        return 0;
        
    }
    // Add struct node n to the specified index in the hash table.
    t->array[index] = n;

    // Append value to newly added struct node.
    array_append(t->array[index]->value, value);
    t->load++;
    return 0; 
}

/* Returns the struct node that matches the given key, if key not
found in the table then return NULL */
static struct node *get_node(struct table *t, char *key){
    // Memory checks for table and array of table.
    if (t == NULL) return NULL;
    if (t->array == NULL) return NULL;
        
        // Get the index of the given key with the has function.
        struct node *current = t->array[t->hash_func((unsigned char *)key) % t->capacity];

        // Check for if desired key is chained in the linked list.
        while (current != NULL){
          
            if (strcmp(current->key, key) == 0){
                return current;
            }
            current = current->next;
        }
    
    return NULL;
}

struct array *table_lookup(struct table *t, char *key) {
    if (t == NULL) return NULL;

    // Use the get_node function to get the specific node with the given key.
    struct node* n = get_node(t, key);

    if (n == NULL) return NULL;
    return n->value;
}

double table_load_factor(struct table *t) {
    if (t == NULL) return 0;
    return ((double)t->load / (double)t->capacity);
}

int table_delete(struct table *t, char *key) {
    // Memory check for table and array of table.
    if (t == NULL) return 1;
    if (t->array == NULL) return 1;

    // Check if node can be found in the table.
    if (get_node(t, key) == NULL) return 1;

    // Get the index of the given key with the hash_func function.
    struct node *current = t->array[t->hash_func((unsigned char *)key) % t->capacity];
    // Create struct to store previous node.
    struct node *prev = current;

    // Check for if desired key is chained in the linked list.
    while (current != NULL){

        // Compare key of ith node with given key.
        if (strcmp(current->key, key) == 0){
                
            // Check if node is at beginning of linked list or has none.
            if (current == prev)  t->array[t->hash_func((unsigned char *)key) % t->capacity] = current->next;
            // Check if node is at the end of the linked list.
            else if (current->next == NULL) prev->next = NULL;
            else {
                prev->next = current->next;
                current->next = NULL;
            }
                
            // Free the values of the node and the node itself.
            free(current->key);
            array_cleanup(current->value);
            free(current);
            return 0;
            }

         // Update current to next node.
        prev = current;
        current = current->next;
        }
    return 1;
}

void table_cleanup(struct table *t) {
    // Memory check for the table.
    if (t == NULL) return;

    // Loop through the table to get each index.
    for (unsigned long i = 0; i < t->capacity; i++){

        // Store current node in current.
        struct node *current = t->array[i];
        
        // Clear the chained nodes if they are present.
        while (current != NULL){
            // Use temp value for cleanup and move current to next node.
            struct node* temp = current;
            current = current->next;

            // Cleanup value array, key and finally the node itself.
            array_cleanup(temp->value);
            free(temp->key);
            free(temp);
        }
    }
    // Cleanup the rest of the table.
    free(t->array);
    free(t);
    return;
}


