#include <stdio.h>





int calculTermeNFibo(int N)  //ITERATIF
{
    int first=0;
    int second=1;
    int result;
    int i;
    for(i=1;i<N;i++)
        {
        result=first+second;    
        first=second;
        second=result;    
            
        }
    return result;
}


int calculTermeNFiboREC(int N)  //RECURSIF
{
    
    if(N==0) return 0;
    if(N==1) return 1;
    
    
    return calculTermeNFiboREC(N-1)+calculTermeNFiboREC(N-2);
}



int main()
{
    
    
    int N=4;
    printf("\n==>Fibo(%d)=%d\n",N,calculTermeNFibo(N));
    printf("\n==>Fiborec(%d)=%d\n",N,calculTermeNFiboREC(N));
    
    
    
    
}
