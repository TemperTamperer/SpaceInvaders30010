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



uint8_t menu_update(uint8_t input)
{
    // FIRE button starts game
    if (input & JOY_UP)
        return 1;
    return 0;
}
