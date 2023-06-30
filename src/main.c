#include <stdint.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/metasprites.h>
#include "./tiles/gbcompologo.h"
#include "./tiles/title.h"
#include "./tiles/overworld.h"
#include "./tiles/sprites.h"

#define skip_intro 1

#define MIN(A,B) ((A)<(B)?(A):(B))
#define bigmap_mapHeight 32
#define bigmap_mapWidth 32

void loadCompoLogoScreen();
void updateCompoLogoScreen();
void loadTitleScreen();
void updateTitleScreen();
void loadGame();
void updateGame();
void set_camera();

uint8_t state = 0; // 0 - Compo Logo 1 - Title, 2 - Game
uint8_t counter = 0;

int16_t camera_x = 0;
int16_t camera_y = 0;
int16_t old_camera_x = 0;
int16_t old_camera_y = 0;
int8_t map_pos_x = 0;
int8_t map_pos_y = 0;
int8_t old_map_pos_x = 0;
int8_t old_map_pos_y = 0;
uint8_t player_x = 72;
uint8_t player_y = 80;

const UWORD palettes[] = {
    RGB_WHITE, RGB_WHITE, RGB_WHITE, RGB_WHITE, // BG Fade-in
    RGB_WHITE, RGB_LIGHTGRAY, RGB_WHITE, RGB_WHITE, // BG Fade-in
    RGB_WHITE, RGB_LIGHTGRAY, RGB_DARKGRAY, RGB_WHITE, // BG Fade-in 2
    RGB_WHITE, RGB_LIGHTGRAY, RGB_DARKGRAY, RGB_BLACK, // BG B&W
    RGB_WHITE, RGB_BLUE, RGB_DARKBLUE, RGB_BLACK, // Sprite Blue
};

void main() {
  DISPLAY_OFF;
  LCDC_REG = LCDCF_OFF | LCDCF_BG8800 | LCDCF_BG9800 | LCDCF_BGON | LCDCF_OBJON;
  SPRITES_8x16;

  // Set pallette defaults
  set_bkg_palette(0, 1, &palettes[0]);
  set_sprite_palette(0, 1, &palettes[4*4]);
  #if skip_intro == 1
    state = 2;
    loadGame();
  #else
    loadCompoLogoScreen();
  #endif

  while (1) {
    wait_vbl_done();

    if (state == 0) {
      updateCompoLogoScreen();
    }

    if (state == 1) {
      updateTitleScreen();
    }

    if (state == 2) {
      updateGame();
    }
  }
}

void loadCompoLogoScreen() {
  set_bkg_data(0x00, gbcompologo_TILE_COUNT, gbcompologo_tiles);
  set_bkg_tiles(0, 0, 20, 18, gbcompologo_map);
  counter = 0;
  set_bkg_palette(0, 1, &palettes[0]);
   
  DISPLAY_ON;
}

void updateCompoLogoScreen() {
  switch (counter) {
    case 5:
      set_bkg_palette(0, 1, &palettes[1*4]);
      break;
    case 10:
      set_bkg_palette(0, 1, &palettes[2*4]);
      break;
    case 15:
      set_bkg_palette(0, 1, &palettes[3*4]);
      break;
    case 120:
      set_bkg_palette(0, 1, &palettes[2*4]);
      break;
    case 125:
      set_bkg_palette(0, 1, &palettes[1*4]);
      break;
    case 130:
      set_bkg_palette(0, 1, &palettes[0]);
      break;
    case 135:
      state = 1;
      loadTitleScreen();
      break;
  }
  counter++;
}

void loadTitleScreen() {
  DISPLAY_OFF;

  set_bkg_data(0x00, title_TILE_COUNT, title_tiles);
  set_bkg_tiles(0, 0, 20, 18, title_map);
  counter = 0;
   
  DISPLAY_ON;
}

void updateTitleScreen() {
  switch (counter) {
    case 5:
      set_bkg_palette(0, 1, &palettes[1*4]);
      break;
    case 10:
      set_bkg_palette(0, 1, &palettes[2*4]);
      break;
    case 15:
      set_bkg_palette(0, 1, &palettes[3*4]);
      break;
    case 160:
      set_bkg_palette(0, 1, &palettes[2*4]);
      break;
    case 165:
      set_bkg_palette(0, 1, &palettes[1*4]);
      break;
    case 170:
      set_bkg_palette(0, 1, &palettes[0]);
      break;
    case 175:
      state = 2;
      loadGame();
      break;
  }
  counter++;
}

