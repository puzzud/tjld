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

#endif
