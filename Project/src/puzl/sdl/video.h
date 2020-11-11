#ifndef PUZL_SDL_VIDEO_H
#define PUZL_SDL_VIDEO_H

#include <SDL2/SDL.h>

#include <video.h>

int InitializeVideo();
void InitializeTilemap();

void ShutdownVideo();

void FreeTilemap();

void Draw();

void ClearScreen();
void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, char colorCode);
void DrawTileMap();

#endif
