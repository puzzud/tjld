#include <sdl/input.h>

#include <SDL2/SDL.h>
#include <string.h>

#include <puzl.h>

byte KeyCodeStates[NUMBER_OF_KEY_CODES];

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

SDL_Joystick* controller;

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
		controller = SDL_JoystickOpen(0);
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
		SDL_JoystickClose(controller);
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

		case SDL_JOYAXISMOTION:
		{
			OnInputControllerAxisEvent(event);

			break;
		}

		case SDL_JOYBUTTONDOWN:
		{
			OnInputControllerButtonEvent(event, 1);

			break;
		}

		case SDL_JOYBUTTONUP:
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
	byte* controllerAxisState = (event->jaxis.axis == 0) ? &ControllerAxisXState : &ControllerAxisYState;
	const int axisValue = event->jaxis.value;
	
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
	const int buttonIndex = event->jbutton.button;
	if (buttonIndex > 3)
	{
		return;
	}

	if (isDown == 0)
	{
		ControllerButtonState &= ~(0x01 << buttonIndex);
	}
	else
	{
		ControllerButtonState |= (0x01 << buttonIndex);
	}
}
