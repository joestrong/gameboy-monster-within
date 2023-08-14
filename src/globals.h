#include <gb/gb.h>
#include <gb/cgb.h>

#ifndef GLOBALS_H
#define GLOBALS_H

#define overworld_baseTile 0
#define font_baseTile overworld_TILE_COUNT
#define dialog_baseTile font_baseTile + font_TILE_COUNT

#define MIN(A,B) ((A)<(B)?(A):(B))

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

void perLineInterrupt();
void loadGame();
void updateGame();
void process_events();
void show_dialog(char* text);
void show_hint(char* text);
void hide_hud();
void update_enemies();
void update_projectiles();
void check_destruct();
void destroy_tile(uint8_t tile_x, uint8_t tile_y);
void check_bkg_collision();
void check_attack_collision();
void check_projectile_collision();
void set_camera();
void show_debug_marker(uint8_t offset, uint8_t x, uint8_t y);

#endif