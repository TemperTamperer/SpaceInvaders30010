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

void set_led(uint8_t input)
{
    if (input == 0)
    {
        GPIOA->ODR |= (1 << 9);
        GPIOC->ODR |= (1 << 7);
        GPIOB->ODR |= (1 << 4);
        return;
    }

    if (input == JOY_UP)    GPIOA->ODR &= ~(1 << 9);
    if (input == JOY_DOWN)  GPIOC->ODR &= ~(1 << 7);
    if (input == JOY_LEFT)  GPIOB->ODR &= ~(1 << 4);
}
