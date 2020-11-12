########################################
# Project
########################################
PROJECT_NAME := tjld

########################################
# General
########################################

TARGET ?= lin

PUZL := puzl

SRC := src
SDIR := ./$(SRC)
PUZLDIR := $(SDIR)/$(PUZL)

SRCS := $(wildcard $(SDIR)/*.c)

BIN := bin
BDIR := ./$(BIN)
BTDIR := $(BDIR)/$(TARGET)
BTPDIR := $(BTDIR)/$(PUZL)

CFLAGS := -I $(PUZLDIR)

########################################
# Target Specifics
########################################
ifeq ($(TARGET),lin)
CC := g++

PLATFORM := sdl

CFLAGS := `sdl2-config --cflags` -g $(CFLAGS)
LFLAGS := `sdl2-config --libs`

EXECUTABLE := ./$(PROJECT_NAME)

EXECUTE := $(EXECUTABLE)
endif

ifeq ($(TARGET),win)
CC := x86_64-w64-mingw32-g++

PLATFORM := sdl

#WIN_SDL_DEV_HOME # Environment variable should be set to root directory that contains include, lib, etc..
#MINGW_INCLUDE # Environment variable should be set to directory that contains include (eg /usr/share/mingw-w64/include).

WIN_SDL_INCLUDE := $(WIN_SDL_DEV_HOME)/include
WIN_SDL_LIB := $(WIN_SDL_DEV_HOME)/lib

CFLAGS := -I $(WIN_SDL_INCLUDE) -I $(MINGW_INCLUDE) $(CFLAGS)
LFLAGS := -L $(WIN_SDL_LIB) -mwindows -lmingw32 -lSDL2main -lSDL2

EXECUTABLE := $(BTDIR)/$(PROJECT_NAME).exe

EXECUTE := wine $(EXECUTABLE)
endif

ifeq ($(TARGET),web)
CC := emcc

PLATFORM := sdl

CFLAGS := -s USE_SDL=2 $(CFLAGS)
LFLAGS := 

HTML := $(PROJECT_NAME).html
EXECUTABLE := $(BTDIR)/$(HTML)

EXECUTE := @echo Serve this follow to test: $(EXECUTABLE)
endif

########################################
# Platform Dependent Settings
########################################
SRCS += $(wildcard $(PUZLDIR)/$(PLATFORM)/*.c)

BTPPDIR := $(BTPDIR)/$(PLATFORM)

OBJS := $(patsubst %.c,%.o,$(SRCS))
OBJS := $(subst $(SDIR),$(BTDIR),$(OBJS))

########################################
# Rules
########################################
.SUFFIXES:
.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

$(BDIR)/$(TARGET)/%.o: $(SDIR)/%.c | $(BTPPDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(LFLAGS)

$(BTPPDIR): | $(BTPDIR)
	mkdir $(BTPPDIR)

$(BTPDIR): | $(BTDIR)
	mkdir $(BTPDIR)

$(BTDIR): | $(BDIR)
	mkdir $(BTDIR)

$(BDIR):
	mkdir $(BDIR)

.PHONY: clean
clean:
	@rm -f $(EXECUTABLE)
	@rm -rf $(BTDIR)

.PHONY: play
play: | all
	$(EXECUTE)
