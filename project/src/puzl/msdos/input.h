#ifndef PUZL_MSDOS_INPUT_H
#define PUZL_MSDOS_INPUT_H

// TODO: Populate these correctly when keyboard iput is implemented.
typedef enum
{
	KEY_CODE_1 = 0,

	KEY_CODE_ESCAPE = 1,

	KEY_CODE_LEFT = 2,
	KEY_CODE_RIGHT = 3,
	KEY_CODE_UP = 4,
	KEY_CODE_DOWN = 5
} KeyCode;

#define NUMBER_OF_KEY_CODES 6

#include <input.h>

#endif
