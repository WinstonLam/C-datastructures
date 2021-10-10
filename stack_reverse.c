
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


struct Stack{
    int top;
    unsigned capacity;
    int* array
};

struct Stack* createStack(unsigned capacity){
    struct Stack* stack= (struct Stack*)malloc(sizeof(struct Stack));
    stack -> capacity = capacity;
    stack -> top = -1;
    stack -> array = (int*)malloc(stack->capacity * sizeof(int));
};
 
 int isFull(struct Stack* stack){
     return stack ->top == stack ->capacity -1; 
 };
 int isEmpty(struct Stack* stack){
     return stack -> top == -1;
 };
 void* Push(struct Stack* stack, int item){
     if (isFull(stack))
         printf("stack is full");
         return;
     
     stack->array[++stack->top]= item;
 }
void* Pop(struct Stack* stack){
    if (isEmpty(stack))
        return;
    return stack->array[stack->top--];
}
void* Top(struct Stack* stack){
    if(isEmpty(stack))
        return;
    return stack -> array[stack->top];
}




void Reverse(char C[],int n){

   struct Stack* stack = createStack(n);


    for(int i=0;i<n;i++){
        Push(stack, C[i]);
    }
    for(int i =0;i<n;i++){
        C[i] = Top(stack);
        printf("%d",Top(stack));
        Pop(stack);
    }
    return C;
}

int main(){

    char C[50];
    printf("please enter your string: ");
    gets(C);
    Reverse(C,strlen(C));
    printf("your reversed string is: %s",C);
};