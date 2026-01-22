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


void menu_draw(void)
{


    draw_border();

    /* Title */
    gotoxy(20, 2);
    printf("SPACE  INVADERS");

    /* Rocket ship */
    gotoxy(30, 15);  printf("^");
    gotoxy(29, 16);  printf("/#\\");
    gotoxy(28, 17);  printf("|=#=|");
    gotoxy(29, 18);  printf("/ \\");

    /* Enemies */
    gotoxy(24,  7);  printf("  \xDB\xDB     \xDB\xDB");
    gotoxy(24,  8);  printf("   \xDB\xDB   \xDB\xDB ");
    gotoxy(24,  9);  printf("  \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB");
    gotoxy(24,  10);  printf(" \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB");
    gotoxy(24,  11);  printf("\xDB\xDB         \xDB\xDB");
    gotoxy(24,  12);  printf("\xDB\xDB         \xDB\xDB");

    /* Menu text */
    gotoxy(20, 22);
    printf("CENTER  -  START GAME");

    gotoxy(20, 24);
    printf("UP      -  HELP");

    /* Footer */
    gotoxy(16, 40);
    printf("DTU 30010  Programming Project");
}

void help_draw(void)
{
	draw_border();


	gotoxy(10, 6);
	    printf("Welcome to Spaceinvaders 30010 edition");

    gotoxy(10, 10);
    printf("To move use the joystick left and right");

    gotoxy(10, 12);
    printf("To shot press the joystick center(press stick down)");

    gotoxy(10, 14);
        printf("Your goal is to kill as many enemy spaceships as ");
        gotoxy(10, 15);
            printf("possible without dying");

    gotoxy(10, 16);
         printf("However the enemy shots back so dont get hit!");

    gotoxy(10, 18);
          printf("Also be aware of the Astroids gravatiy");
          gotoxy(10, 19);
              printf("It can affect your bullets");

    gotoxy(10, 22);
          printf("To hide the game while playing press down");

    gotoxy(20, 30);
    printf("Press Center to start game");

    gotoxy(16, 40);
        printf("DTU 30010  Programming Project");

}
void boss_draw(void)
{

	draw_border();

	gotoxy(10, 6);
		    printf("THIS IS A VERY IMPORTANT WORK DOCUMENT");

	    gotoxy(10, 10);
	    printf("Wake up to reality! Nothing ever goes as planned in");
	    gotoxy(10, 11);
	    printf("this accursed world. The longer you live, the more ");
	    gotoxy(10, 12);
	    printf("you realize that the only things that truly exist ");
	    gotoxy(10, 13);
	    printf("in this reality are merely pain. suffering and");
	    gotoxy(10, 14);
	    printf("futility. Listen, everywhere you look in this world,");
	    gotoxy(10, 15);
	    printf("wherever there is light, there will always be shadows");
	    gotoxy(10, 16);
	    printf("to be found as well. As long as there is a concept");
	    gotoxy(10, 17);
	    printf("of victors the vanquished will also exist. ");
	    gotoxy(10, 18);
        printf("The selfish intentto preserve peace, initiates war.");
        gotoxy(10, 19);
        printf("and hatred is born in order to protect love.");
        gotoxy(10, 20);
        printf("- Madara Uchiha ");

        gotoxy(10,30);
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
