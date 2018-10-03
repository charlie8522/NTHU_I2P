#include <stdio.h>

int main()
{
	char ch;
	char arr[1000];
	int i=-1;

	while(1)
	{
		if(scanf("%c", &ch)==EOF)
			break;

		if((ch == ' ')||(ch=='\n')||(ch=='\t'))
		{
			while(i>=0)
			{
				printf("%c", arr[i]);
				i--;
			}
			printf("%c", ch);
		}
		else
		{
			arr[++i] = ch;
		}
	}
	return 0;
}
