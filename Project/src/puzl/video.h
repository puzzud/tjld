#ifndef VIDEO_H
#define VIDEO_H

#include <puzl.h>

extern void SetBackgroundColor(byte colorCode);

byte GetTileMapShapeCode(byte x, byte y);
byte GetTileMapColorCode(byte x, byte y);
void SetTileMapCell(byte x, byte y, byte shapeCode, byte colorCode);

#endif
