#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
/* For sound effect: In [Project Options]->[Parameters]->[Linker] add the parameter -lwinmm */

#include "console_draw3.h"
#include "kb_input2.h"
/*
drawChar()  �̫��ӰѼƤ��O�O�r���I���M�e�����C��
�i�H�ѦҤU�����C��ȹ�Ӫ�ӳ]�w�A�Q�n���C��
   0: ��     1: �t��   2: �t��   3: �t�C
   4: �t��   5: �t��   6: �t��   7: �L��
   8: �t��   9: �G��  10: �G��  11: �G�C
  12: �G��  13: �G��  14: �G��  15: ��
*/
#define WIDTH     20
#define HEIGHT    20
#define HEIGHT_EXT    21
#define OFFSET_X  3
#define OFFSET_Y  3
#define NUM_KEYS 20
#define NOW 9999
/* �۩w�� struct   �ΨӴy�z������U���ݩ� */
struct t_block {
    int x;
    int y;
    int color;
    int duration;
    int last_tick;
    int stopped;
    int shape[3][3];
};
typedef struct t_block Block;
struct t_game {
    int plate[HEIGHT_EXT][WIDTH];
    int last_tick;//0
    int IsEnding; //0
    int score;//0
    int IsAuto;// 1;
    int refresh_rate;
    Block blk;
};
typedef struct t_game Game;
int timer(float sec);
void setupGame(Game *g);
void showPlate(Game *g);
void genBlock(Block *pblk);
void showBlock(Block *pblk);
int downBlock(Game *g);
int moveBlock(Game *g, int newx, int newy);
void fillPlate(Game *g);
int rotateBlock(Game *g);
void checkPlate(Game *g);
int autoPlay(Game *g);

void setupGame(Game *g)
{
    int i, j;
    g->last_tick = 0;
    g->IsEnding = 0;
    g->score = 0;
    g->IsAuto = 1;
    g->refresh_rate = 20;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            g->plate[i][j] = 0;
        }
    }
    /* �Q�W���䪺����M�̩��U���a�O�@*/
    for (i = 0; i < HEIGHT; i++) {
        g->plate[i][0] = 1;
        g->plate[i][WIDTH-1] = 1;
    }
    for (j = 0; j < WIDTH; j++) {
        g->plate[HEIGHT-1][j] = 1;
        g->plate[HEIGHT_EXT-1][j] = 1;  /* �o�@�B�n�p��  �a�O�h�@�h�O���F�����ɤ�K */
    }
    genBlock(&(g->blk));
}

int main(void)
{
    char logo[] = "SAMPLE PROGRAM PROVIDED BY I2P";
    Game tetris;
    char str[80];
    int cur_tick;
    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP, VK_SHIFT, VK_ESCAPE};
    int k;

    /* ���Ͷü� seed    ���᭱�I�s rand() �i�H�o�줣�P���üƧǦC  */
    srand(time(NULL));
    setupGame(&tetris);
    /* �Ұ���L���� */
    initializeKeyInput();
    /* �L�a�j��  �̭��]�t�F�o�ӵ{�����D�n�u�@
    �ӥB�C���j��|���_��̷s���e����ܨ�ù��W  ���O���ʵe�@�� �C���j���s�@���e�� */
    while (!tetris.IsEnding) {
        clearScreen();
        showPlate(&tetris);
        /* ���p block �w�g�I��a�O�Ӱ���  �N���ͷs�� block   �åB�� stopped ���A�M�� */
        if (tetris.blk.stopped) {
            genBlock(&(tetris.blk));
            tetris.blk.stopped = downBlock(&tetris);
            if (tetris.blk.stopped)
                break;
        }
        if (tetris.IsAuto)
            tetris.blk.duration = 8;

        showBlock(&(tetris.blk));

        drawString(logo, OFFSET_X, OFFSET_Y-2,  3, 14);
        sprintf(str, "Score: %d", tetris.score );
        drawString(str, OFFSET_X, OFFSET_Y-1, 3, 14);

        cur_tick = timer(0.005); /* �C�� tick 0.005 �� */

        if (cur_tick-tetris.last_tick > tetris.refresh_rate) {
            tetris.last_tick = cur_tick;

            /* ���F�n���@�s�� drawChar() ���ʧ@���ͮĪG
               �����n�I�s�@�� drawCmdWindow() �⤧�e�e���������e�@����ܨ�ù��W */
            drawCmdWindow();

        }
        /* ��L���� */
        for (k=0; k<NUM_KEYS; k++) {
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;
                switch (key_val[k]) {
                case VK_SHIFT:
                    tetris.IsAuto = !tetris.IsAuto;
                    break;
                case VK_ESCAPE:
                    tetris.IsEnding = 1;
                    break;
                case VK_LEFT:
                    tetris.IsAuto = 0;
                    moveBlock(&tetris, tetris.blk.x - 1, tetris.blk.y);
                    break;
                case VK_RIGHT:
                    tetris.IsAuto = 0;
                    moveBlock(&tetris, tetris.blk.x + 1, tetris.blk.y);
                    break;
                case VK_DOWN:
                    tetris.blk.duration = 1;
                    break;
                case VK_UP:
                    rotateBlock(&tetris);
                    break;
                }
            }
            if(KEY_UP(key_val[k]) && key_down[k]) {
                key_down[k] = 0;
            }
        }
        if (tetris.IsAuto) {
            moveBlock(&tetris, tetris.blk.x + autoPlay(&tetris), tetris.blk.y);
        }
        tetris.blk.stopped = downBlock(&tetris);
        /* �p�G�Q�צ�  �ثe�� block �N�n�ܦ� plate ���@���� */
        if (tetris.blk.stopped) {
            fillPlate(&tetris);
        }
        checkPlate(&tetris);
    } /* while (1) */
    return 0;
}

