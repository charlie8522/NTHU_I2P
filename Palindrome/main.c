#include <stdio.h>
#include <string.h>

int main()
{
    char a[100000];
    while(scanf("%s",&a) != EOF)
    {
        int length = strlen(a);
        int flag = 1;
        for(int i = 0; i < length/2; i++)
        {
            if(a[i] != a[length-i-1]){flag = 0;printf("No\n");break;}
        }
        if(flag)
        {printf("Yes\n");}
    }
    return 0;
}
