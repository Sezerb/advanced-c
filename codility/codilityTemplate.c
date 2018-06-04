#include <stdio.h>
#include <stdlib.h>

#define DEBUG_EN 1

int solution(int A[], int N) {
    // write your code in C99 (gcc 6.2.0)
    int i;
    
#if (DEBUG_EN == 1)
    for(i = 0; i < N; i++)
    {
        printf("\nA[%d]:%d\n",i, A[i]);
    }
#endif
	
	return 0;
}


int A[] = {1,3,2,1,4,6};

int main(int argc, char *argv[])
{
    printf("\nresult:%d\n",solution(A, 6));
}
