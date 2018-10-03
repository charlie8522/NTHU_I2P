#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a;
    scanf("%d",&a);
    bi_add(a+1);
    return 0;
}

int bi_add(int a)
{
    int temp = a;
    int b[1024];
    int count = 0;
    int count1 = 0;
    for(int i = 0 ; ; i++)
    {
        b[i] = a % 2;
        if(a / 2 == a){break;}
        a = a / 2 ;
        count++;
    }
    for(int j = count-1; j >= 0; j--)
    {
        printf("%d",b[j]);
    }
    printf(" ");


    a = temp - 1;
    int c[1024];
    for(int k = 0 ; ; k++)
    {
        c[k] = a % 2;
        if(a / 2 == a){break;}
        a = a / 2 ;
        count1++;
    }
    int count2 = 0;
    for(int f = 0; f < count ;f++)
    {
        if (c[f] == 1){count2++;}
        else {break;}
    }
    printf("%d",count2);
}
