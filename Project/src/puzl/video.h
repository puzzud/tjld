#ifndef VIDEO_H
#define VIDEO_H

#include <puzl.h>

#define CHARACTER_HEIGHT 8
#define NUMBER_OF_CHARACTERS 256

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16
#define NUMBER_OF_SPRITE_FRAMES 7 * 7
#define NUMBER_OF_SPRITES 8

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

void FASTCALL EnableSprite(byte spriteIndex, byte enable);
void FASTCALL SetSpritePosition(byte spriteIndex, unsigned short x, unsigned short y);
void FASTCALL SetSpriteFrameIndex(byte spriteIndex, byte frameIndex);
void FASTCALL SetSpriteColor(byte spriteIndex, byte colorCode);
void FASTCALL SetSpriteSeconaryColor(byte colorCode);
void FASTCALL SetSpriteTertiaryColor(byte colorCode);

#endif
