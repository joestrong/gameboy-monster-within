#pragma bank 0

#include "globals.h"
#include "sprite_manager.h"
#include "sprites/enemy.h"

uint8_t types[20];
void* objects[20];
uint8_t count = 0;

void add_sprite(uint8_t type, void* object) {
  types[count] = type;
  objects[count] = object;
  count++;
}

void update_sprites() {
  uint8_t oam = OAM_ENEMY_START;
  for (uint8_t i = 0; i < count; i++) {
    switch (types[i]) {
      case SPRITE_TYPE_ENEMY:
        update_enemy((enemy*) objects[i], oam);
        oam += (soldier_TILE_COUNT / 2);
        break;
      // case SPRITE_TYPE_PROJECTILE:
      //   update_projectile((projectile*) objects[i]);
      //   break;
    }
  }
}