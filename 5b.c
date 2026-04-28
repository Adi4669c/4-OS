#include <stdio.h>

int main() {
    int p=3,r=3;
    int alloc[3][3]={{0,1,0},{2,0,0},{3,0,2}};
    int req[3][3]={{0,0,0},{2,0,2},{0,0,1}};
    int avail[3]={0, 0, 0};

    int finish[3]={0};

    printf("Allocation Matrix:\n");
    for(int i=0;i<p;i++){
        for(int j=0;j<r;j++) printf("%d ",alloc[i][j]);
        printf("\n");
    }

    printf("Request Matrix:\n");
    for(int i=0;i<p;i++){
        for(int j=0;j<r;j++) printf("%d ",req[i][j]);
        printf("\n");
    }

    for(int i=0;i<p;i++){
        int zero=1;
        for(int j=0;j<r;j++)
            if(alloc[i][j]!=0) zero=0;
        if(zero) finish[i]=1;
    }

    for(int k=0;k<p;k++)
        for(int i=0;i<p;i++)
            if(!finish[i]){
                int flag=1;
                for(int j=0;j<r;j++)
                    if(req[i][j]>avail[j]) flag=0;

                if(flag){
                    for(int j=0;j<r;j++)
                        avail[j]+=alloc[i][j];
                    finish[i]=1;
                }
            }

    int dead=0;
    for(int i=0;i<p;i++)
        if(!finish[i]){
            printf("Process P%d is in deadlock\n",i);
            dead=1;
        }

    if(!dead) printf("No deadlock");
}
