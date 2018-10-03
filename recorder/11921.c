#include <stdio.h>
#include "11921.h"

#ifdef ONLINE_JUDGE  //you don't need to pay attention to it
#include "NTHUOJLock"
#endif

int main(void){
    int num1, num2, num3, num4;
    scanf("%d %d %d %d", &num1, &num2, &num3, &num4);
    reorder(&num1, &num2, &num3, &num4);
    printf("order number: %d %d %d %d\n", num1, num2, num3, num4);

#ifdef ONLINE_JUDGE  //you don't need to pay attention to it
    nthuoj_unlock();
#endif
    return 0;
}
