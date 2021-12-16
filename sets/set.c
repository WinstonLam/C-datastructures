/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * set.c:
 * DESCRIPION:
 *    In this file all the internal functions of 
 *    the struct set are defined. Set mainly calls up 
 *    the function as defined in tree.c. Using it's header file.
 *    A pointer to the tree is initialized for this specific usage.
 * 
 * USAGE:
 *    This File can be used by including the header file set.h
 *    And then calling the functions by passing the right variables.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "tree.h"

struct set {
    struct tree *tree;
};

struct set *set_init(int turbo) {
    struct set *s = malloc(sizeof(struct set));
    if (s == NULL) return NULL;

    // Initialize the tree to use the tree functions.
    s->tree = tree_init(turbo);
    if (s->tree == NULL) {
        free(s);
        return NULL;
    }
    return s;
}

int set_insert(struct set *s, int num) {
    return tree_insert(s->tree, num);
}

int set_find(struct set *s, int num) {
    return tree_find(s->tree, num);
}

int set_remove(struct set *s, int num) {
    return tree_remove(s->tree, num);
}

void set_cleanup(struct set *s) {
    tree_cleanup(s->tree);
    free(s);
    return;
}

void set_print(struct set *s) {
    tree_print(s->tree);
    return;
}

int set_verify(struct set *s) {
    return tree_check(s->tree);
}
