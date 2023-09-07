#pragma bank 255

#include <stdint.h>
#include <stdlib.h>
#include "hitbox.h"
#include "../globals.h"
#include "../sprites/enemy.h"

hitbox* get_player_hitbox(uint16_t player_screen_x, uint16_t player_screen_y) {
  hitbox* player_hitbox = malloc(sizeof(hitbox));

  player_hitbox->x = player_screen_x - 4;
  player_hitbox->y = player_screen_y - 24;
  player_hitbox->x2 = player_screen_x + 4;
  player_hitbox->y2 = player_screen_y;

  return player_hitbox;
}

hitbox* get_player_punch_hitbox(uint16_t player_screen_x, uint16_t player_screen_y) {
  hitbox* my_hitbox = malloc(sizeof(hitbox));

  my_hitbox->x = player_screen_x - 8;
  my_hitbox->y = player_screen_y - 8;
  my_hitbox->x2 = player_screen_x + 8;
  my_hitbox->y2 = player_screen_y + 8;

  switch (direction) {
    case DIR_UP:
      my_hitbox->y -= 10;
      my_hitbox->y2 -= 10;
      break;
    case DIR_DOWN:
      my_hitbox->y += 10;
      my_hitbox->y2 += 10;
      break;
    case DIR_LEFT:
      my_hitbox->x -= 12;
      my_hitbox->x2 -= 12;
      break;
    case DIR_RIGHT:
      my_hitbox->x += 12;
      my_hitbox->x2 += 12;
      break;
  }

  return my_hitbox;
}

hitbox* get_projectile_hitbox(uint16_t x, uint16_t y) {
  hitbox* hitbox = malloc(sizeof(hitbox));

  hitbox->x = x;
  hitbox->y = y;
  hitbox->x2 = x;
  hitbox->y2 = y;

  return hitbox;
}

hitbox* get_enemy_hitbox(enemy* enemy) {
  hitbox* enemy_hitbox = malloc(sizeof(hitbox));

  enemy_hitbox->x = enemy->x - 4;
  enemy_hitbox->y = enemy->y - 4;
  enemy_hitbox->x2 = enemy->x + 4;
  enemy_hitbox->y2 = enemy->y + 4;

  return enemy_hitbox;
}

uint8_t check_hitbox_overlap(hitbox* a, hitbox* b) {
  if (
    a->x < b->x2 &&
    a->x2 > b->x &&
    a->y < b->y2 &&
    a->y2 > b->y
  ) {
    return 1;
  }

  return 0;
}