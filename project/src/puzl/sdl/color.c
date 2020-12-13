#include <sdl/color.h>

#include <SDL2/SDL.h>

SDL_Color Colors[NUMBER_OF_COLORS];

void SetColorValuesFromInt(SDL_Color* color, unsigned int value);

void InitializeColors(void)
{
	SetColorValuesFromInt(&Colors[COLOR_BLACK], 0x000000);
	SetColorValuesFromInt(&Colors[COLOR_WHITE], 0xffffff);
	SetColorValuesFromInt(&Colors[COLOR_RED], 0x880000);
	SetColorValuesFromInt(&Colors[COLOR_CYAN], 0xaaffee);
	SetColorValuesFromInt(&Colors[COLOR_PURPLE], 0xcc44cc);
	SetColorValuesFromInt(&Colors[COLOR_GREEN], 0x00cc55);
	SetColorValuesFromInt(&Colors[COLOR_BLUE], 0x0000aa);
	SetColorValuesFromInt(&Colors[COLOR_YELLOW], 0xeeee77);
	SetColorValuesFromInt(&Colors[COLOR_ORANGE], 0xdd8855);
	SetColorValuesFromInt(&Colors[COLOR_BROWN], 0x664400);
	SetColorValuesFromInt(&Colors[COLOR_LIGHT_RED], 0xff7777);
	SetColorValuesFromInt(&Colors[COLOR_GREY_1], 0x333333);
	SetColorValuesFromInt(&Colors[COLOR_GREY_2], 0x777777);
	SetColorValuesFromInt(&Colors[COLOR_LIGHT_GREEN], 0xaaff66);
	SetColorValuesFromInt(&Colors[COLOR_LIGHT_BLUE], 0x0088ff);
	SetColorValuesFromInt(&Colors[COLOR_GREY_3], 0xbbbbbb);
}

void SetColorValuesFromInt(SDL_Color* color, unsigned int value)
{
	color->r = (value >> 16) & 0xff;
	color->g = (value >> 8) & 0xff;
	color->b = value & 0xff;
	color->a = 0xff;
}
