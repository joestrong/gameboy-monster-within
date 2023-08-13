#include <stdint.h>
#include <string.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/metasprites.h>
#include <gbdk/font.h>
#include "./helpers/hitbox.h"
#include "./tiles/gbcompologo.h"
#include "./tiles/title.h"
#include "./tiles/overworld.h"
#include "./tiles/player.h"
#include "./tiles/arm_h.h"
#include "./tiles/arm_v.h"
#include "./tiles/arm_v_back.h"
#include "./tiles/dialog.h"
#include "./tiles/soldier.h"

#define skip_intro 1

#define MIN(A,B) ((A)<(B)?(A):(B))
#define bigmap_mapHeight 32
#define bigmap_mapWidth 32

#define font_BYTE_OFFSET 394
#define font_TILE_COUNT 26

#define overworld_baseTile 0
#define font_baseTile overworld_TILE_COUNT
#define dialog_baseTile font_baseTile + font_TILE_COUNT

#define debug_TILE_COUNT 1
#define projectile_TILE_COUNT 1

#define player_baseTile 0
#define arm_h_baseTile player_TILE_COUNT
#define arm_v_baseTile arm_h_baseTile + arm_h_TILE_COUNT
#define arm_v_back_baseTile arm_v_baseTile + arm_v_TILE_COUNT
#define soldier_baseTile arm_v_back_baseTile + arm_v_back_TILE_COUNT
#define debug_baseTile soldier_baseTile + soldier_TILE_COUNT
#define projectile_baseTile debug_baseTile + debug_TILE_COUNT + 1

#define player_sprite_x_offset 8
#define player_sprite_y_offset 12

#define OAM_ENEMY_START 12
#define OAM_PROJECTILE 16
#define OAM_DEBUG 38

#define DIR_UP 1
#define DIR_RIGHT 2
#define DIR_DOWN 4
#define DIR_LEFT 8

#define EVENT_PRISON_CELL_START 0
#define EVENT_PRISON_CELL 1
#define EVENT_PRISON_CELL_POST_SMASH 2
#define EVENT_PRISON_CELL_AFTER_BATTLE 3

#define HUD_Y 104
#define HINT_HEIGHT 16

#define HUD_DIALOG_MODE 1
#define HUD_HINT_MODE 2

#define TRANSFORM_LENGTH 15 * 60 // 15 seconds
#define TRANSFORM_COOLDOWN 15 * 60 // 15 seconds

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
void update_enemies();
void update_projectiles();
void check_destruct();
void destroy_tile(uint8_t tile_x, uint8_t tile_y);
void check_bkg_collision();
void check_attack_collision();
void check_projectile_collision();
void set_camera();
void show_debug_marker(uint8_t offset, uint8_t x, uint8_t y);

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
uint8_t direction = 4;
uint8_t direction_pressed = 0;

uint16_t transform_remaining_counter = 0;
uint16_t transform_cooldown_counter = 0;

#define ARM_LEFT 0
#define ARM_RIGHT 1

uint8_t punch_counter = 0;
uint8_t left_punch_frame = 0; // 0, 1, 2
uint8_t right_punch_frame = 0; // 0, 1, 2
uint8_t active_arm = ARM_LEFT;
uint8_t* punch_frame = &left_punch_frame;
uint8_t attack_flags = 0;

#define ATTACKING_PUNCH 1

const UWORD palettes[] = {
  RGB_WHITE, RGB_WHITE, RGB_WHITE, RGB_WHITE, // BG Fade-in
  RGB_WHITE, RGB_LIGHTGRAY, RGB_WHITE, RGB_WHITE, // BG Fade-in
  RGB_WHITE, RGB_LIGHTGRAY, RGB_DARKGRAY, RGB_WHITE, // BG Fade-in 2
  RGB_WHITE, RGB_LIGHTGRAY, RGB_DARKGRAY, RGB_BLACK, // BG B&W

  RGB_YELLOW, RGB_WHITE, RGB_BLACK, RGB_BLACK, // Dialog

  RGB_WHITE, RGB_BLUE, RGB_DARKBLUE, RGB_BLACK, // Sprite Blue
  RGB_WHITE, RGB(0x15, 9, 9), RGB(0x10, 0, 0), RGB(5, 0, 0), // Sprite Red (Enemy)
  RGB_WHITE, RGB_WHITE, RGB_WHITE, RGB_WHITE, // Projectile
};

