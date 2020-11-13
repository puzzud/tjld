#ifndef PUZL_SDL_VIDEO_H
#define PUZL_SDL_VIDEO_H

#include <SDL2/SDL.h>

#include <video.h>

int InitializeVideo(void);
void InitializeTilemap(void);

void ShutdownVideo(void);

void FreeTilemap(void);

void Draw(void);

void ClearScreen(void);
void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, char colorCode);
void DrawTileMap(void);

#endif
