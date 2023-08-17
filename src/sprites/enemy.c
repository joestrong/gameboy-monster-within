#pragma bank 0

#include <stdlib.h>
#include "enemy.h"
#include "../tiles/soldier.h"
#include "../globals.h"
#include "../pathfinding.h"
#include "../helpers/vector.h"

enemy* create_enemy(uint8_t x, uint8_t y) {
  enemy* new_enemy = malloc(sizeof(enemy));

  new_enemy->x = x;
  new_enemy->y = y;
  new_enemy->direction = DIR_RIGHT;
  new_enemy->flags = ENEMY_SHOW;
  new_enemy->state = ENEMY_STATE_PATROL;
  new_enemy->shoot_cooldown = 0;
  new_enemy->current_target_x = 0;
  new_enemy->current_target_y = 0;
  new_enemy->current_target = 1;
  new_enemy->target = 2;
  new_enemy->patrol_target_1 = 0;
  new_enemy->patrol_target_2 = 2;

  return new_enemy;
}

void update_enemy(enemy* enemy, uint8_t oam) {
  if (enemy->flags & ENEMY_SHOW > 0) {
    switch (enemy->state) {
      case ENEMY_STATE_STOPPED:
        break;
      case ENEMY_STATE_PATROL:
        if (enemy->current_target_x == 0) {
          waypoint current_target;
          current_target = get_waypoint(enemy->current_target);
          enemy->current_target_x = current_target.x << 3;
          enemy->current_target_y = current_target.y << 3;
        }
        if (enemy->x < enemy->current_target_x) {
          enemy->x++;
        }
        if (enemy->x > enemy->current_target_x) {
          enemy->x--;
        }
        if (enemy->y < enemy->current_target_y) {
          enemy->y++;
        }
        if (enemy->y > enemy->current_target_y) {
          enemy->y--;
        }
        if (enemy->x == enemy->current_target_x && enemy->y == enemy->current_target_y) {
          // End of patrol path, set next target
          if (enemy->target == enemy->current_target) {
            if (enemy->patrol_target_2 != enemy->target) {
              enemy->target = enemy->patrol_target_2;
            } else {
              enemy->target = enemy->patrol_target_1;
            }
          }
          // Get next waypoint
          waypoint next_target;
          next_target = get_next_waypoint(enemy->current_target, enemy->target);
          enemy->current_target = next_target.id;
          enemy->current_target_x = next_target.x << 3;
          enemy->current_target_y = next_target.y << 3;
        }
        break;
      case ENEMY_STATE_ATTACKING:
        if (enemy->shoot_cooldown == 0) {
          vector projectile_vector;
          projectile_vector = get_normalised_vector(
            (player_x + camera_x) - enemy->x,
            (player_y + camera_y) - enemy->y
          );

          projectile.x = enemy->x;
          projectile.y = enemy->y - 10;
          projectile.flags |= PROJECTILE_SHOW;
          projectile.dx = projectile_vector.x;
          projectile.dy = projectile_vector.y;
          enemy->shoot_cooldown = 60;
          set_sprite_tile(OAM_PROJECTILE, projectile_baseTile);
          set_sprite_prop(OAM_PROJECTILE, 0x02);
        } else {
          enemy->shoot_cooldown--;
        }
        break;
    }
    // Animate
    switch (enemy->direction) {
      case DIR_RIGHT:
      case DIR_UP:
      case DIR_DOWN:
        move_metasprite(soldier_metasprites[0], soldier_baseTile, oam, enemy->x - camera_x + player_sprite_x_offset, enemy->y - camera_y + player_sprite_y_offset);
        break;
      case DIR_LEFT:
        move_metasprite_vflip(soldier_metasprites[0], soldier_baseTile, oam, enemy->x - camera_x + player_sprite_x_offset, enemy->y - camera_y + player_sprite_y_offset);
        break;
    }
  } else {
    move_metasprite(soldier_metasprites[0], soldier_baseTile, oam, 0, 0);
  }
}
