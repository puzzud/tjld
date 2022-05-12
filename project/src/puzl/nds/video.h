#ifndef PUZL_NDS_VIDEO_H
#define PUZL_NDS_VIDEO_H

#include <video.h>

#include <nds.h>

//#define SCREEN_WIDTH 256
//#define SCREEN_HEIGHT 192

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

#define TILEMAP_WIDTH (SCREEN_WIDTH / TILE_WIDTH)
#define TILEMAP_HEIGHT (SCREEN_HEIGHT / TILE_HEIGHT)

void InitializeVideo(void);
void ShutdownVideo(void);

void InitializeTilemap(void);
void DrawTileMap(void);
void DrawCharacter(unsigned int x, unsigned int y, byte shapeCode, byte colorCode);

void InitializeSprites(void);
void ShutdownSprites(void);
void DrawSprites(void);

#endif
