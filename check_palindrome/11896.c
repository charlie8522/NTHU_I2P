#include <stdio.h>
#include <string.h>
#include "11896.h"

#ifdef ONLINE_JUDGE
#include "NTHUOJLock"
#endif

int main()
{
    char input[1005];
    scanf("%s", input);
    int len = strlen(input);
    for(int i = len; i > 1; --i){  //length of substring
        for(int start = 0; start <= len - i; ++start){  //start position
            if(isPalindrom(input, start, i + start - 1) == 1)
                printf("Yes\n");
            else
                printf("No\n");
        }
    }

#ifdef ONLINE_JUDGE
    nthuoj_unlock();
#endif
    return 0;
}
