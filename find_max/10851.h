#ifndef FUNCTION_H
#define FUNCTION_H

void execInst(int *ptrArr[], char inst, int param1, int param2)
{
     if(inst=='S'){
    *ptrArr[param1] = param2;
    }
    else if(inst=='M'){
    *ptrArr[param1] = param2**(ptrArr[param1]) ;

    }
    else if(inst=='P'){
    ptrArr[param1] = ptrArr[param2];
    }
};
int findMax(int *ptrArr[], int start, int end)
{
    int max = 0;
    for(int i = start; i <= end; i++)
    {
        if (*ptrArr[i] > max){max = *ptrArr[i];}
    }
    return max;
};

#endif
