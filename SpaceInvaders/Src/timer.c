#include "timer.h"

volatile time_tracker system_time = {0, 0, 0, 0};
volatile uint8_t timer_flag;

// Wait for next timer tick
void timer_wait_for_tick(void)
{
    while (!timer_flag) { }
    timer_flag = 0;
}

// Timer15 setup (50 ms tick)
void timer15_init(void)
{
    RCC->APB2ENR |= RCC_APB2Periph_TIM15;

    TIM15->CR1 = 0x0000;

    // 64 MHz / (PSC+1) = 64 MHz / 50 = 1.28 MHz
    // ARR = 64000 -> 64000 / 1.28 MHz = 0.05 s
    TIM15->ARR = 0xFA00;  // 64000
    TIM15->PSC = 0x0031;  // 49

    // Load PSC/ARR immediately and clear the update flag it triggers
    TIM15->EGR |= 0x0001;
    TIM15->SR  &= ~0x0001;

    TIM15->DIER |= 0x0001;
    NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0x0);
    NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);

    TIM15->CR1 = 0x0001;
}

void TIM1_BRK_TIM15_IRQHandler(void)
{
    timer_flag = 1;

    system_time.hundredths++;

    if (system_time.hundredths >= 100) {
        system_time.hundredths = 0;
        system_time.seconds++;

        if (system_time.seconds >= 60) {
            system_time.seconds = 0;
            system_time.minutes++;

            if (system_time.minutes >= 60) {
                system_time.minutes = 0;
                system_time.hours++;

                if (system_time.hours >= 24) {
                    system_time.hours = 0;
                }
            }
        }
    }

    TIM15->SR &= ~0x0001;
}
