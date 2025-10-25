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
#define OFFSET_X  2
#define OFFSET_Y  2
#define NUM_KEYS 7
#define MAX_NUM_TANKS 30
#define REFRESH_RATE 20


int plate[HEIGHT][WIDTH] = {{0}};

/* 自定的 struct   用來描述方塊的各種屬性 */
struct t_Tank
{
    int x;
    int y;
    int direction;
    int color;
    int duration;
    int last_tick;
    int shape[3][3];
};
typedef struct t_Tank Tank;


int TANK_SHAPE[4][9] =
{
    {
        0, 1 ,0,
        3, 2, 3,
        3, 3, 3
    },
    {
        3, 3 ,0,
        3, 2, 1,
        3, 3, 0
    },
    {
        3, 3 ,3,
        3, 2, 3,
        0, 1, 0
    },
    {
        0, 3 ,3,
        1, 2, 3,
        0, 3, 3
    }
};

void delay(float sec);
void genTanks(Tank *tank_p, int num);
void showTanks(Tank *tank_p, int num);
void moveTanks(Tank *tank_p, int num, int tick);

int main(void)
{
    int IsEnding = 0;
    char logo[] = "SAMPLE PROGRAM PROVIDED BY I2P";
    int i, j, k;

    Tank tanks[MAX_NUM_TANKS];
    int num_tanks = 5;

    clock_t startc;
    int tick = 0;
    int frame_update = 0;

    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_A, VK_S, VK_D, VK_F, VK_G, VK_ESCAPE, VK_Q};

    char str[40] = {'\0'};

    /* 用來記錄音效的 struct 陣列 */
    Audio audio[6];

    /* 讀入六個音效檔 */
    openAudioFile("DR220A_CH.wav", &audio[0]);
    openAudioFile("DR220A_SD.wav", &audio[1]);
    openAudioFile("DR220A_HT.wav", &audio[2]);
    openAudioFile("DR220A_CCY.wav", &audio[3]);
    openAudioFile("DR220A_BD.wav", &audio[4]);

    openAudioFile("liberty.wav", &audio[5]);  	/* 背景音樂 */
    audio[5].loop = 1;	/* 重複播放 */

    /* 開始播放背景音樂  */
    playAudio(&audio[5]);




    /* 鋪上兩邊的牆壁和最底下的地板　*/
    for (i = 0; i < HEIGHT; i++)
    {
        plate[i][0] = 1;
        plate[i][WIDTH-1] = 1;
    }
    for (j = 0; j < WIDTH; j++)
    {
        plate[0][j] = 1;
        plate[HEIGHT-1][j] = 1;
    }

    /* 產生亂數 seed    讓後面呼叫 rand() 可以得到不同的亂數序列  */
    srand(time(NULL));

    /* 用來計時 */
    startc = clock();

    /* 啟動鍵盤控制 */
    initializeKeyInput();

    /* 產生坦克 */
    genTanks(tanks, num_tanks);




    /* 無窮迴圈  裡面包含了這個程式的主要工作
    而且每次迴圈會不斷把最新的畫面顯示到螢幕上  像是播動畫一樣 每次迴圈更新一次畫面 */
    while (!IsEnding)
    {

        /* 自己設定的時間單位 tick  每 0.05 等於一個 tick   後面會利用 tick 數來決定方塊移動速度 */
        if ((double)(clock()-startc) > 0.005*CLOCKS_PER_SEC)
        {
            tick++;
            frame_update = 0;
            startc = clock();
        }


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

            /* 把坦克畫出來 */
            showTanks(tanks, num_tanks);


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

                /* 依照你想要的動作來處理按鍵 */
                /* 在這個例子裡 按下 A, S, D, F, G
                可以分別改變 五台坦克的顏色  發出五種不同音效
                但是若按著不放 則只會發生一次作用
                同時很多鍵一起按著 不會互相干擾 */
                switch (key_val[k])
                {
                case VK_A:
                    tanks[0].color=(tanks[0].color+1)%15 + 1;
                    /* 播放對應的音效 */
                    playAudio(&audio[0]);
                    break;
                case VK_S:
                    tanks[1].color=(tanks[1].color+1)%15 + 1;
                    /* 播放對應的音效 */
                    playAudio(&audio[1]);
                    break;
                case VK_D:
                    tanks[2].color=(tanks[2].color+1)%15 + 1;
                    /* 播放對應的音效 */
                    playAudio(&audio[2]);
                    break;
                case VK_F:
                    tanks[3].color=(tanks[3].color+1)%15 + 1;
                    /* 播放對應的音效 */
                    playAudio(&audio[3]);
                    break;
                case VK_G:
                    tanks[4].color=(tanks[4].color+1)%15 + 1;
                    /* 播放對應的音效 */
                    playAudio(&audio[4]);
                    break;
                case VK_ESCAPE:

                    /* 暫停背景音樂 */
                    pauseAudio(&audio[5]);

                    /* 暫時進入一般輸入模式 */
                    SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), (COORD)
                    {
                        30, 20
                    } );
                    SetConsoleMode (GetStdHandle (STD_INPUT_HANDLE), ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_INPUT);
                    scanf("%39s", str);
                    SetConsoleMode (GetStdHandle (STD_INPUT_HANDLE), ENABLE_LINE_INPUT);

                    /* 繼續播放原本的音樂 */
                    restartAudio(&audio[5]);
                    break;
                case VK_Q:
                    IsEnding = 1;
                    break;

                }
                /* 按鍵從原本被按下的狀態 變成放開的狀態  這是為了處理按著不放的情況 */
                if(KEY_UP(key_val[k]) && key_down[k])
                {
                    key_down[k] = 0;
                }
            }
        }


        /* 移動坦克 */
        moveTanks(tanks, num_tanks, tick);



    } /* while (IsEnding) */


    /* 遊戲結束前 暫停 0.5 秒*/

    delay(0.5);
    return 0;
}

