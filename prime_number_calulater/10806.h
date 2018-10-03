#ifndef DEFINE_H
#define DEFINE_H
int numPrime(int start, int end)
{
    int ans = 0;
    for(int i = start; i <= end; ++i)
    {
        //printf("%d ",i);
        int flag = 1;
        if(i == 1){flag = 0;}
        for(int j = i-1; j > 1; --j)
        {
            if(i%j == 0){flag = 0;}
        }
        if(flag){ans++;}
        //printf("*%d*\n",ans);
    }
    return ans;
};
#endif
