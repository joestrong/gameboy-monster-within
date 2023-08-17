#pragma bank 0

#include "globals.h"
#include "sprite_manager.h"
#include "sprites/enemy.h"
#include "sprites/projectile.h"

uint8_t types[20];
void* objects[20];
uint8_t count = 0;

void add_sprite(uint8_t type, void* object) {
  types[count] = type;
  objects[count] = object;
  count++;
}

void update_sprites() {
  uint8_t oam = OAM_MANAGER_START;
  for (uint8_t i = 0; i < count; i++) {
    switch (types[i]) {
      case SPRITE_TYPE_ENEMY:
        update_enemy((enemy*) objects[i], oam);
        oam += (soldier_TILE_COUNT / 2);
        break;
      case SPRITE_TYPE_PROJECTILE:
        update_projectile((projectile*) objects[i], oam);
        oam += 1;
        break;
    }
  }

  for (uint8_t i = oam; i < 40; i++) {
    hide_sprite(i);
  }
}

void destroy_sprite(void* object) {
  for (uint8_t i = 0; i < count; i++) {
    if (object != objects[i]) {
      continue;
    }

    if (i < (count - 1)) {
      objects[i] = objects[count - 1];
    }

    count--;
  }
}