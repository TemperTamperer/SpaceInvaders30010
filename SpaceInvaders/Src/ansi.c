#define ESC 0x1B

#include "ansi.h"


void clrscr() {
    // \x1B[2J  -> Clear entire screen
    // \x1B[H   -> Move cursor to home position (0,0)
    printf("\x1B[2J\x1B[H");
}

void clreol(){
	printf("\x1B[K");
}

void gotoxy(uint8_t x, uint8_t y){
	printf("\x1B[%d;%dH", y, x);
}

void underline(uint8_t on){
	if(on == 1){
		printf("\x1B[5m");
	}
	if(on == 0){
			printf("\x1B[25m");
		}
}

void blink(uint8_t on){
	if(on == 1){
			printf("\x1B[4m");
		}
		if(on == 0){
				printf("\x1B[24m");
			}
}

void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char title[], uint8_t style){
	int width = x2 - x1;
	int height = y2 - y1;


	if(style == 0){
		for(int i = 0; i <= height; i++ ){
			if(i == 0){
				printf("%s\n", title);
			}
			if(i == height){
				printf("\xC8");
				for(int j = 0; j <= width-2; j++){
					printf("\xCD");
				}
				printf("\xBC");
			}
			else{
				printf("\xBA");
				for(int j = 0; j <= width-2; j++){
					printf("\x20");
				}
				printf("\xBA\n");
			}
		}
	}

}


void fgcolor(uint8_t foreground) {
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
  uint8_t type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%dm", ESC, type, foreground+30);
}

void bgcolor(uint8_t background) {
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
 	              way comming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
  printf("%c[%dm", ESC, background+40);
}

void color(uint8_t foreground, uint8_t background) {
// combination of fgcolor() and bgcolor() - uses less bandwidth
  uint8_t type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor() {
// gray on black text, no underline, no blink, no reverse
  printf("%c[m", ESC);
}
