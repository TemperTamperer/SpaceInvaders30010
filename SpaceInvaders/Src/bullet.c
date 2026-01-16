#include "bullet.h"
#include "game_settings.h"

static Bullet bullets[MAX_BULLETS];

void bullet_init(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
        bullets[i].x = 0;
        bullets[i].y = 0;
    }
}

void bullet_fire(int16_t x, int16_t y) {
    // find første ledige slot
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].active = 1;
            bullets[i].x = x;
            bullets[i].y = y;
            return;
        }
    }
    // hvis ingen ledig -> gør ingenting
}

void bullet_update(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;

        bullets[i].y -= 1; // opad

        if (bullets[i].y < 0) {
            bullets[i].active = 0;
        }
    }
}

const Bullet* bullet_get_array(void) {
    return bullets;
}

void bullet_kill(int index) {
    if (index < 0 || index >= MAX_BULLETS) return;
    bullets[index].active = 0;
}