const BYTE destruct_map[] = {
  0x02, // 0x00 Wall
  0x02, // 0x01 Wall top
  0x02,
  0x03,
  0x04,
  0x05,
  0x04, // 0x06 Fence/Gate
  0x05, // 0x07 Fence/Gate
  0x04, // 0x08 Fence/Gate
  0x05, // 0x09 Fence/Gate
  0x02, // 0x0A Fence/Gate
  0x03, // 0x0B Fence/Gate
  0x02, // 0x0C Fence/Gate
};

const BYTE debug_tiles[16] = {
  0x18, 0x18,
  0x18, 0x18,
  0x18, 0x18,
  0xFF, 0xFF,
  0xFF, 0xFF,
  0x18, 0x18,
  0x18, 0x18,
  0x18, 0x18,
};

const BYTE projectile_tiles[16] = {
  0xC0, 0xC0,
  0xC0, 0xC0,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
};

struct enemy {
    uint8_t x;
    uint8_t y;
    uint8_t direction;
    uint8_t oam_id;
    uint8_t flags;
    uint8_t state;
    uint8_t shoot_cooldown;
};
#define ENEMY_SHOW 1
#define ENEMY_STATE_STOPPED 0
#define ENEMY_STATE_PATROL 1
#define ENEMY_STATE_ATTACKING 2

struct enemy enemy1 = {
  .x = 32,
  .y = 32,
  .direction = DIR_RIGHT,
  .oam_id = OAM_ENEMY_START,
  .flags = 0,
  .state = ENEMY_STATE_STOPPED,
  .shoot_cooldown = 0,
};

struct projectile {
  uint8_t x;
  uint8_t y;
  uint8_t flags;
  int8_t dx;
  int8_t dy;
};
#define PROJECTILE_SHOW 1

struct projectile projectile = {
  .x = 0,
  .y = 0,
  .flags = 0,
  .dx = 0,
  .dy = 0,
};

