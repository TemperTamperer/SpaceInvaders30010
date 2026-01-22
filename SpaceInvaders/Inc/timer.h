#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <stdio.h>
#include "30010_io.h"

/* Timer tick flag */
extern volatile uint8_t timer_flag;

/* Time tracking */
typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t hundredths; // 0–99
} time_tracker;

extern volatile time_tracker system_time;

/* Timer control */
void timer_wait_for_tick(void);
void timer15_init(void);
void TIM1_BRK_TIM15_IRQHandler(void);

#endif
