#include <nds/input.h>

#include <stdio.h>
#include <string.h>

#include <puzl.h>

byte KeyCodeStates[NUMBER_OF_KEY_CODES];

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

void InitializeInput(void)
{
	
}

void ShutdownInput(void)
{
	
}

void ProcessInput(void)
{
	// TODO: Check game controllers.
}
