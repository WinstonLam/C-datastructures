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
            }
        
            char *name, *age, *skip;
            skip = ".";
            name = strtok(s, " ");

            if (strcmp(name, skip) == 0) break;

            printf("string = %s\n", name);
            while (token != NULL){
                age = token;
                token = strtok(NULL, " ");
            }
            printf("age = %s\n", age);
            /*void *patient_n;
            // Set patient name and age
            // HOW?
            ((const patient_t *) patient_n)->name;
            ((const patient_t *) patient_n)->age;

            // Insert patient into the prioq
            // HOW
            // Iterate through the tokens.
            while (token[0] != NULL){
                if (token[0] == ".") break;
                // Insert the tokens into the hash_table with the corresponding line_number.
                prioq_insert(queue, patient_n);
                token[0] = strtok(NULL, "\n"); 
            }*/
        }
        /* void *patient_n;
        // Pop the patient ordered on 
        prioq_pop(queue);
        // print patient name 
        ((const patient_t *) patient_n)->name; */

        printf(".\n"); /* End turn. */

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
