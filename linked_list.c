#include <stdio.h>
#include <stdlib.h>


// create node type
struct node{
    int data;
    struct node* link;
}*head; // this points to head to make it a global variable


int main(){
    Createlinkedlist();
    printf("data in the list is:\n");
    Reverseprint(head);
    return 0;

}

int Createlinkedlist(){
    int data, n;
    /*make head node and leave it empty
    store the location of the head node 
    using the malloc function*/
    head = (struct node*)malloc(sizeof(struct node*));
    // ask for user input on the length of the list
    printf("How long should the list be:");
    scanf("%d",&n);
    // ask for first value
    printf("value of node 1 :");
    scanf("%d",&data);
    // fill the head with the user input
    (*head).data = data; // head -> data = data; 
    (*head).link = NULL; // head -> link = NULL;
    // put head data into temp
    struct node *temp = head;

    // Creating n nodes and adding to linked list
    for(int i=2;i<=n;i++){
        // create nth node for the loop
        struct node* nNode = (struct node*)malloc(sizeof(struct node));
        // check if memory can be located
        if(nNode == NULL){
            printf("out of memory");
        }
        // ask for user input of the nth node;
        else{
        printf("Value of node %d :",i);
        scanf("%d",&data);
        // insert data into nth node and leave pointer NULL
        nNode -> data = data;
        nNode -> link = NULL;
        // link previous pointer to nth node;
        temp -> link = nNode;
        temp = temp ->link;
        }
    }
}

void Print(struct node* temp){
    // check if list is empty
    if(temp == NULL){
        return; 
    }
    // print out nodes while data pointer != NULL
    printf("Data = %d\n", temp->data);
    Print(temp->link);
}

void Reverseprint(struct node* temp){
    // check if list is empty
    if(temp == NULL){
        return;
    }
    // print out nodes while data pointer != NULL
    Reverseprint(temp->link);
    printf("Data = %d\n", temp->data);
}

// delete node in certain position
void Delete(int n){
    struct node *temp1 = head;
    if(n==1){
        head = temp1->link; // head points to second node
        free(temp1);
    }
    int i;
    for(i=0;i<n-2;i++){
        temp1 = temp1->link;
    // temp1 points to n-1th node
    }
    struct node *temp2 = temp1->link; //nth node
    temp1->link = temp2->link; // (n+1th node)
    free(temp2); //delete temp2
}

// reverse a linked list
void Reverse(){
    struct node *next;
    // variable to keep track of the prev link
    struct node *prev = NULL;
    // variable to keep track of current link
    struct node* current = head;
    // go through the list
    while(current != NULL){

        next = current->link;
        // change current link to point to prev node
        current->link = prev;
        // update prev node to current adress
        prev = current;
        current = next;
    }
    head = prev;

}
