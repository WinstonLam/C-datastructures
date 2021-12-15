#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

struct tree {
    struct node *root;
    int turbo;
};

struct node {
    int data;
    struct node *lhs;
    struct node *rhs;
    int balance_factor;
};
typedef struct node node;

/* Unique id's for numbering nodes in dot format. */
static int global_node_counter = 0;

/* Helper function: Allocate a new tree node and initialise it with
 * the given parameters. Return a pointer to the new node or NULL on
 * failure. */
static node *make_node(int data) {
    // Malloc the node and perform malloc check.
    node *n = malloc(sizeof(node));
    if (n == NULL) return NULL;

    // Intialize node values.
    n->data = data;
    n->lhs = NULL;
    n->rhs = NULL;
    return n;
}

static int print_tree_dot_r(node *root, FILE *dotf) {
    int left_id, right_id, my_id = global_node_counter++;

    if (root == NULL) {
        fprintf(dotf, "    %d [shape=point];\n", my_id);
        return my_id;
    }

    fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

    left_id = print_tree_dot_r(root->lhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

    right_id = print_tree_dot_r(root->rhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

    return my_id;
}

void tree_dot(struct tree *tree, char *filename) {
    node *root = tree->root;
    global_node_counter = 0;
    FILE *dotf = fopen(filename, "w");
    if (!dotf) {
        printf("error opening file: %s\n", filename);
        exit(1);
    }
    fprintf(dotf, "digraph {\n");
    if (root) {
        print_tree_dot_r(root, dotf);
    }
    fprintf(dotf, "}\n");
    fclose(dotf);
}

static int get_height(node *n) {

    if (n == NULL) {
        return -1;
    }
    
    int left = get_height(n->lhs);
    int right = get_height(n->rhs);

    if (left > right) {
        return left + 1;
    }
    else {
        return right + 1;
    }
    return 0;
}

int tree_check(struct tree *tree) {
    if (tree == NULL) {
        printf("No tree found\n");
        return 1;
    }

    if (tree->root == NULL) {
        printf("Tree is empty\n");
        return 1;
    }
    /* // Check if turbo is on.
    if (tree->turbo == 0) {
        printf("Turbo is not on\n");
    } */

    // See if the tree is balanced accoring to the invarient.
    node *root = tree->root;
    int invarient = 0;

    if (root->lhs == NULL) {
        invarient = (get_height(root->rhs)) * -1;
    }
    else if (root->rhs == NULL) {
        invarient = (get_height(root->lhs));
    }
    else if (root->lhs != NULL && root->rhs != NULL) {
        invarient = get_height(root->lhs) - get_height(root->rhs);
    }


    /* if (abs(invarient) > 1) {
        printf("Over max invarient of 1\n");
        return 1;
    } */

    /* // See how the tree is balanced based on invarient value.
    if (invarient > 0) {
        printf("Tree is balanced, but left heavy.\n");
    } else if (invarient < 0 ) {
        printf("Tree is balanced, but right heavy.\n");
    } else {
        printf("Tree is perfectly balanced. (as all thing should be)\n");
    } */

    return 0;
}

struct tree *tree_init(int turbo) {
    
    // Malloc tree and perform malloc check.
    struct tree *t = malloc(sizeof(struct tree));
    if (t == NULL){
        return NULL;
    }
    
    // Set root as NULL
    t->root = NULL;
    t->turbo = 0;
    // If turbo is on set balance factor to 1;
    if (turbo == 1) {
        t->turbo = 1;
    }
    return t;
}

int tree_insert(struct tree *tree, int data) {
    if (tree == NULL) return -1;

    // Malloc the new node and perform malloc check.
    node *new = make_node(data);
    if (new == NULL) return -1;

    if (tree->root == NULL) {
        tree->root = new;
        return 0;
    }
    // If data was already in the tree then just return.
    if (tree_find(tree, data) == 1) {
        free(new);
        return 1;
    }

    // Node n used as pointer to traverse the tree.
    node *n = tree->root;   
    while (tree_find(tree, data) != 1) {
        while (data < n->data) {
            
            if (n->lhs == NULL) {
                n->lhs = new;
                return 0;
            }
            n = n->lhs;
        }
        
        while (data >= n->data) {
           
            if (n->rhs == NULL) {
                n->rhs = new;
                return 0;
            }
            n = n->rhs;
          
        }
    }
    free(new);
    return -1;
}

int tree_find(struct tree *tree, int data) {
    if (tree == NULL) return 0;
    // Node n used as pointer to traverse the tree.
    
    node *n = tree->root;
    if (n == NULL) return 0;
    // If the data is at root node.
    if (data == n->data) return 1;

    /* Start at root node and see if data is less.
    if so then use while loop to search left subtree. 
    If it is greater than move in to the right subtree. */
    while (data != n->data) {
        
        // If n is in left subtree.
        if (data < n->data) { 
            // Check if left is NULL if so node is not in the tree.
            if (n->lhs == NULL) return 0;
            // Update n to left child.
            n = n->lhs;
        }

        if (data >= n->data) {
            // Check if right is NULL if so node is not in the tree.
            if (n->rhs == NULL) return 0;
            // Update n to right child.
            n = n->rhs;
        }
    }
    return 1;
}

/* This function will return the node with minimum value 
of a sub tree. If it fails it will return NULL. */
struct node *min_right(node *n) {
    if (n == NULL) return NULL;

    struct node *current = n->rhs;
 
    /* loop down to find the leftmost leaf */
    while (current != NULL) {
        current = current->lhs;
    }
    return current;
}

static node *delete_node(node *root, int data) {
    /* Use recurssion to set the link correctly 
    for parent node upon deletion. */
    if (root == NULL) return NULL;

   
    if (data < root->data) {
        root->lhs = delete_node(root->lhs, data);
    } else if (data > root->data) {
        root->rhs = delete_node(root->rhs, data);
    }
    // If at node start deletion process.
    else {
        
        // Case 1: node is a leaf node.
        if (root->lhs == NULL && root->rhs == NULL) {
            free(root);
            root = NULL;
        } 

        // Case 2: node has one child.
        else if (root->lhs == NULL) {
            node *temp = root;
            root = root->rhs;
            free(temp);
        }
        // If node has only right child.
        else if (root->rhs == NULL) {
            node *temp = root;
            root = root->lhs;
            free(temp);
        }

        // Case 3: node has 2 children.
        else {
            node *temp = min_right(root);
            root->data = temp->data;
            // Use recursion to delete the extra node.
            root->rhs = delete_node(root->rhs, temp->data);
        }
    }
    return root;
}


int tree_remove(struct tree *tree, int data) {
    
    if (tree == NULL) return 1;
    // If data is not in the tree.
    if (tree_find(tree, data) == 0) return 1;

    node *root = tree->root;
    if (data == root->data) {
        node *min = min_right(root);
        if (min == NULL) {
            tree->root = root->lhs;
            free(root);
            return 0;
        }
        root->data = min->data;
        // Use recursion to delete the extra node.
        root->rhs = delete_node(root->rhs, min->data);
    }
    
    /* Use the recusive functions delete_node 
    to traverse the tree and delete the node. */
    if (delete_node(tree->root, data) == NULL) return 0;

    return 0;
}

/* This function prints the tree following inorder traversal.
It uses rescursion to find print the following node given a root 
node */
static void inorder_print(node *n) {
    // Check if given node is NULL if so return.
    if (n == NULL) return;

    // Go into the left subtree untill the end.
    inorder_print(n->lhs);
    // Print data if end is reached.
    printf("%d\n", n->data);
    // Go into the right subtree untill the end.
    inorder_print(n->rhs);
    return;
}

void tree_print(struct tree *tree) {
    if (tree == NULL) return;
    if (tree->root == NULL) return;

    // Call to inorder print function to print the tree.
    inorder_print(tree->root);
    return; 

}

/* This function uses recurssion to delete all nodes in 
a tree starting from it's root node. */
static void delete_all(node *n) {
    if (n == NULL) return;

    delete_all(n->lhs);
    delete_all(n->rhs);

    free(n);
}

void tree_cleanup(struct tree *tree) {
    if (tree == NULL) return;

    delete_all(tree->root);
    free(tree);
    return;
}
