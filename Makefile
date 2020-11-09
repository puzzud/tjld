#

########################################

.SUFFIXES:
.PHONY: all
all: tjld

tjld: tjld.c core.c video.c color.c input.c
	g++ -o tjld tjld.c core.c video.c color.c input.c `sdl2-config --cflags --libs`

.PHONY: clean
clean:
	@rm tjld

play:
	./tjld
