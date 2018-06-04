#include <stdio.h>
#include <stdlib.h>

int solution(int A[], int N) {
    // write your code in C99 (gcc 6.2.0)
	int i;
    int iSumLeft = 0, iSumRight = 0;
	int iPNTapeSum;
    int iMinTapeSum;

	//Calculate P=1 case first.
	for(i = 0; i<N ; i++)
	{
		iSumRight += A[i]; //Find the overall sum first.
	}
	iSumLeft = A[0];
	iSumRight = iSumRight - A[0];
	iPNTapeSum = abs(iSumLeft - iSumRight);
	iMinTapeSum = iPNTapeSum;

 	for(i=1; i<N-1; i++)
	{
		iPNTapeSum = abs(iPNTapeSum - 2*abs(A[i]));
		if(iPNTapeSum < iMinTapeSum)
		{
			iMinTapeSum = iPNTapeSum;
		}	
	}

	return iMinTapeSum;	
}


int A[] = {3,1,2,4,3};

int main(int argc, char *argv[])
{
    printf("\nresult:%d\n",solution(A, 5));
}
