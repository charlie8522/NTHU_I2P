#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
/* ���İO�o�n�[�J�o�� */
#include "audio.h"
/* �t�~�٦����U���]�w */
/* For sound effect: In [Project Options]->[Parameters]->[Linker] add the parameter -lwinmm */


#include "console_draw3.h"
#include "kb_input2.h"
#include "font.h"
#include "image.h"
#include "framework.h"

/*
drawChar()  �̫��ӰѼƤ��O�O�r���I���M�e�����C��
�i�H�ѦҤU�����C��ȹ�Ӫ�ӳ]�w�A�Q�n���C��
   0: ��     1: �t��   2: �t��   3: �t�C
   4: �t��   5: �t��   6: �t��   7: �L��
   8: �t��   9: �G��  10: �G��  11: �G�C
  12: �G��  13: �G��  14: �G��  15: ��
*/

#define WIDTH     30
#define HEIGHT    20
#define OFFSET_X  5
#define OFFSET_Y  5

#define NUM_KEYS 7
#define REFRESH_RATE 10


int timer(void);

int my_game_one(void);
int my_game_two(void);

void setMainmenu(Menu *m);


int main(void)
{
    int IsEnding = 0;
    int k;
    char str[40] = {'\0'};
    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_UP, VK_DOWN, VK_ESCAPE, VK_RETURN, VK_SPACE};
    int cur_tick, last_tick;
    Menu mainmenu;
    Image* background;
    background = read_image("background.pixel", "background.color");
    Audio audio;
    openAudioFile("metalplate.wav", &audio);

    /* �Ұ���L���� ��ӵ{�����u�n���@���N��F*/
    initializeKeyInput();
    cur_tick = last_tick = timer();

    setMainmenu(&mainmenu);

    show_image(background, 0, 0);
    saveScreen();

    /* �L�a�j��  �̭��]�t�F�o�ӵ{�����D�n�u�@
    �ӥB�C���j��|���_��̷s���e����ܨ�ù��W
    ���O���ʵe�@�� �C���j���s�@���e�� */
    while (!IsEnding) {
        /* �C�g�L REFRESH_RATE �� ticks �~�|��s�@���e�� */
        cur_tick = timer(); /* �C�� tick 0.01 �� */
        sprintf(str, "%10d", cur_tick/1000);

        if (cur_tick-last_tick > REFRESH_RATE) {

            last_tick = cur_tick;


           clearScreen();
           restoreScreen();

            putStringLarge(mainmenu.large_font, OFFSET_X, OFFSET_Y-1, str, 14);

            /* ����e�X�� */
            showMenu(&mainmenu);

            /* ���F�n���@�s�� drawChar() ���ʧ@���ͮĪG
               �����n�I�s�@�� drawCmdWindow() �⤧�e�e���������e�@����ܨ�ù��W */
            drawCmdWindow();

        } /* end of if (cur_tick % REFRESH_RATE == 0 ... */

        for (k=0; k<NUM_KEYS; k++) {
            /* ����q�쥻�Q���U�����A �ܦ���}�����A  �o�O���F�B�z���ۤ��񪺱��p */
            if(KEY_UP(key_val[k]) && key_down[k]) {
                key_down[k] = 0;
            }
        }

        /* ��L���� �B�z���ۤ��񪺪��p */
        for (k=0; k<NUM_KEYS; k++) {
            /* ����q�쥻�Q���U�����A �ܦ���}�����A  �o�O���F�B�z���ۤ��񪺱��p */
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;

                switch (key_val[k]) {
                case VK_UP:
                    scrollMenu(&mainmenu, -1);
                    break;
                case VK_DOWN:
                    scrollMenu(&mainmenu, +1);
                    break;
                case VK_RETURN:
                    if (IsOnItem(&mainmenu, 2)) {
                        if (IsItemSelected(&mainmenu, 0)) {
                            playAudio(&audio);
                            my_game_one();
                        }
                        if (IsItemSelected(&mainmenu, 1)) {
                            playAudio(&audio);
                            my_game_two();
                        }

                    } else if (IsOnItem(&mainmenu, 3)) {
                        IsEnding = 1;
                    }
                    break;
                case VK_SPACE:
                    /* �i�H�ոլݩ��U�t�@�ؿ��Φ�
                        �P�ɯ঳�h�Ӷ��إi�H�Q���
                        toggleMenu(&mainmenu);
                    */
                    radioMenu(&mainmenu);
                    break;
                }
            }
        }
    } /* while (IsEnding) */
    return 0;
}
int timer(void)
{
    return (clock()/(0.001*CLOCKS_PER_SEC));
}
void setMainmenu(Menu *m)
{
    int i;

    m->large_font = read_font("font.txt");
    m->x = 2;
    m->y = 2;
    m->fgcolor = 7;
    m->bgcolor = 15;
    m->num_options = 5;
    m->cursor = 0;
    for (i=0; i<m->num_options; i++) {
        m->state[i] = 0;
    }
    m->state[m->cursor] = m->state[m->cursor] | 1;  /* �ثe��ܪ����� */

    strcpy(m->text[0], "ONE PLAYER");
    strcpy(m->text[1], "TWO PLAYERS");
    strcpy(m->text[2], "START");
    strcpy(m->text[3], "EXIT");
    strcpy(m->text[4], "ABOUT");
    strcpy(m->alt_text[0], "ONE PLAYER *");
    strcpy(m->alt_text[1], "TWO PLAYERS *");
    strcpy(m->alt_text[2], "START *");
    strcpy(m->alt_text[3], "EXIT *");
    strcpy(m->alt_text[4], "ABOUT *");
}
/*--------------------------------------------------------------*/
int my_game_one(void)
{
    int IsEnding = 0;
    char logo[] = "MY BRILLIANT (OR FUNNY) GAME";
    int i, j, k;
    char str[40] = {'\0'};
    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_UP, VK_DOWN, VK_ESCAPE, VK_RETURN};
    int cur_tick, last_tick;

    /* �Ұ���L���� ��ӵ{�����u�n���@���N��F*/
    initializeKeyInput();
    cur_tick = last_tick = timer();

    while (!IsEnding) {
        /* �C�g�L REFRESH_RATE �� ticks �~�|��s�@���e�� */
        cur_tick = timer();
        if (cur_tick-last_tick > REFRESH_RATE) {
            last_tick = cur_tick;
            clearScreen();

            i = rand()%15;
            j = rand()%15;
            drawChar(0xA1, j*2 + OFFSET_X, i + OFFSET_Y,  0, 8);
            drawChar(0xBD, j*2+1 + OFFSET_X, i + OFFSET_Y,  0, 8);
            drawString(logo,OFFSET_X, OFFSET_Y-2,3,14);
            sprintf(str, "%10d", timer()/100);
            drawString(str,OFFSET_X, OFFSET_Y-1,1,14);
            drawCmdWindow();
        } /* end of if (cur_tick % REFRESH_RATE == 0 ... */
        for (k=0; k<NUM_KEYS; k++) {
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;
                switch (key_val[k]) {
                case VK_ESCAPE:
                    IsEnding = 1;
                    break;
                }
            }
            if(KEY_UP(key_val[k]) && key_down[k]) {
                key_down[k] = 0;
            }
        }
    } /* while (IsEnding) */
    return 0;
}
int my_game_two(void)
{
    int IsEnding = 0;
    char logo[] = "MY BRILLIANT (OR FUNNY) GAME";
    int i, j, k;
    char str[40] = {'\0'};
    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_UP, VK_DOWN, VK_ESCAPE, VK_RETURN};
    int cur_tick, last_tick;

    /* �Ұ���L���� ��ӵ{�����u�n���@���N��F*/
    initializeKeyInput();
    cur_tick = last_tick = timer();

    while (!IsEnding) {
        /* �C�g�L REFRESH_RATE �� ticks �~�|��s�@���e�� */
        cur_tick = timer();
        if (cur_tick-last_tick > REFRESH_RATE) {
            last_tick = timer();
            clearScreen();
            i = rand()%15;
            j = rand()%15;
            drawChar(0xA1, j*2 + OFFSET_X, i + OFFSET_Y,  0, 8);
            drawChar(0xBD, j*2+1 + OFFSET_X, i + OFFSET_Y,  0, 8);
            drawString(logo,OFFSET_X, OFFSET_Y-2,3,14);
            sprintf(str, "%10d", timer()/100);
            drawString(str,OFFSET_X, OFFSET_Y-1,1,14);
            drawCmdWindow();
        } /* end of if (cur_tick % REFRESH_RATE == 0 ... */

        for (k=0; k<NUM_KEYS; k++) {
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;
                switch (key_val[k]) {
                case VK_ESCAPE:
                    IsEnding = 1;
                    break;
                }
            }
            if(KEY_UP(key_val[k]) && key_down[k]) {
                key_down[k] = 0;
            }
        }
    } /* while (IsEnding) */
    return 0;
}
