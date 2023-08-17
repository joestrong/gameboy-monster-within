#pragma bank 0

#include "stdint.h"
#include "hitbox.h"
#include "../globals.h"
#include "../sprites/enemy.h"

hitbox get_player_hitbox(uint16_t player_screen_x, uint16_t player_screen_y) {
    hitbox player_hitbox = {
        .x = player_screen_x - 4,
        .y = player_screen_y - 24,
        .x2 = player_screen_x + 4,
        .y2 = player_screen_y,
    };

    return player_hitbox;
}

hitbox get_player_punch_hitbox(uint16_t player_screen_x, uint16_t player_screen_y) {
  hitbox hitbox = {
      .x = player_screen_x - 8,
      .y = player_screen_y - 8,
      .x2 = player_screen_x + 8,
      .y2 = player_screen_y + 8,
  };

  switch (direction) {
    case DIR_UP:
      hitbox.y -= 10;
      hitbox.y2 -= 10;
      break;
    case DIR_DOWN:
      hitbox.y += 10;
      hitbox.y2 += 10;
      break;
    case DIR_LEFT:
      hitbox.x -= 12;
      hitbox.x2 -= 12;
      break;
    case DIR_RIGHT:
      hitbox.x += 12;
      hitbox.x2 += 12;
      break;
  }

  return hitbox;
}

hitbox get_projectile_hitbox(uint16_t x, uint16_t y) {
    hitbox player_hitbox = {
        .x = x,
        .y = y,
        .x2 = x,
        .y2 = y,
    };

    return player_hitbox;
}

hitbox get_enemy_hitbox(enemy* enemy) {
    hitbox hitbox = {
        .x = enemy->x - 4,
        .y = enemy->y - 4,
        .x2 = enemy->x + 4,
        .y2 = enemy->y + 4,
    };

    return hitbox;
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