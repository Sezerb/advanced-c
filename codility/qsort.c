#include <stdio.h>
#include <stdlib.h>

#define DEBUG_EN 1

int compare(const void* a, const void* b)
{
    return ( *(int*)a - *(int*)b );
}

int solution(int A[], int N) {
    // write your code in C99 (gcc 6.2.0)
    int i;
    
	//SORT    
    qsort(A, N, sizeof(int), compare);

#if (DEBUG_EN == 1)
    for(i = 0; i < N; i++)
    {
        printf("\n A[%d]:%d\n",i, A[i]);
    }
#endif

    for(i = 0; i < N ; i++)
    {
        if(A[i] < 0)
        {
            continue;
        }
        else
        {
            if((i<(N-1)) && ((A[i+1] - A[i]) > 1))
            {
                return (A[i]+1);
            }

        }
    }

    if(A[N-1] < 0)
        return 1;
    else
        return (A[N-1]+1);
}



int A[] = {1,3,2,1,4,6};

int main(int argc, char *argv[])
{
    printf("\nresult:%d\n",solution(A, 6));
}

