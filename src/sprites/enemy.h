#include <stdint.h>

#ifndef ENEMY_H
#define ENEMY_H

struct enemy {
    uint16_t x;
    uint16_t y;
    uint8_t direction;
    uint8_t flags;
    uint8_t state;
    uint8_t shoot_cooldown;
    uint8_t health;
    uint8_t hit_cooldown;
    // Waypoints
    uint16_t current_target_x;
    uint16_t current_target_y;
    uint8_t current_target;
    uint8_t target;
    uint8_t patrol_target_1;
    uint8_t patrol_target_2;
};
typedef struct enemy enemy;

#define ENEMY_STATE_STOPPED 0
#define ENEMY_STATE_PATROL 1
#define ENEMY_STATE_ATTACKING 2
#define ENEMY_STATE_HIT 3

enemy* create_enemy(uint16_t x, uint16_t y);

void update_enemy(enemy* my_enemy, uint8_t oam);

void check_taking_damage(enemy* enemy);

#endif