#ifndef PUZL_H
#define PUZL_H

typedef struct
{
	int x;
	int y;
} Point;

#if defined(__linux__) || defined(_WIN32) | defined(__EMSCRIPTEN__)
#include <sdl/core.h>
#include <sdl/video.h>
#include <sdl/color.h>
#include <sdl/input.h>
#endif

#ifdef __CC65__
#include <c64/core.h>
#include <c64/video.h>
#include <color.h>
#include <c64/input.h>
#endif

#endif
