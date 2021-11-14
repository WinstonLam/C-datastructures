#include "list.h"
#include <stdio.h>

/*
 * TODO: A lot of code missing here. You will need to add implementations for
 * all the functions described in list.h here.
 *
 * Start by adding the definitions for the list and node structs. You may
 * implement any of the Linked List versions discussed in the lecture, which
 * have some different trade-offs for the functions you will need to write.
 *
 * Note: The function prototypes in list.h assume the most basic Singly Linked
 * List. If you build some other version, you may not need all of the function
 * arguments for all of the described functions. This will produce a warning,
 * which you can suppress by adding a simple if-statement to check the value
 * of the unused parameter.
 *
 * Also, do not forget to add any required includes at the top of your file.
 */

struct node {
    int data;
    struct node * next;
    struct node * prev;
};

struct list {
    size_t length;
    struct node * head;
    struct node * tail;
};


struct list *list_init(void) {
    /* Allocate memory for the list, check if memory allocation was succesfull
     and initialize values.*/
    struct list* d_list = malloc(sizeof(struct list));
    if (d_list == NULL) return NULL;
    d_list->length = 0;
    d_list->head = NULL;
    d_list->tail = NULL;
    return d_list;
}

struct node *list_new_node(int num) {
    /* Allocate memory for a node, check if memory allocation was succesfull
     and initialize values.*/
    struct node* n = malloc(sizeof(struct node));
    if (n == NULL) return NULL;
    n->data = num;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

struct node *list_head(struct list *l) {
    if (l->length == 0) return NULL;
    return l->head;
}

struct node *list_next(struct node *n) {
    if (n == NULL) return NULL;
    // Check if node is at the end.
    if (n->next == NULL) return NULL;
    return n->next;
}

int list_add_front(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return 1;
    // If list is empty update head with value of n.
    if (l->head == NULL){
        l->head = n;
        l->length += 1;
        return 0;
    }
    // Otherwise update head node and its pointers.
    struct node *temp = l->head;
    // Update pointers of the node.
    temp->prev = n;
    n->next = temp;
    // Update head node.
    l->head = n;
    l->length += 1;
    return 0;
}

struct node *list_tail(struct list *l) {
    if (l == NULL) return NULL;
    struct node *n = l->head;
    // Check if list is empty.
    if (n == NULL) return NULL;
    // Check if n is already the last node.
    if (n->next == NULL) return n;
    // Traverse through the list till n->next is NULL.
    while (n->next != NULL) n = n->next; 
    return n;
}

struct node *list_prev(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return NULL;
    struct node *temp = l->head;
    // Check if list is empty.
    if (temp == NULL) return NULL;
    // Check if node n is already the head node.
    if (n == temp) return NULL;
    // Check if node n is present in the list.
    if (list_node_present(l,n) == 0) return NULL;
    return n->prev;
}

int list_add_back(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return 1;
    // Store the tail of the list in temp value.
    struct node *temp = list_tail(l);
    // Check if n is the first item.
    if (temp == NULL){
        l->head = n;
    }
    // Otherwise update pointers to add n as the next item.
    else{
        temp->next = n; 
        n->prev = temp; 
    }
    // Update pointers to the new tail.
    n->next = NULL;
    l->tail = n;
    l->length += 1;
    return 0;
}

int list_node_get_value(struct node *n) {
    if (n == NULL) return; 
    if (n->data == NULL) return;// wat moet ik hier returnen?
    return n->data;
}

int list_node_set_value(struct node *n, int value) {
    if (n == NULL) return 1;
    n->data = value;
    return 0;
}

int list_unlink_node(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return 1;
    // Check if list is empty.
    if (l->head == NULL) return 1;
    struct node* next = n->next;
    struct node* prev = n->prev;
    // Check if n is the head node.
    if (n == l->head){
        // If n is the only node in the list, adjust the head pointer.
        if (next == NULL){
            l->head = NULL;
            l->length -= 1;
            return 0;
        }
        /* If there are more elements, 
        increment the head pointer to next node and adjust node pointers*/
        l->head = next;
        n->next = NULL;
        next->prev = NULL;
        l->length -= 1;
        return 0;
    }
    // If n is the last node, decrement the tail pointer and adjust node pointers.
    if (n == l->tail){
        l->tail = prev;
        n->prev = NULL;
        prev->next = NULL;
        l->length -= 1;
        return 0;
    } 
    /* If node is neither at front or end, but is somewhere in the list,
    adjust the node pointers and relink it's previous and next node with eachoter.*/
    if (list_node_present(l,n) == 1){
        prev->next = next;
        next->prev = prev;
        n->next = NULL;
        n->prev = NULL;
        l->length -= 1;
        return 0;
    }
    return 1;   
}

void list_free_node(struct node *n) {
    if (n == NULL) return;
    free(n);
}

int list_cleanup(struct list *l) {
    if (l == NULL) return 1;
    struct node* n = l->head;
    struct node* temp = n;
    // Traverse through the list and free all the nodes.
    while (n != NULL){
        temp = n;
        n = n->next;
        free(temp);
    }  
    // Free the list as last.
    free(l);
    return 0;
}

int list_node_present(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return -1;
    struct node* temp = l->head;  
    /* Start from head and traverse through the list,
    until temp value == n node while incrementing temp. 
    If not found reset i to 0 and return. */
    int i = 1;
    while (temp != n){
        temp = temp->next;
        if (temp == NULL){
            i = 0;
            break;
        }
    }
    return i;
}

int list_insert_after(struct list *l, struct node *n, struct node *m) {
     if (l == NULL || n == NULL || m == NULL) return 1;
     // Check if m is in the list and if n is not in the list.
     if (list_node_present(l,m) == 0) return 1;
     if (list_node_present(l,n) == 1) return 1;
     // Update node pointers.
     m->next = n;
     n->prev = m;
     // If m was the tail, increment the tail pointer to n.
     if (m == l->tail) l->tail = n; 
     l->length += 1;
     return 0;
}

int list_insert_before(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL) return 1;
    // Check if m is in the list and if n is not in the list.
    if (list_node_present(l,m) == 0) return 1;
    if (list_node_present(l,n) == 1) return 1;
    // Update node pointers.
    m->prev = n;
    n->next = m;
    // If m was the head, decrement the head pointer to n.
    if (n == l->head) l->head = n;
    l->length += 1;
    return 0;
}

