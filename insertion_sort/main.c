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

        // Temp will be used to traverse through the sorted list, initially it will be the same as curr.
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

    // Start from head of list and traverse through the list.
    struct node *curr = list_head(sorted);

    /* Check for each node if is odd, 
    if so unlink and free the node then move on to the next node. */
    while (curr != NULL){

        // Store next node, so this does not get lost when freeing odd nodes.
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
            // If given value in the unsorted list is lower insert before current position.
            if (list_node_get_value(curr) < list_node_get_value(temp)){
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

static struct list *combine(struct list *l){
    if (l == NULL) return NULL;
    
    // Initialize curr node as head and next node as the one after curr.
    struct node* curr = list_head(l);
    struct node* next = list_next(list_head(l));

    // Traverse through the list untill next node is NULL;
    while (next != NULL){

        // Store value of sum of curr and next in x;
        int x = list_node_get_value(curr) + list_node_get_value(next);

        // Insert the new node with value x before curr node.
        list_insert_before(l, list_new_node(x), curr);

        // Create pointers to curr and next node, to free them.
        struct node *free_curr = curr; 
        struct node *free_next = next; 

        // Update curr and next for next iteration.
        curr = list_next(next);
        next = list_next(curr);

        // Unlink the old curr and next node, afterwrds free them.
        list_unlink_node(l,free_curr);
        list_unlink_node(l,free_next);
        list_free_node(free_curr);
        list_free_node(free_next); 
    }
    return l;
}

static struct list *zip_alt(struct list* l){
    // Create new list for 2nd half of list l.
    struct list *newlist = l;
    if (newlist == NULL) return NULL;

    /* Store length to run different zipping for odd length list
    and non-odd length list. */
    size_t length = list_length(l);

    // If list has even length split list evenly, and store second half in newlist.
    if (length % 2 != 0 ){
    newlist = list_cut_after(l, list_get_ith(l, (list_length(l) / 2)));
    }
    
    // If list has odd length split so that the second half is shorter, and store second half in newlist.
    else if (length % 2 == 0) { newlist = list_cut_after(l, list_get_ith(l, (list_length(l) / 2) - 1)); } 
    
    // Create zipped list in which list 1 and 2 will be stored.
    struct list *zipped = list_init();
    if (zipped == NULL) return NULL;

    // Initiate temp values to traverse through both lists.
    struct node *temp_1 = list_head(l);
    if (temp_1 == NULL) return NULL;
    struct node *temp_2 = list_head(newlist);
    if (temp_2 == NULL) return NULL;

    // If has even length add temp_1 and temp_2 after each other, and stop if at end of list.
    if ( length % 2 == 0){
        while (temp_1 != NULL){
            list_add_back(zipped, list_new_node(list_node_get_value(temp_1)));
            list_add_back(zipped, list_new_node(list_node_get_value(temp_2)));
       
            temp_1 = list_next(temp_1);
            temp_2 = list_next(temp_2);
        }
    }
    /* If has odd length add temp_1 and temp_2 only if temp_1 is not at the end. 
    Since the second half is shorter than the first.*/
    else if ( length % 2 != 0){
        while (temp_1 != NULL){
            list_add_back(zipped, list_new_node(list_node_get_value(temp_1)));
            if (list_next(temp_1) != NULL) list_add_back(zipped, list_new_node(list_node_get_value(temp_2)));
       
            temp_1 = list_next(temp_1);
            temp_2 = list_next(temp_2);
        }
    }
    
    // Cleanup both halfs of the list and return zipped list.
    list_cleanup(newlist);
    list_cleanup(l);

    return zipped;
} 


int main(int argc, char *argv[]) {
    struct config cfg;
    if (parse_options(&cfg, argc, argv) != 0) {
        return 1;
    }

    // Initialize list.
    struct list *l = list_init();
    if (l == NULL) return 0;
    
    while (fgets(buf, BUF_SIZE, stdin)) {

        // Strip buf on newlines and whitespaces and store in token.
        char *token;
        token = strtok(buf, "\n");
        token = strtok(token, " "); 

        // Iterate through filtered buf.
        while (token) {
            // Transform ascii to long int using strtol.
            long int num = strtol(token, NULL, 0);
            // Create new node with value of num.
            struct node *n = list_new_node((int)num);
            if (n == NULL) return 0;
            // Add new node to the list.
            list_add_front(l,n);
            token = strtok(NULL, " ");
        }
    }

    // Perform basic insertion sort on list.
    l = sort(l); 

    // Perform other function based on which flag is turned on.
    if (cfg.combine != 0) l = combine(l);
    if (cfg.descending_order != 0) l = sort_d(l);
    if (cfg.remove_odd != 0) l = remove_odd(l);
    if (cfg.zip_alternating != 0) l = zip_alt(l);

    // Print out the list.
    struct node *head = list_head(l);

    while (head != NULL){
        printf("%d\n",list_node_get_value(head));
        head = list_next(head);
    }

    // Clean up the list.
    list_cleanup(l);
    return 0;
}



