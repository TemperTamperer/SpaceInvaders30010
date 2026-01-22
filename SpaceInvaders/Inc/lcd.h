#ifndef LCD_H_
#define LCD_H_

#include "charset.h"
#include "30010_io.h"
#include <stdio.h>

/* LCD drawing */
void lcd_draw_score(int32_t score, uint8_t buffer[]);
void lcd_draw_heart(uint8_t health, uint8_t buffer[]);

/* LCD helpers */
void lcd_update(void);
void lcd_scroll(uint8_t string[], int32_t *line);

/* LED control */
void set_led(uint8_t input);

#endif
