#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Results {
  int * A;
  int N; // Length of the array
};

struct Results solution(int A[], int N, int K);

int A[] = {3, 8, 9, 7, 6};

int main(int argc, char *argv[])
{
	int i;
	struct Results stResult;
	stResult = solution(A, 5, 2);
	for(i=0; i<5; i++)
    fprintf(stdout,"\nresult:%d\n",stResult.A[i]);
	return 0;
}

struct Results solution(int A[], int N, int K)
{
	struct Results stRetVal;
	stRetVal.N = N;
	stRetVal.A = (int *)malloc(N*sizeof(int));
	memcpy(stRetVal.A, A, N*sizeof(int));

	int i;
	int iShift = (K % N);
	
	for(i=0; i<N; i++)
	{
		stRetVal.A[(i+iShift)%N] = A[i];
	}

	return stRetVal;
}
