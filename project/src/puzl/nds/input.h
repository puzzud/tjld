#ifndef PUZL_NDS_INPUT_H
#define PUZL_NDS_INPUT_H

#include <nds.h>

#include <input.h>

extern int KeysState;

void InitializeInput(void);
void ShutdownInput(void);

void ProcessInput(void);

#endif
