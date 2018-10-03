#include <stdio.h>
#include <stdlib.h>

int main()
{
    int level,width,height = 0;
    //scanf("%d",&level);
    scanf("%d %d %d",&level,&width,&height);
    //LEAVE

    if (level*2-1 >= width)
    {
        for(int i = 1; i <= level; i++)
        {
            int tmp = level;
            for(int j = i; j <= level-1; j++)
            {
                printf(" ");
                tmp--;
            }
            int need = 1+(tmp-1)*2;
            for(int k = 0; k < need; k++)
            {
                printf("*");
            }
            printf("\n");
        }
    }
    else
    {
        for(int i = 1; i <= level; i++)
        {
            int tmp = level;
            for(int j = i; j <= level-1; j++)
            {
                printf(" ");
                tmp--;
            }
            int need = (width-(level*2-1))/2;
            for(int u = 0; u < need; u++)
            {
                printf(" ");
            }
            int need2 = 1+(tmp-1)*2;
            for(int k = 0; k < need2; k++)
            {
                printf("*");
            }
            printf("\n");
        }
    }

    //TREE
    int space = (1+(level-1)*2)/2-width/2;
    for(int m = 0; m < height; m++)
    {
        for(int n = 0; n < space; n++)
        {
            printf(" ");
        }
        for(int p = 0; p < width; p++)
        {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}
