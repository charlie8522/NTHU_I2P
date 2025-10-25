#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
/* 音效記得要加入這個 */
#include "audio.h"
/* 另外還有底下的設定 */
/* For sound effect: In [Project Options]->[Parameters]->[Linker] add the parameter -lwinmm */


#include "console_draw3.h"
#include "kb_input2.h"
#include "font.h"
#include "image.h"
#include "framework.h"

/*
drawChar()  最後兩個參數分別是字元背景和前景的顏色
可以參考下面的顏色值對照表來設定你想要的顏色
   0: 黑     1: 暗藍   2: 暗綠   3: 暗青
   4: 暗紅   5: 暗紫   6: 暗黃   7: 淺灰
   8: 暗灰   9: 亮藍  10: 亮綠  11: 亮青
  12: 亮紅  13: 亮紫  14: 亮黃  15: 白
*/

#define WIDTH     30
#define HEIGHT    20
#define OFFSET_X  5
#define OFFSET_Y  5

#define NUM_KEYS 7
#define REFRESH_RATE 10

struct c_Character
{
    int x;
    int y;
    int color;
    int HP;
    int last_tick;
    int shape[3][3];
};
typedef struct c_Character Char;
int Char_SHAPE[0][1] =
{
    {
        0, 1, 0,
        3, 2, 3,
        0, 4, 0,
    }
};

