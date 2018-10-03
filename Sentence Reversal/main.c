# include <stdio.h>
# include <stdlib.h>

int main(int argc, char const *argv[])
{
    char line[1024];
    int len = 0;
    while (fgets( line, 1024, stdin) != NULL)
    {
        if (line[0] == '\0') continue;
        for (int i = 0; i < 1024; ++i)
        {
            if (line[i] == '\0')
            {
                len = i;
                break;
            }
        }
        for (int i = len; i >= 0; --i)
        {
            if (line[i] == ' ')
            {
                int j = i + 1;
                while (line[j] != ' ' && line[j] != '\n' && line[j] != '\0')
                {
                    printf("%c", line[j]);
                    ++j;
                }
                printf(" ");
            }
            else if (i == 0)
            {
                int j = i;
                while (line[j] != ' ' && line[j] != '\n' && line[j] != '\0')
                {
                    printf("%c", line[j]);
                    ++j;
                }
            }
        }
        printf("\n");
    }
    return 0;
}
