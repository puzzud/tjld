########################################
# Project
########################################
PROJECT_NAME := tjld

########################################
# General
########################################

TARGET ?= lin

PUZL := puzl

LIBS := $(PUZL) game

SRC := src
SDIR := ./$(SRC)
PUZLDIR := $(SDIR)/$(PUZL)
GAMEDIR := $(SDIR)/game

TOOLSDIR := ../resources/Tools

#ASMS := $(wildcard $(SDIR)/*.asm)
ASMS := $(foreach LIB,$(LIBS),$(wildcard $(SDIR)/$(LIB)/*.asm))
#SRCS := $(wildcard $(SDIR)/*.c)
SRCS := $(foreach LIB,$(LIBS),$(wildcard $(SDIR)/$(LIB)/*.c))

BIN := bin
BDIR := ./$(BIN)
BTDIR := $(BDIR)/$(TARGET)

########################################
# Target Specifics
########################################

#---------------------------------------
# Linux
#---------------------------------------
ifeq ($(TARGET),lin)
CC := g++
CC_FAMILY := $(CC)

PLATFORM := sdl

CFLAGS := -I $(PUZLDIR) -I $(GAMEDIR)
CFLAGS := `sdl2-config --cflags` -g $(CFLAGS)
LFLAGS := `sdl2-config --libs`

EXECUTABLE := $(BTDIR)/$(PROJECT_NAME)_$(TARGET)

EXECUTE := $(EXECUTABLE)
endif

#---------------------------------------
# Raspberry Pi
#---------------------------------------
ifeq ($(TARGET),rpi)
CC := g++
CC_FAMILY := $(CC)

PLATFORM := sdl

CFLAGS := -I $(PUZLDIR) -I $(GAMEDIR)
CFLAGS := `sdl2-config --cflags` -g $(CFLAGS)
LFLAGS := `sdl2-config --libs`

EXECUTABLE := $(BTDIR)/$(PROJECT_NAME)_$(TARGET)

EXECUTE := $(EXECUTABLE)
endif

#---------------------------------------
# Windows
#---------------------------------------
ifeq ($(TARGET),win)
CC := x86_64-w64-mingw32-g++
CC_FAMILY := g++

PLATFORM := sdl

#WIN_SDL_DEV_HOME # Environment variable should be set to root directory that contains include, lib, etc..
#MINGW_INCLUDE # Environment variable should be set to directory that contains include (eg /usr/share/mingw-w64/include).

WIN_SDL_INCLUDE := $(WIN_SDL_DEV_HOME)/include
WIN_SDL_LIB := $(WIN_SDL_DEV_HOME)/lib

CFLAGS := -I $(PUZLDIR) -I $(GAMEDIR)
CFLAGS := -I $(WIN_SDL_INCLUDE) -I $(MINGW_INCLUDE) $(CFLAGS)
LFLAGS := -L $(WIN_SDL_LIB) -mwindows -lmingw32 -lSDL2main -lSDL2

EXECUTABLE := $(BTDIR)/$(PROJECT_NAME).exe

EXECUTE := wine $(EXECUTABLE)
endif

#---------------------------------------
# Web
#---------------------------------------
ifeq ($(TARGET),web)
CC := emcc
CC_FAMILY := g++

PLATFORM := sdl

CFLAGS := -I $(PUZLDIR) -I $(GAMEDIR)
CFLAGS := -s USE_SDL=2 $(CFLAGS)
LFLAGS := 

HTML := $(PROJECT_NAME).html
EXECUTABLE := $(BTDIR)/$(HTML)

EXECUTE := @echo Serve this follow to test: $(EXECUTABLE)
endif

#---------------------------------------
# MS-DOS
#---------------------------------------
ifeq ($(TARGET),msdos)
CC := wcc386
AS := wasm
LD := wlink
CC_FAMILY := watcom

PLATFORM := msdos

#WATCOM # Environment variable should be set to root directory that contains h, lib386.
#WATCOM/binl and #WATCOM/binw need to be in system path (in this order).
#EDPATH # Environment variable should be set to $WATCOM/eddat.

CFLAGS := -I=$(PUZLDIR) -I=$(GAMEDIR)
CFLAGS := -I=$(WATCOM)/h $(CFLAGS)
LFLAGS := libpath $(WATCOM)/lib386 libpath $(WATCOM)/lib386/dos system pmodew

EXECUTABLE := $(BTDIR)/$(PROJECT_NAME).exe

EXECUTE := dosbox $(EXECUTABLE)
endif

#---------------------------------------
# Commodore 64
#---------------------------------------
ifeq ($(TARGET),c64)
CC := cc65
AS := ca65
LD := ld65

CC_FAMILY := $(CC)

PLATFORM := c64
PLATFORM_UC := $(shell echo '$(PLATFORM)' | tr '[:lower:]' '[:upper:]')

CPU := 6502
LDCONFIG := c64.cfg
BIN_EXT := prg
C64_EMU ?= x64
EMU := $(C64_EMU)
LABEL_FILE := $(BTDIR)/vice.lbl
PROGRAM := $(PROJECT_NAME).$(BIN_EXT)
#EXECUTABLE := $(BDIR)/$(PROJECT_NAME).d64

#CA65_LIB # Environment variable should be set to directory that contains c64.lib.

CFLAGS := -I $(PUZLDIR) -I $(GAMEDIR)
CFLAGS := --cpu $(CPU) -O -I $(SDIR) $(CFLAGS) -D__$(PLATFORM_UC)__
AFLAGS := --cpu $(CPU) -I $(SDIR) -I $(SDIR)/$(PLATFORM)
LFLAGS := -C $(PUZLDIR)/$(PLATFORM)/$(LDCONFIG) $(CA65_LIB)/$(PLATFORM).lib -Ln $(LABEL_FILE)

EXECUTABLE := $(BTDIR)/$(PROJECT_NAME).$(BIN_EXT)

EXECUTE := $(EMU) -moncommands $(LABEL_FILE) $(EXECUTABLE)
endif

########################################
# Platform Dependent Settings
########################################
ASMS += $(foreach LIB,$(LIBS),$(wildcard $(SDIR)/$(LIB)/$(PLATFORM)/*.asm))
SRCS += $(foreach LIB,$(LIBS),$(wildcard $(SDIR)/$(LIB)/$(PLATFORM)/*.c))

CASMS := $(patsubst %.c,%_c.asm,$(SRCS))
CASMS := $(subst $(SDIR),$(BTDIR),$(CASMS))

OBJS := $(patsubst %.asm,%.o,$(ASMS))
OBJS += $(patsubst %.c,%.o,$(SRCS))
OBJS := $(subst $(SDIR),$(BTDIR),$(OBJS))

########################################
# Rules
########################################
.SUFFIXES:
.PHONY: all
all: $(EXECUTABLE)

#---------------------------------------
# gcc/g++
#---------------------------------------
ifeq ($(CC_FAMILY),g++)
$(EXECUTABLE): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

$(BDIR)/$(TARGET)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) $(LFLAGS)

#---------------------------------------
# cc65
#---------------------------------------
else ifeq ($(CC_FAMILY),cc65)
$(EXECUTABLE): $(OBJS)
	@mkdir -p $(@D)
	$(LD) -o $@ $^ $(LFLAGS)

$(BTDIR)/%.o: $(SDIR)/%.asm
	@mkdir -p $(@D)
	$(AS) -o $@ $< $(AFLAGS)

$(BTDIR)/%.o: $(BTDIR)/%_c.asm
	@mkdir -p $(@D)
	$(AS) -o $@ $< $(AFLAGS)

$(BTDIR)/%_c.asm: $(SDIR)/%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: transfer
transfer: | $(EXECUTABLE)
	$(TOOLSDIR)/TransferToUltimate.sh $(EXECUTABLE)

.PHONY: play_real
play_real: | transfer
	# Assumes presence of command.
	# https://github.com/markusC64/1541ultimate2/releases/tag/tools-v1.2.1
	1541u2 $(ULTIMATE_HOSTNAME) -c run:$(EXECUTABLE)

#---------------------------------------
# watcom
#---------------------------------------
else ifeq ($(CC_FAMILY),watcom)
$(EXECUTABLE): $(OBJS)
	@mkdir -p $(@D)
	$(LD) $(patsubst %,file %,$(OBJS)) $(LFLAGS) name $@

#$(BTDIR)/%.o: $(BTDIR)/%.asm
#	@mkdir -p $(@D)
#	$(AS) -o $@ $< $(AFLAGS)

$(BTDIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	$(CC) -fo=$@ $(CFLAGS) $<

endif

.PHONY: clean
clean:
	@rm -f $(EXECUTABLE)
	@rm -rf $(BTDIR)

.PHONY: play
play: | all
	$(EXECUTE)
