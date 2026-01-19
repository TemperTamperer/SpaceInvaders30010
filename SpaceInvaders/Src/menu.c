#include "menu.h"
#include "ansi.h"
#include <stdio.h>



void menu_draw(void)
{
    gotoxy(20, 10);
    printf("SPACE INVADERS");

    gotoxy(20, 12);
    printf("Press UP to start");

    gotoxy(20, 14);
    printf("Press down for Help");



}
void help_draw(void)
{
    gotoxy(10, 10);
    printf("To move use the joystick left and right");

    gotoxy(10, 12);
    printf("To shot press the joystick center(press stick down)");

    gotoxy(20, 20);
    printf("Press up to start game");



}

uint8_t menu_update(uint8_t input)
{
    // FIRE button starts game
    if (input & JOY_UP)
        return 1;
    else if (input & JOY_DOWN)
    	return 2;
    return 0;
}
