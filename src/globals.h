#include <gb/gb.h>
#include <gb/cgb.h>
#include "./tiles/overworld.h"
#include "./tiles/player.h"
#include "./tiles/arm_h.h"
#include "./tiles/arm_v.h"
#include "./tiles/arm_v_back.h"
#include "./tiles/dialog.h"
#include "./tiles/soldier.h"

#ifndef GLOBALS_H
#define GLOBALS_H

#define debug_TILE_COUNT 1
#define projectile_TILE_COUNT 1

#define overworld_baseTile 0
#define font_baseTile overworld_TILE_COUNT
#define dialog_baseTile font_baseTile + font_TILE_COUNT

#define player_baseTile 0
#define arm_h_baseTile player_TILE_COUNT
#define arm_v_baseTile arm_h_baseTile + arm_h_TILE_COUNT
#define arm_v_back_baseTile arm_v_baseTile + arm_v_TILE_COUNT
#define soldier_baseTile arm_v_back_baseTile + arm_v_back_TILE_COUNT
#define debug_baseTile soldier_baseTile + soldier_TILE_COUNT
#define projectile_baseTile debug_baseTile + debug_TILE_COUNT + 1

#define player_sprite_x_offset 8
#define player_sprite_y_offset 12

#define MIN(A,B) ((A)<(B)?(A):(B))

#define DIR_UP 1
#define DIR_RIGHT 2
#define DIR_DOWN 4
#define DIR_LEFT 8

#define OAM_MANAGER_START 12
#define OAM_DEBUG 38

extern uint8_t state; // 0 - Compo Logo 1 - Title, 2 - Game
extern uint8_t event_state; // 0 - prison cell start, 1 - prison cell
extern uint16_t counter;
extern uint8_t fade_counter;

extern const UWORD palettes[];

extern int16_t camera_x;
extern int16_t camera_y;
extern int16_t old_camera_x;
extern int16_t old_camera_y;
extern int8_t map_pos_x;
extern int8_t map_pos_y;
extern int8_t old_map_pos_x;
extern int8_t old_map_pos_y;
extern uint8_t player_x;
extern uint8_t player_y;

void perLineInterrupt();
void loadGame();
void updateGame();
void process_events();
void show_dialog(char* text);
void show_hint(char* text);
void hide_hud();
void check_destruct();
void destroy_tile(uint8_t tile_x, uint8_t tile_y);
void check_bkg_collision();
void check_attack_collision();
void set_camera();
void show_debug_marker(uint8_t offset, uint8_t x, uint8_t y);

#endif