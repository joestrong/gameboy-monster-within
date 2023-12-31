#pragma bank 0

#include <stdint.h>
#include <string.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/metasprites.h>
#include <gbdk/font.h>
#include "./music/hUGEDriver.h"
#include "./music/banked.h"
#include "./globals.h"
#include "./intro.h"
#include "./game_over.h"
#include "./game_finished.h"
#include "./overworld.h"
#include "./sprite_manager.h"
#include "./sprites/enemy.h"
#include "./sprites/projectile.h"
#include "./tiles/overworld.h"
#include "./tiles/player.h"
#include "./tiles/player_dead.h"
#include "./tiles/arm_h.h"
#include "./tiles/arm_v.h"
#include "./tiles/arm_v_back.h"
#include "./tiles/dialog.h"
#include "./tiles/soldier.h"

BANKREF_EXTERN(prison)

#define skip_intro 1

#define EVENT_PRISON_CELL_START 0
#define EVENT_PRISON_CELL 1
#define EVENT_PRISON_CELL_POST_SMASH 2
#define EVENT_PRISON_CELL_AFTER_BATTLE 3

#define HUD_DIALOG_Y 103
#define HINT_HEIGHT 16

#define HUD_DIALOG_MODE 1
#define HUD_HINT_MODE 2

#define TRANSFORM_LENGTH 15 * 60 // 15 seconds
#define TRANSFORM_COOLDOWN 15 * 60 // 15 seconds

uint8_t state = 0; // 0 - Compo Logo 1 - Title, 2 - Game, 3 - Game over, 4 - Win
uint8_t event_state = 0; // 0 - prison cell start, 1 - prison cell
uint16_t counter = 0;
uint8_t fade_counter = 0;

uint8_t hud_control;

int16_t camera_x = 64;
int16_t camera_y = 0;
int16_t old_camera_x = 0;
int16_t old_camera_y = 0;
int8_t map_pos_x = 0;
int8_t map_pos_y = 0;
int8_t old_map_pos_x = 0;
int8_t old_map_pos_y = 0;
uint8_t player_x = 72;
uint8_t player_y = 112;
uint8_t player_sprite_x = 0;
uint8_t player_sprite_y = 0;
uint8_t direction = 4;
uint8_t direction_pressed = 0;
uint8_t player_flags = 0;

uint8_t player_health = 8;

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

const UWORD palettes[] = {
  RGB_WHITE, RGB_WHITE, RGB_WHITE, RGB_WHITE, // BG Fade-in
  RGB_WHITE, RGB_LIGHTGRAY, RGB_WHITE, RGB_WHITE, // BG Fade-in
  RGB_WHITE, RGB_LIGHTGRAY, RGB_DARKGRAY, RGB_WHITE, // BG Fade-in 2
  RGB_WHITE, RGB_LIGHTGRAY, RGB_DARKGRAY, RGB_BLACK, // BG B&W

  RGB_YELLOW, RGB_WHITE, RGB_DARKBLUE, RGB_BLACK, // Dialog

  RGB_WHITE, RGB_BLUE, RGB_DARKBLUE, RGB_BLACK, // Sprite Blue
  RGB_WHITE, RGB(0x15, 9, 9), RGB(0x10, 0, 0), RGB(5, 0, 0), // Sprite Red (Enemy)
  RGB_WHITE, RGB_WHITE, RGB_WHITE, RGB_WHITE, // Projectile
};

const BYTE destruct_map[] = {
  0x00,
  0x00,
  0x00, // 0x02 Wall top
  0x00, // 0x03 Wall
  0x04,
  0x05,
  0x04, // 0x06 Fence/Gate
  0x05, // 0x07 Fence/Gate
  0x04, // 0x08 Fence/Gate
  0x05, // 0x09 Fence/Gate
  0x00, // 0x0A Fence/Gate
  0x00, // 0x0B Fence/Gate
  0x00, // 0x0C Fence/Gate
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

enemy* enemy1;
enemy* enemy2;
enemy* enemy3;
enemy* enemy4;

uint8_t enemy_count = 4;

void main() {
  DISPLAY_OFF;
  LCDC_REG = LCDCF_OFF | LCDCF_BG8800 | LCDCF_BG9800 | LCDCF_WIN9C00 | LCDCF_BGON | LCDCF_OBJON;
  SPRITES_8x16;

  // Set pallette defaults
  set_bkg_palette(0, 1, &palettes[0]);
  set_bkg_palette(1, 1, &palettes[4*4]); // Dialog
  set_sprite_palette(0, 3, &palettes[5*4]); // Sprites

  // Music
  NR52_REG = 0x80;
  NR51_REG = 0xFF;
  NR50_REG = 0x77;
  hUGE_init_wrapper(&prison, BANK(prison));

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

    if (state == 3) {
      update_game_over_screen();
    }

    hUGE_dosound_wrapper();
  }
}

