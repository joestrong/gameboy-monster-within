#pragma bank 255

#include <gb/gb.h>
#include "game_finished.h"
#include "./globals.h"
#include "./tiles/game_finished.h"

const UWORD finished_pallette[] = {
  RGB_WHITE, RGB_BLACK, RGB_BLACK, RGB_BLACK,
};

void load_game_finished_screen() BANKED {
  HIDE_SPRITES;
  HIDE_WIN;
  DISPLAY_OFF;

  SCX_REG = 0;
  SCY_REG = 0;

  set_bkg_palette(0, 1, &finished_pallette[0]);

  set_bkg_data(0, game_finished_TILE_COUNT, game_finished_tiles);
  set_bkg_tiles(0, 0, 20, 18, game_finished_map);
   
  DISPLAY_ON;
}