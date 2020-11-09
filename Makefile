#

########################################

.SUFFIXES:
.PHONY: all
all: tjld

tjld: src/tjld.c src/core.c src/video.c src/color.c src/input.c
	g++ -o tjld src/tjld.c src/core.c src/video.c src/color.c src/input.c `sdl2-config --cflags --libs`

.PHONY: clean
clean:
	@rm tjld

play:
	./tjld
