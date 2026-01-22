#include "joystick.h"

uint8_t joystick_just_pressed(uint8_t input, uint8_t mask, uint8_t *prev)
{
    uint8_t now = (input & mask) ? 1 : 0;
    uint8_t jp = (now && !(*prev)) ? 1 : 0;
    *prev = now;
    return jp;
}

uint8_t joystick_get_move(uint8_t raw)
{
    return raw & (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT);
}

uint8_t joystick_center_just_pressed(uint8_t raw, uint8_t *prev)
{
    return joystick_just_pressed(raw, JOY_CENTER, prev);
}

void joystick_update(uint8_t *move,
                     uint8_t *center_just_pressed,
                     uint8_t *prev_center)
{
    uint8_t raw = read_joystick();
    *move = joystick_get_move(raw);
    *center_just_pressed = joystick_center_just_pressed(raw, prev_center);
}

void GPIO_init(void)
{
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;

    GPIOA->MODER &= ~(0x00000003 << (0 * 2));
    GPIOA->MODER |=  (0x00000000 << (0 * 2));
    GPIOA->PUPDR &= ~(0x00000003 << (0 * 2));
    GPIOA->PUPDR |=  (0x00000002 << (0 * 2));

    GPIOB->MODER &= ~(0x00000003 << (0 * 2));
    GPIOB->MODER |=  (0x00000000 << (0 * 2));
    GPIOB->PUPDR &= ~(0x00000003 << (0 * 2));
    GPIOB->PUPDR |=  (0x00000002 << (0 * 2));

    GPIOC->MODER &= ~(0x00000003 << (0 * 2));
    GPIOC->MODER |=  (0x00000000 << (0 * 2));
    GPIOC->PUPDR &= ~(0x00000003 << (0 * 2));
    GPIOC->PUPDR |=  (0x00000002 << (0 * 2));

    GPIOA->MODER &= ~(0x00000003 << (9 * 2));
    GPIOA->MODER |=  (0x00000001 << (9 * 2));

    GPIOC->MODER &= ~(0x00000003 << (7 * 2));
    GPIOC->MODER |=  (0x00000001 << (7 * 2));

    GPIOB->MODER &= ~(0x00000003 << (4 * 2));
    GPIOB->MODER |=  (0x00000001 << (4 * 2));

	//LED

	//Set PA9 (red) to read
	GPIOA->MODER &= ~(0x00000003 << (9 * 2)); // Clear mode register
	GPIOA->MODER |= (0x00000001 << (9 * 2)); // Set mode register (0x00 –Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)

	//Set PC7 (green) to read
	GPIOC->MODER &= ~(0x00000003 << (7 * 2));
	GPIOC->MODER |= (0x00000001 << (7 * 2));

	//Set PB4 (blue) to read
	GPIOB->MODER &= ~(0x00000003 << (4 * 2));
	GPIOB->MODER |= (0x00000001 << (4 * 2));
}

uint8_t read_joystick(void)
{
    if (GPIOA->IDR & (1 << 4)) return JOY_UP;
    if (GPIOB->IDR & (1 << 0)) return JOY_DOWN;
    if (GPIOC->IDR & (1 << 1)) return JOY_LEFT;
    if (GPIOC->IDR & (1 << 0)) return JOY_RIGHT;
    if (GPIOB->IDR & (1 << 5)) return JOY_CENTER;
    return 0;
}
