//AUTOGENERATED FILE FROM png2asset
#ifndef METASPRITE_arm_v_H
#define METASPRITE_arm_v_H

#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

#define arm_v_TILE_ORIGIN 0
#define arm_v_TILE_W 8
#define arm_v_TILE_H 16
#define arm_v_WIDTH 16
#define arm_v_HEIGHT 32
#define arm_v_TILE_COUNT 8
#define arm_v_PALETTE_COUNT 1
#define arm_v_COLORS_PER_PALETTE 4
#define arm_v_TOTAL_COLORS 4
#define arm_v_PIVOT_X 8
#define arm_v_PIVOT_Y 16
#define arm_v_PIVOT_W 16
#define arm_v_PIVOT_H 32

BANKREF_EXTERN(arm_v)

extern const palette_color_t arm_v_palettes[4];
extern const uint8_t arm_v_tiles[128];

extern const metasprite_t* const arm_v_metasprites[1];

#endif