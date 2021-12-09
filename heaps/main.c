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
    
    if ((((const patient_t *) a)->age) 
    - (((const patient_t *) b)->age) == 0){
        return strcmp(((const patient_t *) a)->name, ((const patient_t *) b)->name);
    }
    return (((const patient_t *) a)->age) - (((const patient_t *) b)->age);
    
}
static int print_patients(struct heap *q){
    if (q == NULL) return 0;
    if ((array_size(q->array)) == 0) return 0;
    patient_t *patient_top = array_get(q->array, 0);
    
    // Update the prioq after popping the top item.
    prioq_pop(q);

    if (patient_top->duration && patient_top->duration > 1){
        int i = 1;
        while(i < patient_top->duration){
            printf(".\n");
            i++;
        }
        printf("%s\n", patient_top->name);
        // print patient name 
        free(patient_top->name);
        free(patient_top);
        return i;
    }
    printf("%s\n", patient_top->name);
    
    // print patient name 
    free(patient_top->name);
    free(patient_top);
    return 1;
}

static patient_t *set_info(char *s, char *token, char *name_cpy){
  
    // Malloc token and name_cpy and pefrom malloc checks.
    token = malloc((strlen(s) + 1) * sizeof(char));
    token = strtok(s, " ");
        if (token == NULL){
        free(token);
        return NULL;
    } 
    name_cpy = malloc((strlen(token) + 1) * sizeof(char));
    if (name_cpy == NULL){
        free(name_cpy);
        free(token);
        return NULL;
    }

    // Copy token to name_cpy.
    strcpy(name_cpy, token);
    
    // Set all the data from stdin into an array for later use.
    char *data[3];
    int i = 0;
    while (token != NULL){
        data[i++] = token;
        token = strtok(NULL, " ");
    }
    free(token);

    // Create pointer to struct patient_t for insertion. 
    patient_t *patient_n = malloc(sizeof(patient_t));
    if (patient_n == NULL){
        free(name_cpy);
        free(patient_n);
    }
    // Set patient name and age.
    patient_n->name = name_cpy;
    patient_n->age = atoi(data[1]);
    // If the duration is given set this aswell.
    if (data[2] != NULL){
        patient_n->duration = atoi(data[2]);
    }
    return patient_n;
}   

int main(int argc, char *argv[]) {
    char *token, *name_cpy;
    prioq *queue;
    struct config cfg;
    int waiting = 0;

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
                array_cleanup(queue->array, NULL);
                free(queue);
                return EXIT_FAILURE;

            } else if (s[0] == '.') {
                break;
            }
            
            // Use the set_info function to set the info to the patient from stdin.
            patient_t *patient_n = set_info(s, token, name_cpy);
            if (patient_n == NULL){
                free(name_cpy);
                free(patient_n);
                return EXIT_FAILURE;
            }
        
            // Insert the patient into the prioq.
            if (prioq_insert(queue, patient_n) != 0){
                free(name_cpy);
                free(patient_n);
                return EXIT_FAILURE;
            } 

        }
        if (++iterations == 10) {
            while ((array_size(queue->array) != 0)){
                print_patients(queue);
            }
            break;
        }

        // If max hours is reached continue iterations till breakpoint.
        printf("turn: %d\n", waiting);
        if ((waiting + 1) == 10) {
            continue;
        }
        
        
        if (array_size(queue->array) == 0){
            waiting += 1;
        } else{            
            int i = print_patients(queue);
            
            if(i > 1){
                waiting += (i - 1);
            }
            waiting += 1; 
        }
        printf(".\n"); /* End turn. */
      
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
