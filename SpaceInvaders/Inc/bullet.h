#ifndef BULLET_H
#define BULLET_H

#include <stdint.h>
#include "game_settings.h"

#define MAX_BULLETS 5

typedef struct {
    int x;
    int y;
    int dy;
    uint8_t active;
} bullet;

void bullets_init(bullet bullets[]);
void bullets_fire(bullet bullets[], int start_x, int start_y);
void bullets_update(bullet bullets[]);
void bullets_draw(bullet bullets[], char buffer[][SCREEN_COLS]);


#endif
