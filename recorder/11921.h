#ifndef FUNCTION_H
#define FUNCTION_H

void cmp_and_swap(int first, int second)
{
    if(first >= second)
    {
        int tmp;
        tmp = first;
        first = second;
        second = tmp;
    }
};
void reorder(int* n1, int* n2, int* n3, int* n4)
{
    //printf("%d %d %d %d\n",*n1,*n2,*n3,*n4);
    cmp_and_swap(n1,n2);
    //printf("%d %d %d %d\n",*n1,*n2,*n3,*n4);
    cmp_and_swap(n1,n3);
    //printf("%d %d %d %d\n",*n1,*n2,*n3,*n4);
    cmp_and_swap(n1,n4);
    //printf("%d %d %d %d\n",*n1,*n2,*n3,*n4);
    cmp_and_swap(n2,n3);
    //printf("%d %d %d %d\n",*n1,*n2,*n3,*n4);
    cmp_and_swap(n2,n4);
    //printf("%d %d %d %d\n",*n1,*n2,*n3,*n4);
    cmp_and_swap(n3,n4);
    //printf("%d %d %d %d\n",*n1,*n2,*n3,*n4);


};

#endif
