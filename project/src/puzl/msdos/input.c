#include <msdos/input.h>

#include <stdio.h>
#include <string.h>
#include <dos.h>

#include <puzl.h>

#define KEYBOARD_INTERRUPT 0x09
#define KEY_BUFFER         0x60
#define KEY_CONTROL        0x61
#define PIC_PORT           0x20

void (interrupt FAR* OldKeyboardIsr)(void); // Holds old keyboard interrupt handler.
void interrupt FAR KeyboardDriver(void);
void KeyboardDriver0(void);

word RawKeyScanCode;

byte KeyCodeStates[NUMBER_OF_KEY_CODES];
unsigned int NumberOfActiveKeys;

byte ControllerAxisXState;
byte ControllerAxisYState;
byte ControllerButtonState;

void ClearKeyCodes(void);
void OnKeyPress(unsigned int keyCode);
unsigned char GetScanCode(void);

void InitializeInput(void)
{
	ClearKeyCodes();

	OldKeyboardIsr = _dos_getvect(KEYBOARD_INTERRUPT);
	_dos_setvect(KEYBOARD_INTERRUPT, KeyboardDriver);
}

void ShutdownInput(void)
{
	_dos_setvect(KEYBOARD_INTERRUPT, OldKeyboardIsr);
}

#pragma aux GetScanCode = \
"mov ah, 01h" \
"int 16h" \
"jz bufferIsEmpty" \
"mov ah, 00h" \
"int 16h" \
"mov al, ah" \
"xor ah, ah" \
"jmp done" \
"bufferIsEmpty:" \
"xor ax, ax" \
"done:" \
modify [ah] \
value [al];

inline void ClearKeyCodes(void)
{
	memset(KeyCodeStates, 0, sizeof(KeyCodeStates));

	NumberOfActiveKeys = 0;

	RawKeyScanCode = 0;
}

void ProcessInput(void)
{
	/*unsigned char keyCode;

	ClearKeyCodes();

	keyCode = GetScanCode();

	//if (kbhit() != 0)
	if (keyCode != 0)
	{
		//keyCode = getch();
		//printf("keyCode: %x\n", keyCode);

		OnKeyPress(keyCode);
	}*/
}

inline void OnKeyPress(unsigned int keyCode)
{
	if (keyCode < NUMBER_OF_KEY_CODES)
	{
		KeyCodeStates[keyCode] = 1;
	}
}

// 60h is KEY_BUFFER.
// 61h is KEY_CONTROL.
// 21h is PIC_PORT.
#pragma aux KeyboardDriver0 = \
"sti" \
"in al, 60h" \
"xor ah, ah" \
"mov RawKeyScanCode, ax" \
"in al, 60h" \
"or al, 82h" \
"out 61h, al" \
"and al, 7fh" \
"out 61h, al" \
"mov al, 20h" \
"out 20h, al" \
modify [ax];

void _WCINTERRUPT FAR KeyboardDriver(void)
{
	KeyboardDriver0();

	printf("keyCode: %x\n", RawKeyScanCode);

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
