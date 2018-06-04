#include <stdio.h>
#include <stdlib.h>


int compare (const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}

int solution(int A[], int N){
	int i, j, k;
	int count = 0, max = 0;

	qsort(A, N, sizeof(int), compare);

	for(i = 0; i < N ; i++)
	{
		j = i; //reference index
		k = j + 1;

		while((k < N) && (A[k] - A[j]) < 2)
		{
			count++;
			k++;
		}

		if(count > 0)
		{
			if(count > max)
			{
				max = count;
			}

			count = 0;
		}

	}

	if(max > 0)
		return ++max;
	else
		return 0;	
}


//int A[] = {6,10,6,9,7,8};
//int A[] = {1,1,1,1,1,1};
int A[] = {1,3,5,7,7,7,7,7,8,3,3,3,2,2,2,8,10,12,8,8,8};


int main()
{
	int i;

	printf("Result:%d\n", solution(A, sizeof(A)/sizeof(int)));

 return 0;
}