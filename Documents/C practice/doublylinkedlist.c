#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    int data;
    struct node *next;
    struct node *prev;
} * head;

struct node* NewNode(int x)
{
    struct node *temp = (struct node *)mallox(sizeof(struct node));
    temp->data = x;
    temp->prev = NULL;
    temp->next = NULL;
    return temp;
} 

int main()
{
}
