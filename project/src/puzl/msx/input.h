#ifndef PUZL_MSX_INPUT_H
#define PUZL_MSX_INPUT_H

//#include <msx.h>

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