void loadGame() {
  DISPLAY_OFF;

  set_bkg_data(0x00, overworld_TILE_COUNT, overworld_tiles);
  set_bkg_submap(0, 0, 20, 18, overworld_map, bigmap_mapWidth);
  counter = 0;

  set_sprite_data(0x00, sprites_TILE_COUNT, sprites_tiles);

    move_metasprite(sprites_metasprites[0], 0, 0, player_x, player_y);
    move_metasprite(sprites_metasprites[2], 0, 4, player_x - 8, player_y + 8);
    move_metasprite_vflip(sprites_metasprites[2], 0, 8, player_x + 8, player_y + 8);

  DISPLAY_ON;
}

void updateGame() {
  switch (counter) {
    case 5:
      set_bkg_palette(0, 1, &palettes[1*4]);
      break;
    case 10:
      set_bkg_palette(0, 1, &palettes[2*4]);
      break;
    case 15:
      set_bkg_palette(0, 1, &palettes[3*4]);
      break;
  }
  if (counter < 15) {
    counter++;
  }

  int keys = joypad();
  if (keys & J_UP) {
    move_metasprite(sprites_metasprites[0], 0, 0, player_x, player_y);
    move_metasprite(sprites_metasprites[2], 0, 4, player_x - 8, player_y + 8);
    move_metasprite_vflip(sprites_metasprites[2], 0, 8, player_x + 8, player_y + 8);
    camera_y--;
  }
  if (keys & J_DOWN) {
    move_metasprite(sprites_metasprites[0], 0, 0, player_x, player_y);
    move_metasprite(sprites_metasprites[2], 0, 4, player_x - 8, player_y + 8);
    move_metasprite_vflip(sprites_metasprites[2], 0, 8, player_x + 8, player_y + 8);
    camera_y++;
  }
  if (keys & J_LEFT) {
    move_metasprite(sprites_metasprites[1], 0, 4, player_x, player_y);
    move_metasprite(sprites_metasprites[2], 0, 8, player_x - 8, player_y + 8);
    move_metasprite_vflip(sprites_metasprites[2], 0, 0, player_x + 8, player_y + 8);
    camera_x--;
  }
  if (keys & J_RIGHT) {
    move_metasprite_vflip(sprites_metasprites[1], 0, 4, player_x, player_y);
    move_metasprite(sprites_metasprites[2], 0, 0, player_x - 8, player_y + 8);
    move_metasprite_vflip(sprites_metasprites[2], 0, 8, player_x + 8, player_y + 8);
    camera_x++;
  }

  if (camera_x < 0) {
    camera_x = 0;
  }
  if (camera_y < 0) {
    camera_y = 0;
  }
  
  set_camera();
}

void set_camera() {
    SCY_REG = camera_y;
    SCX_REG = camera_x; 
    
    map_pos_y = (uint8_t)(camera_y >> 3u);
    if (map_pos_y != old_map_pos_y) { 
        if (camera_y < old_camera_y) {
            set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, bigmap_mapWidth-map_pos_x), 1, overworld_map, bigmap_mapWidth);
        } else {
            if ((bigmap_mapHeight - 18u) > map_pos_y) {
              set_bkg_submap(map_pos_x, map_pos_y + 18u, MIN(21u, bigmap_mapWidth-map_pos_x), 1, overworld_map, bigmap_mapWidth);
            }
        }
        old_map_pos_y = map_pos_y; 
    }
    
    map_pos_x = (uint8_t)(camera_x >> 3u);
    if (map_pos_x != old_map_pos_x) {
        if (camera_x < old_camera_x) {
            set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, bigmap_mapHeight - map_pos_y), overworld_map, bigmap_mapWidth);     
        } else {
            if ((bigmap_mapWidth - 20u) > map_pos_x) set_bkg_submap(map_pos_x + 20u, map_pos_y, 1, MIN(19u, bigmap_mapHeight - map_pos_y), overworld_map, bigmap_mapWidth);     
        }
        old_map_pos_x = map_pos_x;
    }
    
    old_camera_x = camera_x;
    old_camera_y = camera_y;
}