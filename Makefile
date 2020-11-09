#

########################################

.SUFFIXES:
.PHONY: all
all: tjld

tjld: tjld.c
	g++ -o tjld tjld.c `sdl2-config --cflags --libs`

.PHONY: clean
clean:
	@rm tjld

play:
	./tjld