void perLineInterrupt() {
  if (LY_REG == 0) {
    LYC_REG = 7;
    SHOW_WIN;
    HIDE_SPRITES;
  }
  if (LY_REG == 7) {
    LYC_REG = HUD_DIALOG_Y;
    HIDE_WIN;
    SHOW_SPRITES;
  }

  if (LY_REG == HUD_DIALOG_Y) {
    if (hud_control & (HUD_DIALOG_MODE | HUD_HINT_MODE)) {
      SHOW_WIN;
    } else {
      HIDE_WIN;
    }
    LYC_REG = HUD_DIALOG_Y + HINT_HEIGHT;
  }
  
  if (LY_REG == HUD_DIALOG_Y + HINT_HEIGHT) {
    if (hud_control & HUD_HINT_MODE) {
      HIDE_WIN;
    }
    LYC_REG = 0;
  }
}

void loadGame() {
  DISPLAY_OFF;

  fade_counter = 0;

  // Hack - Place space character for text
  set_bkg_data(0xEC, 1, dialog_tiles + 32);
  // BKG
  load_overworld();
  // Font
  set_bkg_1bpp_data(font_baseTile, font_TILE_COUNT, &font_spect[font_BYTE_OFFSET]);

  // Window
  set_bkg_data(dialog_baseTile, dialog_TILE_COUNT, dialog_tiles);
  VBK_REG=VBK_ATTRIBUTES;
  set_win_tiles(0, 0, 32, 6, dialog_map_attributes);
  VBK_REG=VBK_TILES;
  set_win_based_tiles(0, 0, 32, 6, dialog_map, dialog_baseTile);
  SHOW_WIN;
  WY_REG = 0;
  WX_REG = 7;
  LYC_REG = 0;

  // Initial HUD
  hide_hud();

  // Sprites
  set_sprite_data(player_baseTile, player_TILE_COUNT, player_tiles);
  set_sprite_data(player_dead_baseTile, player_dead_TILE_COUNT, player_dead_tiles);
  set_sprite_data(arm_h_baseTile, arm_h_TILE_COUNT, arm_h_tiles);
  set_sprite_data(arm_v_baseTile, arm_v_TILE_COUNT, arm_v_tiles);
  set_sprite_data(arm_v_back_baseTile, arm_v_back_TILE_COUNT, arm_v_back_tiles);
  set_sprite_data(soldier_baseTile, soldier_TILE_COUNT, soldier_tiles);
  set_sprite_data(debug_baseTile, debug_TILE_COUNT, debug_tiles);
  set_sprite_data(projectile_baseTile, projectile_TILE_COUNT, projectile_tiles);
  SHOW_SPRITES;

  player_sprite_x = player_x + player_sprite_x_offset;
  player_sprite_y = player_y + player_sprite_y_offset;

  enemy1 = create_enemy(32, 48);
  add_sprite(SPRITE_TYPE_ENEMY, enemy1);

  enemy2 = create_enemy(64, 48);
  add_sprite(SPRITE_TYPE_ENEMY, enemy2);

  enemy3 = create_enemy(296, 176);
  enemy3->patrol_target_1 = 2;
  enemy3->patrol_target_2 = 3;
  enemy3->current_target = 3;
  enemy3->target = 3;
  add_sprite(SPRITE_TYPE_ENEMY, enemy3);

  enemy4 = create_enemy(240, 192);
  enemy4->patrol_target_1 = 4;
  enemy4->patrol_target_2 = 5;
  enemy4->current_target = 5;
  enemy4->target = 5;
  add_sprite(SPRITE_TYPE_ENEMY, enemy4);

  // Set up LY=LYC interrupt
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

  if ((player_flags & PLAYER_DEAD) == 0) {
    player_controls();
  }

  player_animations();

  update_sprites();

  check_bkg_collision();

  if ((attack_flags & ATTACKING_PUNCH) && transform_remaining_counter > 0) {
    check_destruct();
  }

  // If game state has changed, don't process any more of updateGame
  if (state != 2) {
    return;
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

  if ((player_flags & PLAYER_DEAD) > 0) {
    switch (fade_counter) {
      case 80:
        set_bkg_palette(0, 1, &palettes[3*4]);
        break;
      case 85:
        set_bkg_palette(0, 1, &palettes[2*4]);
        break;
      case 90:
        set_bkg_palette(0, 1, &palettes[1*4]);
        break;
      case 95:
        load_game_over_screen();
        state = 3;
        break;
    }
    if (fade_counter < 95) {
      fade_counter++;
    }
  }
}

void player_controls() {
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
  if (keys & J_SELECT && transform_remaining_counter == 0) {
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
}

void player_animations() {
  if ((player_flags & PLAYER_DEAD) > 0) {
    move_metasprite(player_dead_metasprites[0], player_dead_baseTile, 0, player_sprite_x, player_sprite_y);
    hide_sprite(4);
    hide_sprite(5);
    hide_sprite(6);
    hide_sprite(7);
    hide_sprite(8);
    hide_sprite(9);
    hide_sprite(10);
    hide_sprite(11);
    hide_sprite(12);
    return;
  }

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
}

void process_events() {
  switch (event_state) {
    case EVENT_PRISON_CELL_START:
      if (counter == 180) {
        show_dialog("WHO AM I");
      }
      if (counter == 300) {
        show_hint("    PRESS SELECT        TO TRANSFORM    ");
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
      enemy1->state = ENEMY_STATE_ATTACKING;
      enemy2->state = ENEMY_STATE_ATTACKING;
      enemy3->state = ENEMY_STATE_ATTACKING;
      enemy4->state = ENEMY_STATE_ATTACKING;
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
  hud_control = HUD_DIALOG_MODE;
  set_win_based_tiles(0, 0, 32, 6, dialog_map, dialog_baseTile);
  set_win_based_tiles(1, 2, strlen(text), 1, text, font_baseTile - 65);
}

void show_hint(char* text) {
  hud_control = HUD_HINT_MODE;
  fill_win_rect(0, 1, 20, 2, dialog_baseTile + 4);
  set_win_based_tiles(0, 1, 20, 2, text, font_baseTile - 65);
}

void hide_hud() {
  hud_control &= ~(HUD_DIALOG_MODE | HUD_HINT_MODE);
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
  if (tile_id == 2 || tile_id == 3 || (tile_id >= 6 && tile_id <= 0x0C)) {
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

void check_destruct() {
  // 2 points, 2 places to destroy
  uint16_t hit_x_1 = player_x + camera_x;
  uint16_t hit_x_2 = player_x + camera_x;
  uint16_t hit_y_1 = player_y + camera_y;
  uint16_t hit_y_2 = player_y + camera_y;
  switch (direction) {
    case DIR_UP:
      hit_y_1 -= 8;
      hit_y_2 -= 8;
      hit_x_1 -= 4;
      hit_x_2 += 4;
      break;
    case DIR_DOWN:
      hit_y_1 += 10;
      hit_y_2 += 10;
      hit_x_1 -= 4;
      hit_x_2 += 4;
      break;
    case DIR_LEFT:
      hit_x_1 -= 8;
      hit_x_2 -= 8;
      hit_y_1 -= 4;
      hit_y_2 += 4;
      break;
    case DIR_RIGHT:
      hit_x_1 += 8;
      hit_x_2 += 8;
      hit_y_1 -= 4;
      hit_y_2 += 4;
      break;
  }

  // TODO: Allow multiple hits on certain types of blocks before destruction?

  // Debug
  // show_debug_marker(0, hit_x_1 - camera_x - 4, hit_y_1 - camera_y - 4);
  // show_debug_marker(1, hit_x_2 - camera_x - 4, hit_y_2 - camera_y - 4);

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

void show_debug_marker(uint8_t offset, uint8_t x, uint8_t y) {
  set_sprite_tile(OAM_DEBUG + offset, debug_baseTile);
  move_sprite(OAM_DEBUG + offset, x + 8, y + 16);
}

void player_hit() {
  if (player_health > 0) {
    player_health--;

    if (player_health == 0) {
      player_flags |= PLAYER_DEAD;
    }
  }

  set_win_tile_xy(player_health, 0, 0x29);

  if (player_health > 1) {
    VBK_REG=VBK_ATTRIBUTES;
    set_win_tile_xy(player_health - 1, 0, 0x21);
    VBK_REG=VBK_TILES;
  }
  set_win_tile_xy(player_health - 1, 0, 0x27);
}

void enemy_killed() {
  enemy_count--;
  if (enemy_count == 0) {
    load_game_finished_screen();
    state = 4;
  }
}