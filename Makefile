#

########################################

.SUFFIXES:
.PHONY: all
all: tjld

tjld: src/*.c
	g++ -o tjld src/*.c `sdl2-config --cflags --libs`

.PHONY: clean
clean:
	@rm tjld

play:
	./tjld
