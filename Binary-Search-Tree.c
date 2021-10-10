#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node{
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* GetNode(int data){
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode ->data = data;
    newNode->left = newNode ->right = NULL;
    return newNode;
};

/* '**' means a pointer to pointer so that Bstnode can be accesed 
in the insert function and also updated
for ease we will define insert a bstnode instead*/
struct Node* Insert(struct Node* root, int data){
    if(root == NULL){
        root = GetNode(data);
    }
    else if(data <= root->data){
        root->left = Insert(root->left, data);
    }
    else {
        root->right = Insert(root->right, data);
    }
    return root;
};

bool Search(struct Node* root, int data){
    if(root == NULL){
        return false;
    }
    if(root->data == data){
        return true;
    }
    else if(data <= root->data){
        return Search(root->left,data);
    }
    else{
        return Search(root->right,data);
    }
}


int main(){
    struct Node* root = NULL;
    root = Insert(root, 15);
    root = Insert(root, 10);
    root = Insert(root, 20);
    root = Insert(root, 25);
    int number;
    printf("Enter Number to be searched \n");
    scanf("%d",&number);
    if(Search(root, number) == true){
        printf("Your number is in the tree");
    }
    else{printf("Your number is not in the tree");}
    




}

