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
    struct list* d_list = malloc(sizeof(struct list));
    if (d_list == NULL) return NULL;
    d_list->length = 0;
    d_list->head = NULL;
    d_list->tail = NULL;
    return d_list;
}

struct node *list_new_node(int num) {
    // Make node in the memory.
    struct node* n = malloc(sizeof(struct node));
    if (n == NULL) return NULL;
    // Assign data to the node.
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
    if (n->next == NULL) return NULL;
    return n->next;
}

int list_add_front(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return 1;
    if (l->head == NULL){
        l->head = n;
        l->length += 1;
        return 0;
    }
    struct node *temp = l->head;
    temp->prev = n;
    n->next = temp;
    l->head = n;
    l->length += 1;
    return 0;
}

struct node *list_tail(struct list *l) {
    if (l == NULL) return NULL;
    struct node *n = l->head;
    if (n == NULL) return NULL;
    if (n->next == NULL) return n;
    while (n->next != NULL) n = n->next; 
    return n;
}

struct node *list_prev(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return NULL;
    struct node *temp = l->head;
    if (temp == NULL) return NULL;
    if (n == temp) return NULL;
    if (list_node_present(l,n) == 0) return NULL;
    return n->prev;
}

int list_add_back(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return 1;
    struct node *temp = list_tail(l);
    if (temp == NULL){
        l->head = n;
    }
    else{
        temp->next = n; 
        n->prev = temp; 
    }
    n->next = NULL;
    l->tail = n;
    l->length += 1;
    return 0;
}

int list_node_get_value(struct node *n) {
    if (n == NULL) return 0; // wat moet ik hier returnen?
    return n->data;
}

int list_node_set_value(struct node *n, int value) {
    if (n == NULL) return 1;
    n->data = value;
    return 0;
}

int list_unlink_node(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return 1;
    if (l->head == NULL) return 1;
    struct node* next = n->next;
    struct node* prev = n->prev;
    if (n == l->head){
        if (next == NULL){
            l->head = NULL;
            l->length -= 1;
            return 0;
        }
        l->head = next;
        n->next = NULL;
        next->prev = NULL;
        l->length -= 1;
        return 0;
    }
    if (n == l->tail){
        l->tail = prev;
        n->prev = NULL;
        prev->next = NULL;
        l->length -= 1;
        return 0;
    } 
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
    while (n != NULL){
        temp = n;
        n = n->next;
        free(temp);
    }  
    free(l);
    return 0;
}

int list_node_present(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) return -1;
    struct node* temp = l->head;  
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
     if (list_node_present(l,m) == 0) return 1;
     if (list_node_present(l,n) == 1) return 1;
     m->next = n;
     n->prev = m;
     if (m == l->tail) l->tail = n; 
     l->length += 1;
     return 0;
}

int list_insert_before(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL) return 1;
    if (list_node_present(l,m) == 0) return 1;
    if (list_node_present(l,n) == 1) return 1;
    m->prev = n;
    n->next = m;
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
    struct list* new_list = list_init();
    if (new_list == NULL) return NULL;

    l->tail = n;
    new_list->head = temp;
    new_list->tail = list_tail(new_list);

    n->next = NULL;
    temp->prev = NULL;
    return new_list;
}

/* void main(){
    struct list* test = list_init();
    struct node* n = list_new_node(29);
    struct node* n1 = list_new_node(1);
    struct node* n2 = list_new_node(7);
    struct node* n3 = list_new_node(6);
    struct node* n4 = list_new_node(5);
    list_add_front(test,n);
    list_add_front(test,n1);
   
    list_add_back(test,n4);
    printf("%d",test->length);
} */