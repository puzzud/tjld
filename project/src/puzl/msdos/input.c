#include <msdos/input.h>

#include <stdio.h>
#include <string.h>
#include <conio.h>

#include <puzl.h>

byte KeyCodeStates[NUMBER_OF_KEY_CODES];

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

void ClearKeyCodes(void);
void OnKeyPress(unsigned int keyCode);

void InitializeInput(void)
{
	ClearKeyCodes();
}

inline void ClearKeyCodes(void)
{
	memset(KeyCodeStates, 0, sizeof(KeyCodeStates));
}

void ProcessInput(void)
{
	int keyCode;

	ClearKeyCodes();

	if (kbhit() != 0)
	{
		keyCode = getch();
		//printf("keyCode: %x\n", keyCode);

		OnKeyPress(keyCode);
	}
}

inline void OnKeyPress(unsigned int keyCode)
{
	if (keyCode < NUMBER_OF_KEY_CODES)
	{
		KeyCodeStates[keyCode] = 1;
	}
}
