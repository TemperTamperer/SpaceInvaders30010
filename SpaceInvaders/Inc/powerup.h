#ifndef POWERUP_H_
#define POWERUP_H_

#include <stdint.h>

typedef struct Bullet Bullet;

typedef enum {
    PU_NONE = 0,
    PU_TRIPLE_POS = 1,
    PU_SPREAD_5 = 2
} PowerupType;

typedef struct {
    PowerupType active;
    uint16_t ticks_left;

    // companions til triple
    uint16_t comp_ticks_left;

    // gentagelses-thresholds
    uint32_t next_triple_score;
    uint32_t next_spread_score;
} PowerupState;
uint8_t powerup_get_companions(const PowerupState* s,
                               int x_center, int y,
                               int *lx, int *ly,
                               int *rx, int *ry);

void powerup_init(PowerupState* s);
void powerup_update_from_score(PowerupState* s, uint32_t score);
void powerup_tick(PowerupState* s);

void powerup_shoot(PowerupState* s,
                   Bullet *bullets,
                   int count,
                   uint8_t shoot_just_pressed,
                   int x_center, int y);

#endif
