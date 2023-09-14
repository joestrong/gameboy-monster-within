#pragma bank 255

#include <stdlib.h>
#include "projectile.h"
#include "../sprite_manager.h"
#include "../globals.h"
#include "../helpers/hitbox.h"

const uint8_t projectile_tiles[16] = {
  0xC0, 0xC0,
  0xC0, 0xC0,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
};

projectile* create_projectile(uint8_t x, uint8_t y, int8_t dx, int8_t dy) {
  projectile* projectile = malloc(sizeof(projectile));

  projectile->x = x;
  projectile->y = y;
  projectile->flags = 0;
  projectile->dx = dx;
  projectile->dy = dy;

  return projectile;
}

void update_projectile(projectile* projectile, uint8_t oam) {
  projectile->x += projectile->dx;
  projectile->y += projectile->dy;

  check_projectile_collision(projectile);

  set_sprite_tile(oam, projectile_baseTile);
  set_sprite_prop(oam, 0x02);

  uint16_t projectile_pos_x = projectile->x - camera_x;
  uint16_t projectile_pos_y = projectile->y - camera_y;
  if (
    projectile_pos_x >= 0 &&
    projectile_pos_x <= 160 &&
    projectile_pos_y >= 0 &&
    projectile_pos_y <= 144
  ) {
    move_sprite(oam, projectile->x - camera_x + 8, projectile->y - camera_y + 16);
  } else {
    move_sprite(oam, 0, 0);
  }
}

void check_projectile_collision(projectile* projectile) {
  // Collide with BG
  uint8_t tile_id = get_bkg_tile_xy(projectile->x >> 3, projectile->y >> 3);
  if (tile_id == 2 || tile_id == 3 || (tile_id >= 6 && tile_id <= 0x0C)) {
    destroy_sprite(projectile);
    return;
  }

  // Collide with Player
  hitbox* player_hitbox = get_player_hitbox(player_x + camera_x, player_y + camera_y);
  hitbox* projectile_hitbox = get_projectile_hitbox(projectile->x, projectile->y);

  // Debug
  // show_debug_marker(0, player_hitbox.x - camera_x, player_hitbox.y - camera_y);
  // show_debug_marker(1, player_hitbox.x2 - camera_x, player_hitbox.y2 - camera_y);

  if (check_hitbox_overlap(player_hitbox, projectile_hitbox)) {
    destroy_sprite(projectile);
    player_hit();
  }

  free(player_hitbox);
  free(projectile_hitbox);
}