#include "menu.h"
#include "ansi.h"
#include "game_settings.h"
#include <stdio.h>



void menu_draw(void)
{
	draw_border();

    gotoxy(20, 10);
    printf("SPACE INVADERS");

    gotoxy(20, 12);
    printf("Press Center to start");

    gotoxy(20, 14);
    printf("Press UP for Help");

    gotoxy(20, 16);
       printf("to hide game while playing press down");


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

    if (input & JOY_CENTER)
        return 1;
    else if (input & JOY_UP)
    	return 2;
    else if (input & JOY_DOWN)
    	return 3;
    return 0;
}
