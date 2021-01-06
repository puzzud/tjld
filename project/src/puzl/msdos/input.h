#ifndef PUZL_MSDOS_INPUT_H
#define PUZL_MSDOS_INPUT_H

// TODO: Populate these correctly when keyboard iput is implemented.
typedef enum
{
	//KEY_CODE_1 = 0,

	KEY_CODE_ESCAPE = 0x1b,

	KEY_CODE_LEFT = 0x4b,
	KEY_CODE_RIGHT = 0x4d,
	KEY_CODE_UP = 0x48,
	KEY_CODE_DOWN = 0x50
} KeyCode;

#define NUMBER_OF_KEY_CODES 128

#include <input.h>

void InitializeInput(void);

void ProcessInput(void);

#endif
