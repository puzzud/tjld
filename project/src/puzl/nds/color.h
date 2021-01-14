#ifndef PUZL_NDS_COLOR_H
#define PUZL_NDS_COLOR_H

#include <nds.h>

#define R_FROM_RGB24(c) ((c >> 16) & 0xff)
#define G_FROM_RGB24(c) ((c >> 8) & 0xff)
#define B_FROM_RGB24(c) (c & 0xff)
#define C8TO5(c) (c >> 3)

#define RGB24_TO_ARGB16(c) ((1 << 15) | C8TO5(R_FROM_RGB24(c)) | (C8TO5(G_FROM_RGB24(c)) << 5) | (C8TO5(B_FROM_RGB24(c)) << 10))

typedef enum
{
	COLOR_BLACK,
	COLOR_WHITE,
	COLOR_RED,
	COLOR_CYAN,
	COLOR_PURPLE,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_ORANGE,
	COLOR_BROWN,
	COLOR_LIGHT_RED,
	COLOR_GREY_1,
	COLOR_GREY_2,
	COLOR_LIGHT_GREEN,
	COLOR_LIGHT_BLUE,
	COLOR_GREY_3,

	NUMBER_OF_COLORS
} ColorCode;

extern u16 Colors[NUMBER_OF_COLORS];

#endif
