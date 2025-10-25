#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <mmsystem.h>
/* ���İO�o�n�[�J�o�� */
#include "audio.h"
/* �t�~�٦����U���]�w */
/* For sound effect: In [Project Options]->[Parameters]->[Linker] add the parameter -lwinmm */
#include "console_draw3.h"
#include "kb_input2.h"
#include "font.h"
/*
drawChar()  �̫��ӰѼƤ��O�O�r���I���M�e�����C��
�i�H�ѦҤU�����C��ȹ�Ӫ�ӳ]�w�A�Q�n���C��

   0: ��     1: �t��   2: �t��   3: �t�C
   4: �t��   5: �t��   6: �t��   7: �L��
   8: �t��   9: �G��  10: �G��  11: �G�C
  12: �G��  13: �G��  14: �G��  15: ��
*/

#define REFRESH_DURATION 500

int main(void)
{
    int IsEnding = 0;

    /* �Ψӭp�� time.h */
    clock_t startc;
    int game_tick = 500;

    Font *large_font = read_font("font.txt");

	/* �Ұ���L���� */
    initializeKeyInput();

    startc = clock();
    while (!IsEnding) { /* game loop */

        if ((double)(clock()-startc) > 0.01*CLOCKS_PER_SEC) {
            game_tick++;  /* game_tick 0.01 �� */
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
