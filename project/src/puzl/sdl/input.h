#ifndef PUZL_SDL_INPUT_H
#define PUZL_SDL_INPUT_H

#include <SDL2/SDL.h>

typedef enum
{
	KEY_CODE_1 = SDL_SCANCODE_1,

	KEY_CODE_ESCAPE = SDL_SCANCODE_ESCAPE,

	KEY_CODE_LEFT = SDL_SCANCODE_LEFT,
	KEY_CODE_RIGHT = SDL_SCANCODE_RIGHT,
	KEY_CODE_UP = SDL_SCANCODE_UP,
	KEY_CODE_DOWN = SDL_SCANCODE_DOWN
} KeyCode;

#define NUMBER_OF_KEY_CODES SDL_NUM_SCANCODES

#include <input.h>

void InitializeInput(void);
void InitializeControllers(void);
void ShutdownInput(void);
void ShutdownControllers(void);

void OnInputEvent(SDL_Event* event);

#endif
