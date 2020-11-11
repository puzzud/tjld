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
endif

ifeq ($(TARGET),web)
CC := emcc

PLATFORM := sdl

CFLAGS := -s USE_SDL=2 $(CFLAGS)
LFLAGS := 

HTML := $(PROJECT_NAME).html
EXECUTABLE := $(BTDIR)/$(HTML)
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
play:
	$(EXECUTABLE)
