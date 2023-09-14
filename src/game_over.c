#pragma bank 255

#include <gb/gb.h>
#include "game_over.h"
#include "./globals.h"
#include "./tiles/game_over.h"

void load_game_over_screen() BANKED {
  HIDE_SPRITES;
  HIDE_WIN;
  DISPLAY_OFF;

  fade_counter = 0;
  SCX_REG = 0;
  SCY_REG = 0;

  set_bkg_data(0, game_over_TILE_COUNT, game_over_tiles);
  set_bkg_tiles(0, 0, 20, 18, game_over_map);
   
  DISPLAY_ON;
}

void update_game_over_screen() BANKED {
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
      reset();
      break;
  }
  fade_counter++;
}