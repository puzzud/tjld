#ifndef PUZL_MSDOS_COLOR_H
#define PUZL_MSDOS_COLOR_H

typedef enum
{
	COLOR_BLACK = 0,
	COLOR_WHITE = 15,
	COLOR_RED = 4,
	COLOR_CYAN = 11,
	COLOR_PURPLE = 5,
	COLOR_GREEN = 2,
	COLOR_BLUE = 1,
	COLOR_YELLOW = 14,
	COLOR_ORANGE = 12,
	COLOR_BROWN = 6,
	COLOR_LIGHT_RED = 13,
	COLOR_GREY_1 = 7,
	COLOR_GREY_2 = (7 + 16),
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_BLUE = 9,
	COLOR_GREY_3 = 8,

	NUMBER_OF_COLORS
} ColorCode;

#endif
