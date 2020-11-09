#include "input.h"

#include <SDL2/SDL.h>
#include <string.h>

uint KeyScanCodeStates[SDL_NUM_SCANCODES];

void InitializeInput()
{
	memset(KeyScanCodeStates, 0, SDL_NUM_SCANCODES * sizeof(uint));
}

void OnInputEvent(SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_KEYDOWN:
		{
			if (event->key.repeat == 0)
			{
				KeyScanCodeStates[event->key.keysym.scancode] = 1;
			}

			break;
		}

		case SDL_KEYUP:
		{
			if (event->key.repeat == 0)
			{
				KeyScanCodeStates[event->key.keysym.scancode] = 0;
			}

			break;
		}
	}
}
