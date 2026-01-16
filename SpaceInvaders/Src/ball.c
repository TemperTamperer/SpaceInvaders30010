#include "ball.h"
#include "ansi.h";


void walls(){
	int width = 79;
	int height = 22;


	for(int i = 0; i <= height; i++ ){
		if(i == 0){
			printf("\xC9");
			for(int j = 0; j <= width-2; j++){
				printf("\xCD");
			}
			printf("\xBB\n");
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

void update_ball_pos(ball *p){
	p->x += p->vx;
	p->y += p->vy;

}

void check_ball_collision(ball *p){
	//Magic numbers are based on width = 79, height = 22
		if(p->x > 78 || p->x < 3){
			p->vx = -(p->vx);
			p->counter += 1;
		}
		if(p->y > 21 || p->y < 3){
			p->vy = -(p->vy);
			p->counter += 1;
	   	}

}

void draw_ball(ball *p){
	gotoxy(p->x, p->y);
	printf("0");
}

void erase_ball(ball *p){
	gotoxy(p->x, p->y);
	printf("\x20");
}

void draw_score(ball *p){
	gotoxy(5, 3);
		printf("%d", p->counter);
}