void main() {
  DISPLAY_OFF;
  LCDC_REG = LCDCF_OFF | LCDCF_BG8800 | LCDCF_BG9800 | LCDCF_WIN9C00 | LCDCF_BGON | LCDCF_OBJON;
  SPRITES_8x16;

  // Set pallette defaults
  set_bkg_palette(0, 1, &palettes[0]);
  set_bkg_palette(1, 1, &palettes[4*4]); // Dialog
  set_sprite_palette(0, 3, &palettes[5*4]); // Sprites
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

  // Sprites
  set_sprite_data(0x00, player_TILE_COUNT, player_tiles);
  set_sprite_data(arm_h_baseTile, arm_h_TILE_COUNT, arm_h_tiles);
  set_sprite_data(arm_v_baseTile, arm_v_TILE_COUNT, arm_v_tiles);
  set_sprite_data(arm_v_back_baseTile, arm_v_back_TILE_COUNT, arm_v_back_tiles);
  set_sprite_data(soldier_baseTile, soldier_TILE_COUNT, soldier_tiles);
  set_sprite_data(debug_baseTile, debug_TILE_COUNT, debug_tiles);
  set_sprite_data(projectile_baseTile, projectile_TILE_COUNT, projectile_tiles);

  player_sprite_x = player_x + player_sprite_x_offset;
  player_sprite_y = player_y + player_sprite_y_offset;

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

  direction_pressed = 0;

  int keys = joypad();

  // Controls

  if (keys & J_UP) {
    direction_pressed |= DIR_UP;
    direction = DIR_UP;
    player_y--;
  } else if (keys & J_DOWN) {
    direction_pressed |= DIR_DOWN;
    direction = DIR_DOWN;
    player_y++;
  }

  if (keys & J_LEFT) {
    direction_pressed |= DIR_LEFT;
    direction = DIR_LEFT;
    player_x--;
  } else if (keys & J_RIGHT) {
    direction_pressed |= DIR_RIGHT;
    direction = DIR_RIGHT;
    player_x++;
  }

  // Transform
  if (keys & J_A && keys & J_B && transform_remaining_counter == 0) {
    transform_remaining_counter = TRANSFORM_LENGTH;
  }

  // Punch
  if (keys & J_B && transform_remaining_counter > 0 && punch_counter == 0) {
    punch_counter = 25;
    if (active_arm == ARM_LEFT) {
      active_arm = ARM_RIGHT;
      punch_frame = &right_punch_frame;
    } else {
      active_arm = ARM_LEFT;
      punch_frame = &left_punch_frame;
    }
  }

  // Animations

  if (punch_counter <= 10) {
    *punch_frame = 0;
  } else if (punch_counter > 13) {
    *punch_frame = 2;
    attack_flags |= ATTACKING_PUNCH;
  } else if (punch_counter > 10) {
    attack_flags &= ~ATTACKING_PUNCH;
    *punch_frame = 1;
  }

  if (direction == DIR_DOWN) {
    move_metasprite(player_metasprites[0], player_baseTile, 0, player_sprite_x, player_sprite_y);
    if (transform_remaining_counter > 0) {
      move_metasprite(arm_v_metasprites[right_punch_frame], arm_v_baseTile, 4, player_sprite_x - 8, player_sprite_y + 8);
      move_metasprite_vflip(arm_v_metasprites[left_punch_frame], arm_v_baseTile, 8, player_sprite_x + 8, player_sprite_y + 8);
    } else {
      move_metasprite(arm_v_metasprites[0], arm_v_baseTile, 4, 0, 0);
      move_metasprite_vflip(arm_v_metasprites[0], arm_v_baseTile, 8, 0, 0);
    }
  }
  if (direction == DIR_UP) {
    move_metasprite(player_metasprites[0], player_baseTile, 0, player_sprite_x, player_sprite_y);
    if (transform_remaining_counter > 0) {
      move_metasprite(arm_v_metasprites[left_punch_frame], arm_v_back_baseTile, 4, player_sprite_x - 8, player_sprite_y + 8 - 22);
      move_metasprite_vflip(arm_v_metasprites[right_punch_frame], arm_v_back_baseTile, 8, player_sprite_x + 8, player_sprite_y + 8 - 22);
    } else {
      move_metasprite(arm_v_metasprites[0], arm_v_back_baseTile, 4, 0, 0);
      move_metasprite_vflip(arm_v_metasprites[0], arm_v_back_baseTile, 8, 0, 0);
    }
  }
  if (direction == DIR_LEFT) {
    move_metasprite(player_metasprites[1], player_baseTile, 3, player_sprite_x, player_sprite_y);
    if (transform_remaining_counter > 0) {
      move_metasprite(arm_h_metasprites[left_punch_frame], arm_h_baseTile, 0, player_sprite_x - 8, player_sprite_y);
      move_metasprite(arm_h_metasprites[right_punch_frame], arm_h_baseTile, 7, player_sprite_x - 8, player_sprite_y - 2);
    } else {
      move_metasprite(arm_h_metasprites[0], arm_h_baseTile, 0, 0, 0);
      move_metasprite(arm_h_metasprites[0], arm_h_baseTile, 7, 0, 0);
    }
    hide_sprite(10);
    hide_sprite(11);
  }
  if (direction == DIR_RIGHT) {
    move_metasprite_vflip(player_metasprites[1], player_baseTile, 3, player_sprite_x, player_sprite_y);
    if (transform_remaining_counter > 0) {
      move_metasprite_vflip(arm_h_metasprites[right_punch_frame], arm_h_baseTile, 0, player_sprite_x + 8, player_sprite_y);
      move_metasprite_vflip(arm_h_metasprites[left_punch_frame], arm_h_baseTile, 7, player_sprite_x + 8, player_sprite_y - 2);
    } else {
      move_metasprite_vflip(arm_h_metasprites[0], arm_h_baseTile, 0, 0, 0);
      move_metasprite_vflip(arm_h_metasprites[0], arm_h_baseTile, 7, 0, 0);
    }
    hide_sprite(10);
    hide_sprite(11);
  }

  update_enemies();
  update_projectiles();

  check_bkg_collision();
  check_attack_collision();
  check_projectile_collision();

  if ((attack_flags & ATTACKING_PUNCH) && transform_remaining_counter > 0) {
    check_destruct();
  }

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
  if (transform_remaining_counter > 0) {
    transform_remaining_counter--;
  }
  if (transform_cooldown_counter > 0) {
    transform_cooldown_counter--;
  }
  if (punch_counter > 0) {
    punch_counter--;
  }
}

