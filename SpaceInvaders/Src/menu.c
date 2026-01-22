#include "menu.h"
#include "ansi.h"
#include "game_settings.h"
#include <stdio.h>


#define TL  "\xC9"   // ╔
#define TR  "\xBB"   // ╗
#define BL  "\xC8"   // ╚
#define BR  "\xBC"   // ╝
#define HL  "\xCD"   // ═
#define VL  "\xBA"   // ║"


static void draw_hline(int x, int y, int len)
{
    gotoxy(x, y);
    for (int i = 0; i < len; i++)
        printf(HL);
}

void menu_draw(void)
{
    int x = 18;
    int y = 5;
    int w = 44;
    int h = 28;

    /* Frame */
    gotoxy(x, y);         printf(TL);
    draw_hline(x + 1, y, w - 2);
    printf(TR);

    for (int i = 1; i < h - 1; i++)
    {
        gotoxy(x, y + i);         printf(VL);
        gotoxy(x + w - 1, y + i); printf(VL);
    }

    gotoxy(x, y + h - 1); printf(BL);
    draw_hline(x + 1, y + h - 1, w - 2);
    printf(BR);

    /* Title */
    gotoxy(x + 12, y + 2);
    printf("SPACE  INVADERS");

    /* Rocket ship */
    gotoxy(x + 21, y + 8);  printf("^");
    gotoxy(x + 20, y + 9);  printf("/#\\");
    gotoxy(x + 19, y + 10);  printf("|=#=|");
    gotoxy(x + 20, y + 11);  printf("/ \\");

    /* Enemies */
    gotoxy(x + 14,  y + 4);  printf("\xDB\xDB  \xDB\xDB  \xDB\xDB  \xDB\xDB");


    /* Menu text */
    gotoxy(x + 10, y + 16);
    printf("CENTER  -  START GAME");

    gotoxy(x + 10, y + 18);
    printf("UP      -  HELP");

    /* Footer */
    gotoxy(x + 6, y + h - 2);
    printf("DTU 30010  Programming Project");
}

void help_draw(void)
{
	draw_border();

    gotoxy(10, 10);
    printf("To move use the joystick left and right");

    gotoxy(10, 12);
    printf("To shot press the joystick center(press stick down)");

    gotoxy(20, 20);
    printf("Press Center to start game");

}
void boss_draw(void)
{

	draw_border();

    gotoxy(40, 20);
    printf("SIKE");
    gotoxy(60, 20);
        printf("To continue press Center");


}

uint8_t menu_update(uint8_t input)
{
    // FIRE button starts game
    if (input & JOY_CENTER)
        return 1;
    else if (input & JOY_UP)
    	return 2;
    else if (input & JOY_DOWN)
    	return 3;
    return 0;
}
