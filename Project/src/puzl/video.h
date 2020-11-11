#ifndef VIDEO_H
#define VIDEO_H

#include <puzl.h>

extern char BackgroundColorCode;

extern Point TileMapDimensions;

char GetTileMapShapeCode(unsigned int x, unsigned int y);
char GetTileMapColorCode(unsigned int x, unsigned int y);
void SetTileMapCell(unsigned int x, unsigned int y, char shapeCode, char colorCode);

#endif
