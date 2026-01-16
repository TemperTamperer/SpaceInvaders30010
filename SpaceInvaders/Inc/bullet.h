#pragma once
#include <stdint.h>
#define MAX_BULLETS 6
typedef struct {
  int16_t x, y;     // position in cells
  uint8_t active;   // 1 = active, 0 = inactive
} Bullet;

void bullet_init(void);

// Fire one bullet from (x,y). If a bullet is already active, do nothing.
void bullet_fire(int16_t x, int16_t y);

// Call once per game tick
void bullet_update(void);

// Read bullet for render/collision
const Bullet* bullet_get_array(void);
void bullet_kill(int index);
