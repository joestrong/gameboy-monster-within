//AUTOGENERATED FILE FROM png2asset
#ifndef METASPRITE_sprites_H
#define METASPRITE_sprites_H

#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

#define sprites_TILE_ORIGIN 0
#define sprites_TILE_W 8
#define sprites_TILE_H 16
#define sprites_WIDTH 16
#define sprites_HEIGHT 32
#define sprites_TILE_COUNT 20
#define sprites_PALETTE_COUNT 1
#define sprites_COLORS_PER_PALETTE 4
#define sprites_TOTAL_COLORS 4
#define sprites_PIVOT_X 8
#define sprites_PIVOT_Y 16
#define sprites_PIVOT_W 16
#define sprites_PIVOT_H 32

BANKREF_EXTERN(sprites)

extern const palette_color_t sprites_palettes[4];
extern const uint8_t sprites_tiles[320];

extern const metasprite_t* const sprites_metasprites[4];

#endif
