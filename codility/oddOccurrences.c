// you can write to stdout for debugging purposes, e.g.
// printf("this is a debug message\n");
#include <stdlib.h>
#define printf(args...) //Disable printf

int compare(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

int solution(int A[], int N) {
    // write your code in C99 (gcc 6.2.0)
    int i;
    
    qsort(A, N, sizeof(int), compare);
    
    for(i=0; i<N; i++)
    {
        printf("\nA[%d]:%d\n",i,A[i]);
    }
    
    if(N == 1)
     return A[0];
     
    for(i=0; i<N; i++)
    {
        if((i<N-1 && A[i] == A[i+1]) || (i>0 && A[i] == A[i-1]))
        {
            continue;
        }
        else
            return A[i];
    }
 
    return 1;   
}



