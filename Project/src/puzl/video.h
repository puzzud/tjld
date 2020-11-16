#ifndef VIDEO_H
#define VIDEO_H

#include <puzl.h>

extern byte PrintX;
extern byte PrintY;
extern byte PrintColor;

extern void SetBackgroundColor(byte colorCode);

byte GetTileMapShapeCode(byte x, byte y);
byte GetTileMapColorCode(byte x, byte y);
void SetTileMapCellShape(byte x, byte y, byte shapeCode);
void SetTileMapCellColor(byte x, byte y, byte colorCode);

extern void FASTCALL PrintText(const char* text, byte x, byte y);

#endif
