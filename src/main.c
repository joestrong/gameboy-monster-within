#include <stdint.h>
#include <string.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/metasprites.h>
#include <gbdk/font.h>
#include "./tiles/gbcompologo.h"
#include "./tiles/title.h"
#include "./tiles/overworld.h"
#include "./tiles/player.h"
#include "./tiles/arm_h.h"
#include "./tiles/arm_v.h"
#include "./tiles/dialog.h"

#define skip_intro 1

#define MIN(A,B) ((A)<(B)?(A):(B))
#define bigmap_mapHeight 32
#define bigmap_mapWidth 32

#define font_BYTE_OFFSET 394
#define font_TILE_COUNT 26

#define overworld_baseTile 0
#define font_baseTile overworld_TILE_COUNT
#define dialog_baseTile font_baseTile + font_TILE_COUNT

#define player_baseTile 0
#define arm_h_baseTile player_TILE_COUNT
#define arm_v_baseTile arm_h_baseTile + arm_h_TILE_COUNT

#define player_sprite_x_offset 16
#define player_sprite_y_offset 16

#define DIR_UP 1
#define DIR_RIGHT 2
#define DIR_DOWN 4
#define DIR_LEFT 8

#define EVENT_PRISON_CELL_START 0
#define EVENT_PRISON_CELL 1
#define EVENT_PRISON_CELL_POST_SMASH 2

#define HUD_Y 104
#define HINT_HEIGHT 16

#define HUD_DIALOG_MODE 1
#define HUD_HINT_MODE 2

void perLineInterrupt();
void loadCompoLogoScreen();
void updateCompoLogoScreen();
void loadTitleScreen();
void updateTitleScreen();
void loadGame();
void updateGame();
void process_events();
void show_dialog(char* text);
void show_hint(char* text);
void hide_hud();
void check_collision();
void set_camera();

uint8_t state = 0; // 0 - Compo Logo 1 - Title, 2 - Game
uint8_t event_state = 0; // 0 - prison cell start, 1 - prison cell
uint16_t counter = 0;
uint8_t fade_counter = 0;

uint8_t hud_control;

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
uint8_t player_sprite_x = 0;
uint8_t player_sprite_y = 0;
uint8_t direction = 0;

const UWORD palettes[] = {
    RGB_WHITE, RGB_WHITE, RGB_WHITE, RGB_WHITE, // BG Fade-in
    RGB_WHITE, RGB_LIGHTGRAY, RGB_WHITE, RGB_WHITE, // BG Fade-in
    RGB_WHITE, RGB_LIGHTGRAY, RGB_DARKGRAY, RGB_WHITE, // BG Fade-in 2
    RGB_WHITE, RGB_LIGHTGRAY, RGB_DARKGRAY, RGB_BLACK, // BG B&W
    RGB_WHITE, RGB_BLUE, RGB_DARKBLUE, RGB_BLACK, // Sprite Blue
    RGB_YELLOW, RGB_WHITE, RGB_BLACK, RGB_BLACK, // Dialog
};

