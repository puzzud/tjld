#ifndef PUZL_C64_CORE_H
#define PUZL_C64_CORE_H

#include <core.h>

#pragma zpsym ("Running"); // TODO: Will cause compilation warning if used in C.

extern byte SpeedBitPatternByteTimer;
extern byte SpeedBitFlag;

extern byte NthBitFlag[8];

#endif
