#include <nds/input.h>

#include <stdio.h>
#include <string.h>

#include <nds.h>

#include <puzl.h>

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

int KeysState;

void InitializeInput(void)
{
	ControllerAxisXState = 0;
	ControllerAxisYState = 0;
	ControllerButtonState = 0;
}

void ShutdownInput(void)
{
	
}

inline void ProcessInput(void)
{
	scanKeys();
	KeysState = keysCurrent();

	// Buttons.
	// First 4 buttons (matches NES? Or is A & B reverse?).
	// TODO: Get X, Y, L, R (in that order or whatever the SNES does).
	// Then L & R (order swapped).
	ControllerButtonState = KeysState & 0x0f;

	// NOTE: Touch & Lid were ignored.

	// Axes.
	ControllerAxisXState = 0;
	ControllerAxisYState = 0;
	
	if (KeysState != 0)
	{
		if (KeysState & KEY_LEFT)
		{
			--ControllerAxisXState;
		}

		if (KeysState & KEY_RIGHT)
		{
			++ControllerAxisXState;
		}

		if (KeysState & KEY_UP)
		{
			--ControllerAxisYState;
		}

		if (KeysState & KEY_DOWN)
		{
			++ControllerAxisYState;
		}
	}
}
