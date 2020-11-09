#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

extern uint KeyScanCodeStates[SDL_NUM_SCANCODES];

void InitializeInput();

void OnInputEvent(SDL_Event* event);

#endif
