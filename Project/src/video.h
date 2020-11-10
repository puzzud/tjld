#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>

extern char BackgroundColorCode;

extern SDL_Point TileMapDimensions;

int InitializeVideo();
void InitializeTilemap();

void ShutdownVideo();

void FreeTilemap();

void Draw();

void ClearScreen();
void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, char colorCode);
void DrawTileMap();

char GetTileMapShapeCode(unsigned int x, unsigned int y);
char GetTileMapColorCode(unsigned int x, unsigned int y);
void SetTileMapCell(unsigned int x, unsigned int y, char shapeCode, char colorCode);

#endif
