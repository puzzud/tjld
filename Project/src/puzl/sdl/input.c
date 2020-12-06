#include <sdl/input.h>

#include <SDL2/SDL.h>
#include <string.h>

#include <puzl.h>

byte KeyCodeStates[NUMBER_OF_KEY_CODES];

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

SDL_GameController* controller;

byte* DpadButtonAxisMap[] =
{
	&ControllerAxisYState, // SDL_CONTROLLER_BUTTON_DPAD_UP
	&ControllerAxisYState, // SDL_CONTROLLER_BUTTON_DPAD_DOWN
	&ControllerAxisXState, // SDL_CONTROLLER_BUTTON_DPAD_LEFT
	&ControllerAxisXState  // SDL_CONTROLLER_BUTTON_DPAD_RIGHT	
};

int DpadButtonAxisStateMap[] =
{
	-1, // SDL_CONTROLLER_BUTTON_DPAD_UP
	1,  // SDL_CONTROLLER_BUTTON_DPAD_DOWN
	-1, // SDL_CONTROLLER_BUTTON_DPAD_LEFT
	1   // SDL_CONTROLLER_BUTTON_DPAD_RIGHT	
};

void InitializeKeyboard(void);
void InitializeControllers(void);

void OnInputKeyEvent(SDL_Event* event, unsigned int isDown);
void OnInputControllerAxisEvent(SDL_ControllerAxisEvent* event);
void OnInputControllerButtonEvent(SDL_ControllerButtonEvent* event, unsigned int isDown);
void OnInputControllerDpadEvent(SDL_ControllerButtonEvent* event, unsigned int isDown);

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
			OnInputControllerAxisEvent(&event->caxis);

			break;
		}

		case SDL_CONTROLLERBUTTONDOWN:
		{
			OnInputControllerButtonEvent(&event->cbutton, 1);

			break;
		}

		case SDL_CONTROLLERBUTTONUP:
		{
			OnInputControllerButtonEvent(&event->cbutton, 0);

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

void OnInputControllerAxisEvent(SDL_ControllerAxisEvent* event)
{
	const int axisValue = event->value;
	
	byte* controllerAxisState = NULL;

	switch (event->axis)
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

void OnInputControllerButtonEvent(SDL_ControllerButtonEvent* event, unsigned int isDown)
{
	const int buttonIndex = event->button;
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
		if (buttonIndex >= SDL_CONTROLLER_BUTTON_DPAD_UP && buttonIndex <= SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		{
			OnInputControllerDpadEvent(event, isDown);
		}
	}
}

void OnInputControllerDpadEvent(SDL_ControllerButtonEvent* event, unsigned int isDown)
{
	unsigned int dpadButtonAxisMapIndex = event->button - SDL_CONTROLLER_BUTTON_DPAD_UP;
	byte* controllerAxisState = DpadButtonAxisMap[dpadButtonAxisMapIndex];
	
	*controllerAxisState = (isDown == 0) ? 0 : DpadButtonAxisStateMap[dpadButtonAxisMapIndex];
}
