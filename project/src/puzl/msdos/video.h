#ifndef PUZL_MSDOS_VIDEO_H
#define PUZL_MSDOS_VIDEO_H

#include <video.h>

#define VIDEO_INT          0x10

#define SET_MODE           0x00
#define WRITE_DOT          0x0c

#define VGA_256_COLOR_MODE 0x13
#define TEXT_MODE          0x03

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

#define TILEMAP_WIDTH (SCREEN_WIDTH / TILE_WIDTH)
#define TILEMAP_HEIGHT (SCREEN_HEIGHT / TILE_HEIGHT)

typedef struct
{
	byte enabled;
	ScreenPoint position;
	byte frameIndex;
	const byte** animationSet;
	byte animationId;
	byte colorCode;
	Vector2d velocity;
} Sprite;

extern byte FAR* DoubleBuffer;

void InitializeVideo(void);
void ShutdownVideo(void);

void Draw(void);

void DrawRectangle(signed short x, signed short y, unsigned short width, unsigned short height, byte colorCode);

void InitializeTilemap(void);
void DrawTileMap(void);
void DrawCharacter(unsigned int x, unsigned int y, byte shapeCode, byte colorCode);

void InitializeSprites(void);
void DrawSprites(void);

#endif
