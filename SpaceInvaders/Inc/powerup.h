#ifndef POWERUP_H_
#define POWERUP_H_

#include <stdint.h>
#include "game_settings.h"
#include "bullet.h"
#include "player.h"

// powerup types
typedef enum PowerupType{
    PU_NONE = 0,
    PU_TRIPLE_POS = 1,
    PU_SPREAD_5 = 2
} PowerupType;

// powerup state
typedef struct PowerupState{
    PowerupType active;
    uint16_t ticks_left;
    uint8_t trig_250;
    uint8_t trig_750;
} PowerupState;

void powerup_init(PowerupState* s);
void powerup_update_from_score(PowerupState* s, uint32_t score);
void powerup_tick(PowerupState* s);

void powerup_shoot(PowerupState* s,
                   Bullet bullets[], int count,
                   uint8_t shoot_just_pressed,
				   player p,
				   uint8_t buffer[][SCREEN_COLS]);

#endif
