#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int *A;

int main(int argc, char *argv[])
{
    int ARRAYSIZE = 0;
    ARRAYSIZE = atoi(argv[1]);
    int n;
    FILE *fp1;
    FILE *fp2;
    fp1 = fopen("source.txt","w");
    fp2 = fopen("sorted.txt","w");
    A = (int *) malloc(sizeof(int) * ARRAYSIZE);
    srand((unsigned) time(NULL));

    for( n = 0 ; n < ARRAYSIZE ; n++)
        A[n]=rand();      

    printf("Before sorting the list is: \n");
    for( n = 0 ; n < ARRAYSIZE; n++ ) 
    {
//        printf("%d ", A[n]);
        fprintf(fp1,"%d\n",A[n]);
    }
    fclose(fp1);
    qsort(A, ARRAYSIZE, sizeof(int), cmpfunc);

    printf("\nAfter sorting the list is: \n");
    for( n = 0 ; n < ARRAYSIZE; n++ ) 
    {   
//        printf("%d ", A[n]);
        fprintf(fp2,"%d\n",A[n]);
    }
    fclose(fp2);
    return(0);
}
