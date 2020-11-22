#ifndef VIDEO_H
#define VIDEO_H

#include <puzl.h>

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16

typedef struct
{
	byte x;
	byte y;
} TilePoint;

extern byte PrintX;
extern byte PrintY;
extern byte PrintColor;

void FASTCALL SetBackgroundColor(byte colorCode);

byte FASTCALL GetTileMapShapeCode(byte x, byte y);
byte FASTCALL GetTileMapColorCode(byte x, byte y);
void FASTCALL SetTileMapCellShape(byte x, byte y, byte shapeCode);
void FASTCALL SetTileMapCellColor(byte x, byte y, byte colorCode);

extern void FASTCALL PrintText(const char* text, byte x, byte y);

#endif