int timer(float sec)
{
    static clock_t StartC; /* �Q�� static �ܼ�  �֭p */
    static int Tick; /* �C�g�L sec �� �N�� Tick �[�@, �]�N�O�� �C�� Tick �۷�� sec �� */
    if ((double)(clock()-StartC) > sec*CLOCKS_PER_SEC) { /* �� if ����P�_�@�� �ˬd�O�_�w�F��w�w���ɶ� */
        Tick++;
        StartC = clock();
    }
    return Tick;
}
/****************************************************************************************************/
void genBlock(Block *pblk)
{
    pblk->x = WIDTH/2-1;
    pblk->y = 0;
    pblk->color = rand()%7 + 9; /* �u�� 7 �ثG�� */
    pblk->duration = 40;
    pblk->last_tick = 0;
    pblk->stopped = 0;
    switch (rand()%6) {
    case 0:
        pblk->shape[0][0] = 0;
        pblk->shape[0][1] = 0;
        pblk->shape[0][2] = 0;
        pblk->shape[1][0] = 1;
        pblk->shape[1][1] = 0;
        pblk->shape[1][2] = 0;
        pblk->shape[2][0] = 1;
        pblk->shape[2][1] = 1;
        pblk->shape[2][2] = 1;
        break;
    case 1:
        pblk->shape[0][0] = 0;
        pblk->shape[0][1] = 0;
        pblk->shape[0][2] = 0;
        pblk->shape[1][0] = 1;
        pblk->shape[1][1] = 1;
        pblk->shape[1][2] = 0;
        pblk->shape[2][0] = 1;
        pblk->shape[2][1] = 1;
        pblk->shape[2][2] = 0;
        break;
    case 2:
        pblk->shape[0][0] = 0;
        pblk->shape[0][1] = 0;
        pblk->shape[0][2] = 0;
        pblk->shape[1][0] = 1;
        pblk->shape[1][1] = 1;
        pblk->shape[1][2] = 0;
        pblk->shape[2][0] = 0;
        pblk->shape[2][1] = 1;
        pblk->shape[2][2] = 1;
        break;
    case 3:
        pblk->shape[0][0] = 0;
        pblk->shape[0][1] = 0;
        pblk->shape[0][2] = 0;
        pblk->shape[1][0] = 0;
        pblk->shape[1][1] = 1;
        pblk->shape[1][2] = 0;
        pblk->shape[2][0] = 1;
        pblk->shape[2][1] = 1;
        pblk->shape[2][2] = 1;
        break;
    case 4:
        pblk->shape[0][0] = 0;
        pblk->shape[0][1] = 0;
        pblk->shape[0][2] = 0;
        pblk->shape[1][0] = 0;
        pblk->shape[1][1] = 0;
        pblk->shape[1][2] = 1;
        pblk->shape[2][0] = 1;
        pblk->shape[2][1] = 1;
        pblk->shape[2][2] = 1;
        break;
    case 5:
        pblk->shape[0][0] = 0;
        pblk->shape[0][1] = 0;
        pblk->shape[0][2] = 0;
        pblk->shape[1][0] = 0;
        pblk->shape[1][1] = 1;
        pblk->shape[1][2] = 1;
        pblk->shape[2][0] = 1;
        pblk->shape[2][1] = 1;
        pblk->shape[2][2] = 0;
        break;
    }
}
void showBlock(Block *pblk)
{
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (pblk->shape[i][j] == 1) {
                drawChar( 0xA1, (pblk->x+j)*2 + OFFSET_X, (pblk->y+i) + OFFSET_Y, 0, pblk->color);
                drawChar( 0xBD, (pblk->x+j)*2 + 1 + OFFSET_X, (pblk->y+i) + OFFSET_Y, 0, pblk->color);
            }
        }
    }
}
int downBlock(Game *g)
{
    int i, j;
    int valid_move = 1;
    int newy;
    int newx;
    Block *pblk;
    int cur_tick = timer(NOW);
    pblk = &(g->blk);
    newx = pblk->x;
    newy = pblk->y+1;
    if (cur_tick-pblk->last_tick > pblk->duration) {
        pblk->last_tick = cur_tick;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (pblk->shape[i][j] && g->plate[newy+i][newx+j])
                    valid_move = 0;
            }
        }
        if (valid_move) {
            pblk->x = newx;
            pblk->y = newy;
        } else if (pblk->y < newy) {
            return 1; /* stopped by some blocks underneath */
        }
    }
    return 0;
}
int moveBlock(Game *g, int newx, int newy)
{
    int i, j;
    int valid_move = 1;
    Block *pblk;
    pblk = &(g->blk);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (pblk->shape[i][j] && g->plate[newy+i][newx+j])
                valid_move = 0;
        }
    }
    if (valid_move) {
        pblk->x = newx;
        pblk->y = newy;
    } else if (pblk->y < newy) {
        return 1; /* stopped by some blocks underneath */
    }
    return 0;
}
void fillPlate(Game *g)
{
    int i, j;
    Block *pblk;
    pblk = &(g->blk);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            g->plate[pblk->y+i][pblk->x+j] = g->plate[pblk->y+i][pblk->x+j] | pblk->shape[i][j];  /* bit operator OR*/
        }
    }
}
int rotateBlock(Game *g)
{
    int i;
    int idx[] = {2, 5, 8, 1, 4, 7, 0, 3, 6};
    int tmp[3][3];
    Block *pblk;
    pblk = &(g->blk);

    for (i = 0; i < 9; i++) {
        *(&tmp[0][0] + i) = *(&(pblk->shape[0][0]) + idx[i]);
    }
    for (i = 0; i < 3; i++) {
        if (tmp[i][0] && g->plate[pblk->y+i][pblk->x]) {
            pblk->x++;
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (tmp[i][2] && g->plate[pblk->y+i][pblk->x+2]) {
            pblk->x--;
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (tmp[2][i] && g->plate[pblk->y+2][pblk->x+i]) {
            pblk->y--;
            break;
        }
    }
    for (i = 0; i < 9; i++) {
        *(&(pblk->shape[0][0]) + i) = *(&tmp[0][0] + i);
    }
    return 0;
}
void showPlate(Game *g)
{
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (g->plate[i][j] == 1) {
                drawChar(0xA1, j*2 + OFFSET_X, i + OFFSET_Y,  0, 8);  /* fg_color=8 �N��e�����C��O�Ǧ� */
                //putASCII2( j*2+1 + OFFSET_X, i + OFFSET_Y, 0xBD, 8, 0);
                drawChar(0xBD, j*2+1 + OFFSET_X, i + OFFSET_Y,  0, 8);
            } else {
                drawChar(' ', j*2 + OFFSET_X, i + OFFSET_Y,  0, 0);
                drawChar(' ', j*2+1 + OFFSET_X, i + OFFSET_Y,  0, 0);
            }
        }
    }
}
void checkPlate(Game *g)
{
    int i, j, cut, t=0;
    for (i = HEIGHT-2; i > 1; i--) {
        cut = 1;
        for (j = 1; j < WIDTH-1; j++) {
            if (!g->plate[i][j]) {
                cut = 0;
                break;
            }
        }
        if (cut) {
            t = i;
            g->score++;
            break;
        }
    }
    for (i = t; i > 1; i--) {
        for (j = 1; j < WIDTH-1; j++) {
            g->plate[i][j] = g->plate[i-1][j];
        }
    }
}
int autoPlay(Game *g)
{
    int i, j, k, m, n;
    int stopped;
    int best_j = 1, best_k = 0;
    int best_value=0;
    int value;
    Game tmp_game;
    /* ���թҦ��i�઺���ʩM����覡 ��C�@�ؤ覡���p���� */
    for (j=1; j<WIDTH-2; j++) {
        tmp_game = *g;
        for (k=0; k<3; k++) {
            for (i=1; i<HEIGHT-1; i++) {
                stopped = moveBlock(&tmp_game, j, i);
                if (stopped) break;
                if  (tmp_game.blk.x == j && tmp_game.blk.y == i) {
                    value = 0;
                    for (m=0; m<3; m++) {
                        for (n=0; n<3; n++) {
                            if (tmp_game.plate[tmp_game.blk.y+m][tmp_game.blk.x+n]) {
                                value = value + 5*m;
                            }

                            if (tmp_game.blk.shape[m][n])
                                value = value + 10*m;
                        }
                    }
                    value = value + 18*i;
                    if (value > best_value) {
                        best_value = value;
                        best_j = j;
                        best_k = k;
                    }
                }
            }
            rotateBlock(&tmp_game);
        }
    }
    for (k=0; k<best_k; k++)
        rotateBlock(g);
    return best_j-g->blk.x;
}
