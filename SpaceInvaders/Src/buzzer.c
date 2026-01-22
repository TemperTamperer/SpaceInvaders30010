#include "buzzer.h"
#include "stm32f30x_conf.h"

// Hardware mapping: PB10 -> TIM2_CH3 (PWM output)
#define SPK_GPIO        GPIOB
#define SPK_PIN         GPIO_Pin_10
#define SPK_PIN_SOURCE  GPIO_PinSource10
#define SPK_GPIO_CLK    RCC_AHBPeriph_GPIOB

#define SPK_AF          GPIO_AF_1
#define SPK_TIM         TIM2
#define SPK_TIM_CLK     RCC_APB1Periph_TIM2

#define SYSCLK_HZ       64000000u
#define PRESCALER       (64 - 1)                 // 64 MHz / 64 = 1 MHz timer tick
#define TICK_HZ         (SYSCLK_HZ / 64)

#define BUZZER_REST     0
#define TIM_EGR_UG      1u
#define TIMER_DEFAULT_PERIOD 1000u              // Initial period (overwritten when playing notes)

// Internal state for a song player
typedef struct {
    buzzer_song_t song;
    size_t idx;
    uint16_t remaining_ms;
    uint8_t playing;
    uint8_t loop;
} seq_player_t;

static seq_player_t bg = {0};
static seq_player_t fx = {0};

static uint8_t bg_was_playing = 0;
static size_t bg_saved_idx = 0;
static uint16_t bg_saved_remaining = 0;

// Sound effects (freq in Hz, duration in ms)
static const buzzer_note_t SFX_SHOOT_NOTES[] = {
    {1200, 30}, {BUZZER_REST, 10}
};

static const buzzer_note_t SFX_ENEMY_DEATH_NOTES[] = {
    {600, 50}, {400, 80}, {BUZZER_REST, 30}
};

static const buzzer_note_t SFX_PLAYER_HIT_NOTES[] = {
    {250, 120}, {BUZZER_REST, 40}
};

static const buzzer_note_t SFX_BONUS_NOTES[] = {
    {900, 60}, {1100, 60}, {1300, 100}, {BUZZER_REST, 40}
};

static const buzzer_note_t SFX_VICTORY_NOTES[] = {
    {784, 120}, {988, 120}, {1175, 220}, {BUZZER_REST, 60}
};

static const buzzer_note_t SFX_GAMEOVER_NOTES[] = {
    {523, 150}, {494, 150}, {440, 300}, {BUZZER_REST, 80}
};

#define SONG(arr) (buzzer_song_t){ (arr), sizeof(arr)/sizeof((arr)[0]) }

// Set PWM frequency. freq = 0 means silence.
static void speaker_set_freq(uint16_t freq)
{
    if (freq == BUZZER_REST) {
        SPK_TIM->CCR3 = 0;   // duty = 0 -> silent
        return;
    }

    uint32_t arr = (TICK_HZ / freq) - 1;
    if (arr > 0xFFFF) {
        arr = 0xFFFF;
    }

    SPK_TIM->ARR  = arr;
    SPK_TIM->CCR3 = arr / 2;     // ~50% duty cycle
    SPK_TIM->EGR  = TIM_EGR_UG;  // apply new ARR/CCR
}

void buzzer_init(void)
{
    // PB10 as alternate function (TIM2_CH3)
    RCC_AHBPeriphClockCmd(SPK_GPIO_CLK, ENABLE);

    GPIO_InitTypeDef g;
    g.GPIO_Pin = SPK_PIN;
    g.GPIO_Mode = GPIO_Mode_AF;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_Speed = GPIO_Speed_10MHz;
    g.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SPK_GPIO, &g);

    GPIO_PinAFConfig(SPK_GPIO, SPK_PIN_SOURCE, SPK_AF);

    // Timer 2 setup
    RCC_APB1PeriphClockCmd(SPK_TIM_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef t;
    TIM_TimeBaseStructInit(&t);
    t.TIM_Prescaler = PRESCALER;
    t.TIM_Period = TIMER_DEFAULT_PERIOD;
    TIM_TimeBaseInit(SPK_TIM, &t);

    TIM_OCInitTypeDef oc;
    TIM_OCStructInit(&oc);
    oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_Pulse = 0;
    TIM_OC3Init(SPK_TIM, &oc);
    TIM_OC3PreloadConfig(SPK_TIM, TIM_OCPreload_Enable);

    TIM_Cmd(SPK_TIM, ENABLE);
}

// Start playing from current index
static void seq_start(seq_player_t *p)
{
    p->playing = 1;
    p->remaining_ms = p->song.notes[p->idx].dur_ms;
    speaker_set_freq(p->song.notes[p->idx].freq_hz);
}

// Move to next note (or stop)
static void seq_next(seq_player_t *p)
{
    p->idx++;

    if (p->idx >= p->song.len) {
        if (p->loop) {
            p->idx = 0;
        } else {
            p->playing = 0;
            speaker_set_freq(BUZZER_REST);
            return;
        }
    }

    p->remaining_ms = p->song.notes[p->idx].dur_ms;
    speaker_set_freq(p->song.notes[p->idx].freq_hz);
}

void buzzer_set_bg(buzzer_song_t song, uint8_t loop)
{
    bg.song = song;
    bg.loop = loop;
    bg.idx = 0;
    bg.playing = 0;
}

void buzzer_bg_start(void)
{
    seq_start(&bg);
}

void buzzer_play_sfx(buzzer_sfx_t sfx)
{
    // Save background state so we can resume after the SFX
    bg_was_playing = bg.playing;
    bg_saved_idx = bg.idx;
    bg_saved_remaining = bg.remaining_ms;
    bg.playing = 0;

    switch (sfx) {
        case SFX_SHOOT:       fx.song = SONG(SFX_SHOOT_NOTES); break;
        case SFX_ENEMY_DEATH: fx.song = SONG(SFX_ENEMY_DEATH_NOTES); break;
        case SFX_PLAYER_HIT:  fx.song = SONG(SFX_PLAYER_HIT_NOTES); break;
        case SFX_BONUS:       fx.song = SONG(SFX_BONUS_NOTES); break;
        case SFX_VICTORY:     fx.song = SONG(SFX_VICTORY_NOTES); break;
        case SFX_GAMEOVER:    fx.song = SONG(SFX_GAMEOVER_NOTES); break;
    }

    fx.idx = 0;
    fx.loop = 0;
    seq_start(&fx);
}

void buzzer_update(uint16_t elapsed_ms)
{
    // If SFX is playing, it has priority
    if (fx.playing) {
        if (fx.remaining_ms > elapsed_ms) {
            fx.remaining_ms -= elapsed_ms;
        } else {
            seq_next(&fx);
        }

        // Resume background when SFX ends
        if (!fx.playing && bg_was_playing) {
            bg.playing = 1;
            bg.idx = bg_saved_idx;
            bg.remaining_ms = bg_saved_remaining;
            speaker_set_freq(bg.song.notes[bg.idx].freq_hz);
        }
        return;
    }

    // Background music update
    if (bg.playing) {
        if (bg.remaining_ms > elapsed_ms) {
            bg.remaining_ms -= elapsed_ms;
        } else {
            seq_next(&bg);
        }
    }
}
