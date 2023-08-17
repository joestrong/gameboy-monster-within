#pragma bank 0

#include <stdlib.h>
#include "enemy.h"
#include "projectile.h"
#include "../sprite_manager.h"
#include "../tiles/soldier.h"
#include "../globals.h"
#include "../pathfinding.h"
#include "../helpers/hitbox.h"
#include "../helpers/vector.h"

enemy* create_enemy(uint8_t x, uint8_t y) {
  enemy* new_enemy = malloc(sizeof(enemy));

  new_enemy->x = x;
  new_enemy->y = y;
  new_enemy->direction = DIR_RIGHT;
  new_enemy->flags = 0;
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
  check_taking_damage(enemy);

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

        projectile* projectile = create_projectile(
          enemy->x,
          enemy->y - 10, 
          projectile_vector.x, 
          projectile_vector.y
        );
        add_sprite(SPRITE_TYPE_PROJECTILE, projectile);
        enemy->shoot_cooldown = 60;
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
}

void check_taking_damage(enemy* enemy) {
  if ((attack_flags & ATTACKING_PUNCH) == 0) {
    return;
  }

  hitbox player_punch_box;
  player_punch_box = get_player_punch_hitbox(player_x + camera_x, player_y + camera_y);

  hitbox enemy_hitbox;
  enemy_hitbox = get_enemy_hitbox(enemy);

  // Debug
  // show_debug_marker(0, punch_box_x, punch_box_y);
  // show_debug_marker(1, enemy_box_x, enemy_box_y);

  if (check_hitbox_overlap(player_punch_box, enemy_hitbox)) {
    // Overlap
    switch (direction) {
      case DIR_UP:
        enemy->y -= 5;
        break;
      case DIR_DOWN:
        enemy->y += 5;
        break;
      case DIR_LEFT:
        enemy->x -= 5;
        break;
      case DIR_RIGHT:
        enemy->x += 5;
        break;
    }
  }
}
