#include <stdint.h>
#include "../sprites/enemy.h"

#ifndef HITBOX_H
#define HITBOX_H

struct hitbox {
  uint16_t x;
  uint16_t y;
  uint16_t x2;
  uint16_t y2;
};
typedef struct hitbox hitbox;

hitbox* get_player_hitbox(uint16_t player_screen_x, uint16_t player_screen_y);

hitbox* get_player_punch_hitbox(uint16_t player_screen_x, uint16_t player_screen_y);

hitbox* get_projectile_hitbox(uint16_t x, uint16_t y);

hitbox* get_enemy_hitbox(enemy* enemy);

uint8_t check_hitbox_overlap(hitbox* a, hitbox* b);

#endif