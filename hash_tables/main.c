/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * main.c:
 * DESCRIPION:
 *    This file takes a file .txt as input and adds each word in that file
 *    to a hash table with the line on which it appears as a value in it's array.
 *    Then the user can input a word and a list of line numbers will be shown of where 
 *    the word appears in the text.
 * USAGE:
 *    This function can be used by intializing the programm with a .txt file and then inputing
 *    a word in the standerdin input.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "array.h"
#include "hash_func.h"
#include "hash_table.h"

#define LINE_LENGTH 256

#define TABLE_START_SIZE 2
#define MAX_LOAD_FACTOR 0.6
#define HASH_FUNCTION hash_too_simple

#define START_TESTS 2
#define MAX_TESTS 2
#define HASH_TESTS 1

/* Replace every non-ascii char with a space and lowercase every char. */
static void cleanup_string(char *line) {
    for (char *c = line; *c != '\0'; c++) {
        *c = (char) tolower(*c);
        if (!isalpha(*c)) {
            *c = ' ';
        }
    }
}

/* Return a pointer to a heap allocated string with all the characters we
 * use as word delimiters. Return NULL on failure. */
static char *calc_delim(void) {
    const size_t ascii_table_size = 128;
    char *res = malloc(sizeof(char) * ascii_table_size);
    if (!res) {
        return NULL;
    }

    int res_index = 0;
    for (unsigned char c = 1; c < ascii_table_size; c++) {
        if (!isalpha(c)) {   /* if it's not an alpha it's a delimiter char. */
            res[res_index++] = (char) c;
        }
    }
    res[res_index] = '\0';
    return res;
}

/* Creates a hash table with a word index for the specified file and
 * parameters. Return a pointer to hash table or NULL if an error occured.
 */
static struct table *create_from_file(char *filename,
                               unsigned long start_size,
                               double max_load,
                               unsigned long (*hash_func)(unsigned char *)) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return NULL;

    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line) {
        fclose(fp);
        return NULL;
    }

    // Initialize hash table and check for memory allocation.
    struct table *hash_table = table_init(start_size, max_load, hash_func);
    if (hash_table == NULL) return NULL;

    // Initialize calc_delim function for line cleanup.
    char *delim = calc_delim();
    if (!delim){
        return NULL;
    }
    // Keep count of line number as value for table insertion.
    int line_number = 0;

    while (fgets(line, LINE_LENGTH, fp)) {

        // Update line number each time a line is read.
        line_number++;

        // Store token which is cleaned using cleanup_string and calc_delim function.
        char* token;
        cleanup_string(line);
        token = strtok(line, delim);

        // Iterate through the tokens.
        while (token != NULL){
            // Insert the tokens into the hash_table with the corresponding line_number.
            table_insert(hash_table, token, line_number);
            token = strtok(NULL, delim);
        } 
    }
    fclose(fp);
    // Free allocated strings.
    free(delim);
    free(line);
    

    return hash_table;
}

/* Reads words from stdin and prints line lookup results per word.
 * Return 0 if succesful and 1 on failure. */
static int stdin_lookup(struct table *hash_table) {
    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line) {
        return 1;
    }
    // Initialize calc_delim function for line cleanup.
    char *delim = calc_delim();
    if (!delim){
        return 1;
    }

    while (fgets(line, LINE_LENGTH, stdin)) {

        // Store token which is cleaned using cleanup_string and calc_delim function.
        char* token;
        cleanup_string(line);
        token = strtok(line, delim);
        
        // Iterate through the tokens.
        while (token != NULL){
            // Get the value array of each token from the hash_table.
            struct array *values = table_lookup(hash_table, token);
            printf("%s\n",token);
            // Loop though the value array and print each line_number.
            for (unsigned long i = 0; i < array_size(values); i++){
                printf("* %d\n",array_get(values, i));
            }
            printf("\n");
            token = strtok(NULL, delim);
        }
    }
    // Free allocated strings.
    free(delim);
    free(line);
    return 0;
}

static void timed_construction(char *filename) {
    /* Here you can edit the hash table testing parameters: Starting size,
     * maximum load factor and hash function used, and see the the effect
     * on the time it takes to build the table.
     * You can edit the tested values in the 3 arrays below. If you change
     * the number of elements in the array, change the defined constants
     * at the top of the file too, to change the size of the arrays. */
    unsigned long start_sizes[START_TESTS] = { 2, 65536 };
    double max_loads[MAX_TESTS] = { 0.2, 1.0 };
    unsigned long (*hash_funcs[HASH_TESTS])(unsigned char *) = { hash_too_simple };

    for (int i = 0; i < START_TESTS; i++) {
        for (int j = 0; j < MAX_TESTS; j++) {
            for (int k = 0; k < HASH_TESTS; k++) {
                clock_t start = clock();
                struct table *hash_table =
                create_from_file(filename, start_sizes[i], max_loads[j], hash_funcs[k]);
                clock_t end = clock();

                printf("Start: %ld\tMax: %.1f\tHash: %d\t -> Time: %ld "
                       "microsecs\n",
                       start_sizes[i], max_loads[j], k, end - start);
                table_cleanup(hash_table);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s text_file [-t]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3 && !strcmp(argv[2], "-t")) {
        timed_construction(argv[1]);
    } else {
        struct table *hash_table =
        create_from_file(argv[1], TABLE_START_SIZE, MAX_LOAD_FACTOR, HASH_FUNCTION);
        if (hash_table == NULL) {
            printf("An error occured creating the hash table, exiting..\n");
            return EXIT_FAILURE;
        }
        if (stdin_lookup(hash_table) != 0) {
            table_cleanup(hash_table);
            return EXIT_FAILURE;
        }
        table_cleanup(hash_table);
    }

    return EXIT_SUCCESS;
}
