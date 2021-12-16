/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * tree.c:
 * DESCRIPION:
 *    In this file all the internal functions of 
 *    the struct tree are defined. Function such as
 *    initializing nodes, checking balance and performing
 *    rotations on the tree. Also in this file removing, inserting
 *    nodes in the tree are defined. At last it is also defined
 *    how the tree will be cleaned.
 * 
 * USAGE:
 *    This File can be used by including the header file tree.h
 *    And then calling the functions by passing the right variables.
 */

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
    struct node *parent;

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
    n->parent = NULL;
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

/* This function gets a height of a given node.
On succes it will return the height otherwise -1. */
static int get_height(node *n) {

    if (n == NULL) return -1;

    /* Use recursion to get the height of 
    left and right subtree. */
    int left = get_height(n->lhs);
    int right = get_height(n->rhs);

    // Check which height to return.
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

    // Empty tree check.
    if (tree->root == NULL) {
        printf("Tree is empty\n");
        return 1;
    }

    // See if the tree is balanced accoring to the invarient.
    node *root = tree->root;
    int invarient = 0;

    // Set invarient aaccoring to the difference heights of node's children.
    if (root->lhs == NULL) {
        invarient = (get_height(root->rhs)) * -1;
    }
    else if (root->rhs == NULL) {
        invarient = (get_height(root->lhs));
    }
    else if (root->lhs != NULL && root->rhs != NULL) {
        invarient = get_height(root->lhs) - get_height(root->rhs);
    }

    // Check if the absolute value of the invarient exceeds 1.
    if (abs(invarient) > 1) {
        return 1;
    } 
    return 0;
}

struct tree *tree_init(int turbo) {
    
    // Malloc tree and perform malloc check.
    struct tree *t = malloc(sizeof(struct tree));
    if (t == NULL) return NULL;

    // Set root as NULL
    t->root = NULL;
    t->turbo = 0;

    // If turbo is on set balance factor to 1;
    if (turbo == 1) {
        t->turbo = 1;
    }
    return t;
}

/* This function performs a left rotation on a given node.
it will return -1 if it fails and 0 if it succeeds. */
static int left_rotation(node *n, struct tree *t) {
    if (n == NULL) return -1;

    node *parent = n->parent;
    node *left = n->lhs;
    if (left == NULL) return -1;
 
    /* Set the right child of the left child 
    as the left child pointer of the current node. */
    n->lhs = left->rhs;

    if (left->rhs) {
           // Update that right child's parent pointer
            left->rhs->parent = n;
    }

    
    // Set right child of the left child to given node.
    left->rhs = n;
    // Update given node parent to left child.
    left->parent = n->parent;
    n->parent = left;

    // If given node had parent, update the pointer of the parent.
    if (parent) {
        if (parent->lhs == n) {
            parent->lhs = left; 
        } else {
            parent->rhs = left;
        }
    }
    if (!parent) t->root = left;
    return 0;
}

/* This function performs a right rotation on a given node.
it will return -1 if it fails and 0 if it succeeds. */
static int right_rotation(node *n, struct tree *t) {
    if (n == NULL) return -1;

    node *parent = n->parent;
    node *right = n->rhs;
    if (right == NULL) return -1;
 
    /* Set the left child of the right child 
    as the right child pointer of the current node. */
    n->rhs = right->lhs;

    if (right->lhs) {
        // Update that right child's parent pointer
        right->lhs->parent = n;
    }
 
    // Set left child of the right child to given node.
    right->lhs = n;
    // Update given node parent to right child.
    right->parent = n->parent;
    n->parent = right;

   
    // If given node had parent, update the pointer of the parent.
    if (parent) {
        if (parent->lhs == n) {
            parent->lhs = right; 
        } else {
            parent->rhs = right;
        }
        right->parent = parent;
    }

    if (!parent) t->root = right;
 
    return 0;
}

