#ifndef POWERUP_H_
#define POWERUP_H_

#include <stdint.h>
#include "bullet.h"

/* Powerup types */
typedef enum {
    PU_NONE = 0,
    PU_TRIPLE_POS = 1,
    PU_SPREAD_5 = 2
} PowerupType;

/* Powerup state */
typedef struct {
    PowerupType active;
    uint16_t ticks_left;
    uint8_t trig_250;
    uint8_t trig_750;
} PowerupState;

/* Init and update */
void powerup_init(PowerupState* s);
void powerup_update_from_score(PowerupState* s, uint32_t score);
void powerup_tick(PowerupState* s);

/* Shooting behavior */
void powerup_shoot(PowerupState* s,
                   Bullet bullets[], int count,
                   uint8_t shoot_just_pressed,
                   int x_center, int y);

#endif
