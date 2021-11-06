#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int x[], y[];

void main(){
    int x[3],y[3] = {-1,0,1};

    for (int c=0; c<3;c++){
        for(int v=0; v<3; v++){
            if (x[c] != y[v] && (x[c]+y[v]) != 0){
                printf("%d,%d \n",x[c],y[v]);
            }
        }
    }
}