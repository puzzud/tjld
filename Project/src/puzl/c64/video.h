#ifndef PUZL_C64_VIDEO_H
#define PUZL_C64_VIDEO_H

#include <video.h>

#include <puzl.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

#define TILEMAP_WIDTH (SCREEN_WIDTH / TILE_WIDTH)
#define TILEMAP_HEIGHT (SCREEN_HEIGHT / TILE_HEIGHT)

void FASTCALL InitializeVideo(void);

#endif