void main() {
  DISPLAY_OFF;
  LCDC_REG = LCDCF_OFF | LCDCF_BG8800 | LCDCF_BG9800 | LCDCF_WIN9C00 | LCDCF_BGON | LCDCF_OBJON;
  SPRITES_8x16;

  // Set pallette defaults
  set_bkg_palette(0, 1, &palettes[0]);
  set_sprite_palette(0, 1, &palettes[4*4]);
  set_bkg_palette(1, 1, &palettes[5*4]); // Dialog
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

void perLineInterrupt() {
  if (LY_REG == HUD_Y) {
    SHOW_WIN;
    if (hud_control & HUD_HINT_MODE) {
      LYC_REG = HUD_Y + HINT_HEIGHT;
    }
  }
  // Hint Mode end line
  if (LY_REG == HUD_Y + HINT_HEIGHT) {
    HIDE_WIN;
    LYC_REG = HUD_Y;
  }
}

void loadCompoLogoScreen() {
  set_bkg_data(0x00, gbcompologo_TILE_COUNT, gbcompologo_tiles);
  set_bkg_tiles(0, 0, 20, 18, gbcompologo_map);
  fade_counter = 0;
  set_bkg_palette(0, 1, &palettes[0]);
   
  DISPLAY_ON;
}

void updateCompoLogoScreen() {
  switch (fade_counter) {
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
  fade_counter++;
}

void loadTitleScreen() {
  DISPLAY_OFF;

  set_bkg_data(0x00, title_TILE_COUNT, title_tiles);
  set_bkg_tiles(0, 0, 20, 18, title_map);
  fade_counter = 0;
   
  DISPLAY_ON;
}

void updateTitleScreen() {
  switch (fade_counter) {
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
  fade_counter++;
}

void loadGame() {
  DISPLAY_OFF;

  fade_counter = 0;

  // BKG
  set_bkg_data(overworld_baseTile, overworld_TILE_COUNT, overworld_tiles);
  set_bkg_submap(0, 0, 21, 19, overworld_map, bigmap_mapWidth);
  // Font
  set_bkg_1bpp_data(font_baseTile, font_TILE_COUNT, &font_spect[font_BYTE_OFFSET]);

  // Window
  set_bkg_data(dialog_baseTile, dialog_TILE_COUNT, dialog_tiles);
  VBK_REG=VBK_ATTRIBUTES;
  set_win_tiles(0, 0, 32, 5, dialog_map_attributes);
  VBK_REG=VBK_TILES;
  set_win_based_tiles(0, 0, 32, 5, dialog_map, dialog_baseTile);
  WX_REG = 167;
  WY_REG = 144;
  SHOW_WIN;

  set_sprite_data(0x00, player_TILE_COUNT, player_tiles);
  set_sprite_data(arm_h_baseTile, arm_h_TILE_COUNT, arm_h_tiles);
  set_sprite_data(arm_v_baseTile, arm_v_TILE_COUNT, arm_v_tiles);

  player_sprite_x = player_x + player_sprite_x_offset;
  player_sprite_y = player_y + player_sprite_y_offset;

  move_metasprite(player_metasprites[0], player_baseTile, 0, player_sprite_x, player_sprite_y);
  move_metasprite(arm_v_metasprites[0], arm_v_baseTile, 4, player_sprite_x - 8, player_sprite_y + 8);
  move_metasprite_vflip(arm_v_metasprites[0], arm_v_baseTile, 8, player_sprite_x + 8, player_sprite_y + 8);

  // Set up LY=LYC interrupt
  LYC_REG = HUD_Y;
  STAT_REG |= STATF_LYC;
  add_LCD(perLineInterrupt);
  set_interrupts(VBL_IFLAG | LCD_IFLAG);

  DISPLAY_ON;
}

void updateGame() {
  switch (fade_counter) {
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
  if (fade_counter < 15) {
    fade_counter++;
  }

  process_events();

  player_sprite_x = player_x + player_sprite_x_offset;
  player_sprite_y = player_y + player_sprite_y_offset;

  direction = 0;

  int keys = joypad();

  if (keys & J_UP) {
    move_metasprite(player_metasprites[0], player_baseTile, 0, player_sprite_x, player_sprite_y);
    move_metasprite(arm_v_metasprites[0], arm_v_baseTile, 4, player_sprite_x - 8, player_sprite_y + 8);
    move_metasprite_vflip(arm_v_metasprites[0], arm_v_baseTile, 8, player_sprite_x + 8, player_sprite_y + 8);
    direction |= DIR_UP;
    player_y--;
  } else if (keys & J_DOWN) {
    move_metasprite(player_metasprites[0], player_baseTile, 0, player_sprite_x, player_sprite_y);
    move_metasprite(arm_v_metasprites[0], arm_v_baseTile, 4, player_sprite_x - 8, player_sprite_y + 8);
    move_metasprite_vflip(arm_v_metasprites[0], arm_v_baseTile, 8, player_sprite_x + 8, player_sprite_y + 8);
    direction |= DIR_DOWN;
    player_y++;
  }

  if (keys & J_LEFT) {
    move_metasprite(arm_h_metasprites[0], arm_h_baseTile, 0, player_sprite_x - 8, player_sprite_y);
    move_metasprite(player_metasprites[1], player_baseTile, 3, player_sprite_x, player_sprite_y);
    move_metasprite(arm_h_metasprites[0], arm_h_baseTile, 7, player_sprite_x - 8, player_sprite_y - 2);
    hide_sprite(10);
    hide_sprite(11);
    direction |= DIR_LEFT;
    player_x--;
  } else if (keys & J_RIGHT) {
    move_metasprite_vflip(arm_h_metasprites[0], arm_h_baseTile, 0, player_sprite_x + 8, player_sprite_y);
    move_metasprite_vflip(player_metasprites[1], player_baseTile, 3, player_sprite_x, player_sprite_y);
    move_metasprite_vflip(arm_h_metasprites[0], arm_h_baseTile, 7, player_sprite_x + 8, player_sprite_y - 2);
    hide_sprite(10);
    hide_sprite(11);
    direction |= DIR_RIGHT;
    player_x++;
  }

  check_collision();

  // Camera updates

  if (player_x < 72 && camera_x > 0) {
    player_x = 72;
    camera_x--;
  }
  if (player_x > 72 && camera_x + 160 < overworld_WIDTH) {
    player_x = 72;
    camera_x++;
  }
  if (player_y < 80 && camera_y > 0) {
    player_y = 80;
    camera_y--;
  }
  if (player_y > 80 && camera_y + 144 < overworld_HEIGHT) {
    player_y = 80;
    camera_y++;
  }
  
  set_camera();

  counter++;
}

void process_events() {
  switch (event_state) {
    case EVENT_PRISON_CELL_START:
      if (counter == 180) {
        show_dialog("WHO AM I");
      }
      if (counter == 300) {
        show_hint("     HOLD A AND B       TO TRANSFORM    ");
      }
      break;
    case EVENT_PRISON_CELL:
      break;
    case EVENT_PRISON_CELL_POST_SMASH:
      if (counter == 180) {
        show_dialog("WHAT AM I");
      }
      if (counter == 300) {
        hide_hud();
      }
      break;
      break;
  }
}

void show_dialog(char* text) {
  hud_control &= ~HUD_HINT_MODE;
  WX_REG = 7;
  WY_REG = HUD_Y;
  fill_win_rect(1, 1, 18, 3, dialog_baseTile + 4);
  set_win_based_tiles(1, 1, strlen(text), 1, text, font_baseTile - 65);
}

void show_hint(char* text) {
  hud_control |= HUD_HINT_MODE;
  WX_REG = 7;
  WY_REG = HUD_Y;
  fill_win_rect(0, 0, 20, 2, dialog_baseTile + 4);
  set_win_based_tiles(0, 0, 20, 2, text, font_baseTile - 65);
}

void hide_hud() {
  WX_REG = 167;
  WY_REG = 144;
}

void check_collision() {
  uint8_t offset_x = (direction & DIR_RIGHT) ? 13 : 3;
  uint8_t offset_y = (direction & DIR_DOWN) ? 6 : 2;
  uint8_t x_tile = ((camera_x + player_x + offset_x) % 255) >> 3u;
  uint8_t y_tile = ((camera_y + player_y + offset_y) % 255) >> 3u;
  uint8_t tile_id = get_bkg_tile_xy(x_tile, y_tile);
  if (tile_id < 2 || (tile_id > 5 && tile_id < 0x0C)) {
    // Collide
    if (direction & DIR_UP) {
      player_y++;
    }
    if (direction & DIR_DOWN) {
      player_y--;
    }
    if (direction & DIR_LEFT) {
      player_x++;
    }
    if (direction & DIR_RIGHT) {
      player_x--;
    }
  }
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