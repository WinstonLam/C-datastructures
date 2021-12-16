/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * main.c:
 * DESCRIPION:
 *    This file makes an bst based on user input.
 *    It can insert, remove, lookup and print out the tree.
 *    If the tree is initialized using turbo. It will output
 *    A tree which is self balancing.
 * 
 * USAGE:
 *    This File can be used by running it and writing command prompts
 *    in the stdin.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

#define BUF_SIZE 256

void exit_failure(char *buf, struct set *s) {
    if (s) {
        set_cleanup(s);
    }
    free(buf);
    exit(EXIT_FAILURE);
}

int main(void) {
    char *buf = malloc(BUF_SIZE);
    if (!buf) {
        perror("Could not allocate input buffer");
        return EXIT_FAILURE;
    }

    // Intialize set, here turbo is switched on.
    struct set *s = set_init(1);   
    if (s == NULL) {
        free(buf);
        set_cleanup(s);
        return EXIT_FAILURE;
    }
  
    while (fgets(buf, BUF_SIZE, stdin)) {
        char *endptr;
        char *command;
        char *num_str;
        int num = 0;
        int insert = -1;

        command = strtok(buf, " ");    
        if (strchr("+-?", *command)) { 
            num_str = strtok(NULL, "\n");
            if (!num_str) {
                printf("set operation '%c' requires integer operand\n", *command);
                exit_failure(buf, s);
            }
            num = (int) strtol(num_str, &endptr, 10);
            if (*endptr != '\0') {
                printf("error converting '%s' to an integer\n", num_str);
                exit_failure(buf, s);
            }
        }
        
        switch (*command) {
            // If command is + perform an insert.
            case '+':
                insert = set_insert(s, num);
                // If insert fails return exit.
                if (insert == -1) {
                    set_cleanup(s);
                    return EXIT_FAILURE;
                }
                // If node is already in the tree just skip.
                else if(insert == 1) {
                    continue;
                }
                continue;
            // If the command is - perform a deletion.   
            case '-':
                set_remove(s, num);
                continue;
            // If the command is ? perform a lookup.
            case '?':
                // Check if data is found or not.
                if (set_find(s, num) == 1) {
                    printf("found: %d\n", num);
                } else {
                    printf("not found: %d\n", num);
                } 
                continue;
            // If the command is p print the tree in sorted order.
            case 'p':
                set_print(s);  
                continue;   
        }
    }

    if (set_verify(s)) { /* debug function */
        fprintf(stderr, "Set implementation failed verification!\n");
    }
    free(buf);
    set_cleanup(s);
    return EXIT_SUCCESS;
}
