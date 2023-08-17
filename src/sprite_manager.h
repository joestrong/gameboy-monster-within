#include "stdint.h"

#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#define SPRITE_TYPE_ENEMY 0
#define SPRITE_TYPE_PROJECTILE 1

void add_sprite(uint8_t type, void* object);

void update_sprites();

void destroy_sprite(void* object);

#endif