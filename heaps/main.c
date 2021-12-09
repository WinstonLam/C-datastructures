/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * main.c:
 * DESCRIPION:
 *    This file gets the patients from stdin and adds them to
 *    a priorty queue. Then the patients will be printed followed 
 *    by . This file also is able to sort patients not only based on
 *    name but also age in the order that they will be treated. Once a 
 *    patient is treated they will be removed from the queue. 
 * USAGE:
 *    This File can be used by running it with a txt file or writing
 *    on the stdin.
 */
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "prioq.h"

#define BUF_SIZE 1024

static char buf[BUF_SIZE];

struct config {
    /* You can ignore these options until/unless you implement the */
    /* bonus features. */
    /* Set to 1 if -y is specified, 0 otherwise. */
    int year;
};

static int parse_options(struct config *cfg, int argc, char *argv[]);
typedef struct {
    char *name;
    int age;
    int duration;
} patient_t;

static int compare_patient_name(const void *a, const void *b) {
    return strcmp(((const patient_t *) a)->name, ((const patient_t *) b)->name);
}

static int compare_patient_age(const void *a, const void *b) {
    // If age is the same order on name
    if ((((const patient_t *) a)->age) 
    - (((const patient_t *) b)->age) == 0){
        return strcmp(((const patient_t *) a)->name, ((const patient_t *) b)->name);
    }

    // Return difference between the age.
    return (((const patient_t *) a)->age) - (((const patient_t *) b)->age);
    
}
/* This function takes prioq as input and determines 
what should be printed based on int value's. if it fails
it will return 0. If patient takes longer to treat it will return 2.*/
static int print_patients(struct heap *q){
    if (q == NULL) return 0;
    if ((array_size(q->array)) == 0) return 2;
    patient_t *patient_top = array_get(q->array, 0);
    
    if (patient_top->duration && patient_top->duration > 1){
        patient_top->duration -= 1;
        return 2;
    }
    // Update the prioq after popping the top item.
    prioq_pop(q);
    return 1;
    }

/* This function takes care of setting all the info 
from stdin to the patient_t struct. */
static patient_t *set_info(char *s){
  
    // Malloc token and name_cpy and pefrom malloc checks.
    char *token = strtok(s, " ");
    char *name_cpy = malloc((strlen(token) + 1) * sizeof(char));
    if (name_cpy == NULL) {
        return NULL;
    }
    // Copy token to name_cpy.
    strcpy(name_cpy, token);
    
    // Set all the data from stdin into an array for later use.
    char *data[3];
    data[2] = NULL;
    int i = 0;
    while (token != NULL) {
        data[i++] = token;
        token = strtok(NULL, " ");
    }
    // Create pointer to struct patient_t for insertion. 
    patient_t *patient_n = malloc(sizeof(patient_t));
    if (patient_n == NULL) {
        free(name_cpy);
    }

    // Set patient name and age.
    patient_n->name = name_cpy;
    patient_n->age = atoi(data[1]);

    // If the duration is given set this aswell.
    if (data[2] != NULL) {
        patient_n->duration = atoi(data[2]);
    }
    return patient_n;
}   
int main(int argc, char *argv[]) {
    prioq *queue;
    struct config cfg;
    if (parse_options(&cfg, argc, argv) != 0) {
        return EXIT_FAILURE;
    }
    if (cfg.year) {
        queue = prioq_init(&compare_patient_age);
    } else {
        queue = prioq_init(&compare_patient_name);
    }
    // Queue malloc check.
    if (queue == NULL){
        array_cleanup(queue->array, NULL);
        free(queue);
        return EXIT_FAILURE; 
    }
    for (int iterations = 0;;) {
        while (1) {
            char *s = fgets(buf, BUF_SIZE, stdin);
            if (s == NULL) {
                fprintf(stderr, "Unexpected end of file. exiting\n");
                // Cleanup array on failure
                array_cleanup(queue->array, NULL);
                free(queue);
                return EXIT_FAILURE;
            } else if (s[0] == '.') {
                break;
            }
            
            // Use the set_info function to set the info to the patient from stdin.
            patient_t *patient_n = set_info(s);
            if (patient_n == NULL){
                free(patient_n);
                return EXIT_FAILURE;
            }
        
            // Insert the patient into the prioq.
            if (prioq_insert(queue, patient_n) != 0) {
                free(patient_n);
                return EXIT_FAILURE;
            } 
        }
        patient_t *patient_n = array_get(queue->array, 0);
        int print = print_patients(queue);

        // Decide what to print based on print_patients output.           
        if (print == 0){
            prioq_cleanup(queue, NULL);
            return EXIT_FAILURE;
        } else if (print == 1){
            printf("%s\n", patient_n->name);

            // print patient name 
            free(patient_n->name);
            free(patient_n);
        }
          
        printf(".\n"); /* End turn. */

        // If max hours are reached pop all items and print them.
        if (++iterations == 10) {
            patient_t *p = prioq_pop(queue);
            while(p) {
                printf("%s\n", p->name);

                free(p->name);
                free(p);
                p = prioq_pop(queue);
            }
            break;
        }
    }
    prioq_cleanup(queue, NULL);
    return EXIT_SUCCESS;
}
int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "y")) != -1) {
        switch (c) {
        case 'y':
            cfg->year = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}