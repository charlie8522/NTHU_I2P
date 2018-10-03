#include <stdio.h>
#include "10806.h"

int main(){

    int start, end, num;

    scanf("%d %d", &start, &end);

    num = numPrime(start, end);

    printf("%d", num);

    return 0;

}
