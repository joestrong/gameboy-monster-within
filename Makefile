.PHONY: gfx

BINARY = out/main.gb

OBJECTS = src/main.o \
		src/intro.o \
		src/overworld.o \
		src/pathfinding.o \
		src/sprite_manager.o \
		src/sprites/enemy.o \
		src/sprites/projectile.o \
		src/tiles/gbcompologo.o \
		src/tiles/title.o \
		src/tiles/overworld.o \
		src/tiles/player.o \
		src/tiles/arm_h.o \
		src/tiles/arm_v.o \
		src/tiles/arm_v_back.o \
		src/tiles/dialog.o \
		src/tiles/soldier.o \
		src/helpers/hitbox.o \
		src/helpers/vector.o \
		src/music/prison.c

LIBRARIES = -Wl-lsrc/music/hUGEDriver.lib

CC = ${GBDK_HOME}/bin/lcc -debug -autobank -Wm-yC -Wm-yn"MonsterIns" -Wm-yt0x19 -Wm-yoA

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(LIBRARIES) -o $(BINARY) $(OBJECTS)

%.o: %.c
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
	rm -f **/*.o **/*.lst **/*.map **/*.gb **/*~ **/*.rel **/*.cdb **/*.ihx **/*.lnk **/*.sym **/*.asm **/*.noi **/*.adb
