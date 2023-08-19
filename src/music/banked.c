#pragma bank 0

#include <stdint.h>
#include "hUGEDriver.h"

uint8_t music_bank;

void hUGE_init_wrapper(const hUGESong_t * song, uint8_t bank) {
  uint8_t b = _current_bank;
  SWITCH_ROM(bank);
  hUGE_init(song);
  music_bank = bank;
  SWITCH_ROM(b);
}

void hUGE_dosound_wrapper() {
  uint8_t b = _current_bank;
  SWITCH_ROM(music_bank);
  hUGE_dosound();
  SWITCH_ROM(b);
}