#ifndef PUZL_SDL_COLOR_H
#define PUZL_SDL_COLOR_H

#include <color.h>

#include <SDL2/SDL.h>

extern SDL_Color Colors[NUMBER_OF_COLORS];

void InitializeColors(void);

void SetColorValuesFromInt(SDL_Color* color, unsigned int value);

#endif
