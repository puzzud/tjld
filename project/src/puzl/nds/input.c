#include <nds/input.h>

#include <stdio.h>
#include <string.h>

#include <nds.h>

#include <puzl.h>

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

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
	static int keys;
	
	scanKeys();
	keys = keysCurrent();

	// Buttons.
	// First 4 buttons (matches NES? Or is A & B reverse?).
	// TODO: Get X, Y, L, R (in that order or whatever the SNES does).
	// Then L & R (order swapped).
	ControllerButtonState = keys & 0x0f;

	// NOTE: Touch & Lid were ignored.

	// Axes.
	ControllerAxisXState = 0;
	
	if (keys & KEY_LEFT)
	{
		--ControllerAxisXState;
	}

	if (keys & KEY_RIGHT)
	{
		++ControllerAxisXState;
	}

	ControllerAxisYState = 0;

	if (keys & KEY_UP)
	{
		--ControllerAxisYState;
	}

	if (keys & KEY_DOWN)
	{
		++ControllerAxisYState;
	}
}
