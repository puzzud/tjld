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

int InitializeVideo(void);
void InitializeTilemap(void);

void ShutdownVideo(void);

void FreeTilemap(void);

void Draw(void);

void ClearScreen(void);
void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, char colorCode);
void DrawTileMap(void);

#endif
