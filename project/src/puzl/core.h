#ifndef CORE_H
#define CORE_H

#include <puzl.h>

#define NUMBER_OF_SPEED_BIT_PATTERN_BYTES 3

extern byte Running;

void FASTCALL InitializeNodeTree(void);
void FASTCALL Process(void);

extern void FASTCALL InitalizeSpeed(void);
extern void FASTCALL CycleSpeedBit(void);
extern byte FASTCALL IsMoving(byte speedPatternIndex);

#endif
