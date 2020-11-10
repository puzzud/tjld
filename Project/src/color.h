#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>

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
	COLOR_GREY_3
} ColorCode;

#define NUMBER_OF_COLORS 16
extern SDL_Color Colors[NUMBER_OF_COLORS];

void InitializeColors();

void SetColorValuesFromWord(SDL_Color* color, unsigned int word);

#endif
