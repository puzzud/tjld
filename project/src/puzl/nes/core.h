#ifndef PUZL_NES_CORE_H
#define PUZL_NES_CORE_H

#include <core.h>

extern byte Running;
#pragma zpsym ("Running"); // TODO: Will cause compilation warning if used in C.

// TODO: Shouldn't be in puzl.
extern byte SpeedBitPatternByteTimer;
extern byte SpeedBitFlag;

// TODO: Probably don't need.
extern byte NthBitFlags[8];
extern byte InverseNthBitFlags[8];

#endif
