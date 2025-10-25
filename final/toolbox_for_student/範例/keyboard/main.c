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
#define MAX_NUM_KEYS 30
#define REFRESH_RATE 2

struct t_Keyboard {
   int x;
   int y;
   int color;
   int keydown;
   int keyval;
   int pitch;
   int drum;
   int volume;
   int shape[3][1];
};
typedef struct t_Keyboard Keyboard;


void delay(float sec);

void genKeyboard(Keyboard *key_p, int num);
void showKeyboard(Keyboard *key_p, int num);

int key_const[] =
{VK_1, VK_2, VK_3, VK_4, VK_5, VK_6, VK_7, VK_8, VK_9, VK_0,
VK_Q, VK_W, VK_E, VK_R, VK_T, VK_Y, VK_U, VK_I, VK_O, VK_P};

int pitch_const[] =
{60, 62, 64, 65, 67, 69, 71, 72, 74, 76,
77, 79, 81, 83, 84, 86, 88, 89, 91, 93};

int drum_const[] =
{55, 56, 57, 58, 59, 60, 61, 62, 63, 64
,65, 66, 67, 68, 69, 70, 71, 72, 73, 74};

int pitch_or_drum = 0;

int main(void)
{
   int IsEnding = 0;
   char logo[] = "SAMPLE PROGRAM PROVIDED BY I2P";
   int k;

   Keyboard keys[MAX_NUM_KEYS];
   int num_keys = 20;

   clock_t startc;
   int tick = 0;
   int frame_update = 0;

   char str[40] = {'\0'};


   openMidi();


   startc = clock();

   initializeKeyInput();


   genKeyboard(keys, num_keys);

   while (!IsEnding) {

      if ((double)(clock()-startc) > 0.005*CLOCKS_PER_SEC) {
         tick++;
         frame_update = 0;
         startc = clock();
      }


      if (tick % REFRESH_RATE == 0 && !frame_update) {

         frame_update = 1;

         showKeyboard(keys, num_keys);

         drawString(logo, OFFSET_X, OFFSET_Y-2,  3, 14);

         /* sprintf(str, "%10ld", getPosition(&audio[0])); */
         drawString(str, OFFSET_X, OFFSET_Y-1,  1, 14);


         drawCmdWindow();

      }

      for (k=0; k<num_keys; k++) {
         if(KEY_DOWN(keys[k].keyval) && !keys[k].keydown) {
            keys[k].keydown = 1;

            if (pitch_or_drum)
               playMidi(0, keys[k].pitch, keys[k].volume);
            else
               playMidi(1, keys[k].drum, keys[k].volume);

         }

         if(KEY_UP(keys[k].keyval) && keys[k].keydown) {
            keys[k].keydown = 0;
         }

      }

      if (KEY_DOWN(VK_X)) {
      	pitch_or_drum = !pitch_or_drum;
      }


   } /* while (!IsEnding) */

   closeMidi();

   delay(0.1);
   return 0;
}




void delay(float sec)
{
   clock_t startc;
   startc = clock();
   for ( ; ; ) {
	  if ((float)(clock()-startc)/CLOCKS_PER_SEC > sec) break;
   }
}


/****************************************************************************************************/


void genKeyboard(Keyboard *key_p, int num)
{
   int i, k;

   for (k=0; k<num; k++) {
      key_p[k].x = 5 + k;
      key_p[k].y = 10;
      key_p[k].color = 10;

      key_p[k].keydown = 0;
      key_p[k].keyval = key_const[k];
      key_p[k].pitch = pitch_const[k];
      key_p[k].drum = drum_const[k];
      key_p[k].volume = 100;

      for (i=0; i<3; i++) {
         key_p[k].shape[i][0] = 1;
      }

   }

}

void showKeyboard(Keyboard *key_p, int num)
{
   int i, k;

   for (k=0; k<num; k++) {
      for (i=0; i<3; i++) {
         if (key_p[k].keydown) {
           if (key_p[k].shape[i][0]) {
               drawChar( 0xA1,
                        (key_p[k].x)*2  + OFFSET_X,
                         key_p[k].y + i + OFFSET_Y,  0, key_p[k].color);
               drawChar( 0xBD,
                        (key_p[k].x)*2+1  + OFFSET_X,
                         key_p[k].y + i +  OFFSET_Y, 0, key_p[k].color);
            }
         }
         else {
           if (key_p[k].shape[i][0]) {
               drawChar( 0xA1,
                        (key_p[k].x)*2  + OFFSET_X,
                         key_p[k].y + i + OFFSET_Y,  0, 5);
               drawChar( 0xBD,
                        (key_p[k].x)*2+1  + OFFSET_X,
                         key_p[k].y + i + OFFSET_Y, 0, 5);
            }
         }
      }
   }

}
