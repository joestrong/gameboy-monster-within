#pragma bank 0

#include <stdlib.h>
#include "enemy.h"
#include "../globals.h"

enemy* create_enemy(uint8_t x, uint8_t y) {
  enemy* new_enemy = malloc(sizeof(enemy));

  new_enemy->x = x;
  new_enemy->y = y;
  new_enemy->direction = DIR_RIGHT;
  new_enemy->oam_id = OAM_ENEMY_START;
  new_enemy->flags = 0;
  new_enemy->state = ENEMY_STATE_STOPPED;
  new_enemy->shoot_cooldown = 0;
  new_enemy->current_target_x = 0;
  new_enemy->current_target_y = 0;
  new_enemy->current_target = 1;
  new_enemy->target = 2;
  new_enemy->patrol_target_1 = 0;
  new_enemy->patrol_target_2 = 2;

  return new_enemy;
}

void update_enemy(enemy* my_enemy) {
  // TODO
}