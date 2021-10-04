#include <stdio.h>
#include <stdlib.h>

int temp = 0;
int data;
int top = -1;

int createStack(int n)
{
    temp = n;
}

int isEmpty()
{
    if (top == -1)

        return 1;
}
int isFull()
{
    if (top == temp)
        return 1;
}
int push(data, stack)
{
    if (isFull() == 1)
        return printf("stack is full");
    top = top + 1;
    stack[&top] = data;
}
int pop(stack)
{
    int data;
    if (isEmpty)
        return printf("stack is already empty");
    data = stack[&top];
    top = top - 1;
    return data;
}
void printStack(stack)
{
    while (stack[&top] != NULL)
        printf("%d ", stack[&top]);
    top = top - 1;
}
int main()
{
    createStack(5);
    int *arr = (int *)malloc(sizeof(int) * temp);
    push(3, arr);
    push(2, arr);
    push(1, arr);
    push(0, arr);
    printStack(arr);
}