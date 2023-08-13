#include <stdint.h>

#ifndef HITBOX_H
#define HITBOX_H

struct hitbox {
  uint8_t x;
  uint8_t y;
  uint8_t x2;
  uint8_t y2;
};
typedef struct hitbox hitbox;

hitbox get_player_hitbox(uint8_t player_screen_x, uint8_t player_screen_y);

hitbox get_projectile_hitbox(uint8_t x, uint8_t y);

uint8_t check_hitbox_overlap(hitbox a, hitbox b);

#endif