/* 讓程式暫停 sec 秒 */
void delay(float sec)
{
    clock_t startc;
    startc = clock();
    for ( ; ; )
    {
        if ((float)(clock()-startc)/CLOCKS_PER_SEC > sec) break;
    }
}

/****************************************************************************************************/


void genTanks(Tank *tank_p, int num)
{
    int i, j, k;

    for (k=0; k<num; k++)
    {
        tank_p[k].x = rand()%(WIDTH-4) + 1;
        tank_p[k].y = rand()%(HEIGHT-4) + 1;
        tank_p[k].color = rand()%15 + 1;

        tank_p[k].duration = 30 + rand()%20;
        tank_p[k].last_tick = 0;

        tank_p[k].direction = rand()%3;

        for (i=0; i<3; i++)
        {
            for (j=0; j<3; j++)
            {
                tank_p[k].shape[i][j]
                    = TANK_SHAPE[tank_p[k].direction][i*3+j];
            }
        }

    }
}

void showTanks(Tank *tank_p, int num)
{
    int i, j, k;

    for (k=0; k<num; k++)
    {
        for (i=0; i<3; i++)
        {
            for (j=0; j<3; j++)
            {
                switch (tank_p[k].shape[i][j])
                {
                case 1:
                    drawChar( 0x2B,
                             (tank_p[k].x+ j)*2  + OFFSET_X,
                              tank_p[k].y + i + OFFSET_Y, 0, tank_p[k].color);
                    drawChar( 0x0,
                             (tank_p[k].x+ j)*2+1  + OFFSET_X,
                              tank_p[k].y + i +  OFFSET_Y, 0, tank_p[k].color);
                    break;

                case 2:
                    drawChar( 0xA1,
                             (tank_p[k].x+ j)*2  + OFFSET_X,
                              tank_p[k].y + i + OFFSET_Y,  0, tank_p[k].color);
                    drawChar( 0xB4,
                             (tank_p[k].x+ j)*2+1  + OFFSET_X,
                              tank_p[k].y + i +  OFFSET_Y, 0, tank_p[k].color);
                    break;

                case 3:
                    drawChar( 0xCA,
                             (tank_p[k].x+ j)*2  + OFFSET_X,
                              tank_p[k].y + i + OFFSET_Y, 0, tank_p[k].color);
                    drawChar( 0xA8,
                             (tank_p[k].x+ j)*2+1  + OFFSET_X,
                              tank_p[k].y + i +  OFFSET_Y, 0, tank_p[k].color);
                    break;

                }
            }
        }
    }

}

void moveTanks(Tank *tank_p, int num, int tick)
{
    int i, j, k;

    for (k=0; k<num; k++)
    {
        if (tick-tank_p[k].last_tick > tank_p[k].duration)
        {
            switch (tank_p[k].direction)
            {
            case 0:
                tank_p[k].y -= 1;
                break;
            case 1:
                tank_p[k].x += 1;
                break;
            case 2:
                tank_p[k].y += 1;
                break;
            case 3:
                tank_p[k].x -= 1;
                break;
            }

            if (tank_p[k].x < 1)
            {
                tank_p[k].direction = rand()%4;
                tank_p[k].x += 1;
            }
            else if (tank_p[k].x > WIDTH-4)
            {
                tank_p[k].direction = rand()%4;
                tank_p[k].x -= 1;
            }
            else if (tank_p[k].y < 1)
            {
                tank_p[k].direction = rand()%4;
                tank_p[k].y += 1;
            }
            else if (tank_p[k].y > HEIGHT-4)
            {
                tank_p[k].direction = rand()%4;
                tank_p[k].y -= 1;
            }

            /* 有百分之二十的機率會再改變方向 */
            if (rand()%10 > 7)
            {
                tank_p[k].direction = rand()%4;
            }
            /* 有百分之二十的機率會再改變速度 */
            if (rand()%10 > 7)
            {
                tank_p[k].duration = 30 + rand()%20;
            }

            tank_p[k].last_tick = tick;

            for (i=0; i<3; i++)
            {
                for (j=0; j<3; j++)
                {
                    tank_p[k].shape[i][j]
                        = TANK_SHAPE[tank_p[k].direction][i*3+j];
                }
            }
        }

    }

}

