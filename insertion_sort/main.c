#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#define BUF_SIZE 1024


char buf[BUF_SIZE];

struct config {
    /* You can ignore these options until you implement the
       extra command-line arguments. */

    /* Set to 1 if -d is specified, 0 otherwise. */
    int descending_order;

    // Set to 1 if -c is specified, 0 otherwise.
    int combine;

    // Set to 1 if -o is specified, 0 otherwise.
    int remove_odd;

    /* Set to 1 if -z is specified, 0 otherwise. */
    int zip_alternating;
};


int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "dcoz")) != -1) {
        switch (c) {
        case 'd':
            cfg->descending_order = 1;
            break;
        case 'c':
            cfg->combine = 1;
            break;
        case 'o':
            cfg->remove_odd = 1;
            break;
        case 'z':
            cfg->zip_alternating = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}

static struct list *sort(struct list *unsorted){
    if (unsorted == NULL) return NULL;
    // Initiate a current node at head of unsorted list.
    struct node *curr = list_head(unsorted);
    // Traverse through the unsorted list and updating the current node.
    while (curr != NULL){
        // Create a temp value which will compare the curr value for all nodes in the list.
        struct node *temp = list_head(unsorted);
        // Save the next node of current so this does not get mixed up when node is unlinked.
        struct node *next = list_next(curr);
        // Start from the head of sorted list.
        while (temp != NULL){
            // If given value in the unsorted list is higher insert after current position.
            if (list_node_get_value(curr) > list_node_get_value(temp)){
                // Unlink the node in current position and move it behind temp position.
                list_unlink_node(unsorted,curr);
                list_insert_after(unsorted, curr, temp);
            }
            // Move temp node to next node to compare the next one.
            temp = list_next(temp);
        }
        // Iterate current to next of current.
        curr = next;
    }
    return unsorted;
}
static struct list *remove_odd(struct list *sorted){
    if (sorted == NULL) return NULL;
    struct node *curr = list_head(sorted);
    while (curr != NULL){
        struct node *next = list_next(curr);
        if ((list_node_get_value(curr) % 2) != 0){
            list_unlink_node(sorted,curr);
            list_free_node(curr);
        }
        curr = next;
    }
    return sorted;
}
 
static struct list *sort_d(struct list *unsorted){
    if (unsorted == NULL) return NULL;
    // Initiate a current node at head of unsorted list.
    struct node *curr = list_head(unsorted);
    // Traverse through the unsorted list and updating the current node.
    while (curr != NULL){
        // Create a temp value which will compare the curr value for all nodes in the list.
        struct node *temp = list_head(unsorted);
        // Save the next node of current so this does not get mixed up when node is unlinked.
        struct node *next = list_next(curr);
        // Start from the head of sorted list.
        while (temp != NULL){
            // If given value in the unsorted list is higher insert after current position.
            if (list_node_get_value(curr) > list_node_get_value(temp)){
                // Unlink the node in current position and move it behind temp position.
                list_unlink_node(unsorted,curr);
                list_insert_after(unsorted, curr, temp);
            }
            // Move temp node to next node to compare the next one.
            temp = list_next(temp);
        }
        // Iterate current to next of current.
        curr = next;
    }
    return unsorted;
}
 

int main(int argc, char *argv[]) {
    struct config cfg;
    if (parse_options(&cfg, argc, argv) != 0) {
        return 1;
    }
    
   
    struct list *l = list_init();
    if (l == NULL) return 0;
    
    while (fgets(buf, BUF_SIZE, stdin)) {
        
        char *token;
        token = strtok(buf, " ");

        while (token) {
            long int num = strtol(token, NULL, 0);
            struct node *n = list_new_node((int)num);
            if (n == NULL) return 0;
            list_add_front(l,n);
            token = strtok(NULL, " ");
        }
    }
 
    sort(l); 
    remove_odd(l);
    struct node *head = list_head(l);
    while (head != NULL){
        printf("%d\n",list_node_get_value(head));
        head = list_next(head);
    }
    list_cleanup(l);
    

    return 0;

}

