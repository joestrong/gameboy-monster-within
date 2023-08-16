#pragma bank 0

#include "sprite_manager.h"
#include "sprites/enemy.h"

uint8_t types[20];
void* objects[20];

void add_sprite(uint8_t type, void* object) {
  types[0] = type;
  objects[0] = object;
}

void update_sprites() {
  switch (types[0]) {
    case SPRITE_TYPE_ENEMY:
      update_enemy((enemy*) objects[0]);
      break;
  }
}