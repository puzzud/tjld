#include <sdl/input.h>

#include <SDL2/SDL.h>
#include <string.h>

#include <puzl.h>

byte KeyCodeStates[NUMBER_OF_KEY_CODES];

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

SDL_GameController* controller;

void InitializeKeyboard(void);
void InitializeControllers(void);

void OnInputKeyEvent(SDL_Event* event, unsigned int isDown);
void OnInputControllerAxisEvent(SDL_Event* event);
void OnInputControllerButtonEvent(SDL_Event* event, unsigned int isDown);

void InitializeInput(void)
{
	InitializeKeyboard();
	InitializeControllers();
}

void InitializeKeyboard(void)
{
	memset(KeyCodeStates, 0, NUMBER_OF_KEY_CODES * sizeof(byte));
}

void InitializeControllers(void)
{
	ControllerAxisXState = 0;
	ControllerAxisYState = 0;
	ControllerButtonState = 0x00;

	if (SDL_NumJoysticks() > 0)
	{
		if (SDL_IsGameController(0) == SDL_TRUE)
		{
			controller = SDL_GameControllerOpen(0);
		}
	}
}

void ShutdownInput(void)
{
	ShutdownControllers();
}

void ShutdownControllers(void)
{
	if (controller != NULL)
	{
		SDL_GameControllerClose(controller);
		controller = NULL;
	}
}

void OnInputEvent(SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_KEYDOWN:
		{
			OnInputKeyEvent(event, 0);

			break;
		}

		case SDL_KEYUP:
		{
			OnInputKeyEvent(event, 1);

			break;
		}

		case SDL_CONTROLLERAXISMOTION:
		{
			OnInputControllerAxisEvent(event);

			break;
		}

		case SDL_CONTROLLERBUTTONDOWN:
		{
			OnInputControllerButtonEvent(event, 1);

			break;
		}

		case SDL_CONTROLLERBUTTONUP:
		{
			OnInputControllerButtonEvent(event, 0);

			break;
		}
	}
}

void OnInputKeyEvent(SDL_Event* event, unsigned int isDown)
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

void OnInputControllerAxisEvent(SDL_Event* event)
{
	const int axisValue = event->caxis.value;
	
	byte* controllerAxisState = NULL;

	switch (event->caxis.axis)
	{
		case 0:
		{
			controllerAxisState = &ControllerAxisXState;

			break;
		}

		case 1:
		{
			controllerAxisState = &ControllerAxisYState;

			break;
		}

		default:
		{
			return;
		}
	}

	if (axisValue < -16000)
	{
		*controllerAxisState = -1;
	}
	else if (axisValue > 16000)
	{
		*controllerAxisState = 1;
	}
	else
	{
		*controllerAxisState = 0;
	}
}

void OnInputControllerButtonEvent(SDL_Event* event, unsigned int isDown)
{
	const int buttonIndex = event->cbutton.button; 
	if (buttonIndex < 4)
	{
		if (isDown == 0)
		{
			ControllerButtonState &= ~(0x01 << buttonIndex);
		}
		else
		{
			ControllerButtonState |= (0x01 << buttonIndex);
		}
	}
	else
	{
		switch (buttonIndex)
		{
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			{
				ControllerAxisXState = (isDown == 0) ? 0 : -1;

				break;
			}

			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			{
				ControllerAxisXState = (isDown == 0) ? 0 : 1;

				break;
			}

			case SDL_CONTROLLER_BUTTON_DPAD_UP:
			{
				ControllerAxisYState = (isDown == 0) ? 0 : -1;

				break;
			}

			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			{
				ControllerAxisYState = (isDown == 0) ? 0 : 1;

				break;
			}
		}
	}
}
