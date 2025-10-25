#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <mmsystem.h>
/* 音效記得要加入這個 */
#include "audio.h"
/* 另外還有底下的設定 */
/* For sound effect: In [Project Options]->[Parameters]->[Linker] add the parameter -lwinmm */
#include "console_draw3.h"
#include "kb_input2.h"
#include "font.h"
/*
drawChar()  最後兩個參數分別是字元背景和前景的顏色
可以參考下面的顏色值對照表來設定你想要的顏色

   0: 黑     1: 暗藍   2: 暗綠   3: 暗青
   4: 暗紅   5: 暗紫   6: 暗黃   7: 淺灰
   8: 暗灰   9: 亮藍  10: 亮綠  11: 亮青
  12: 亮紅  13: 亮紫  14: 亮黃  15: 白
*/

#define REFRESH_DURATION 500

int main(void)
{
    int IsEnding = 0;

    /* 用來計時 time.h */
    clock_t startc;
    int game_tick = 500;

    Font *large_font = read_font("font.txt");

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


            putStringLarge(large_font, 10, 10, "SAMPLE PROGRAM", 10);
            putStringLarge(large_font, 10, 16, "PROVIDED BY I2P", 10);

            drawCmdWindow();  /* update window immediately */

        }

        if (KEY_DOWN(VK_ESCAPE)) {
            IsEnding = 1;
        }

    } /* while (IsEnding) */

    return 0;
}
