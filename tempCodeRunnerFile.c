#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int x[];
int y[];

void main(){
    int x[3] = {-1,0,1};
    int y[3] = {-1,0,1};
    for (int c=0; c<3;c++){
        for(int v=0; v<3; v++){
            if (x[c] != y[v]){
                printf("%d,%d",x[c],y[v]);
            }
        }
    }
}