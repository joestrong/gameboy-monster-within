.PHONY: gfx

CC = ${GBDK_HOME}/bin/lcc -debug -autobank -Wm-yC -Wm-yn"MonsterIns" -Wm-yt0x19 -Wm-yoA

all: gfx main

main: src/main.c
	$(CC) -o out/main.gb \
		src/main.c \
		src/intro.c \
		src/overworld.c \
		src/pathfinding.c \
		src/tiles/gbcompologo.c \
		src/tiles/title.c \
		src/tiles/overworld.c \
		src/tiles/player.c \
		src/tiles/arm_h.c \
		src/tiles/arm_v.c \
		src/tiles/arm_v_back.c \
		src/tiles/dialog.c \
		src/tiles/soldier.c \
		src/helpers/hitbox.c \
		src/helpers/vector.c

gfx: gfx/*.png
	png2asset gfx/gbcompologo.png -c src/tiles/gbcompologo.c -map -b 255
	png2asset gfx/title.png -c src/tiles/title.c -map -b 255
	png2asset gfx/overworld.png -c src/tiles/overworld.c -map -b 255
	png2asset gfx/player.png -c src/tiles/player.c -spr8x16 -sw 16 -sh 32
	png2asset gfx/arm_h.png -c src/tiles/arm_h.c -spr8x16 -sw 24 -sh 16
	png2asset gfx/arm_v.png -c src/tiles/arm_v.c -spr8x16 -sw 16 -sh 32
	png2asset gfx/arm_v_back.png -c src/tiles/arm_v_back.c -spr8x16 -sw 16 -sh 32
	png2asset gfx/dialog.png -c src/tiles/dialog.c -map -use_map_attributes -sp 0x01
	png2asset gfx/soldier.png -c src/tiles/soldier.c -spr8x16 -sw 16 -sh 32 -sp 0x01

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi

