#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Done 26

int main()
{
    int square_size,clockwise,initial_row,initial_column;
    scanf("%d %d %d %d",&square_size,&clockwise,&initial_row,&initial_column);

    int num = 0;
    char matrix[20][20] = {0};

    matrix[initial_row][initial_column] = num;

    while(num <= square_size*square_size)
    {
        if(~clockwise) //¶¶®É°w
        {
            for(int i = 1; i < size - initial_column; i++) //Round 1 Right
            {
                num++;
                matrix[initial_row][initial_column + i] = num;
                if (initial_row == square_size-1 && (initial_column+i) == square_size-1) //If is on final row
                {
                    for(int j = 0; j < square_size; j++)
                    {
                        for(int k = 0; k < square_size-1; k++)
                        {
                            printf("%d ",matrix[j][k]);
                        }
                        printf("%d\n",matrix[j][square_size]);
                    }
                    num = Done;
                }
            }
            for(int i = 1; i < size - initial_row; i++) //Round 1 Down
            {
                num++;
                matrix[initial_row + i][square_size] = num;
            }
            for(int i = 1; i < size - initial_row; i++) //Round 1 Left
            {
                num++;

            }
            for(int i = 1; i < size - initial_row; i++) //Round 1 UP
            {
                num++;

            }

        }
        else //°f®É°w
        {


        }
    }






    return 0;
}


    int amount = size*4 - 4;

    // up: 1 right: 2 down: 3 left: 4
    int direct = 2;
    int change_direct = 0;
    // start row, col
    i = 1;
    j = 1;

    while (amount < size*size)
    {
        input = getchar();
        if (direct == 1)
        {
            if (change_direct)
            {
                i -= 1;
                change_direct = 0;
            }
            output[i][j] = input;

            if (output[i - 1][j] != ')')
            {
                direct = 2;
                change_direct = 1;
            }
            else
            {
                i -= 1;
            }
        }
        else if (direct == 2)
        {
            if (change_direct)
            {
                j += 1;
                change_direct = 0;
            }
            output[i][j] = input;

            if (output[i][j + 1] != ')')
            {
                direct = 3;
                change_direct = 1;
            }
            else
            {
                j += 1;
            }
        }
        else if (direct == 3)
        {
            if (change_direct)
            {
                i += 1;
                change_direct = 0;
            }
            output[i][j] = input;

            if (output[i + 1][j] != ')')
            {
                direct = 4;
                change_direct = 1;
            }
            else
            {
                i += 1;
            }

        }
        else if (direct == 4)
        {
            if (change_direct)
            {
                j -= 1;
                change_direct = 0;
            }
            output[i][j] = input;

            if (output[i][j - 1] != ')')
            {
                direct = 1;
                change_direct = 1;
            }
            else
            {
                j -= 1;
            }
        }
        amount++;
    }

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            printf("%c", output[i][j]);
        }
        printf("\n");
    }
