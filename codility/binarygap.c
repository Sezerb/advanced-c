#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int solution(int N)
{
	bool bisPrevBitOne = false;
	int iZeroBitCnt = 0;
	int iMaxZeroCnt = 0;

	do
	{
		if(!(N & 1) && bisPrevBitOne == true)
		{
			++iZeroBitCnt;
			if(iZeroBitCnt > iMaxZeroCnt)
				iMaxZeroCnt = iZeroBitCnt;
		}
		
		if(N & 1)
		{
			bisPrevBitOne = true;
			iZeroBitCnt = 0;	
		}
	}
	while(N>>=1);

	return iMaxZeroCnt;
}


int main(int argc, char *argv[])
{
    fprintf(stdout,"\nresult:%d\n",solution(9));
}

