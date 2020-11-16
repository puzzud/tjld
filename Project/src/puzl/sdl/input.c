#include <sdl/input.h>

#include <SDL2/SDL.h>
#include <string.h>

#include <puzl.h>

byte KeyCodeStates[NUMBER_OF_KEY_CODES];

void InitializeInput(void)
{
	memset(KeyCodeStates, 0, NUMBER_OF_KEY_CODES * sizeof(byte));
}

void OnInputEvent(SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_KEYDOWN:
		{
			if (event->key.repeat == 0)
			{
				KeyCodeStates[event->key.keysym.scancode] = 1;
			}

			break;
		}

		case SDL_KEYUP:
		{
			if (event->key.repeat == 0)
			{
				KeyCodeStates[event->key.keysym.scancode] = 0;
			}

			break;
		}
	}
}
