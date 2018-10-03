#ifndef FUNCTION_H
#define FUNCTION_H

int isPalindrom(char str[], int start, int end)
{
    while(start < end)
    {
        if(str[start] != str[end]){return 0;}
        else{start++;end--;}
    }
    return 1;
};

#endif
