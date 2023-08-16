#include <stdint.h>

#ifndef ENEMY_H
#define ENEMY_H

struct enemy {
    uint8_t x;
    uint8_t y;
    uint8_t direction;
    uint8_t oam_id;
    uint8_t flags;
    uint8_t state;
    uint8_t shoot_cooldown;
    // Waypoints
    uint8_t current_target_x;
    uint8_t current_target_y;
    uint8_t current_target;
    uint8_t target;
    uint8_t patrol_target_1;
    uint8_t patrol_target_2;
};
typedef struct enemy enemy;

#define ENEMY_SHOW 1
#define ENEMY_STATE_STOPPED 0
#define ENEMY_STATE_PATROL 1
#define ENEMY_STATE_ATTACKING 2

enemy* create_enemy(uint8_t x, uint8_t y);

void update_enemy(enemy* my_enemy);

#endif