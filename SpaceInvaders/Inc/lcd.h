#ifndef LCD_H_
#define LCD_H_

#include "charset.h"
#include "30010_io.h"

void lcd_write_string(uint8_t string[], int32_t line);
void lcd_update();
void lcd_scroll(uint8_t string[], int32_t *line);

#endif /* LCD_H_ */
