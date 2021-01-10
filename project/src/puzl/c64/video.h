#ifndef PUZL_C64_VIDEO_H
#define PUZL_C64_VIDEO_H

#include <video.h>

#include <puzl.h>

#include <c64/c64.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

#define TILEMAP_WIDTH (SCREEN_WIDTH / TILE_WIDTH)
#define TILEMAP_HEIGHT (SCREEN_HEIGHT / TILE_HEIGHT)

extern byte TileMapCollisionCodes[SCREEN_CHAR_SIZE];

#pragma zpsym ("SpriteCollisionMasks")
#pragma zpsym ("SpriteCollisions")

void FASTCALL InitializeVideo(void);

void FASTCALL SetSpritePositionX(byte spriteIndex, signed short x);
void FASTCALL SetSpritePositionY(byte spriteIndex, signed short y);

#endif
