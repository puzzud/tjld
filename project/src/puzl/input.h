#ifndef INPUT_H
#define INPUT_H

#include <puzl.h>

#ifdef HAS_KEYBOARD
extern byte KeyCodeStates[NUMBER_OF_KEY_CODES];
#endif

extern byte ControllerAxisXState;
extern byte ControllerAxisYState;
extern byte ControllerButtonState;

#endif
