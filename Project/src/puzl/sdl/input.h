#ifndef PUZL_SDL_INPUT_H
#define PUZL_SDL_INPUT_H

#include <SDL2/SDL.h>

#define NUM_OF_KEY_CODES SDL_NUM_SCANCODES

#include <input.h>

typedef enum
{
	KEY_CODE_LEFT = SDL_SCANCODE_LEFT,
	KEY_CODE_RIGHT = SDL_SCANCODE_RIGHT,
	KEY_CODE_UP = SDL_SCANCODE_UP,
	KEY_CODE_DOWN = SDL_SCANCODE_DOWN
} KeyCode;

void InitializeInput();

void OnInputEvent(SDL_Event* event);

#endif
