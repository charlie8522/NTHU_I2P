#include <stdio.h>

#define RIGHT 0
#define LEFT  1
#define UP    2
#define DOWN  3


int main(void)
{
    int N, dir, x, y;  // input numbers
    int S[20][20] = {0};
    int i, j;          // loop iterator
    int current_dir;   // UP/LEFT/DOWN/RIGHT
    int change_count;  // number of direction changes

    // get inputs
    scanf("%d%d%d%d", &N, &dir, &x, &y);

    if (dir == 0) //clockwise
    {
        i = 0;               // the first element is 0
        S[x][y] = -1;        // the first should be 0, but if we put 0,
                             // we cannot distinguish it from empty cells.
                             // so we let it be -1, and change it back to 0 later.
        current_dir = RIGHT; // initial direction is RIGHT
        change_count = 0;    // if we change direction twice without any movement, we should stop

        while(i<N*N) {       // while not reaching the last element
            switch(current_dir){   // use current direction to decide what to do
                case RIGHT:
                    // if the right, (x, y+1), has space and not on the boundary
                    if (y+1<N && S[x][y+1]==0){
                        S[x][++y] = ++i;     // move to the right
                        change_count = 0;    // since we have a movement, change_count should be reset.
                    } else {                 // the right has no space to move.
                        current_dir = DOWN;  // since it is clockwise, change current direction to DOWN
                        change_count++;      // increase the counter of changing direction.
                    }
                    break;
                case LEFT:
                    // if the left, (x, y-1), has space and not on the boundary
                    if (y-1>=0 && S[x][y-1]==0){
                        S[x][--y] = ++i;
                        change_count = 0;
                    } else {
                        current_dir = UP;
                        change_count++;
                    }
                    break;
                case UP:
                    // if the up, (x-1, y), has space and not on the boundary
                    if (x-1>=0 && S[x-1][y]==0){
                        S[--x][y] = ++i;
                        change_count = 0;
                    } else {
                        current_dir = RIGHT;
                        change_count++;
                    }
                    break;
                case DOWN:
                    // if the down, (x+1, y), has space and not on the boundary
                    if (x+1<N && S[x+1][y]==0){
                        S[++x][y] = ++i;
                        change_count = 0;
                    } else {
                        current_dir = LEFT;
                        change_count++;
                    }
                    break;
            }
            if (change_count >= 2) break;
        }

    } else if (dir == 1) // counterclockwise
    {
        i = 0;               // the first element is 0
        S[x][y] = -1;        // the first should be 0, but if we put 0,
                             // we cannot distinguish it from empty cells.
                             // so we let it be -1, and change it back to 0 later.
        current_dir = LEFT;  // initial direction is LEFT
        change_count = 0;    // if we change direction twice without any movement, we should stop

        while(i<N*N) {       // while not reaching the last element
            switch(current_dir){   // use current direction to decide what to do
                case RIGHT:
                    // if the right, (x, y+1), has space and not on the boundary
                    if (y+1<N && S[x][y+1]==0){
                        S[x][++y] = ++i;     // move to the right
                        change_count = 0;    // since we have a movement, change_count should be reset.
                    } else {                 // the right has no space to move.
                        current_dir = UP;    // since it is clockwise, change current direction to DOWN
                        change_count++;      // increase the counter of changing direction.
                    }
                    break;
                case LEFT:
                    // if the left, (x, y-1), has space and not on the boundary
                    if (y-1>=0 && S[x][y-1]==0){
                        S[x][--y] = ++i;
                        change_count = 0;
                    } else {
                        current_dir = DOWN;
                        change_count++;
                    }
                    break;
                case UP:
                    // if the up, (x-1, y), has space and not on the boundary
                    if (x-1>=0 && S[x-1][y]==0){
                        S[--x][y] = ++i;
                        change_count = 0;
                    } else {
                        current_dir = LEFT;
                        change_count++;
                    }
                    break;
                case DOWN:
                    // if the down, (x+1, y), has space and not on the boundary
                    if (x+1<N && S[x+1][y]==0){
                        S[++x][y] = ++i;
                        change_count = 0;
                    } else {
                        current_dir = RIGHT;
                        change_count++;
                    }
                    break;
            }
            if (change_count >= 2) break;
        }
    }

    // print out the results
    for(i=0; i<N; i++){
        for (j=0; j<N; j++)
            if (S[i][j]==-1)  // since we let the first element be -1, we need to
                printf(" 0"); // change it back to 0.
            else
                printf(" %d", S[i][j]);
        printf("\n");
    }
    return 0;
}
