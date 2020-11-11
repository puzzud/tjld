#include <sdl/color.h>

#include <SDL2/SDL.h>

SDL_Color Colors[NUMBER_OF_COLORS];

void InitializeColors()
{
	SetColorValuesFromWord(&Colors[COLOR_BLACK], 0x000000);
	SetColorValuesFromWord(&Colors[COLOR_WHITE], 0xffffff);
	SetColorValuesFromWord(&Colors[COLOR_RED], 0x880000);
	SetColorValuesFromWord(&Colors[COLOR_CYAN], 0xaaffee);
	SetColorValuesFromWord(&Colors[COLOR_PURPLE], 0xcc44cc);
	SetColorValuesFromWord(&Colors[COLOR_GREEN], 0x00cc55);
	SetColorValuesFromWord(&Colors[COLOR_BLUE], 0x0000aa);
	SetColorValuesFromWord(&Colors[COLOR_YELLOW], 0xeeee77);
	SetColorValuesFromWord(&Colors[COLOR_ORANGE], 0xdd8855);
	SetColorValuesFromWord(&Colors[COLOR_BROWN], 0x664400);
	SetColorValuesFromWord(&Colors[COLOR_LIGHT_RED], 0xff7777);
	SetColorValuesFromWord(&Colors[COLOR_GREY_1], 0x333333);
	SetColorValuesFromWord(&Colors[COLOR_GREY_2], 0x777777);
	SetColorValuesFromWord(&Colors[COLOR_LIGHT_GREEN], 0xaaff66);
	SetColorValuesFromWord(&Colors[COLOR_LIGHT_BLUE], 0x0088ff);
	SetColorValuesFromWord(&Colors[COLOR_GREY_3], 0xbbbbbb);
}

void SetColorValuesFromWord(SDL_Color* color, unsigned int word)
{
	color->r = (word >> 16) & 0xff;
	color->g = (word >> 8) & 0xff;
	color->b = word & 0xff;
	color->a = 0xff;
}
