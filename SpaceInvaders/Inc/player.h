#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdint.h>
#include "game_settings.h"

/* Forward declaration (VIGTIG) */
struct Bullet;

typedef struct {
    uint16_t x, y;
    uint8_t sx, sy;
    uint8_t hp;
    uint8_t hit_count;
} player;

void player_update_pos(uint8_t input, player *p1);
void player_push_buffer(uint8_t buffer[][SCREEN_COLS], player p1);

/* ✅ KORREKT prototype */
void player_hit_by_enemy_bullets(struct Bullet *enemyBullets,
                                 int count,
                                 player *p);

#endif
