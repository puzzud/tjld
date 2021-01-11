#include <msdos/input.h>

#include <stdio.h>
#include <string.h>
#include <dos.h>

#include <puzl.h>

#define KEYBOARD_INTERRUPT 0x09
#define KEY_BUFFER         0x60
#define KEY_CONTROL        0x61
#define PIC_PORT           0x20

void InitializeKeyboard(void);
void _WCINTERRUPT FAR KeyboardDriver(void);
void ScanKeyForInterrupt(void);

void _WCINTERRUPT (FAR* OldKeyboardIsr)(void); // Holds old keyboard interrupt handler.
word RawKeyScanCode;
byte KeyCodeStates[NUMBER_OF_KEY_CODES];
byte NumberOfActiveKeys;

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

void InitializeInput(void)
{
	InitializeKeyboard();
}

void ShutdownInput(void)
{
	// Shutdown keyboard.
	_dos_setvect(KEYBOARD_INTERRUPT, OldKeyboardIsr);
}

void InitializeKeyboard(void)
{
	memset(KeyCodeStates, 0, sizeof(KeyCodeStates));
	NumberOfActiveKeys = 0;
	RawKeyScanCode = 0;

	OldKeyboardIsr = _dos_getvect(KEYBOARD_INTERRUPT);
	_dos_setvect(KEYBOARD_INTERRUPT, KeyboardDriver);
}

void ProcessInput(void)
{
	// TODO: Check game controllers.
}

// 60h is KEY_BUFFER.
// 61h is KEY_CONTROL.
// 21h is PIC_PORT.
#pragma aux ScanKeyForInterrupt = \
"in al, 60h" \
"xor ah, ah" \
"mov RawKeyScanCode, ax" \
"in al, 61h" \
"or al, 82h" \
"out 61h, al" \
"and al, 7fh" \
"out 61h, al" \
"mov al, 20h" \
"out 20h, al" \
modify [ax];

void _WCINTERRUPT FAR KeyboardDriver(void)
{
	ScanKeyForInterrupt();

	if (RawKeyScanCode < 128)
	{
		if (KeyCodeStates[RawKeyScanCode] == 0)
		{
			++NumberOfActiveKeys;

			KeyCodeStates[RawKeyScanCode] = 1;
		}
	}
	else
	{
		if (KeyCodeStates[RawKeyScanCode - 128] == 1)
		{
			--NumberOfActiveKeys;

			KeyCodeStates[RawKeyScanCode - 128] = 0;
		}
	}
}
