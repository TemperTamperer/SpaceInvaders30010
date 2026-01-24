#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>
#include "30010_io.h"

// joystick bit masks
#define JOY_UP     (1 << 0)
#define JOY_DOWN   (1 << 1)
#define JOY_LEFT   (1 << 2)
#define JOY_RIGHT  (1 << 3)
#define JOY_CENTER (1 << 4)

// gpio setup
void GPIO_init(void);
void GPIO_30010_init(void);

// raw input
uint8_t read_joystick(void);
uint8_t read_30010_joystick(void);

// led output
void set_led(uint8_t input);

// input helpers
uint8_t joystick_just_pressed(uint8_t input, uint8_t mask, uint8_t *prev);
uint8_t joystick_get_move(uint8_t raw);
uint8_t joystick_center_just_pressed(uint8_t raw, uint8_t *prev);

void joystick_update(uint8_t *move,
                     uint8_t *center_just_pressed,
                     uint8_t *prev_center);

#endif /* JOYSTICK_H_ */