void process_events() {
  switch (event_state) {
    case EVENT_PRISON_CELL_START:
      enemy1.flags |= ENEMY_SHOW;
      enemy1.state |= ENEMY_STATE_PATROL;
      if (counter == 180) {
        show_dialog("WHO AM I");
      }
      if (counter == 300) {
        show_hint("     HOLD A AND B       TO TRANSFORM    ");
      }
      if (transform_remaining_counter > 0) {
        hide_hud();
        event_state = EVENT_PRISON_CELL;
        counter = 0;
      }
      if (counter > 600) {
        counter = 500;
      }
      break;
    case EVENT_PRISON_CELL:
      // TODO: Press B to punch
      enemy1.state = ENEMY_STATE_ATTACKING;
      if (counter > 600) {
        counter = 500;
      }
      break;
    case EVENT_PRISON_CELL_POST_SMASH:
      // TODO: Make soldier attack?
      break;
    case EVENT_PRISON_CELL_AFTER_BATTLE:
      if (counter == 180) {
        show_dialog("WHAT AM I");
      }
      if (counter == 300) {
        hide_hud();
      }
      if (counter > 600) {
        counter = 500;
      }
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

void update_enemies() {
  if (enemy1.flags & ENEMY_SHOW > 0) {
    switch (enemy1.state) {
      case ENEMY_STATE_STOPPED:
        break;
      case ENEMY_STATE_PATROL:
        switch (enemy1.direction) {
          case DIR_RIGHT:
            if (enemy1.x > 200) {
              enemy1.direction = DIR_LEFT;
            } else {
              enemy1.x++;
            }
            break;
          case DIR_LEFT:
            if (enemy1.x < 16) {
              enemy1.direction = DIR_RIGHT;
            } else {
              enemy1.x--;
            }
            break;
          case DIR_DOWN:
            enemy1.y++;
            break;
          case DIR_UP:
            enemy1.y--;
            break;
        }
        break;
      case ENEMY_STATE_ATTACKING:
        if (enemy1.shoot_cooldown == 0) {
          projectile.x = enemy1.x + 8 - 16 + 8;
          projectile.y = enemy1.y + 16 - 16;
          projectile.flags |= PROJECTILE_SHOW;
          projectile.dx = 1;
          projectile.dy = 2;
          enemy1.shoot_cooldown = 60;
          set_sprite_tile(OAM_PROJECTILE, projectile_baseTile);
          set_sprite_prop(OAM_PROJECTILE, 0x02);
        } else {
          enemy1.shoot_cooldown--;
        }
        break;
    }
    // Animate
    switch (enemy1.direction) {
      case DIR_RIGHT:
      case DIR_UP:
      case DIR_DOWN:
        move_metasprite(soldier_metasprites[0], soldier_baseTile, enemy1.oam_id, enemy1.x - camera_x + player_sprite_x_offset, enemy1.y - camera_y + player_sprite_y_offset);
        break;
      case DIR_LEFT:
        move_metasprite_vflip(soldier_metasprites[0], soldier_baseTile, enemy1.oam_id, enemy1.x - camera_x + player_sprite_x_offset, enemy1.y - camera_y + player_sprite_y_offset);
        break;
    }
  } else {
    move_metasprite(soldier_metasprites[0], soldier_baseTile, enemy1.oam_id, 0, 0);
  }
}

void update_projectiles() {
  if ((projectile.flags & PROJECTILE_SHOW) != 0) {
    projectile.x += projectile.dx;
    projectile.y += projectile.dy;

    move_sprite(OAM_PROJECTILE, projectile.x - camera_x + 8, projectile.y - camera_y + 16);
  } else {
    move_sprite(OAM_PROJECTILE, 0, 0);
  }
}

void check_bkg_collision() {
  int8_t offset_x = 0;
  int8_t offset_y = 0;
  if (direction_pressed == DIR_LEFT) {
    offset_x = -4;
  }
  if (direction_pressed == DIR_RIGHT) {
    offset_x = 4;
  }
  if (direction_pressed == DIR_UP) {
    offset_y = -2;
  }
  if (direction_pressed == DIR_DOWN) {
    offset_y = 3;
  }

  // Debug
  // show_debug_marker(0, player_x + offset_x - 4, player_y + offset_y - 4);

  uint8_t x_tile = ((camera_x + player_x + offset_x) % 255) >> 3u;
  uint8_t y_tile = ((camera_y + player_y + offset_y) % 255) >> 3u;
  uint8_t tile_id = get_bkg_tile_xy(x_tile, y_tile);
  if (tile_id <= 1 || (tile_id >= 6 && tile_id <= 0x0C)) {
    // Collide
    if (direction_pressed & DIR_UP) {
      player_y++;
    }
    if (direction_pressed & DIR_DOWN) {
      player_y--;
    }
    if (direction_pressed & DIR_LEFT) {
      player_x++;
    }
    if (direction_pressed & DIR_RIGHT) {
      player_x--;
    }
  }
}

void check_attack_collision() {
  if ((attack_flags & ATTACKING_PUNCH) == 0) {
    return;
  }

  uint16_t punch_box_x = player_x - 8;
  uint16_t punch_box_x_2 = player_x + 8;
  uint16_t punch_box_y = player_y - 8;
  uint16_t punch_box_y_2 = player_y + 8;
  switch (direction) {
    case DIR_UP:
      punch_box_y -= 10;
      punch_box_y_2 -= 10;
      break;
    case DIR_DOWN:
      punch_box_y += 10;
      punch_box_y_2 += 10;
      break;
    case DIR_LEFT:
      punch_box_x -= 12;
      punch_box_x_2 -= 12;
      break;
    case DIR_RIGHT:
      punch_box_x += 12;
      punch_box_x_2 += 12;
      break;
  }

  uint16_t enemy_box_x = enemy1.x - 4 - camera_x;
  uint16_t enemy_box_x_2 = enemy1.x + 4 - camera_x;
  uint16_t enemy_box_y = enemy1.y - 4 - camera_y;
  uint16_t enemy_box_y_2 = enemy1.y + 4 - camera_y;

  // Debug
  // show_debug_marker(0, punch_box_x, punch_box_y);
  // show_debug_marker(1, enemy_box_x, enemy_box_y);

  if (
    punch_box_x < enemy_box_x_2 && 
    punch_box_x_2 > enemy_box_x &&
    punch_box_y < enemy_box_y_2 &&
    punch_box_y_2 > enemy_box_y
  ) {
    // Overlap
    switch (direction) {
      case DIR_UP:
        enemy1.y -= 5;
        break;
      case DIR_DOWN:
        enemy1.y += 5;
        break;
      case DIR_LEFT:
        enemy1.x -= 5;
        break;
      case DIR_RIGHT:
        enemy1.x += 5;
        break;
    }
  }
}

void check_projectile_collision() {
  if ((projectile.flags & PROJECTILE_SHOW) == 0) {
    return;
  }

  // Collide with BG
  uint8_t tile_id = get_bkg_tile_xy(projectile.x >> 3, projectile.y >> 3);
  if (tile_id <= 1 || (tile_id >= 6 && tile_id <= 0x0C)) {
    projectile.flags &= ~PROJECTILE_SHOW;
    return;
  }

  // Collide with Player
  hitbox player_hitbox;
  hitbox projectile_hitbox;
  player_hitbox = get_player_hitbox(player_x + camera_x, player_y + camera_y);
  projectile_hitbox = get_projectile_hitbox(projectile.x, projectile.y);

  // Debug
  // show_debug_marker(0, player_hitbox.x - camera_x, player_hitbox.y - camera_y);
  // show_debug_marker(1, player_hitbox.x2 - camera_x, player_hitbox.y2 - camera_y);

  if (check_hitbox_overlap(player_hitbox, projectile_hitbox)) {
    projectile.flags &= ~PROJECTILE_SHOW;
    return;
  }
}

void check_destruct() {
  uint16_t hit_x_1 = player_x;
  uint16_t hit_x_2 = player_x;
  uint16_t hit_y_1 = player_y;
  uint16_t hit_y_2 = player_y;
  switch (direction) {
    case DIR_UP:
      hit_y_1 = hit_y_2 -= 8;
      hit_x_1 -= 4;
      hit_x_2 += 4;
      break;
    case DIR_DOWN:
      hit_y_1 = hit_y_2 += 8;
      hit_x_1 -= 4;
      hit_x_2 += 4;
      break;
    case DIR_LEFT:
      hit_x_1 = hit_x_2 -= 8;
      hit_y_1 -= 4;
      hit_y_2 += 4;
      break;
    case DIR_RIGHT:
      hit_x_1 = hit_x_2 += 8;
      hit_y_1 -= 4;
      hit_y_2 += 4;
      break;
  }

  // TODO: Allow multiple hits on certain types of blocks before destruction?

  // Debug
  // show_debug_marker(0, hit_x_1 - 4, hit_y_1 - 4);
  // show_debug_marker(1, hit_x_2 - 4, hit_y_2 - 4);

  hit_x_1 += camera_x;
  hit_x_2 += camera_x;
  hit_y_1 += camera_y;
  hit_y_2 += camera_y;

  destroy_tile((hit_x_1 >> 3), (hit_y_1 >> 3));
  destroy_tile((hit_x_2 >> 3), (hit_y_2 >> 3));
}

void destroy_tile(uint8_t tile_x, uint8_t tile_y) {
  // Destruct tile, and loop up through all higher tiles (for tall walls)
  uint8_t count = 0;
  while (tile_y > 0 && count < 3) {
    uint8_t tile = get_bkg_tile_xy(tile_x, tile_y);
    // TODO: Use destruct tilemap to map tilemaps, rather than mapping tiles
    uint8_t new_tile = destruct_map[tile];
    if (tile == new_tile) {
      break; // No tile destroyed, so don't scan up further
    }
    set_bkg_tile_xy(tile_x, tile_y, new_tile);
    // TODO: Use a tilemap that stores how many tiles to destroy up? Heightmap?
    // Check next tile up
    tile_y--;
    count++;
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

void show_debug_marker(uint8_t offset, uint8_t x, uint8_t y) {
  set_sprite_tile(OAM_DEBUG + offset, debug_baseTile);
  move_sprite(OAM_DEBUG + offset, x + 8, y + 16);
}