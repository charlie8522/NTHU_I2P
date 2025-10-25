#include <stdio.h>
#include <stdlib.h>    /* ���F�� srand() �M rand() */
#include <string.h>
#include <time.h>      /* ���F�� time() ���Ͷüƺؤl */
#include "console_draw3.h"  /* �b DOS command window �e�� */
#include "kb_input2.h"

#define LINE_LEN 40    /* ���]��r���פ��W�L 40 �Ӧr�� */
#define NUM_WORDS 5000 /* �`�@�� 5000 �ӳ�r */

#define OFFSET_X  10
#define OFFSET_Y  5

char dictionary[NUM_WORDS][LINE_LEN]; /* �x�s��r���}�C  �C�� row ���O�@�Ӧr��  �x�s�@�ӳ�r */
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
   FILE *fp; /* �}�ɮ� �ݭn�@�ӫ��Ы���}�Ҫ��ɮ� */
   int nw;
   int i, j, pick, k, color;
   int key;

   int marked[2];
   int cur_index=0;
   int correct[2] = {0};

   int countdown = 10;
   char str[80];



   /* fopen() ���Ϊk�N�O�ǤJ�ɦW�r�� �H�ζ}�ɤ覡  "r" ��ܥu�nŪ��  */
   /* fopen() �|�Ǧ^�����ɮת�����  ���Ѫ��Ψ��ӫ��з|�O NULL ��ܨS���������a�� */
   if ( (fp = fopen("5000_words.txt", "r")) == NULL ) {
      printf("Cannot open file.\n");
      exit(1);
   }

   /* �� fscanf() ��Ҧ�����r��Ū�i�� */
   /* fscanf() �M scanf() ���Ϊk�ۦP   */
   /* �u�O�n�h�Ǥ@�ӰѼ�  ���ɮ׫��жǵ� fscanf() */
   /* �~������D�O�n�q�����ɮ�Ū��� */
   /* feof(fp) �i�H�P�_�ɮ� fp �O�_�w�gŪ��F���Y */
   /* �p�G�w�g��F�ɮ׵��� feof(fp) �|�Ǧ^ true */
   /* fgetc(fp) �h�u�|Ū���@�Ӧr��  �ڭ̥u�QŪ����r */
   /* �ҥH�� fgetc(fp) ��ѤU����r�������L �@��Ū�촫��  �A�~��Ū�U�ӳ�r */
   nw = 0;
   while (!feof(fp) && nw < NUM_WORDS) {
      fscanf(fp, "%s", dictionary[nw]);
      while ( !feof(fp) && fgetc(fp) != '\n' ) ;
      nw++;
   }
   fclose(fp);




   /* �ήɶ����ܦ] �M�w�H���ǦC�һݭn�� seed */
   srand(time(NULL));

   /* �H����X��r */
   /* dictionary[][] ���C�@�� row ���O�@�Ӧr�� */
   /* �Ҧp dictionary[1002] �x�s���O "convertible" �o�Ӧr��*/
   /* �ҥH dictionary[1002][0] �N�O 'c'  �� dictionary[1002][1] �N�O 'o' */
   /* �H������ dictionary[1002][10] �O 'e' �M�� dictionary[1002][11] �O '\0' */
   pick = rand()%NUM_WORDS;

   for (i=0; i<2; i++) {
      marked[i] = rand()%strlen(dictionary[pick]);
   }


   initializeKeyInput(); /* �Ұ���L���� */

   while (1) { /* �L�a�j�� */

      /* �Ϊťզr��  ��e���M�� */
      for (i=0; i<20; i++) {
         for (j=0; j<50; j++) {
            drawChar(' ', j + OFFSET_X,  i + OFFSET_Y, 0, 12);
         }
      }

      /* ���r�̪� ASCII2 �r���@�Ӥ@����ܥX�� */
      for (i=0; i<strlen(dictionary[pick]); i++) {
         drawChar(dictionary[pick][i], i + OFFSET_X,  OFFSET_Y, 0, 13);
      }

      /* �\��Y�Ǧr�� */
      for (i=0; i<2; i++) {
         if(correct[i]==0) {
            drawChar('*', marked[i] + OFFSET_X,  OFFSET_Y, 0, 10);
         }
      }
      /* �Τ��P�C��ХܥX�ثe��J����m */
      if(correct[cur_index]==0) {
         drawChar('*', marked[cur_index] + OFFSET_X,  OFFSET_Y, 3, 14);
      }



      if (0) { /* �Эק�o�ӳ��� �[�J�˼ƭp�ɪ��\��  �@�k���� main() �᭱�� delay() �{���X */
         countdown--;
      }
      sprintf(str, "%d", countdown); /* ���ƿ�X��r��� */
      for (i=0; i<strlen(str); i++) {
         drawChar(str[i], i + OFFSET_X,  2 + OFFSET_Y,  0, 12);
      }

      /* �ɶ��� */
      if (countdown == 0) break;



      /* ���r�̪��r�������쪺���Φr��@�Ӥ@����ܥX�� */
      /* ���Φr���Ψ�� bytes    �ҥH��������r�� drawChar ��� */
      /* �o�����Х�y�W���b�Φr�����k�ӳB�z */
      for (i=0; i<strlen(dictionary[pick]); i++) {
         k = dictionary[pick][i] - 'a';
         color = rand()%15;
         drawChar(letters[k][0], 2*i + OFFSET_X,  5 + OFFSET_Y, 0 , color);
         drawChar(letters[k][1], 2*i + 1 + OFFSET_X,  5 + OFFSET_Y, 0, color);
      }



      /* ���n���ù����e��ܥX�� */
      drawCmdWindow();


      /* ���d 0.5 ���ݿ�J �p�G�ɶ���F�S����J����F�� �N�|�Ǧ^ 0
         �p�G�b 0.5 ��������J �N�|�ߧY�Ǧ^ 1 */
      if(waitForKeyDown(0.05)) {
         key=getKeyEventASCII();	/* Ū����J���r�� */

         if (key == ' ') {
            cur_index = (cur_index+1)%2;
         }
         if (key == '/') {
            break; /* ��J���O '/' �N���X�L�a�j�� */
         }
         /* �ϥΪ̿�J���O���T���� */
         if(key==dictionary[pick][marked[cur_index]]) {
            correct[cur_index] = 1;
         }
      }

      /* �[�J�A���{���X  �b�o�̧P�_�O�_�Ҧ���r������F  �p�G����F�N�H���D��U�@�ӳ�r */





      delay(0.05);
   } /* while (1) �L�a�j�� */

   return 0;
}


void delay(float sec)
{
   clock_t startc;  /* �p�ⰱ�d���ɶ� */
   startc = clock();
   for ( ; ; ) {
	  if ((float)(clock()-startc)/CLOCKS_PER_SEC > sec) break;
   }
}
