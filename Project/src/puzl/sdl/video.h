#ifndef PUZL_SDL_VIDEO_H
#define PUZL_SDL_VIDEO_H

#include <SDL2/SDL.h>

#include <video.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

#define TILEMAP_WIDTH (SCREEN_WIDTH / TILE_WIDTH)
#define TILEMAP_HEIGHT (SCREEN_HEIGHT / TILE_HEIGHT)

typedef struct
{
	byte enabled;
	signed short x;
	signed short y;
	byte frameIndex;
	//byte colorCode;
} Sprite;

extern const byte CharacterSet[NUMBER_OF_CHARACTERS][CHARACTER_HEIGHT];
extern const byte SpriteSet[NUMBER_OF_SPRITE_FRAMES][SPRITE_WIDTH][SPRITE_WIDTH];

int InitializeVideo(void);

void ShutdownVideo(void);

void Draw(void);

#endif
