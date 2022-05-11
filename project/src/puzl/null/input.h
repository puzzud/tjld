#ifndef PUZL_NULL_INPUT_H
#define PUZL_NULL_INPUT_H

//#include <null.h>

typedef enum
{
	KEY_CODE_LEFT,
	KEY_CODE_RIGHT,
	KEY_CODE_UP,
	KEY_CODE_DOWN,
	KEY_CODE_ESCAPE,

	NUMBER_OF_KEY_CODES
} KeyCode;

#include <input.h>

void InitializeInput(void);
void ShutdownInput(void);

void ProcessInput(void);

#endif
