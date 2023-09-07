#include <stdint.h>

#ifndef PROJECTILE_H
#define PROJECTILE_H

extern const uint8_t projectile_tiles[16];

struct projectile {
  uint16_t x;
  uint16_t y;
  uint8_t flags;
  int8_t dx;
  int8_t dy;
};
typedef struct projectile projectile;

projectile* create_projectile(uint8_t x, uint8_t y, int8_t dx, int8_t dy);

void update_projectile(projectile* projectile, uint8_t oam);

void check_projectile_collision(projectile* projectile);

#endif