size_t list_length(struct list *l) {
    if (l == NULL) return 0;
    return l->length;
}

struct node *list_get_ith(struct list *l, size_t i) {
    if (l == NULL) return NULL;
    if (l->length < i) return NULL;
    /* Use x as variable to stop at ith spot in the list, by incrementing it.
    Traverse through the list from head node untill x is same as ith position. */
    size_t x = 0;
    struct node* n = l->head;
    while (x != i){
        n = n->next;
        x++;
    }
    return n;
}

struct list *list_cut_after(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return NULL;
    struct node* temp = n->next;
    // Create new list and check if memory allocation was succesfull.
    struct list* new_list = list_init();
    if (new_list == NULL) return NULL;
    // Update tail pointer of first half.
    l->tail = n;
    // Set head and tail pointer for second half.
    new_list->head = temp;
    new_list->tail = list_tail(new_list);
    // Update node pointers
    n->next = NULL;
    temp->prev = NULL;
    return new_list;
}

// Sorts item return 1 if succesfull, 0 if not
struct list* insertion_sort(int arr[], int arr_size){
    // Initialize list and check if succesfull
    struct list *l = list_init();
    if (l == NULL) return NULL;
    struct node *head = l->head;
    // Loop through length of given array. And add node sorted to the list.
    for (int i = 0; i < arr_size; i++){
        // Create a node for ith element in the array.
        struct node *n = list_new_node(arr[i]);

        if (n == NULL) return NULL;
        // Check if list is empty and ith node is the first element.
  
        struct node* temp = l->head;
        while (temp != NULL && n->data > temp->data){
            temp = temp->next;
            printf("%d\n", temp->data);
        }
        if (l->head == NULL) list_add_front(l,n);
        else if (n->data <= temp->data) list_insert_before(l,n,temp);
        else list_insert_after(l,n,temp);
      
        
               

    }
    return l;
}

void main(){
    int arr[] = {5,3,4,1,2};
    int arr_size = sizeof(arr) / sizeof(arr[0]);
 
    struct list* l = insertion_sort(arr, arr_size);
    struct node* n = l->head;
    struct node* m = n->next;
    if (m== NULL) printf("Big rip");
    printf("%d\n",m->data);
    while (n != NULL) {
        printf("%d\n",n->data);
        n = n->next;
    }
}
