#pragma bank 255

#include "intro.h"
#include "./globals.h"
#include "./tiles/gbcompologo.h"
#include "./tiles/title.h"

void loadCompoLogoScreen() BANKED {
  set_bkg_data(0x00, gbcompologo_TILE_COUNT, gbcompologo_tiles);
  set_bkg_tiles(0, 0, 20, 18, gbcompologo_map);
  fade_counter = 0;
  set_bkg_palette(0, 1, &palettes[0]);
   
  DISPLAY_ON;
}

void updateCompoLogoScreen() BANKED {
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

void loadTitleScreen() BANKED {
  DISPLAY_OFF;

  set_bkg_data(0x00, title_TILE_COUNT, title_tiles);
  set_bkg_tiles(0, 0, 20, 18, title_map);
  fade_counter = 0;
   
  DISPLAY_ON;
}

void updateTitleScreen() BANKED {
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