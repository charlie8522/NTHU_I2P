#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    char str1[1000];
    while(scanf("%s",&str1) != EOF)
    {
        int length = strlen(str1);
        for(int i = length-1; i >= 0; i--)
        {
            printf("%c",str1[i]);
        }
        printf("\n");
    }
    return 0;
}
