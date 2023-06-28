.PHONY: gfx

CC	= ${GBDK_HOME}/bin/lcc -Wa-l -Wl-m -Wl-j

all: gfx main

main: src/main.c
	$(CC) -o out/main.gb \
		src/main.c \
		src/tiles/gbcompologo.c \
		src/tiles/title.c \
		src/tiles/overworld.c \
		src/tiles/sprites.c

gfx: gfx/*.png
	png2asset gfx/gbcompologo.png -c src/tiles/gbcompologo.c -map
	png2asset gfx/title.png -c src/tiles/title.c -map
	png2asset gfx/overworld.png -c src/tiles/overworld.c -map
	png2asset gfx/sprites.png -c src/tiles/sprites.c -spr8x8 -sw 16 -sh 24

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi

