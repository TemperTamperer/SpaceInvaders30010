#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>
#include "30010_io.h"

#define JOY_UP     (1 << 0)
#define JOY_DOWN   (1 << 1)
#define JOY_LEFT   (1 << 2)
#define JOY_RIGHT  (1 << 3)
#define JOY_CENTER (1 << 4)

void GPIO_init(void);
uint8_t read_joystick(void);
void set_led(uint8_t input);

uint8_t joystick_just_pressed(uint8_t input, uint8_t mask, uint8_t *prev);
uint8_t joystick_get_move(uint8_t raw);
uint8_t joystick_center_just_pressed(uint8_t raw, uint8_t *prev);

void joystick_update(uint8_t *move,
                     uint8_t *center_just_pressed,
                     uint8_t *prev_center);

#endif /* JOYSTICK_H_ */
