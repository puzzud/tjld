#
PROJECT_NAME := tjld

TARGET := sdl

SDIR := ./src
PUZLDIR := $(SDIR)/puzl
BDIR := ./bin/$(TARGET)

SRC := $(wildcard $(SDIR)/*.c)
SRC += $(wildcard $(PUZLDIR)/$(TARGET)/*.c)

CFLAGS := `sdl2-config --cflags` -I $(PUZLDIR)
LFLAGS := `sdl2-config --libs`

EXE := ./$(PROJECT_NAME)
########################################

.SUFFIXES:
.PHONY: all
all: $(EXE)

$(EXE): $(SRC)
	g++ -o $@ $(SRC) $(CFLAGS) $(LFLAGS)

.PHONY: clean
clean:
	@rm -f $(EXE)

.PHONY: play
play:
	$(EXE)
