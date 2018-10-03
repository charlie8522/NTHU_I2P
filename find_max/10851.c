#include <stdio.h>
#include "10851.h"

#define SIZE 100

int main() {
	int *ptrArr[SIZE];
	int dataArr[SIZE] = {0};
	char inst;
	int dataNum, instNum;
	int param1, param2;
	int start, end;
	int i;

	/* input */
	scanf("%d %d", &dataNum, &instNum);

	/* initialize the ptrArr */
	for (i = 0; i < dataNum; i++)
		ptrArr[i] = &dataArr[i];

	for (i = 0; i < instNum; i++) {
		scanf(" %c %d %d", &inst, &param1, &param2);
		execInst(ptrArr, inst, param1, param2);
	}
    scanf("%d %d", &start, &end);
	/* output */
	for (i = 0; i < dataNum - 1; i++) {
		printf("%d ", dataArr[i]);
	}
	printf("%d\n", dataArr[i]);
	for (i = 0; i < dataNum - 1; i++) {
		printf("%d ", *ptrArr[i]);
	}
	printf("%d\n", *ptrArr[i]);


	printf("%d\n", findMax(ptrArr , start, end));

	return 0;
}
