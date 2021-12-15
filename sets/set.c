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
