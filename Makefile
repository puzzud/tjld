#

########################################

.SUFFIXES:
.PHONY: all
all: tjld

tjld: tjld.c
	g++ -o tjld tjld.c -lSDL2main -lSDL2

.PHONY: clean
clean:
	@rm tjld

play:
	./tjld
