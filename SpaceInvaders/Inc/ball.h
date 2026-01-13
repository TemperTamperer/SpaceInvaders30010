#ifndef BALL_H_
#define BALL_H_

#include "stdint.h"
#include "stdio.h"

typedef struct{
	uint16_t x, y, counter;
	int32_t vx, vy;
} ball;


void walls();

void update_ball_pos(ball *p);

void draw_ball(ball *p);

void erase_ball(ball *p);

void check_ball_collision(ball *p);

void draw_score(ball *p);

#endif /* BALL_H_ */