/* This function goes from a given node up the tree 
to see if it is balanced. If it comes across a node 
with a balance factor which is off it will rebalance the tree.
it returns 0 on succes and -1 on failure. */
static int set_balance(node* n, struct tree *t) {
    if (n == NULL) return -1;
   
    // Set the balance factor of the given node.
    n->balance_factor = (get_height(n->lhs) - get_height(n->rhs));
    
    // Go up the tree and set for each node it's balance factor.
    while (n->parent != NULL) {
        n = n->parent;
 
        n->balance_factor = (get_height(n->lhs) - get_height(n->rhs));

        // If balance factor exceeds tree varient break.
        if (abs(n->balance_factor) > t->turbo) {
            break;
        }
        // If tree is balanced return.
        if (n->parent == NULL) return 0;
    }

    // Rebelance the tree using the right and left rotation functions.
    if (n->balance_factor < 0) {
        if (right_rotation(n, t) == -1) return -1;
    } else {
        if (left_rotation(n, t) == -1) return -1;
    }
    return 0;
} 


int tree_insert(struct tree *tree, int data) {
    if (tree == NULL) return -1;

    // Malloc the new node and perform malloc check.
    node *new = make_node(data);
    if (new == NULL) return -1;

    // Check for empty tree.
    if (tree->root == NULL) {
        tree->root = new;
        new->parent = NULL;
        return 0;
    }

    // If data was already in the tree then just return.
    if (tree_find(tree, data) == 1) {
        free(new);
        return 1;
    }

    // Node n used as pointer to traverse the tree.
    node *n = tree->root; 

    // Traverse the tree until data is added to it.
    while (tree_find(tree, data) != 1) {
        // Go into left tree if data is lesser.
        while (data < n->data) {

            // If at the end at the new child.
            if (n->lhs == NULL) {
                n->lhs = new;
                new->parent = n;

                // Check if turbo is turned on, if so balance the tree.
                if (tree->turbo != 0) {
                    if (set_balance(new, tree) == -1) {
                        free(new);
                        return -1;
                    }
                }
                return 0;
            }
            // If not at the end update n to keep going.
            n = n->lhs;
        }
        // Go into right tree if data is greater.
        while (data > n->data) {
           
            // If at the end at the new child.
            if (n->rhs == NULL) {
                n->rhs = new;
                new->parent = n;
                
                // Check if turbo is turned on, if so balance the tree.
                if (tree->turbo != 0) {
                    if (set_balance(new, tree) == -1) {
                        free(new);
                        return -1;
                    }
                }
                return 0;
            }
            // If not at the end update n to keep going.
            n = n->rhs;
          
        }
    }
    // If error occured delete the node.
    free(new);
    return -1;
}

int tree_find(struct tree *tree, int data) {
    if (tree == NULL) return 0;
    // Node n used as pointer to traverse the tree.
    
    node *n = tree->root;
    if (n == NULL) return 0;
    // If the data is at root node.
    if (data == n->data) {
        return 1;
    }
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

        if (data > n->data) {
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
    if (current == NULL) return NULL;
 
    /* loop down to find the leftmost leaf */
    while (current->lhs) {
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
            // Set parent of child node to it's grandparent.
            root->rhs->parent = root->parent;
            node *temp = root;
            root = root->rhs;
            free(temp);
        }
        // If node has only left child.
        else if (root->rhs == NULL) {
            // Set parent of child node to it's grandparent.
            root->lhs->parent = root->parent;
            node *temp = root;
            root = root->lhs;
            free(temp);
        }

        // Case 3: node has 2 children.
        else {
            node *temp = min_right(root);
            root->data = temp->data;

            // Set parent of children node to their grandparent.
            root->rhs->parent = root->parent;
            root->lhs->parent = root->parent;

            // Use recursion to delete the extra node.
            root->rhs = delete_node(root->rhs, temp->data);
        }
    }
    return root;
}

int tree_remove(struct tree *tree, int data) {
    
    if (tree == NULL) return 1;
    // If data is not in the tree.
    if (tree_find(tree, data) == 0) {
        return 1;
    }
    node *root = tree->root;

    // If to be delted node is at root.
    if (data == root->data) {
        /* Use the min_right function to get 
        the most left node in the right subtree. */
        node *min = min_right(root);
        
        /* If this min node is not found. 
        That means we can assume there is no right subtree. */
        if (min == NULL) {
            // Set left child of root as new root.
            tree->root = root->lhs;
            
            // Update parent of lhs child if it has one.
            if (root->lhs) {
                root->lhs->parent = NULL;
            }

            // free old root.
            free(root);
            return 0;
        }

        /* If to be deleted node is not at root
        Update root data min node data. */
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
