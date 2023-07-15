//AUTOGENERATED FILE FROM png2asset
#ifndef METASPRITE_dialog_H
#define METASPRITE_dialog_H

#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

#define dialog_TILE_ORIGIN 0
#define dialog_TILE_W 8
#define dialog_TILE_H 8
#define dialog_WIDTH 256
#define dialog_HEIGHT 256
#define dialog_TILE_COUNT 5
#define dialog_PALETTE_COUNT 1
#define dialog_COLORS_PER_PALETTE 4
#define dialog_TOTAL_COLORS 4
#define dialog_MAP_ATTRIBUTES dialog_map_attributes
#define dialog_MAP_ATTRIBUTES_WIDTH 32
#define dialog_MAP_ATTRIBUTES_HEIGHT 32
#define dialog_MAP_ATTRIBUTES_PACKED_WIDTH 32
#define dialog_MAP_ATTRIBUTES_PACKED_HEIGHT 32

BANKREF_EXTERN(dialog)

extern const palette_color_t dialog_palettes[4];
extern const uint8_t dialog_tiles[80];

extern const unsigned char dialog_map[1024];
extern const unsigned char dialog_map_attributes[1024];

#endif