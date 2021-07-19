//
// MSX generic definitions.

// ---------------------------------------------------------------------------
// Screen size

#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 192

// ---------------------------------------------------------------------------
// MSX colors
#define MSX_COLOR_TRANSPARENT 0
#define MSX_COLOR_BLACK 1
#define MSX_COLOR_GREEN 2
#define MSX_COLOR_LIGHT_GREEN 3
#define MSX_COLOR_DARK_BLUE 4
#define MSX_COLOR_LIGHT_BLUE 5
#define MSX_COLOR_DARK_RED 6
#define MSX_COLOR_SKY_BLUE 7
#define MSX_COLOR_RED 8
#define MSX_COLOR_BRIGHT_RED 9
#define MSX_COLOR_YELLOW 10
#define MSX_COLOR_LIGHT_YELLOW 11
#define MSX_COLOR_DARK_GREEN 12
#define MSX_COLOR_PURPLE 13
#define MSX_COLOR_GREY 14
#define MSX_COLOR_WHITE 15

// ---------------------------------------------------------------------------
// MSX system variables
#define FORCLR 0xf3e9 // Foreground color
#define BAKCLR 0xf3eA // Background color
#define BDRCLR 0xf3eB // Border color

// ---------------------------------------------------------------------------
// MSX VDP port numbers.
#define VdpDataPort 0x98
#define VdpCommandPort 0x99

// ---------------------------------------------------------------------------
// MSX BIOS routine addresses
// TODO: Incomplete.
#define CHGMOD 0x005f
#define CHGCLR 0x0062
#define INIT32 0x006f
#define SETTXT 0x0078
#define SETGRP 0x007e
#define CALPAT 0x0084
#define CALATR 0x0087
#define GSPSIZ 0x008a
#define GRPPRT 0x008d
#define CHSNS 0x009c
#define CHGET 0x009f
#define CHPUT 0x00a2
#define BEEP 0x00c0
#define CLS 0x00c3
#define POSIT 0x00c6
#define GTSTCK 0x00d5
#define GTTRIG 0x00d8
