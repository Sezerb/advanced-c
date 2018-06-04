// you can write to stdout for debugging purposes, e.g.
// printf("this is a debug message\n");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAR 0
#define CUR 1
#define SQR 2
//#define printf(args...) 

typedef struct{
	unsigned char uType;
	int iIdx;
}stBracket;

int solution(char *S) {
    // write your code in C99 (gcc 6.2.0)
    char *pTempStr = S;
    int i, retVal = 1;
    int N = (int)strlen(S);
    int iTopOpen = 0;
    int iTopClose = 0;	
    printf("\nN:%d",N);	
	stBracket openStack[N];
	stBracket closeStack[N];
 
	memset(openStack, 0, N*sizeof(stBracket));
    memset(closeStack, 0, N*sizeof(stBracket));
    
 
    for(i=0; i<N; i++)
    {
		if((i<N-1) && ((*(pTempStr+i+1) - *(pTempStr+i)) == 2 || (*(pTempStr+i+1) - *(pTempStr+i)) == 1))
		{
			i++;
			continue;
		}

        char cTemp = *(pTempStr+i);

        if(cTemp == '(')
        {
            openStack[iTopOpen].uType = PAR;
			openStack[iTopOpen].iIdx = i;
			iTopOpen++;
        }
        else if(cTemp == '{')
        {
            openStack[iTopOpen].uType = CUR;
			openStack[iTopOpen].iIdx = i;
			iTopOpen++;    
        }
        else if(cTemp == '[')
        {
            openStack[iTopOpen].uType = SQR;
			openStack[iTopOpen].iIdx = i;
			iTopOpen++;    
        }
        else if(cTemp == ')')
        {
            closeStack[iTopClose].uType = PAR;
			closeStack[iTopClose].iIdx = i;
			iTopClose++;     
        }
        else if(cTemp == '}')
        {
            closeStack[iTopClose].uType = CUR;
			closeStack[iTopClose].iIdx = i;
			iTopClose++;    
        }
        else if(cTemp == ']')
        {
            closeStack[iTopClose].uType = SQR;
			closeStack[iTopClose].iIdx = i;
			iTopClose++;    
        }
        else
        {
            retVal = 0;
        }
    }
    
    if(iTopOpen != iTopClose)
        retVal = 0;
    else
    {
        printf("\niTop:%d iClose:%d", iTopOpen, iTopClose);
        for(i = 0; i<iTopOpen; i++)
        {
          printf("\nopen:%u close:%u", openStack[i].uType,closeStack[iTopOpen-i-1].uType);
		  printf("\nopenIdx:%u closeIdx:%u", openStack[i].iIdx,closeStack[iTopOpen-i-1].iIdx); 	
          if(openStack[i].uType == closeStack[iTopOpen-i-1].uType \
          		&& openStack[i].iIdx < closeStack[iTopOpen-i-1].iIdx) //Brackets do not correspond each other.
          {
            retVal = 1;
          }
          else
          {
          	retVal = 0;
          	break;
          }
        }
    }
    
    return retVal;
}

char A[] = "({[]})";
char B[] = ")(";
char C[] = "{[()()]}";

int main(int argc, char *argv[])
{
    fprintf(stdout,"\nresult:%d\n",solution(A));
}

