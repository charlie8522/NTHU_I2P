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
#include "image.h"

/*
drawChar()  最後兩個參數分別是字元背景和前景的顏色
可以參考下面的顏色值對照表來設定你想要的顏色

   0: 黑     1: 暗藍   2: 暗綠   3: 暗青
   4: 暗紅   5: 暗紫   6: 暗黃   7: 淺灰
   8: 暗灰   9: 亮藍  10: 亮綠  11: 亮青
  12: 亮紅  13: 亮紫  14: 亮黃  15: 白
*/

#define REFRESH_DURATION 5


typedef struct _robot {
    int x;
    int y;
    int blink_mode;
    int explode_mode;
    int moveleft_mode;
    int moveright_mode;
    Image *explode[3];
    Image *blink[2];
    int tick;
} Robot;

void show_robot(Robot *rp);
void move_robot(Robot *rp);


int main(void)
{
    int IsEnding = 0;
    char logo[] = "SAMPLE PROGRAM PROVIDED BY I2P";

    /* 用來計時 time.h */
    clock_t startc;
    int game_tick = 0;

    Robot robot;

    robot.blink[0] = read_image("robot_1.pixel", "robot_1.color");
    robot.blink[1] = read_image("robot_2.pixel", "robot_2.color");
    robot.explode[0] = read_image("explode_1.pixel", "explode_1.color");
    robot.explode[1] = read_image("explode_2.pixel", "explode_2.color");
    robot.explode[2] = read_image("explode_3.pixel", "explode_3.color");
    robot.x = 10;
    robot.y = 10;
    robot.blink_mode = 0;
    robot.explode_mode = 0;
    robot.moveleft_mode = 0;
    robot.moveright_mode = 0;

	/* 啟動鍵盤控制 */
    initializeKeyInput();

    startc = clock();
    while (!IsEnding) { /* game loop */

        if ((double)(clock()-startc) > 0.01*CLOCKS_PER_SEC) {
            game_tick++;  /* game_tick 0.01 秒 */
            startc = clock();
        }


        if (game_tick > REFRESH_DURATION) {
            game_tick = 0;
            clearScreen();

            show_robot(&robot);
            drawString(logo,2,2,3,14);

            drawCmdWindow();  /* update window immediately */

            move_robot(&robot);
        }

        if (KEY_DOWN(VK_ESCAPE)) {
            IsEnding = 1;
        }
        if (KEY_DOWN(VK_LEFT)) {
            if (robot.moveleft_mode==0)
                robot.moveleft_mode = 1;
        }
        if (KEY_DOWN(VK_RIGHT)) {
            if (robot.moveright_mode==0)
                robot.moveright_mode = 1;
        }
        if (KEY_DOWN(VK_SPACE)) {
            robot.blink_mode = 8;
        }
        if (KEY_DOWN(VK_X)) {
            robot.explode_mode = 9;
        }

    } /* while (IsEnding) */

    return 0;
}


void show_robot(Robot *rp)
{
    show_image(rp->blink[rp->blink_mode%2], rp->x, rp->y);
    if (rp->blink_mode>0) rp->blink_mode--;

    if (rp->explode_mode>0) {
        show_image(rp->explode[rp->explode_mode%3], rp->x+30, rp->y+10);
        rp->explode_mode--;
    }
}

void move_robot(Robot *rp)
{

    if (rp->moveleft_mode>0) {
        if (rp->x>10) rp->x-=3;
        rp->moveleft_mode--;
    }

    if (rp->moveright_mode>0) {
        if (rp->x<150) rp->x+=3;
        rp->moveright_mode--;
    }

}
