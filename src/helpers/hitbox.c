#include "hitbox.h"

hitbox get_player_hitbox(uint8_t player_screen_x, uint8_t player_screen_y) {
    hitbox player_hitbox = {
        .x = player_screen_x - 4,
        .y = player_screen_y - 24,
        .x2 = player_screen_x + 4,
        .y2 = player_screen_y,
    };

    return player_hitbox;
}

hitbox get_projectile_hitbox(uint8_t x, uint8_t y) {
    hitbox player_hitbox = {
        .x = x,
        .y = y,
        .x2 = x,
        .y2 = y,
    };

    return player_hitbox;
}

uint8_t check_hitbox_overlap(hitbox a, hitbox b) {
    if (
        a.x < b.x2 &&
        a.x2 > b.x &&
        a.y < b.y2 &&
        a.y2 > b.y
    ) {
        return 1;
    }

    return 0;
}