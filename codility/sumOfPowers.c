#include <stdio.h>
#include <stdlib.h>


int compare(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}

int powTwo(int N)
{
	return (1<<N);
}

int solution(int A[], int N) {
    // write your code in C99 (gcc 6.2.0)
    int i;
    int comElemType= 0; //even
    int B[N];
    int comElem;
    int sumElem = 0;

    //Note that result = comElem * sumElem;

    //Sort first to use the smallest element as common element.
   	qsort(A, N, sizeof(int), compare);
   	 
	if(A[0]%2) //odd
	{
		comElemType = 1; //odd
	}

	for(i = 0; i < N; i++)
	{
		B[i] = powTwo(A[i] - A[0]);

		if(comElemType != A[i]%2) //Signs are different
		{
			B[i] *= (-1); 
		}
		printf("\nA[%d]:%d  B[%d]:%d",i, A[i], i, B[i]);	
	}

	comElem = (comElemType == 1) ? (-1)*powTwo(A[0]) : powTwo(A[0]);

	for(i = 0; i < N; i++)
	{
		sumElem += B[i];
	}

	if(sumElem == 0)
		return 0;
	else 
		return comElem*sumElem;	
}


int A[] = {100, 99, 99};
//int A[] = {1, 2, 3};
 
int main(int argc, char *argv[])
{
    printf("\nresult:%d\n",solution(A, 3));
}
