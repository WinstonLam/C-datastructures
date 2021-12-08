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
} patient_t;

static int compare_patient_name(const void *a, const void *b) {
    return strcmp(((const patient_t *) a)->name, ((const patient_t *) b)->name);
}

static int compare_patient_age(const void *a, const void *b) {
    if (a > b){
        return 1;
    } 
    else if (a == b){
        return 0;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    char *token, *name_cpy;
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
                continue;
            }
          /*   // Copy the key given in a string value.
            char* string = malloc((strlen(key) + 1) * sizeof(char));
            if (string == NULL) return 1;
            strcpy(string, key); */

            char *age;

            token = malloc((strlen(s) + 1) * sizeof(char));
            if (token == NULL){
                free(token);
                return EXIT_FAILURE;
            } 

          /*   strcpy(s, token); */
            
            free(token);
            token = strtok(s, " ");
            name_cpy = malloc((strlen(token) + 1) * sizeof(char));
            
            if (name_cpy == NULL){
                free(name_cpy);
                free(token);
                return EXIT_FAILURE;
            }

            strcpy(name_cpy, token);
            printf("name %s\n", name_cpy);

            while (token != NULL){
                age = token;
                token = strtok(NULL, " ");
            }
            
            free(token);

            printf("age %s\n", age);
            
            // Set patient name and age.
            patient_t *patient_n = malloc(sizeof(patient_t));
            if (patient_n == NULL){
                free(name_cpy);
                free(patient_n);
            }

            patient_n->name = name_cpy;
            patient_n->age = atoi(age);

            // Insert the patient.
            if (prioq_insert(queue, patient_n) != 0) return EXIT_FAILURE;
            printf("Insertion succeeded\n");
            
/* 
            printf("top %s\n", ((const patient_t *)array_get(queue->array, 0))->name); */
        
        }

        
        patient_t *patient_top = array_get(queue->array, 0);
        printf("%s\n", patient_top->name);
        
        // Pop the patient ordered on name. 
        prioq_pop(queue);

        // print patient name 
        printf("%s\n", patient_top->name);
        printf(".\n"); /* End turn. */
        free(patient_top->name);
        free(patient_top);

        if (++iterations == 10) {

            /* ... CODE MISSING HERE .... */

            break;
        }
    }

    /* ... CODE MISSING HERE .... */

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
