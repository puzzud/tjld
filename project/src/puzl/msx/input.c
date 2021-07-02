#include <msx/input.h>

#include <stdio.h>
#include <string.h>

//#include <msx.h>

#include <puzl.h>

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

byte KeyCodeStates[NUMBER_OF_KEY_CODES];

void InitializeInput(void)
{
	ControllerAxisXState = 0;
	ControllerAxisYState = 0;
	ControllerButtonState = 0;
}

void ShutdownInput(void)
{
	
}

void ProcessInput(void)
{
	ControllerAxisXState = 0;
	ControllerAxisYState = 0;
	ControllerButtonState = 0;
}
