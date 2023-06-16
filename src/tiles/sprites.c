//AUTOGENERATED FILE FROM png2asset

#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

BANKREF(sprites)

const palette_color_t sprites_palettes[4] = {
	RGB8(  0,  0,  0), RGB8( 30, 48, 45), RGB8(  0,  0,  0), RGB8( 38,127,  0)
	
};

const uint8_t sprites_tiles[48] = {
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x1f,0x00,0x1f,0x00,
	0x1f,0x00,0x1f,0x00,
	
0x1f,0x00,0x1f,0x00,
	0x1f,0x00,0x1f,0x00,
	0x1f,0x00,0x1f,0x00,
	0x1f,0x00,0x1f,0x00,
	
0x1f,0x00,0x1f,0x00,
	0x1f,0x00,0x1f,0x00,
	0x1f,0x20,0x00,0x3f,
	0x00,0x3f,0x00,0x00
	
};

const metasprite_t sprites_metasprite0[] = {
	METASPR_ITEM(-8, -8, 0, 0), METASPR_ITEM(0, 8, 0, 32), METASPR_ITEM(8, -8, 1, 0), METASPR_ITEM(0, 8, 1, 32), METASPR_TERM
};

const metasprite_t sprites_metasprite1[] = {
	METASPR_ITEM(-8, -8, 2, 0), METASPR_ITEM(0, 8, 2, 32), METASPR_TERM
};

const metasprite_t* const sprites_metasprites[2] = {
	sprites_metasprite0, sprites_metasprite1
};