int timer(void);
int plate[HEIGHT][WIDTH] = {{0}};
int my_game_one(void);
int my_game_two(void);
void gen_background(void);
void gen_char(Char *char_p);
void gen_enemy(Char *char_p);
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
    Audio audio[2];
    openAudioFile("metalplate.wav", &audio[0]);
    openAudioFile("selectsound.wav", &audio[1]);
    playAudio(&audio[0]);

    /* 啟動鍵盤控制 整個程式中只要做一次就行了*/
    initializeKeyInput();
    cur_tick = last_tick = timer();

    setMainmenu(&mainmenu);

    show_image(background, 0, 0);
    saveScreen();

    /* 無窮迴圈  裡面包含了這個程式的主要工作
    而且每次迴圈會不斷把最新的畫面顯示到螢幕上
    像是播動畫一樣 每次迴圈更新一次畫面 */
    while (!IsEnding) {
        /* 每經過 REFRESH_RATE 個 ticks 才會更新一次畫面 */
        cur_tick = timer(); /* 每個 tick 0.01 秒 */
        sprintf(str, "%10d", cur_tick/1000+87);

        if (cur_tick-last_tick > REFRESH_RATE) {

            last_tick = cur_tick;


           clearScreen();
           restoreScreen();

            putStringLarge(mainmenu.large_font, OFFSET_X, OFFSET_Y-1, str, 14);

            /* 把選單畫出來 */
            showMenu(&mainmenu);

            /* 為了要讓一連串 drawChar() 的動作產生效果
               必須要呼叫一次 drawCmdWindow() 把之前畫的全部內容一次顯示到螢幕上 */
            drawCmdWindow();

        } /* end of if (cur_tick % REFRESH_RATE == 0 ... */

        for (k=0; k<NUM_KEYS; k++) {
            /* 按鍵從原本被按下的狀態 變成放開的狀態  這是為了處理按著不放的情況 */
            if(KEY_UP(key_val[k]) && key_down[k]) {
                key_down[k] = 0;
            }
        }

        /* 鍵盤控制 處理按著不放的狀況 */
        for (k=0; k<NUM_KEYS; k++) {
            /* 按鍵從原本被按下的狀態 變成放開的狀態  這是為了處理按著不放的情況 */
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;

                switch (key_val[k]) {
                case VK_UP:
                    scrollMenu(&mainmenu, -1);
                    playAudio(&audio[1]);
                    break;
                case VK_DOWN:
                    scrollMenu(&mainmenu, +1);
                    playAudio(&audio[1]);
                    break;
                case VK_RETURN:
                    if (IsOnItem(&mainmenu, 2)) {
                        if (IsItemSelected(&mainmenu, 0)) {
                            my_game_one();
                        }
                        if (IsItemSelected(&mainmenu, 1)) {
                            my_game_two();
                        }

                    } else if (IsOnItem(&mainmenu, 3)) {
                        IsEnding = 1;
                    }
                    break;
                case VK_SPACE:
                    /* 可以試試看底下另一種選單形式
                        同時能有多個項目可以被選到
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
    m->state[m->cursor] = m->state[m->cursor] | 1;  /* 目前選擇的項目 */

    strcpy(m->text[0], "SOLO");
    strcpy(m->text[1], "DUO");
    strcpy(m->text[2], "START");
    strcpy(m->text[3], "EXIT");
    strcpy(m->text[4], "ABOUT");
    strcpy(m->alt_text[0], "SOLO *");
    strcpy(m->alt_text[1], "DUO *");
    strcpy(m->alt_text[2], "START *");
    strcpy(m->alt_text[3], "EXIT *");
    strcpy(m->alt_text[4], "ABOUT *");
}
/*--------------------------------------------------------------*/
void gen_background()
{/*創造背景方格*/
    int j;
    for (j = 0; j < WIDTH; j++)
    {
        plate[0][j] = 1;
        plate[HEIGHT-1][j] = 1;
    }
}

void gen_char(Char *char_p)
{/*創造角色*/

    int i, j, k;
    int shape[3][3];
    int Char_SHAPE[3][3] =

    {
        0, 1, 0,
        3, 2, 3,
        0, 4, 0,
    }


        for (i=0; i<3; i++)
        {
            for (j=0; j<3; j++)
            {
                switch (char_p.shape[i][j])
                {
                case 1:
                    drawChar( 0xC4,
                             (char_p.x+ j)*2  + OFFSET_X,
                              char_p.y + i + OFFSET_Y, 0, char_p.color);
                    drawChar( 0x6F,
                             (char_p.x+ j)*2+1  + OFFSET_X,
                              char_p.y + i +  OFFSET_Y, 0, char_p.color);
                    break;

                case 2:
                    drawChar( 0xA4,
                             (char_p.x+ j)*2  + OFFSET_X,
                              char_p.y + i + OFFSET_Y,  0, char_p.color);
                    drawChar( 0xEC,
                             (char_p.x+ j)*2+1  + OFFSET_X,
                              char_p.y + i +  OFFSET_Y, 0, char_p.color);
                    break;

                case 3:
                    drawChar( 0x2B,
                             (char_p.x+ j)*2  + OFFSET_X,
                              char_p.y + i + OFFSET_Y, 0, char_p.color);
                    drawChar( 0x0,
                             (char_p.x+ j)*2+1  + OFFSET_X,
                              char_p.y + i +  OFFSET_Y, 0, char_p.color);
                    break;
                case 4:
                    drawChar( 0xA4,
                             (char_p.x+ j)*2  + OFFSET_X,
                              char_p.y + i + OFFSET_Y, 0, char_p.color);
                    drawChar( 0x4B,
                             (char_p.x+ j)*2+1  + OFFSET_X,
                              char_p.y + i +  OFFSET_Y, 0, char_p.color);
                    break;

                }
            }
        }


}

void gen_enemy(Char *char_p)
{/*創造敵人*/
    int shape[3][3];
    int Char_SHAPE[3][3] =
    {
        1, 1, 0,
        0, 1, 0,
        0, 1, 1,
    }
      for (k=0; k<1; k++)
    {
        for (i=0; i<3; i++)
        {
            for (j=0; j<3; j++)
            {
                switch (char_p.shape[i][j])
                {
                case 1:
                    drawChar( 0xA2,
                             (char_p.x+ j)*2  + OFFSET_X,
                              char_p.y + i + OFFSET_Y, 0, char_p.color);
                    drawChar( 0x69,
                             (char_p.x+ j)*2+1  + OFFSET_X,
                              char_p.y + i +  OFFSET_Y, 0, char_p.color);
                    break;
                }
            }
        }
    }
}

int my_game_one(void)
{
    int IsEnding = 0;
    char logo[] = "CREATED BY CHARLIE";
    int i, j, k;
    char str[40] = {'\0'};
    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_UP, VK_LEFT, VK_RIGHT, VK_A, VK_S};
    int cur_tick, last_tick;


    /* 啟動鍵盤控制 整個程式中只要做一次就行了*/
    initializeKeyInput();
    cur_tick = last_tick = timer();
    void gen_background();


    while (!IsEnding) {
    /* 每經過 REFRESH_RATE 個 ticks 才會更新一次畫面 */
    /* 把目前遊戲狀況 用 drawChar() 顯示出來
        每經過 REFRESH_RATE 個 ticks 才會更新一次畫面 */
        if (tick % REFRESH_RATE == 0 && !frame_update)
        {
            frame_update = 1;
            for (i = 0; i < HEIGHT; i++)
            {
                for (j = 0; j < WIDTH; j++)
                {
                    if (plate[i][j] == 1)
                    {
                        drawChar(0xA1, j*2 + OFFSET_X, i + OFFSET_Y,  0, 8);  /* fg_color=8 代表前景的顏色是灰色 */
                        drawChar(0xBD, j*2+1 + OFFSET_X, i + OFFSET_Y,  0, 8);
                    }
                    else
                    {
                        drawChar(' ', j*2 + OFFSET_X, i + OFFSET_Y, 0, 0);
                        drawChar(' ', j*2+1 + OFFSET_X, i + OFFSET_Y, 0, 0);
                    }
                }
            }
        }

            void gen_char(char);
            void gen_enemy(char);

            /*  把 logo 用 drawString() 顯示出來
               drawString() 是自定的 function    它能在特定位置顯示字串
               這個 function 的程式碼也放在這個檔案裡  在 main() 之後
               程式碼其實很簡短  只是在迴圈裡呼叫 drawChar() 而已       */
            drawString(logo, OFFSET_X, OFFSET_Y-2, 3, 14);

            drawString(str, OFFSET_X, OFFSET_Y-1, 1, 14);


            /* 為了要讓一連串 drawChar() 的動作產生效果
               必須要呼叫一次 drawCmdWindow() 把之前畫的全部內容一次顯示到螢幕上 */
            drawCmdWindow();

}
        /* end of if (tick%REFRESH_RATE) */



        /* 鍵盤控制 處理按著不放的狀況 */
        for (k=0; k<NUM_KEYS; k++)
        {
            /* 按鍵處在被按下的狀態  把這個事件記錄在 key_down[k] */
            if(KEY_DOWN(key_val[k]) && !key_down[k])
            {
                key_down[k] = 1;


                switch (key_val[k])
            {
                case VK_UP:         /*往上跳一格*/


                    break;
                case VK_LEFT:       /*往左走一格*/


                    break;
                case VK_RIGHT:      /*往右走一格*/


                    break;
                case VK_A:          /*施放絕招A*/

                    break;
                case VK_S:          /*施放絕招S*/

                    break;
            }
            return 0;
            }
        }
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

    /* 啟動鍵盤控制 整個程式中只要做一次就行了*/
    initializeKeyInput();
    cur_tick = last_tick = timer();

    while (!IsEnding) {
        /* 每經過 REFRESH_RATE 個 ticks 才會更新一次畫面 */
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
