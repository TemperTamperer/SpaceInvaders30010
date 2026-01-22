#include "menu.h"
#include "ansi.h"
#include "game_settings.h"
#include <stdio.h>

#define TL  "\xC9"   // top-left corner
#define TR  "\xBB"   // top-right corner
#define BL  "\xC8"   // bottom-left corner
#define BR  "\xBC"   // bottom-right corner
#define HL  "\xCD"   // horizontal line
#define VL  "\xBA"   // vertical line

#define MENU_X 18
#define MENU_Y 5
#define MENU_W 44
#define MENU_H 28

// Draw helper
static void draw_hline(int x, int y, int len)
{
    gotoxy(x, y);
    for (int i = 0; i < len; i++)
        printf(HL);
}

// Main menu screen
void menu_draw(void)
{
    int x = MENU_X;
    int y = MENU_Y;
    int w = MENU_W;
    int h = MENU_H;

    // Frame
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

    // Title
    gotoxy(x + 12, y + 2);
    printf("SPACE  INVADERS");

    // Rocket ship
    gotoxy(x + 21, y + 8);   printf("^");
    gotoxy(x + 20, y + 9);   printf("/#\\");
    gotoxy(x + 19, y + 10);  printf("|=#=|");
    gotoxy(x + 20, y + 11);  printf("/ \\");

    // Enemies
    gotoxy(x + 14, y + 4);
    printf("\xDB\xDB  \xDB\xDB  \xDB\xDB  \xDB\xDB");

    // Menu text
    gotoxy(x + 10, y + 16);
    printf("CENTER  -  START GAME");

    gotoxy(x + 10, y + 18);
    printf("UP      -  HELP");

    // Footer
    gotoxy(x + 6, y + h - 2);
    printf("DTU 30010  Programming Project");
}

// Help screen
void help_draw(void)
{
    draw_border();

    gotoxy(10, 10);
    printf("Move with the joystick left and right");

    gotoxy(10, 12);
    printf("Shoot with joystick center (press down)");
