#ifndef PUZL_H
#define PUZL_H

// NOTE: Bad hack to get VS Code to shut up about cc65
// keywords that are not ISO.
#ifdef __CC65__
#define FASTCALL __fastcall__
#else
#define FASTCALL
#endif

// NOTE: Bad hack to get VS Code to shut up about far
// keyword (not ISO?).
#ifdef __WATCOM__
#define FAR far
#else
#define FAR
#endif

// z88dk does not recognize inline.
#ifdef __SCCZ80
#define INLINE
#else
#define INLINE inline
#endif

#define SetBits(x,bits)   (x | bits)
#define ClearBits(x,bits) (x & ~bits)

#define LowByte(n)  (n & 0x00ff)
#define HighByte(n) ((n>>8) & 0x00ff)

typedef unsigned char byte;
typedef unsigned short word;

#if defined(__NULL__)
#define HAS_KEYBOARD

#include <null/core.h>
#include <null/video.h>
#include <null/color.h>
#include <null/input.h>
#include <null/audio.h>
#else

#if defined(__linux__) || defined(_WIN32) | defined(__EMSCRIPTEN__)
#define HAS_KEYBOARD

#include <sdl/core.h>
#include <sdl/video.h>
#include <sdl/color.h>
#include <sdl/input.h>
#include <sdl/audio.h>
#else

//#ifdef __CC65__

#ifdef __C64__
#define HAS_KEYBOARD

#pragma rodata-name ("RODATA")

#include <c64/core.h>
#include <c64/video.h>
#include <c64/input.h>
#include <audio.h>
#else

#ifdef __NES__

#pragma rodata-name ("RODATA")
#include <nes/core.h>
#include <nes/video.h>
#include <nes/input.h>
#include <audio.h>

#else

#ifdef __MSX__
#define HAS_KEYBOARD

#include <msx/core.h>
#include <msx/video.h>
#include <msx/color.h>
#include <msx/input.h>
#include <msx/audio.h>
#else

#ifdef __WATCOMC__
#define HAS_KEYBOARD

#include <msdos/core.h>
#include <msdos/video.h>
#include <msdos/color.h>
#include <msdos/input.h>
#include <msdos/audio.h>
#else

#ifdef __NDS__
#include <nds/core.h>
#include <nds/video.h>
#include <nds/color.h>
#include <nds/input.h>
#include <nds/audio.h>
#else

#endif
#endif
#endif
#endif
#endif
#endif
#endif

#endif
