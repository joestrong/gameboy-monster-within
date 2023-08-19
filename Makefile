.PHONY: gfx

BINARY = build/monster-inside.gb

OBJECTS = obj/src/main.o \
		obj/src/intro.o \
		obj/src/overworld.o \
		obj/src/pathfinding.o \
		obj/src/sprite_manager.o \
		obj/src/sprites/enemy.o \
		obj/src/sprites/projectile.o \
		obj/src/tiles/gbcompologo.o \
		obj/src/tiles/title.o \
		obj/src/tiles/overworld.o \
		obj/src/tiles/player.o \
		obj/src/tiles/arm_h.o \
		obj/src/tiles/arm_v.o \
		obj/src/tiles/arm_v_back.o \
		obj/src/tiles/dialog.o \
		obj/src/tiles/soldier.o \
		obj/src/helpers/hitbox.o \
		obj/src/helpers/vector.o \
		obj/src/music/banked.o \
		obj/src/music/prison.o

LIBRARIES = -Wl-lsrc/music/hUGEDriver.lib

CC = ${GBDK_HOME}/bin/lcc -debug -autobank -Wm-yC -Wm-yn"MonsterIns" -Wm-yt0x19 -Wm-yoA

all: $(BINARY)

$(BINARY): $(OBJECTS)
	mkdir build
	$(CC) $(LIBRARIES) -o $(BINARY) $(OBJECTS)

obj/%.o: %.c
	mkdir -p obj/src/music obj/src/helpers obj/src/tiles obj/src/sprites
	$(CC) -c -o $@ $<

src/tiles/gbcompologo.c: gfx/gbcompologo.png
	png2asset gfx/gbcompologo.png -c src/tiles/gbcompologo.c -map -b 255

src/tiles/title.c: gfx/title.png
	png2asset gfx/title.png -c src/tiles/title.c -map -b 255

src/tiles/overworld.c: gfx/overworld.png
	png2asset gfx/overworld.png -c src/tiles/overworld.c -map -b 255

src/tiles/player.c: gfx/player.png
	png2asset gfx/player.png -c src/tiles/player.c -spr8x16 -sw 16 -sh 32

src/tiles/arm_h.c: gfx/arm_h.png
	png2asset gfx/arm_h.png -c src/tiles/arm_h.c -spr8x16 -sw 24 -sh 16

src/tiles/arm_v.c: gfx/arm_v.png
	png2asset gfx/arm_v.png -c src/tiles/arm_v.c -spr8x16 -sw 16 -sh 32

src/tiles/arm_v_back.c: gfx/arm_v_back.png
	png2asset gfx/arm_v_back.png -c src/tiles/arm_v_back.c -spr8x16 -sw 16 -sh 32

src/tiles/dialog.c: gfx/dialog.png
	png2asset gfx/dialog.png -c src/tiles/dialog.c -map -use_map_attributes -sp 0x01

src/tiles/soldier.c: gfx/soldier.png
	png2asset gfx/soldier.png -c src/tiles/soldier.c -spr8x16 -sw 16 -sh 32 -sp 0x01

clean:
	rm -rf build obj
