#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>

extern uint BackgroundColorCode;

extern uint TileMapWidth;
extern uint TileMapHeight;

int InitializeVideo();
void InitializeTilemap();

void ShutdownVideo();

void FreeTilemap();

void Draw();

void ClearScreen();
void DrawRectangle(uint x, uint y, uint width, uint height, SDL_Color* color);
void DrawTileMap();

char GetTileMapShapeCode(int x, int y);
char GetTileMapColorCode(int x, int y);
void SetTileMapCell(int x, int y, char shapeCode, char colorCode);

#endif
