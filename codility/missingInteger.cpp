// you can use includes, for example:
#include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // write your code in C++14 (g++ 6.2.0)
    int retVal = 1;
    
    sort(A.begin(), A.end());
    
    for(unsigned int i=0; i<A.size(); i++)
    {
        if(A[i] < 0)
        {
            continue;
        }    
        else
        {
            if((i<(A.size()-1)) && ((A[i+1] - A[i]) > 1))
            {
                return ++A[i];
            }
            
        } 
    }
    
    if(A.back() < 0)
        return 1;
    else
        return ++A.back();
}
