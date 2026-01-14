#ifndef DRAW_H_
#define DRAW_H_

#include "stdint.h"
#include "stdio.h"
#include "game_settings.h"
#include "ansi.h"

void draw_buffer(uint8_t buffer[][SCREEN_COLS], uint8_t shadow_buffer[][SCREEN_COLS]);
void clear_buffer(uint8_t current_buffer[][SCREEN_COLS]);
void draw_border();



#endif /* DRAW_H_ */
