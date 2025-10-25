#include <stdio.h>
#include <stdlib.h>    /* 為了用 srand() 和 rand() */
#include <string.h>
#include <time.h>      /* 為了用 time() 產生亂數種子 */
#include "console_draw3.h"  /* 在 DOS command window 畫圖 */
#include "kb_input2.h"

#define LINE_LEN 40    /* 假設單字長度不超過 40 個字元 */
#define NUM_WORDS 5000 /* 總共有 5000 個單字 */

#define OFFSET_X  10
#define OFFSET_Y  5

char dictionary[NUM_WORDS][LINE_LEN]; /* 儲存單字的陣列  每個 row 都是一個字串  儲存一個單字 */
void delay(float sec);

char letters[26][2] = {

   {0xA2, 0xCF}, {0xA2, 0xD0}, {0xA2, 0xD1}, {0xA2, 0xD2}, {0xA2, 0xD3},
   {0xA2, 0xD4}, {0xA2, 0xD5}, {0xA2, 0xD6}, {0xA2, 0xD7}, {0xA2, 0xD8},
   {0xA2, 0xD9}, {0xA2, 0xDA}, {0xA2, 0xDB},
   {0xA2, 0xDC}, {0xA2, 0xDD}, {0xA2, 0xDE}, {0xA2, 0xDF}, {0xA2, 0xE0},
   {0xA2, 0xE1}, {0xA2, 0xE2}, {0xA2, 0xE3}, {0xA2, 0xE4}, {0xA2, 0xE5},
   {0xA2, 0xE6}, {0xA2, 0xE7}, {0xA2, 0xE8}

   };

int main(void)
{
   FILE *fp; /* 開檔案 需要一個指標指到開啟的檔案 */
   int nw;
   int i, j, pick, k, color;
   int key;

   int marked[2];
   int cur_index=0;
   int correct[2] = {0};

   int countdown = 10;
   char str[80];



   /* fopen() 的用法就是傳入檔名字串 以及開檔方式  "r" 表示只要讀檔  */
   /* fopen() 會傳回指到檔案的指標  失敗的或那個指標會是 NULL 表示沒有指到任何地方 */
   if ( (fp = fopen("5000_words.txt", "r")) == NULL ) {
      printf("Cannot open file.\n");
      exit(1);
   }

   /* 用 fscanf() 把所有的單字都讀進來 */
   /* fscanf() 和 scanf() 的用法相同   */
   /* 只是要多傳一個參數  把檔案指標傳給 fscanf() */
   /* 才能夠知道是要從哪個檔案讀資料 */
   /* feof(fp) 可以判斷檔案 fp 是否已經讀到了盡頭 */
   /* 如果已經到了檔案結尾 feof(fp) 會傳回 true */
   /* fgetc(fp) 則只會讀取一個字元  我們只想讀取單字 */
   /* 所以用 fgetc(fp) 把剩下的單字解釋略過 一直讀到換行  再繼續讀下個單字 */
   nw = 0;
   while (!feof(fp) && nw < NUM_WORDS) {
      fscanf(fp, "%s", dictionary[nw]);
      while ( !feof(fp) && fgetc(fp) != '\n' ) ;
      nw++;
   }
   fclose(fp);




   /* 用時間當做變因 決定隨機序列所需要的 seed */
   srand(time(NULL));

   /* 隨機選出單字 */
   /* dictionary[][] 的每一個 row 都是一個字串 */
   /* 例如 dictionary[1002] 儲存的是 "convertible" 這個字串*/
   /* 所以 dictionary[1002][0] 就是 'c'  而 dictionary[1002][1] 就是 'o' */
   /* 以此類推 dictionary[1002][10] 是 'e' 然後 dictionary[1002][11] 是 '\0' */
   pick = rand()%NUM_WORDS;

   for (i=0; i<2; i++) {
      marked[i] = rand()%strlen(dictionary[pick]);
   }


   initializeKeyInput(); /* 啟動鍵盤控制 */

   while (1) { /* 無窮迴圈 */

      /* 用空白字元  把畫面清除 */
      for (i=0; i<20; i++) {
         for (j=0; j<50; j++) {
            drawChar(' ', j + OFFSET_X,  i + OFFSET_Y, 0, 12);
         }
      }

      /* 把單字裡的 ASCII2 字元一個一個顯示出來 */
      for (i=0; i<strlen(dictionary[pick]); i++) {
         drawChar(dictionary[pick][i], i + OFFSET_X,  OFFSET_Y, 0, 13);
      }

      /* 蓋住某些字母 */
      for (i=0; i<2; i++) {
         if(correct[i]==0) {
            drawChar('*', marked[i] + OFFSET_X,  OFFSET_Y, 0, 10);
         }
      }
      /* 用不同顏色標示出目前輸入的位置 */
      if(correct[cur_index]==0) {
         drawChar('*', marked[cur_index] + OFFSET_X,  OFFSET_Y, 3, 14);
      }



      if (0) { /* 請修改這個部分 加入倒數計時的功能  作法類似 main() 後面的 delay() 程式碼 */
         countdown--;
      }
      sprintf(str, "%d", countdown); /* 把整數輸出到字串裡 */
      for (i=0; i<strlen(str); i++) {
         drawChar(str[i], i + OFFSET_X,  2 + OFFSET_Y,  0, 12);
      }

      /* 時間到 */
      if (countdown == 0) break;



      /* 把單字裡的字元對應到的全形字體一個一個顯示出來 */
      /* 全形字佔用兩個 bytes    所以必須分兩字用 drawChar 顯示 */
      /* 這部分請仿造上面半形字的做法來處理 */
      for (i=0; i<strlen(dictionary[pick]); i++) {
         k = dictionary[pick][i] - 'a';
         color = rand()%15;
         drawChar(letters[k][0], 2*i + OFFSET_X,  5 + OFFSET_Y, 0 , color);
         drawChar(letters[k][1], 2*i + 1 + OFFSET_X,  5 + OFFSET_Y, 0, color);
      }



      /* 把填好的螢幕內容顯示出來 */
      drawCmdWindow();


      /* 停留 0.5 秒等待輸入 如果時間到了沒有輸入任何東西 就會傳回 0
         如果在 0.5 秒之內有輸入 就會立即傳回 1 */
      if(waitForKeyDown(0.05)) {
         key=getKeyEventASCII();	/* 讀取輸入的字元 */

         if (key == ' ') {
            cur_index = (cur_index+1)%2;
         }
         if (key == '/') {
            break; /* 輸入的是 '/' 就跳出無窮迴圈 */
         }
         /* 使用者輸入的是正確答案 */
         if(key==dictionary[pick][marked[cur_index]]) {
            correct[cur_index] = 1;
         }
      }

      /* 加入你的程式碼  在這裡判斷是否所有單字都答對了  如果答對了就隨機挑選下一個單字 */





      delay(0.05);
   } /* while (1) 無窮迴圈 */

   return 0;
}


void delay(float sec)
{
   clock_t startc;  /* 計算停留的時間 */
   startc = clock();
   for ( ; ; ) {
	  if ((float)(clock()-startc)/CLOCKS_PER_SEC > sec) break;
   }
}
