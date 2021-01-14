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

#define SetBits(x,bits)   (x | bits)
#define ClearBits(x,bits) (x & ~bits)

#define LowByte(n)  (n & 0x00ff)
#define HighByte(n) ((n>>8) & 0x00ff)

typedef unsigned char byte;
typedef unsigned short word;

#if defined(__linux__) || defined(_WIN32) | defined(__EMSCRIPTEN__)
#include <sdl/core.h>
#include <sdl/video.h>
#include <sdl/color.h>
#include <sdl/input.h>
#include <sdl/audio.h>
#endif

#ifdef __CC65__
#pragma rodata-name ("RODATA")

#include <c64/core.h>
#include <c64/video.h>
#include <c64/input.h>
#include <audio.h>
#endif

#ifdef __WATCOMC__
#include <msdos/core.h>
#include <msdos/video.h>
#include <msdos/color.h>
#include <msdos/input.h>
#include <msdos/audio.h>
#endif

#ifdef __NDS__
#include <nds/core.h>
#include <nds/video.h>
#include <nds/color.h>
#include <nds/input.h>
#include <nds/audio.h>
#endif

#endif
