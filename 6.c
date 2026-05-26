#include <stdio.h>

void allocate(int b[], int m, int p[], int n, int type)
{
    int a[20], i, j, idx;

    for(i=0;i<n;i++) a[i] = -1;

    for(i=0;i<n;i++)
    {
        idx = -1;

        for(j=0;j<m;j++)
        {
            if(b[j] >= p[i])
            {
                if(idx == -1)
                    idx = j;

                if(type==1 && b[j] < b[idx]) idx=j; // Best Fit
                if(type==2 && b[j] > b[idx]) idx=j; // Worst Fit
            }
        }

        if(idx != -1)
        {
            a[i] = idx;
            b[idx] -= p[i];
        }
    }

    if(type==0) printf("\nFirst Fit\n");
    else if(type==1) printf("\nBest Fit\n");
    else printf("\nWorst Fit\n");

    printf("Process\tBlock\n");

    for(i=0;i<n;i++)
    {
        printf("%d\t", p[i]);

        if(a[i]!=-1) printf("%d\n", a[i]+1);
        else printf("Not Allocated\n");
    }
}

int main()
{
    int m, n, i;

    printf("Enter number of memory blocks: ");
    scanf("%d", &m);

    int b1[m], b2[m], b3[m];

    printf("Enter block sizes:\n");
    for(i=0;i<m;i++)
    {
        scanf("%d", &b1[i]);
        b2[i] = b3[i] = b1[i];
    }

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int p[n];

    printf("Enter process sizes:\n");
    for(i=0;i<n;i++)
        scanf("%d", &p[i]);

    allocate(b1,m,p,n,0); // First Fit
    allocate(b2,m,p,n,1); // Best Fit
    allocate(b3,m,p,n,2); // Worst Fit

    return 0;
}
