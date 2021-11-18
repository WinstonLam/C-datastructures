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

static struct list *sort(struct list *l){
    if (l == NULL) return NULL;
    
    struct node *curr = list_head(l);
    size_t i = 0;

    
    while (curr != NULL){
        printf("curr = %d\n",list_node_get_value(curr));
        
        struct node *temp = list_head(l);
        struct node *next = list_next(curr);
        // Start from the head of sorted list.
        while (temp != NULL){
            // If given value in the unsorted list is less or equal insert before current position.
           
            if (list_node_get_value(curr) > list_node_get_value(temp)){
                printf("STOP curr = %d, komt achter %d\n",list_node_get_value(curr), list_node_get_value(temp));
                list_unlink_node(l,curr);
                list_insert_after(l, curr, temp);
            }
            // Move current node to next node of the sorted list. 

            temp = list_next(temp);
         
        
        }
        
        curr = next;
        i++;
        

    }
    return l;
}
 

int main(int argc, char *argv[]) {
    struct config cfg;
    if (parse_options(&cfg, argc, argv) != 0) {
        return 1;
    }
    struct list *l = list_init();
    if (l == NULL) return 0;
    
    while (fgets(buf, BUF_SIZE, stdin)) {

        int num = atoi(buf); 
        struct node *n = list_new_node(num);
        if (n == NULL) return 0;
        list_add_front(l,n);
    }


    struct list *unsorterd = l;
    struct node *x = list_head(unsorterd);
    while (x != NULL){
        printf("list 1: %d\n",list_node_get_value(x));
        x = list_next(x);
    }
     printf("\n");

    struct list *sorted = sort(l); 
    struct node *y = list_head(sorted);
    while (y != NULL){
        printf("list 2: %d\n",list_node_get_value(y));
        y    = list_next(y);
    }
     
    list_cleanup(l);
    

    return 0;

}

